//
// "$Id: svgdocument.cc 3372 2010-09-17 11:43:18Z audunc $"
//
//   svgdocument class function for the GLP library, an OpenGL printing toolkit.
//
//   The GLP library is distributed under the terms of the GNU Library
//   General Public License which is described in the file "COPYING.LIB".
//   If you use this library in your program, please include a line reading
//   "OpenGL Printing Toolkit by Michael Sweet" in your version or copyright
//   output.
//
// Revision History:
//
//   $Log$
//   Revision 1.0  2011/02/23  12:00:00  audunc
//   Initial revision
//

//
// Include necessary headers.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "svgdocument.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

//
// SvgDocument constructor
//

SvgDocument::SvgDocument(char *print_name, // I - Name of file/printer
    int print_options, // I - Output options
    int print_size, // I - Size of feedback buffer
    map<string, string> *mc) // manual commands
{
  // Initialize private class data and allocate a feedback buffer...

  if (mc)
    mancom = *mc;
  options = print_options;
  bboxes = NULL;
  feedsize = print_size > 0 ? print_size : 1024 * 1024;
  feedback = new GLfloat[feedsize];
  colorsize = 0;
  colormap = NULL;
  page_count = 0;
  pointscale = 0.5;

  usefakestipple = false;
  fakepattern = GLPstip_diagleft;
  stipplescale = 1.0;

#ifdef DEBUG
  cout << "feedsize = " << feedsize << ", feedback = " << (long int)feedback << "\n" << flush;
#endif /* DEBUG */

  // Next open the indicated file...
  outfile = new fstream(print_name, ios::out);
  if (outfile == NULL)
    return;
}

//
// SvgDocument destructor; like the constructor, this just does the basics.
// You'll want to implement your own...
//

SvgDocument::~SvgDocument()
{
  // Free any memory we've allocated...

  *outfile << "<!-- Pages: " << page_count << "-->\n";

  if (mancom.count("TRAILER") > 0)
    *outfile << mancom["TRAILER"];

  delete outfile;
}

// find intersection point (Px,Py) between line-segments AB and CD
// returns true if such exists
// Algorithm from comp.graphics.algorithms FAQ subject 1.03:
//-------------------------------------------------------------------
//     Let A,B,C,D be 2-space position vectors.  Then the directed line
//     segments AB & CD are given by:

//         AB=A+r(B-A), r in [0,1]
//         CD=C+s(D-C), s in [0,1]

//     If AB & CD intersect, then

//         A+r(B-A)=C+s(D-C), or

//         Ax+r(Bx-Ax)=Cx+s(Dx-Cx)
//         Ay+r(By-Ay)=Cy+s(Dy-Cy)  for some r,s in [0,1]

//     Solving the above for r and s yields

//             (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
//         r = -----------------------------  (eqn 1)
//             (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

//             (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
//         s = -----------------------------  (eqn 2)
//             (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

//     Let P be the position vector of the intersection point, then

//         P=A+r(B-A) or

//         Px=Ax+r(Bx-Ax)
//         Py=Ay+r(By-Ay)

//     By examining the values of r & s, you can also determine some
//     other limiting conditions:

//         If 0<=r<=1 & 0<=s<=1, intersection exists
//             r<0 or r>1 or s<0 or s>1 line segments do not intersect

//         If the denominator in eqn 1 is zero, AB & CD are parallel
//         If the numerator in eqn 1 is also zero, AB & CD are collinear.

//     If they are collinear, then the segments may be projected to the x-
//     or y-axis, and overlap of the projected intervals checked.

//     If the intersection point of the 2 lines are needed (lines in this
//     context mean infinite lines) regardless whether the two line
//     segments intersect, then

//         If r>1, P is located on extension of AB
//         If r<0, P is located on extension of BA
//         If s>1, P is located on extension of CD
//         If s<0, P is located on extension of DC

//     Also note that the denominators of eqn 1 & 2 are identical.

//     References:

//     [O'Rourke (C)] pp. 249-51
//     [Gems III] pp. 199-202 "Faster Line Segment Intersection,"
//------------------------------------------------------------------
static bool intersection(float Ax, float Ay, float Bx, float By, float Cx,
    float Cy, float Dx, float Dy, float& r, float& s, float &Px, float& Py)
{

  const float slimit = 0.000001;
  float r1 = (Ay - Cy) * (Dx - Cx) - (Ax - Cx) * (Dy - Cy);
  float s1 = (Ay - Cy) * (Bx - Ax) - (Ax - Cx) * (By - Ay);

  float den = (Bx - Ax) * (Dy - Cy) - (By - Ay) * (Dx - Cx);

  if (fabsf(den) < slimit) {
    return false;
  }

  r = r1 / den;
  s = s1 / den;

  Px = Ax + r * (Bx - Ax);
  Py = Ay + r * (By - Ay);
  //   cerr << "intersecting at Px:" << Px << " Py:" << Py << endl;

  return (r >= 0.0 && r <= 1.0 && s >= 0.0 && s <= 1.0);
}

// // check if x,y inside polygon defined by xp,yp
// int pnpoly(int npol, float *xp, float *yp, float x, float y)
// {
//   int i, j, c = 0;
//   for (i = 0, j = npol-1; i < npol; j = i++) {
//     if ((((yp[i]<=y) && (y<yp[j])) ||
//   ((yp[j]<=y) && (y<yp[i]))) &&
//  (x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))

//       c = !c;
//   }
//   return c;
// }


void SvgDocument::Write_Header()
{
  // Output the SVG file header...

  int plot_x1 = viewport[0];
  int plot_x2 = viewport[0] + viewport[2];
  int plot_y1 = viewport[1];
  int plot_y2 = viewport[1] + viewport[3];

  *outfile << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
      << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";

  *outfile << "<svg " << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
      << "width=\"25cm\" height=\"20cm\" viewBox=\"" << plot_x1 << " "
      << plot_y1 << " " << plot_x2 << " " << plot_y2 << "\" "
      << "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";

  if (mancom.count("COMMENT") > 0)
    *outfile << mancom["COMMENT"];

}

string SvgDocument::svg_dash(unsigned int pattern, int repeat)
{
  ostringstream ss;
  vector<int> lens;
  // linetype bits and bitmask
  const int numbits = 16;
  const unsigned int bmask[numbits] =
    { 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2,
        1 };

  int num = 0, i, n;
  bool on, oldon, first = true, fblank = false;
  for (int j = 0; j < numbits; j++) {
    on = (pattern & bmask[j]);
    num++;
    // if change in bitmask
    if (j > 0 && oldon != on) {
      lens.push_back(num - 1);
      num = 1;
      if (first && !on)
        fblank = true;
      first = false;
    }
    oldon = on;
  }
  lens.push_back(num);
  n = lens.size();

  // One entry means solid line
  if (n == 1) {
    return "none";
  }

  // if first segment blank...set offset
  if (fblank) {
    lens.erase(lens.begin(), lens.begin());
    n--;
  }

  // check if all lengths equal
  num = lens[0];
  for (i = 0; i < n && lens[i] == num; i++)
    ;
  // if all equal...reduce to one
  if (i == n) {
    lens.erase(lens.begin() + 1, lens.end());
    n = 1;
  }

  for (i = 0; i < n; i++) {
    ss << repeat * lens[i] << " ";
  }

  return ss.str();
}

string SvgDocument::svg_line_properties(GLPprimitive* prim)
{
  ostringstream ss;
  if (!prim)
    return "";

  // set line-dash
  std::string dashpattern = "none";
  if (prim->state.linedash) {
    dashpattern = svg_dash(prim->state.dashpattern, prim->state.dashrepeat);
  }
  ss << "stroke-dasharray=\"" << dashpattern << "\" ";

  // set linewidth
  ss << "stroke-width=\"" << prim->state.linewidth << "\" ";

  // set opacity
  ss << "stroke-opacity=\"" << prim->verts[0].rgba[3] << "\" ";

  return ss.str();
}

string SvgDocument::svg_fill_properties(GLPprimitive* prim)
{
  ostringstream ss;
  if (!prim)
    return "";

  // set opacity
  ss << "fill-opacity=\"" << prim->verts[0].rgba[3] << "\" ";

  return ss.str();
}

string SvgDocument::svg_image(const GLPimage* image, float pheight)
{
  ostringstream ss;
  int width;
  int height;
  int ncomp = 3;
  float sx, sy;
  int truencomp = (image->format == GL_RGB ? 3 : 4);
  unsigned char* data = (unsigned char*) image->pixels;

  width = image->ix2 - image->ix1 + 1;
  height = image->iy2 - image->iy1 + 1;
  sx = width * image->sx;
  sy = height * image->sy;

  ss << "<!-- draw bitmap at " << image->x << " " << image->y
      << " Size (pixels):" << width << "x" << height << " Scaled to:" << sx
      << "x" << sy << " -->\n";

  //TODO: add support for images

  return ss.str();

  //<image x="200" y="200" width="100px" height="100px" xlink:href="myimage.png">

  //  <g style="opacity:0.2" id="g108">
  //
  //     <image
  //     xlink:href="data:;base64,iVBORw0KGgoAAAANSUhEUgAAABcAAAAUCAYAAABmvqYOAAAACXBIWXMAAAsSAAALEgHS3X78AAAA BGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAE0SURB VHjaYmQweaPPwMAwgYH64ANAADGCKZM354GkAZUNbwAIICYoo4DKBj8AhQZAAEEMPyNyEEguoKar gWZ+BAggRjjX5I08kLwAxAIUGnwAaLAjiAEQQExwoTMiD6kUsQ0wBkAAMaFJTICGF7lgATSIwQAg gFANB4YTBZH7AV0vQAAxYSg5I7IRHG6kgwlQx8EBQAAxYlUGyVgXSEp6Z0QU0QUBAogJq9IzIhdJ TJpYgxIggBhxKjd5ww+NXAFikx46AAggJpxaIOFHTNJMwCUBEECMBLWavLkPJBXwRGIhLq0AAcRE gcs+IGcYbAAggAgbDskUB3CVH/i0AgQQI1FpAVLuPCCU9NABQAAxEWU4ZrmTQIw2gAAiHoGSpsmb 90C8nlgtAAFEqgXx0CAiCgAEGABcb0VT9kFCGQAAAABJRU5ErkJggg=="
  //     width="22"
  //     height="19"
  //     transform="translate(75.81100,123.4932)"
  //     id="image110" />
  //  </g>

  ss << "<image " << "x=\"" << image->x << "\" " << "y=\"" << pheight
      - image->y - sy << "\" " << "width=\"" << width << "\" " << "height=\""
      << height << "\" " << "xlink:href=\"data:";

  for (int j = image->iy1; j <= image->iy2; j++) {
    int numchars = 0;
    for (int i = image->ix1; i <= image->ix2; i++) {
      for (int k = 0; k < ncomp; k++) {
        ss << hex << setw(2) << setfill('0') << int(data[j * truencomp
            * image->w + i * truencomp + k]);
      }
      // max no. of characters per line = 255
      numchars += ncomp * 2;
      if (numchars >= 254 - ncomp * 2) {
        numchars = 0;
        //ss << "\n";
      }
    }
    //ss << "\n";
  }

  ss << "\" " << "/>\n";

  return ss.str();

}

string SvgDocument::svg_masked_triangle(GLPprimitive* prim, GLint vp[4])
{
  ostringstream ss;

  ss << "<!-- Stippling-patterened triangle not supported yet -->\n";

  return ss.str();

  // TODO: add support for patterned triangle

  //  <pattern id="pattern2" x="0" y="0" width="20" height="20"
  //        patternUnits="userSpaceOnUse">
  //     <rect x="0" y="0" height="10" width="10"
  //            style="fill: blue; stroke:black;" />
  //   </pattern>

}

string SvgDocument::svg_pattern_triangle(GLPprimitive* prim, int numlines,
    float stipple[4], float deltax, float deltay)
{
  ostringstream ss;
  return ss.str();

  // generate polygon-stipple-pattern

  bool res;
  float Px[3], Py[3];
  float r, s;
  int v;
  int numsegs;

  for (int k = 0; k < numlines; k++) {
    stipple[0] += deltax;
    stipple[2] += deltax;
    stipple[1] += deltay;
    stipple[3] += deltay;
    numsegs = 0;
    // find possible intersections between stipple-line
    // and the three triangle line-segments
    for (int i = 0; i < 3; i++) {
      v = (i < 2 ? i + 1 : 0);
      res = intersection(stipple[0], stipple[1], stipple[2], stipple[3],
          prim->verts[i].xyz[0], prim->verts[i].xyz[1], prim->verts[v].xyz[0],
          prim->verts[v].xyz[1], r, s, Px[i], Py[i]);
      if (res)
        numsegs++;
    }
    // should find at least 2 intersections
    if (numsegs < 2)
      continue;

    // draw a line between the two intersection points
//    ss << Px[seg[0]] << " " << Py[seg[0]] << " " << Px[seg[1]] << " "
//        << Py[seg[1]] << " " << prim->verts[0].rgba[0] << " "
//        << prim->verts[0].rgba[1] << " " << prim->verts[0].rgba[2] << " FL\n";
  }

  return ss.str();
}

// SVG-code for triangles
// 1) Simple filled triangle
// 2) Colour-shaded triangle
// 3) Triangle with stipple-pattern

string SvgDocument::svg_triangle(GLPprimitive* prim, float height, GLint vp[4])
{
  ostringstream ss;

  if (!prim)
    return ss.str();

  ss << "<polygon "
      << "fill=\"" << svg_colour(prim) << "\" " << svg_fill_properties(prim)
      << "stroke=\"" << svg_colour(prim) << "\" " << svg_line_properties(prim)
      << "points=\"" << prim->verts[2].xyz[0] << "," << height
      - prim->verts[2].xyz[1] << " " << prim->verts[1].xyz[0] << ","
      << height - prim->verts[1].xyz[1] << " " << prim->verts[0].xyz[0]
      << "," << height - prim->verts[0].xyz[1] << " "
      << "\" />";

  return ss.str();

  if (!prim->state.polstiple) {
    // shaded or non-shaded filled

    if (prim->state.shade) {// Colour-shaded triangle
    //      ss << prim->verts[2].xyz[0] << " " << prim->verts[2].xyz[1] << " "
    //          << prim->verts[2].rgba[0] << " " << prim->verts[2].rgba[1] << " "
    //          << prim->verts[2].rgba[2] << " " << prim->verts[1].xyz[0] << " "
    //          << prim->verts[1].xyz[1] << " " << prim->verts[1].rgba[0] << " "
    //          << prim->verts[1].rgba[1] << " " << prim->verts[1].rgba[2] << " "
    //          << prim->verts[0].xyz[0] << " " << prim->verts[0].xyz[1] << " "
    //          << prim->verts[0].rgba[0] << " " << prim->verts[0].rgba[1] << " "
    //          << prim->verts[0].rgba[2] << " ST\n";


    } else {// Simple filled triangle

      ss << "<polygon "
          << "fill=\"" << svg_colour(prim) << "\" " << svg_fill_properties(prim)
          << "stroke=\"" << "none" << "\" " << svg_line_properties(prim)
          << "points=\"" << prim->verts[2].xyz[0] << "," << height
          - prim->verts[2].xyz[1] << " " << prim->verts[1].xyz[0] << ","
          << height - prim->verts[1].xyz[1] << " " << prim->verts[0].xyz[0]
          << "," << height - prim->verts[0].xyz[1] << " "
          << "\" />";

    }

  } else {

    if (usefakestipple) {
      ss << "<!-- Triangle with fake polygon-stipple-pattern not supported!! -->\n";
      return ss.str();

    } else {
      // use proper OpenGL mask as stippling pattern
      ss << svg_masked_triangle(prim, vp);
    }
  }

  return ss.str();
}

// Set colour (for flat shading)

string SvgDocument::svg_colour(GLPprimitive* prim)
{
  ostringstream ss;

  if (!prim)
    return "rgb(0,0,0)";

  ss << "rgb(" << int(prim->verts[0].rgba[0] * 255) << "," << int(
      prim->verts[0].rgba[1] * 255) << "," << int(prim->verts[0].rgba[2] * 255)
      << ")";

  return ss.str();
}

// Add clippath
// TODO: add support for clippath
string SvgDocument::svg_clippath(GLPclippath& gcp)
{
  if (gcp.v.size() < 3)
    return "<!-- ERROR. clippath requested with less than 3 points.. -->\n";
  ostringstream ss;

  if (gcp.rectangle) { // rectangle region, use rectclip-operator
    ss << "<!-- Rectangular clipping not supported yet -->\n";
    //ss << "gsave " << x << " " << y << " " << width << " " << height
    //    << " rectclip\n";

  } else { // arbitrary polygon, use clip-operator
    ss << "<!-- Add new clippath from Polygon not supported yet -->\n";

    //    ss << "gsave newpath \n";
    //    ss << gcp.v[0].x << " " << gcp.v[0].y << " moveto\n";
    //    for (int i = 1; i < gcp.v.size(); i++) {
    //      ss << gcp.v[i].x << " " << gcp.v[i].y << " lineto\n";
    //    }
    //    ss << "closepath clip\n";
    ss << "<!-- Clippath-definition finished -->\n";
  }
  clipstack++;
  return ss.str();
}

// Remove a clippath
// TODO: add support for clippath

string SvgDocument::svg_remove_clippath()
{
  if (clipstack <= 0)
    return "";
  clipstack--;
  ostringstream ss;

  ss << "<!-- REMOVE previously set clippath not supported yet -->\n";
  //ss << "grestore \n";
  ss << "<!-- Clippath-removal finished -->\n";

  return ss.str();
}

// Start new polyline

string SvgDocument::svg_startpolyline(GLPprimitive* prim)
{
  ostringstream ss;

  if (!prim)
    return "";

  ss << "<polyline " << "fill=\"" << "none" << "\" " << "stroke=\""
      << svg_colour(prim) << "\" " << svg_line_properties(prim) << "points=\"";

  inaline = true;

  return ss.str();
}

// End polyline

string SvgDocument::svg_endpolyline()
{
  if (!inaline)
    return "";

  inaline = false;

  // finish off previous line
  return "\" />\n";
}

//
// 'EndPage' function; this does nothing except parse the bounding boxes
// and output any remaining primitives.  It then frees the bounding box
// and primitive lists...
//

int SvgDocument::EndPage()
{
  GLPbbox *bbox; // Current bounding box
  GLPprimitive *prim; // Current primitive
  int i; // Color index background...
  int imagenum = 0;

  inaline = false;
  GLfloat prevx, prevy;
  clipstack = 0; // reset clipping stack counter

#ifdef DEBUG
  cout << "EndPage()\n" << flush;
#endif /* DEBUG */

  // write header to file (only first page)
  if (page_count == 0)
    Write_Header();

  // Stop doing feedback...
  UpdatePage(GL_FALSE);

  // Return an error if there was no feedback data used...

  if (bboxes == NULL)
    return (GLP_NO_FEEDBACK);

  Reset();

  // Loop through all bounding boxes and primitives...

#ifdef DEBUG
  cout << "EndPage() - writing page.\n" << flush;
#endif /* DEBUG */

  int plot_x1 = viewport[0];
  int plot_y1 = viewport[1];
  int plot_width = viewport[2];
  int plot_height = viewport[3];

  page_count++;

  if (mancom.count("PAGE") > 0)
    *outfile << mancom["PAGE"];

  // init background to white
  b_rgba[0] = b_rgba[1] = b_rgba[2] = 1.0;

  if (options & GLP_DRAW_BACKGROUND && !(options & GLP_BLACKWHITE)) {
    if (feedmode == GL_RGBA || colorsize == 0)
      glGetFloatv(GL_COLOR_CLEAR_VALUE, b_rgba);
    else {
      glGetIntegerv(GL_INDEX_CLEAR_VALUE, &i);
      b_rgba[0] = colormap[i][0];
      b_rgba[1] = colormap[i][1];
      b_rgba[2] = colormap[i][2];
    };

    *outfile << "<!--  Draw background... -->\n";
    *outfile << "<rect x=\"" << plot_x1 << "\" y=\"" << plot_y1 << "\" "
        << "width=\"" << plot_width << "\" " << "height=\"" << plot_height
        << "\" " << "fill=\"rgb(" << b_rgba[0] * 255 << "," << b_rgba[1] * 255
        << "," << b_rgba[2] * 255 << ")\" " << "stroke=\"black\"" << "/>";
    //prim->verts[0].rgba[0]
    *outfile << "\n";
  };

  *outfile << "<!-- Start primitives... -->\n";

  for (bbox = bboxes; bbox != NULL; bbox = bbox->next) {

    for (prim = bbox->primitives; prim != NULL; prim = prim->next) {

      switch (prim->num_verts) {
      case 0: /* Custom */
        *outfile << svg_endpolyline();
        if (prim->primtype == GL_CUSTOM_TOKEN) {
          if (customcoms.size() > 0) {
            string comm = customcoms[0];
            customcoms.pop_front();
            *outfile << comm;
          }
        } else if (prim->primtype == GL_RESET_TOKEN) {
          Reset();
        }
        break;

      case 1: /* Point */
        *outfile << svg_endpolyline();

        if (prim->primtype == GL_POINT_TOKEN) {
          // draw one point
          //          *outfile << prim->verts[0].xyz[0] << " " << prim->verts[0].xyz[1]
          //              << " " << prim->state.pointsize * pointscale << " " << " P\n";

        } else if (prim->primtype == GL_DRAW_PIXEL_TOKEN) {
          /* NA */

        } else if (prim->primtype == GL_IMAGE_TOKEN) {
          if (imagenum < nimages) {
            *outfile << svg_image(&(images[imagenum]), plot_height);
            imagenum++;
          } else {
            cerr << "---- Illegal imagenum!:" << imagenum << endl;
          }

        } else if (prim->primtype == GL_TEXT_TOKEN) {
          if (textlist.size() > 0) {
            GLPtext gpt = textlist[0];
            textlist.pop_front();
            // write text
            *outfile << "<text " << "x=\"" << gpt.x << "\" " << "y=\""
                << plot_height - gpt.y << "\" " << "rotate=\"" << gpt.angle
                << "\" " << "fill=\"" << svg_colour(prim) << "\" "
                << "text-anchor=\"" << "left" << "\" " << "font-family=\""
                << gpt.font << "\" " << "font-size=\"" << gpt.size << "\" "
                << ">\n";

            *outfile << gpt.s << "\n";

            *outfile << "</text>\n";
          }

        } else if (prim->primtype == GL_RCLIP_TOKEN) {
          *outfile << svg_remove_clippath();

        } else if (prim->primtype == GL_CLIP_TOKEN) {
          if (cliplist.size() > 0) {
            GLPclippath gcp = cliplist[0];
            cliplist.pop_front();

            *outfile << svg_clippath(gcp);
          }
        }
        break;

      case 2: /* Line */

        // check if this is part of previous line
        // by comparing vertices and colour/line-properties
        if (prevx != prim->verts[0].xyz[0] || prevy != prim->verts[0].xyz[1]) {
          *outfile << svg_endpolyline();
        }
        if (!inaline) {
          *outfile << svg_startpolyline(prim);

          // first point
          *outfile << prim->verts[0].xyz[0] << "," << plot_height
              - prim->verts[0].xyz[1] << " ";
        }
        // draw new line-segment
        *outfile << prim->verts[1].xyz[0] << "," << plot_height
            - prim->verts[1].xyz[1] << " ";

        // remember last vertex in segment
        prevx = prim->verts[1].xyz[0];
        prevy = prim->verts[1].xyz[1];

        break;

      case 3: /* Triangle */
        *outfile << svg_endpolyline();
        *outfile << svg_triangle(prim, plot_height, viewport);
        break;
      };
    };
  };

  *outfile << svg_endpolyline();

  *outfile << "</svg>\n";
  *outfile << flush;

  // Delete everything from the bounding box and primitive lists...

  delete_all();

#ifdef DEBUG
  cout << "EndPage() - done.\n" << flush;
#endif /* DEBUG */

  return (GLP_SUCCESS);
}

void SvgDocument::Reset()
{
}

//
// End of "$Id: file.cc 3372 2010-09-17 11:43:18Z audunc $".
//
