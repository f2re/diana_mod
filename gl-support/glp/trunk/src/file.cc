//
// "$Id: file.cc 4306 2013-12-17 12:55:57Z juergens $"
//
//   GLPfile class function for the GLP library, an OpenGL printing toolkit.
//
//   The GLP library is distributed under the terms of the GNU Library
//   General Public License which is described in the file "COPYING.LIB".
//   If you use this library in your program, please include a line reading
//   "OpenGL Printing Toolkit by Michael Sweet" in your version or copyright
//   output.
//
// Contents:
//
//   GLPfile, ~GLPfile, EndPage
//
// Revision History:
//
//   $Log$
//   Revision 1.3  2005/12/12 14:10:03  audunc
//   make eps-output more Adobe-friendly with new location for %%BoundingBox
//
//   Revision 1.2  2005/02/21 10:59:15  audunc
//   Added support for resetting state variables for multiple plots per page
//
//   Revision 1.1  2004/09/08 12:45:16  juergens
//   *** empty log message ***
//
//   Revision 1.2  1996/07/13  12:52:02  mike
//   Fixed shaded triangle procedure in PostScript prolog (ST).
//   Added auto-rotation for GLP_FIT_TO_PAGE option.
//
//   Revision 1.1  1996/06/27  03:06:36  mike
//   Initial revision
//

//
// Include necessary headers.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glpfile.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

//
// GLPfile constructor; this really doesn't do much, but instead you are
// expected to steal this code as the basis of your own GLP subclass...
//

GLPfile::GLPfile(char *print_name, // I - Name of file/printer
    int print_options, // I - Output options
    int print_size, // I - Size of feedback buffer
    map<string, string> *mc, // manual commands
    bool makeepsf) // make EPS file
:
  epsf(makeepsf)
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
  linescale = 0.5;
  pointscale = 0.5;
  currlw = -1;
  dashinuse = true;

  usefakestipple = false;
  fakepattern = GLPstip_diagleft;
  stipplescale = 1.0;

#ifdef DEBUG
  cout << "feedsize = " << feedsize << ", feedback = " << (long int)feedback << "\n" << flush;
#endif /* DEBUG */

  // Next open the indicated file...

  // The mode in the original call didn't work -- removed by LBS 1999-10-25
  //   outfile = new fstream(print_name, ios::out, 0666);
  outfile = new fstream(print_name, ios::out);
  if (outfile == NULL)
    return;

}

//
// GLPfile destructor; like the constructor, this just does the basics.
// You'll want to implement your own...
//

GLPfile::~GLPfile()
{
  // Free any memory we've allocated...

  // adc - linux, GLPcontext destructor is virtual
  //   delete_all();
  //   if (feedback != NULL)
  //     delete feedback;
  //   if (colormap != NULL)
  //     delete colormap;

  *outfile << "%%Pages: " << page_count << "\n";

  if (mancom.count("TRAILER") > 0)
    *outfile << mancom["TRAILER"];

  *outfile << "%%EOF\n";
  delete outfile;
}

void GLPfile::Write_Header()
{
  // Output the PostScript file header...

  int plot_x1 = viewport[0];
  int plot_x2 = viewport[0] + viewport[2];
  int plot_y1 = viewport[1];
  int plot_y2 = viewport[1] + viewport[3];

  *outfile << (epsf ? "%!PS-Adobe-3.0 EPSF-1.2\n" : "%!PS-Adobe-3.0\n");
  *outfile << "%%LanguageLevel: 2\n";
  *outfile << "%%Creator: GLP 2.0 by Michael Sweet, Audun Christoffersen\n";
  *outfile << "%%Pages: (atend)\n";

  if (epsf) {
    *outfile << "%%BoundingBox: " << plot_x1 << " " << plot_y1 << " "
        << plot_x2 << " " << plot_y2 << "\n";
  }

  if (mancom.count("COMMENT") > 0)
    *outfile << mancom["COMMENT"];

  *outfile << "%%EndComments\n";

}

void GLPfile::Write_Prolog()
{
  // Output the PostScript file prolog...
  *outfile << "%%BeginProlog\n";

  if (mancom.count("PROLOG") > 0)
    *outfile << mancom["PROLOG"];

  if (options & GLP_GREYSCALE) {
    *outfile << "% Greyscale color command - r g b C\n";
    if (options & GLP_REVERSE)
      *outfile
          << "/C { 0.0820 mul exch 0.6094 mul add exch 0.3086 mul add neg 1.0 add setgray } bind def\n";
    else
      *outfile
          << "/C { 0.0820 mul exch 0.6094 mul add exch 0.3086 mul add setgray } bind def\n";
  } else if (options & GLP_BLACKWHITE) {
    *outfile << "% Black-white color command - r g b C\n";
    *outfile << "/C { pop pop pop 0.0 setgray } bind def\n";
  } else {
    *outfile << "% RGB color command - r g b C\n";
    *outfile << "/C { setrgbcolor } bind def\n";
  };
  *outfile << "\n";

  //   *outfile << "% Point primitive - x y radius r g b P\n";
  //   *outfile << "/P { C newpath 0.0 360.0 arc closepath fill } bind def\n";
  //   *outfile << "\n";

  *outfile << "% Point primitive - x y radius P\n";
  *outfile << "/P { newpath 0.0 360.0 arc closepath fill } bind def\n";
  *outfile << "\n";

  //   *outfile << "% Flat-shaded line - x2 y2 x1 y1 r g b L\n";
  //   *outfile << "/L { C moveto lineto stroke } bind def\n";
  //   *outfile << "\n";
  //   *outfile << "% Flat-shaded line - x2 y2 x1 y1 FL\n";
  //   *outfile << "/FL { moveto lineto stroke } bind def\n";
  //   *outfile << "\n";
  *outfile << "% Flat-shaded line - x2 y2 x1 y1 r g b FL\n";
  *outfile << "/FL { C moveto lineto stroke } bind def\n";
  *outfile << "\n";

  *outfile << "% moveto - x1 y1 M\n";
  *outfile << "/M { moveto } bind def\n";
  *outfile << "\n";

  *outfile << "% lineto - x1 y1 L\n";
  *outfile << "/L { lineto } bind def\n";
  *outfile << "\n";

  *outfile << "% stroke - S\n";
  *outfile << "/S { stroke } bind def\n";
  *outfile << "\n";

  *outfile << "% Smooth-shaded line - x2 y2 r2 g2 b2 x1 y1 r1 g1 b1 SL\n";
  *outfile << "/SL {\n";
  *outfile << "	/b1 exch def\n";
  *outfile << "	/g1 exch def\n";
  *outfile << "	/r1 exch def\n";
  *outfile << "	/y1 exch def\n";
  *outfile << "	/x1 exch def\n";
  *outfile << "	/b2 exch def\n";
  *outfile << "	/g2 exch def\n";
  *outfile << "	/r2 exch def\n";
  *outfile << "	/y2 exch def\n";
  *outfile << "	/x2 exch def\n";
  *outfile << "\n";
  *outfile << "	b2 b1 sub abs 0.01 gt\n";
  *outfile << "	g2 g1 sub abs 0.005 gt\n";
  *outfile << "	r2 r1 sub abs 0.008 gt\n";
  *outfile << "     or or {\n";
  *outfile << "		/bm b1 b2 add 0.5 mul def\n";
  *outfile << "		/gm g1 g2 add 0.5 mul def\n";
  *outfile << "		/rm r1 r2 add 0.5 mul def\n";
  *outfile << "		/ym y1 y2 add 0.5 mul def\n";
  *outfile << "		/xm x1 x2 add 0.5 mul def\n";
  *outfile << "\n";
  *outfile << "		x1 y1 r1 g1 b1 xm ym rm gm bm SL\n";
  *outfile << "		xm ym rm gm bm x2 y2 r2 g2 b2 SL\n";
  *outfile << "	} {\n";
  *outfile << "		x1 y1 x2 y2 r1 g1 b1 FL\n";
  *outfile << "	} ifelse\n";
  *outfile << "} bind def\n";
  *outfile << "\n";

  *outfile
      << "% Flat-shaded triangle with colour - x3 y3 x2 y2 x1 y1 r g b TC\n";
  *outfile
      << "/TC { C newpath moveto lineto lineto closepath fill } bind def\n";
  *outfile << "\n";

  *outfile << "% Flat-shaded triangle - x3 y3 x2 y2 x1 y1 T\n";
  *outfile << "/T { newpath moveto lineto lineto closepath fill } bind def\n";
  *outfile << "\n";

  *outfile
      << "% Smooth-shaded triangle - x3 y3 r3 g3 b3 x2 y2 r2 g2 b2 x1 y1 r1 g1 b1 ST\n";
  *outfile << "/ST {\n";
  *outfile << "	/b1 exch def\n";
  *outfile << "	/g1 exch def\n";
  *outfile << "	/r1 exch def\n";
  *outfile << "	/y1 exch def\n";
  *outfile << "	/x1 exch def\n";
  *outfile << "	/b2 exch def\n";
  *outfile << "	/g2 exch def\n";
  *outfile << "	/r2 exch def\n";
  *outfile << "	/y2 exch def\n";
  *outfile << "	/x2 exch def\n";
  *outfile << "	/b3 exch def\n";
  *outfile << "	/g3 exch def\n";
  *outfile << "	/r3 exch def\n";
  *outfile << "	/y3 exch def\n";
  *outfile << "	/x3 exch def\n";
  *outfile << "\n";
  *outfile << "	b2 b1 sub abs 0.05 gt\n";
  *outfile << "	g2 g1 sub abs 0.017 gt\n";
  *outfile << "	r2 r1 sub abs 0.032 gt\n";
  *outfile << "	b3 b1 sub abs 0.05 gt\n";
  *outfile << "	g3 g1 sub abs 0.017 gt\n";
  *outfile << "	r3 r1 sub abs 0.032 gt\n";
  *outfile << "	b2 b3 sub abs 0.05 gt\n";
  *outfile << "	g2 g3 sub abs 0.017 gt\n";
  *outfile << "	r2 r3 sub abs 0.032 gt\n";
  *outfile << "	or or or or or or or or {\n";
  *outfile << "		/b12 b1 b2 add 0.5 mul def\n";
  *outfile << "		/g12 g1 g2 add 0.5 mul def\n";
  *outfile << "		/r12 r1 r2 add 0.5 mul def\n";
  *outfile << "		/y12 y1 y2 add 0.5 mul def\n";
  *outfile << "		/x12 x1 x2 add 0.5 mul def\n";
  *outfile << "\n";
  *outfile << "		/b13 b1 b3 add 0.5 mul def\n";
  *outfile << "		/g13 g1 g3 add 0.5 mul def\n";
  *outfile << "		/r13 r1 r3 add 0.5 mul def\n";
  *outfile << "		/y13 y1 y3 add 0.5 mul def\n";
  *outfile << "		/x13 x1 x3 add 0.5 mul def\n";
  *outfile << "\n";
  *outfile << "		/b32 b3 b2 add 0.5 mul def\n";
  *outfile << "		/g32 g3 g2 add 0.5 mul def\n";
  *outfile << "		/r32 r3 r2 add 0.5 mul def\n";
  *outfile << "		/y32 y3 y2 add 0.5 mul def\n";
  *outfile << "		/x32 x3 x2 add 0.5 mul def\n";
  *outfile << "\n";
  *outfile << "		x1 y1 r1 g1 b1 x12 y12 r12 g12 b12 x13 y13 r13 g13 b13\n";
  *outfile << "		x2 y2 r2 g2 b2 x12 y12 r12 g12 b12 x32 y32 r32 g32 b32\n";
  *outfile << "		x3 y3 r3 g3 b3 x32 y32 r32 g32 b32 x13 y13 r13 g13 b13\n";
  *outfile << "		x32 y32 r32 g32 b32 x12 y12 r12 g12 b12 x13 y13 r13 g13 b13\n";
  *outfile << "		ST ST ST ST\n";
  *outfile << "	} {\n";
  *outfile << "		x1 y1 x2 y2 x3 y3 r1 g1 b1 TC\n";
  *outfile << "	} ifelse\n";
  *outfile << "} bind def\n";
  *outfile << "\n";

  //*outfile << "% Show text at position: (TEXT) x y MS\n";
  //*outfile << "/MS  { moveto show } bind def\n";
  *outfile
      << "% Show text at position x/y and rotated by angle a: -a (TEXT) a x y MS\n";
  *outfile << "/MS  { moveto rotate show rotate } bind def\n";
  *outfile << "\n";
  *outfile << "% Change font: scale fontname FC\n";
  *outfile << "/FC { findfont exch scalefont setfont } bind def\n";
  *outfile << "\n";

  // define all fonts used
  set<string>::iterator ssi = deffonts.begin();
  for (; ssi != deffonts.end(); ssi++)
    *outfile << ps_definefont(*ssi);

  *outfile << "\n";
  *outfile << "%%EndProlog\n";

}

string GLPfile::ps_definefont(string f)
{
  ostringstream ss;
  if (f.length() == 0)
    return "";

  ss << "% Font definition for:" << f << endl;
  ss << "% Change the encoding vector to ISOLatin1" << endl;
  ss << "/" << f << " findfont" << endl;
  ss << "dup length dict begin" << endl;
  ss << "{1 index /FID ne {def} {pop pop} ifelse} forall" << endl;
  ss << "/Encoding ISOLatin1Encoding def" << endl;
  ss << "currentdict" << endl;
  ss << "end" << endl;
  //ss << "%%  /Helvetica-ISOLatin1 exch definefont pop" << endl;
  ss << "/" << f << " exch definefont pop" << endl;
  ss << endl;

  return ss.str();
}

string GLPfile::ps_line_properties(GLPprimitive* prim)
{
  ostringstream ss;
  if (!prim)
    return "";

  // set line-dash
  if (prim->state.linedash) {
    if (!dashinuse || prim->state.dashpattern != curr_pattern
        || prim->state.dashrepeat != curr_repeat) {
      dashinuse = true;
      curr_pattern = prim->state.dashpattern;
      curr_repeat = prim->state.dashrepeat;
      //  ---- Set new dashpattern
      ss << ps_dash(curr_pattern, curr_repeat) << " setdash\n";
    }
  } else if (dashinuse) {
    dashinuse = false;
    // ---- Reset dashpattern
    ss << "[] 0 setdash\n";
  }
  // ---- Set linewidth
  float lw = prim->state.linewidth;
  if (int(lw * 100) != int(currlw * 100)) {
    ss << lw * linescale << " setlinewidth\n";
    currlw = lw;
  }

  return ss.str();
}

string GLPfile::ps_dash(unsigned int pattern, int repeat)
{
  ostringstream ss;
  vector<int> lens;
  // linetype bits and bitmask
  const int numbits = 16;
  const unsigned int bmask[numbits] =
    { 32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2,
        1 };

  int num = 0, i, n, offset = 0;
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
  if (n == 1)
    return "[]0 ";

  // if first segment blank...set offset
  if (fblank) {
    if (n > 2) {
      offset = lens[1] + lens[2] - lens[0];
    } else {
      offset = lens[1];
    }
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

  ss << "[";
  for (i = 0; i < n; i++)
    ss << repeat * lens[i] << " ";
  ss << "] " << repeat * offset << " ";

  return ss.str();
}

string GLPfile::ps_image(const GLPimage* image)
{
  ostringstream ss;
  int width;
  int height;
  int nbits = 8;
  int ncomp = 3;
  float sx, sy;
  int strsize;
  int truencomp = (image->format == GL_RGB ? 3 : 4);
  unsigned char* data = (unsigned char*) image->pixels;

  width = image->ix2 - image->ix1 + 1;
  height = image->iy2 - image->iy1 + 1;
  strsize = width * ncomp;
  sx = width * image->sx;
  sy = height * image->sy;

  ss << "% ---- draw bitmap at " << image->x << " " << image->y
      << " Size (pixels):" << width << "x" << height << " Scaled to:" << sx
      << "x" << sy << "\n";

  ss << "gsave\n";
  // image-data size
  ss << "/picstr_" << strsize << " " << strsize << " string def\n";
  // set rasterpos
  ss << image->x << " " << image->y << " translate\n";
  //   ss << image->x << " " << image->y << " moveto\n";
  // scaling
  ss << sx << " " << sy << " scale\n";
  // image width, height and bits/component
  ss << width << " " << height << " " << nbits << "\n";
  // matrix: map unit square to source
  ss << "[" << width << " 0 0 " << height << " 0 0 ]\n";
  // read image data
  ss << "{currentfile picstr_" << strsize << " readhexstring pop}\n";
  // single data source, number of components
  ss << "false " << ncomp << "\n";
  // the operator
  ss << "colorimage\n";

  // the datastring
  if (image->format == GL_RGB || !image->blend) { // no blending
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
          ss << "\n";
        }
      }
      ss << "\n";
    }

  } else { // blend to background

    //cerr << "BLENDING TO BACKGROUND" << endl;
    unsigned char rgba[4], brgb[3];
    // scale background colour to [0..255]
    brgb[0] = static_cast<unsigned char> (b_rgba[0] * 255);
    brgb[1] = static_cast<unsigned char> (b_rgba[1] * 255);
    brgb[2] = static_cast<unsigned char> (b_rgba[2] * 255);

    for (int j = image->iy1; j <= image->iy2; j++) {
      for (int i = image->ix1; i <= image->ix2; i++) {
        for (int k = 0; k < truencomp; k++) {
          rgba[k] = data[j * truencomp * image->w + i * truencomp + k];
        }
        if (rgba[3] < 255) { // do the blend
          for (int l = 0; l < ncomp; l++)
            rgba[l] = static_cast<unsigned char> (((255 - rgba[3]) / 255.0)
                * brgb[l] + (rgba[3] / 255.0) * rgba[l]);
        }
        ss << hex << setw(2) << setfill('0') << int(rgba[0]) << hex << setw(2)
            << setfill('0') << int(rgba[1]) << hex << setw(2) << setfill('0')
            << int(rgba[2]);
      }
      ss << "\n";
    }
  }
  ss << "grestore\n";
  ss << "% End of imagedata\n";

  return ss.str();
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
// 	 ((yp[j]<=y) && (y<yp[i]))) &&
// 	(x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))

//       c = !c;
//   }
//   return c;
// }


string GLPfile::ps_masked_triangle(GLPprimitive* prim, GLint vp[4])
{
  ostringstream ss;

  const int maskw = 32, maskh = 32;
  float sx = prim->verts[0].xyz[0];
  float sy = prim->verts[0].xyz[1];
  float sizex = maskw * prim->state.stipscale;
  float sizey = maskh * prim->state.stipscale;

  ss << "%- Start of stippling-patterened triangle\n";
  ss << "gsave\n";
  ss << "% Pattern dictionary\n";
  ss << "<<\n";
  ss << "/PaintType 2                 % uncolored pattern \n";
  ss
      << "/PatternType 1 /TilingType 1 % patterntype always 1/constant spacing\n";
  ss << "/BBox [0 0 " << sizex << " " << sizey
      << "]  % size of one pattern cell\n";
  ss << "/XStep " << sizex << " /YStep " << sizey
      << " % spacing between pattern cells\n";

  ss << "/PaintProc {\n";
  ss << "pop\n";
  ss << sizex << " " << sizey << " scale %- pattern scale\n";
  ss << maskw << " " << maskh << "       %- dimension of source mask\n";
  ss << "true         % paint the 1 bits\n";
  ss << "[" << maskw << " 0 0 " << maskh << " 0 0]"
      << "  %- transformation matrix\n";
  ss << "{<";
  for (int i = 0; i < 128; i++)
    ss << hex << setw(2) << setfill('0') << int(prim->state.stipmask[i]);
  ss << ">}   %- mask data\n";
  ss << "imagemask\n";
  ss << "}\n";

  ss << ">>\n";
  ss << "% make a pattern from dictionary, and name it\n";
  ss << "matrix\n";
  ss << "makepattern\n";
  ss << "/Pat exch def\n";
  ss << "% path from triangle, and fill it\n";
  ss << sx << " " << sy << " translate\n";
  ss << "0 0 moveto\n";
  ss << prim->verts[1].xyz[0] - sx << " " << prim->verts[1].xyz[1] - sy
      << " lineto\n";
  ss << prim->verts[2].xyz[0] - sx << " " << prim->verts[2].xyz[1] - sy
      << " lineto\n";
  ss << "closepath\n";

  ss << "[/Pattern [/DeviceRGB]] setcolorspace\n";
  ss << prim->verts[0].rgba[0] << " " << prim->verts[0].rgba[1] << " "
      << prim->verts[0].rgba[2] << " Pat setcolor fill\n";

  ss << "grestore\n";

  return ss.str();
}

string GLPfile::ps_pattern_triangle(GLPprimitive* prim, int numlines,
    float stipple[4], float deltax, float deltay)
{
  ostringstream ss;

  // generate polygon-stipple-pattern

  bool res, first;
  float Px[3], Py[3];
  float r, s;
  int v;
  int seg[2], numsegs;

  for (int k = 0; k < numlines; k++) {
    stipple[0] += deltax;
    stipple[2] += deltax;
    stipple[1] += deltay;
    stipple[3] += deltay;
    first = true;
    numsegs = 0;
    // find possible intersections between stipple-line
    // and the three triangle line-segments
    for (int i = 0; i < 3; i++) {
      v = (i < 2 ? i + 1 : 0);
      res = intersection(stipple[0], stipple[1], stipple[2], stipple[3],
          prim->verts[i].xyz[0], prim->verts[i].xyz[1], prim->verts[v].xyz[0],
          prim->verts[v].xyz[1], r, s, Px[i], Py[i]);
      if (res) {
        if (first)
          seg[0] = i;
        else
          seg[1] = i;
        first = false;
        numsegs++;
      }
    }
    // should find at least 2 intersections
    if (numsegs < 2)
      continue;

    // draw a line between the two intersection points
    ss << Px[seg[0]] << " " << Py[seg[0]] << " " << Px[seg[1]] << " "
        << Py[seg[1]] << " " << prim->verts[0].rgba[0] << " "
        << prim->verts[0].rgba[1] << " " << prim->verts[0].rgba[2] << " FL\n";
  }

  return ss.str();
}

// Postscript-code for triangles
// 1) Simple filled triangle
// 2) Colour-shaded triangle
// 3) Triangle with stipple-pattern

string GLPfile::ps_triangle(GLPprimitive* prim, GLint vp[4])
{
  ostringstream ss;

  if (!prim)
    return ss.str();

  if (!prim->state.polstiple) {
    // shaded or non-shaded filled

    if (prim->state.shade) {// Colour-shaded triangle
      ss << prim->verts[2].xyz[0] << " " << prim->verts[2].xyz[1] << " "
          << prim->verts[2].rgba[0] << " " << prim->verts[2].rgba[1] << " "
          << prim->verts[2].rgba[2] << " " << prim->verts[1].xyz[0] << " "
          << prim->verts[1].xyz[1] << " " << prim->verts[1].rgba[0] << " "
          << prim->verts[1].rgba[1] << " " << prim->verts[1].rgba[2] << " "
          << prim->verts[0].xyz[0] << " " << prim->verts[0].xyz[1] << " "
          << prim->verts[0].rgba[0] << " " << prim->verts[0].rgba[1] << " "
          << prim->verts[0].rgba[2] << " ST\n";

      curr_colinuse = false;

    } else {// Simple filled triangle
      ss << ps_colour(prim);
      ss << prim->verts[2].xyz[0] << " " << prim->verts[2].xyz[1] << " "
          << prim->verts[1].xyz[0] << " " << prim->verts[1].xyz[1] << " "
          << prim->verts[0].xyz[0] << " " << prim->verts[0].xyz[1] << " "
          << " T\n";
    }

  } else {

    curr_colinuse = false;

    if (usefakestipple) {
      ss << "% - Triangle with fake polygon-stipple-pattern\n";

      // set correct line-properties
      ss << ps_line_properties(prim);

      // generate fake polygon-stipple-pattern
      ss << "% ---- The stippling lines\n";

      float stipple[4];
      int numlines;
      float vw = vp[2] - vp[0];
      float vh = vp[3] - vp[1];
      float delta, deltax, deltay;

      switch (prim->state.stippat) {
      case GLPstip_horisontal:
        numlines = static_cast<int> (200 * prim->state.stipscale);
        delta = (vh) / numlines;
        deltax = 0;
        deltay = delta;
        stipple[0] = vp[0];
        stipple[1] = vp[1];
        stipple[2] = vp[2];
        stipple[3] = vp[1];
        ss << ps_pattern_triangle(prim, numlines, stipple, deltax, deltay);
        break;
      case GLPstip_vertical:
        numlines = static_cast<int> (200 * prim->state.stipscale);
        delta = (vw) / numlines;
        deltax = delta;
        deltay = 0;
        stipple[0] = vp[0];
        stipple[1] = vp[1];
        stipple[2] = vp[0];
        stipple[3] = vp[3];
        ss << ps_pattern_triangle(prim, numlines, stipple, deltax, deltay);
        break;
      case GLPstip_diagleft:
        numlines = static_cast<int> (250 * prim->state.stipscale);
        delta = (vw + vh) / numlines;
        deltax = 0;
        deltay = -(delta);
        stipple[0] = vp[0];
        stipple[1] = vp[3];
        stipple[2] = vp[2];
        stipple[3] = vp[3] + vw;
        ss << ps_pattern_triangle(prim, numlines, stipple, deltax, deltay);
        break;
      case GLPstip_diagright:
        numlines = static_cast<int> (250 * prim->state.stipscale);
        delta = (vw + vh) / numlines;
        deltax = 0;
        deltay = -(delta);
        stipple[0] = vp[0];
        stipple[1] = vp[3] + vw;
        stipple[2] = vp[2];
        stipple[3] = vp[3];
        ss << ps_pattern_triangle(prim, numlines, stipple, deltax, deltay);
        break;
      case GLPstip_diagcross:
        numlines = static_cast<int> (250 * prim->state.stipscale);
        delta = (vw + vh) / numlines;
        deltax = 0;
        deltay = -(delta);
        stipple[0] = vp[0];
        stipple[1] = vp[3];
        stipple[2] = vp[2];
        stipple[3] = vp[3] + vw;
        ss << ps_pattern_triangle(prim, numlines, stipple, deltax, deltay);
        stipple[0] = vp[0];
        stipple[1] = vp[3] + vw;
        stipple[2] = vp[2];
        stipple[3] = vp[3];
        ss << ps_pattern_triangle(prim, numlines, stipple, deltax, deltay);
        break;
      case GLPstip_none:
      default:
        break;
      };

    } else {
      // use proper OpenGL mask as stippling pattern
      ss << ps_masked_triangle(prim, vp);
    }
  }

  return ss.str();
}

// Set colour (for flat shading)

string GLPfile::ps_colour(GLPprimitive* prim)
{
  ostringstream ss;

  if (!prim)
    return "";

  if (!curr_colinuse || prim->verts[0].rgba[0] != curr_colour[0]
      || prim->verts[0].rgba[1] != curr_colour[1] || prim->verts[0].rgba[2]
      != curr_colour[2] || prim->verts[0].rgba[3] != curr_colour[3]) {

    ss << prim->verts[0].rgba[0] << " " << prim->verts[0].rgba[1] << " "
        << prim->verts[0].rgba[2] << " C\n";

    curr_colinuse = true;
    curr_colour[0] = prim->verts[0].rgba[0];
    curr_colour[1] = prim->verts[0].rgba[1];
    curr_colour[2] = prim->verts[0].rgba[2];
    curr_colour[3] = prim->verts[0].rgba[3];
  }

  return ss.str();
}

// Set new font

string GLPfile::ps_setfont(GLPtext& gpt)
{
  ostringstream ss;

  // Always set font! Fix for some HP printers
  // if (gpt.font != currfont || gpt.size != fontsize) {

  ss << gpt.size << " /" << gpt.font << " FC\n";

  currfont = gpt.font;
  fontsize = gpt.size;

  //  }

  return ss.str();
}

// Add clippath

string GLPfile::ps_clippath(GLPclippath& gcp)
{
  if (gcp.v.size() < 3)
    return "% ERROR. clippath requested with less than 3 points..\n";
  ostringstream ss;

  if (gcp.rectangle) { // rectangle region, use rectclip-operator
    float x = gcp.v[0].x;
    float y = gcp.v[0].y;
    float width = gcp.v[2].x - x;
    float height = gcp.v[2].y - y;
    ss << "% Rectangular clipping\n";
    ss << "gsave " << x << " " << y << " " << width << " " << height
        << " rectclip\n";

  } else { // arbitrary polygon, use clip-operator
    ss << "% Add new clippath from Polygon\n";
    ss << "gsave newpath \n";
    ss << gcp.v[0].x << " " << gcp.v[0].y << " moveto\n";
    for (int i = 1; i < gcp.v.size(); i++) {
      ss << gcp.v[i].x << " " << gcp.v[i].y << " lineto\n";
    }
    ss << "closepath clip\n";
    ss << "% Clippath-definition finished\n";
  }
  clipstack++;
  return ss.str();
}

/*
 To implement a more complex clippath, involving two or more subregions:
 % ADD a clippath with two subpaths
 gsave newpath
 95.26 8.25998 moveto
 252.2 165.2 lineto
 417.4 82.6 lineto
 417.4 247.8 lineto
 252.2 206.5 lineto
 95.26 330.4 lineto
 500 413 lineto
 500 8.25998 lineto
 95.26 8.25998 lineto
 % new subpath
 433.92 346.92 moveto   % note the moveto
 483.48 346.92 lineto
 483.48 396.48 lineto
 433.92 396.48 lineto
 closepath clip
 % Clippath-definition finished

 Note that the winding of the two regions decide if the second
 subregion should be included or cut out of the first...
 */

// Remove a clippath

string GLPfile::ps_remove_clippath()
{
  if (clipstack <= 0)
    return "";
  clipstack--;
  ostringstream ss;

  ss << "% REMOVE previously set clippath\n";
  ss << "grestore \n";
  ss << "% Clippath-removal finished\n";

  return ss.str();
}

//
// 'EndPage' function; this does nothing except parse the bounding boxes
// and output any remaining primitives.  It then frees the bounding box
// and primitive lists...
//

int GLPfile::EndPage()
{
  GLPbbox *bbox; // Current bounding box
  GLPprimitive *prim; // Current primitive
  int i; // Color index background...
  int imagenum = 0;

  bool inaline = false;
  GLfloat prevx, prevy;
  clipstack = 0; // reset clipping stack counter

#ifdef DEBUG
  cout << "EndPage()\n" << flush;
#endif /* DEBUG */

  // write header to file (only first page)
  if (page_count == 0)
    Write_Header();

  // write prolog to file (only first page)
  if (page_count == 0)
    Write_Prolog();

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
  int plot_x2 = viewport[0] + viewport[2];
  int plot_y1 = viewport[1];
  int plot_y2 = viewport[1] + viewport[3];
  int plot_width = viewport[2];
  int plot_height = viewport[3];

  page_count++;
  *outfile << "%%Page: " << page_count << " " << page_count << "\n";
  *outfile << "%%PageBoundingBox: " << plot_x1 << " " << plot_y1 << " "
      << plot_x2 << " " << plot_y2 << "\n";

  if (mancom.count("PAGE") > 0)
    *outfile << mancom["PAGE"];

  *outfile << "gsave\n";

  // fit plot to page (NOT if EPS)
  if (options & GLP_FIT_TO_PAGE && !epsf) {
    *outfile << "% Fit to page...\n";
    *outfile << "newpath clippath pathbbox\n";
    *outfile << "/URy exch def\n";
    *outfile << "/URx exch def\n";
    *outfile << "/LLy exch def\n";
    *outfile << "/LLx exch def\n";
    *outfile << "/Width  URx LLx sub 0.005 sub def\n";
    *outfile << "/Height URy LLy sub 0.005 sub def\n";
    *outfile << "LLx LLy translate\n";
    //       *outfile << "/XZoom Width " << viewport[2] << " div def\n";
    //       *outfile << "/YZoom Height " << viewport[3] << " div def\n";
    *outfile << "/XZoom Width " << plot_width << " div def\n";
    *outfile << "/YZoom Height " << plot_height << " div def\n";
    if (options & GLP_AUTO_ORIENT) {
      *outfile << "Width YZoom mul Height XZoom mul gt {\n";
      *outfile << "	90 rotate\n";
      *outfile << "	0 Width neg translate\n";
      *outfile << "	Width Height /Width exch def /Height exch def\n";
      // 	*outfile << "	/XZoom Width " << viewport[2] << " div def\n";
      // 	*outfile << "	/YZoom Height " << viewport[3] << " div def\n";
      *outfile << "	/XZoom Width " << plot_width << " div def\n";
      *outfile << "	/YZoom Height " << plot_height << " div def\n";
      *outfile << "} if\n";
    } else if (options & GLP_LANDSCAPE) {
      *outfile << "	90 rotate\n";
      *outfile << "	0 Width neg translate\n";
      *outfile << "	Width Height /Width exch def /Height exch def\n";
      // 	*outfile << "	/XZoom Width " << viewport[2] << " div def\n";
      // 	*outfile << "	/YZoom Height " << viewport[3] << " div def\n";
      *outfile << "	/XZoom Width " << plot_width << " div def\n";
      *outfile << "	/YZoom Height " << plot_height << " div def\n";
    }
    *outfile << "XZoom YZoom gt {\n";
    *outfile << "	/YSize Height def\n";
    *outfile << "	/XSize " << plot_width << " YZoom mul def\n";
    *outfile << "	/Scale YZoom def\n";
    *outfile << "} {\n";
    *outfile << "	/XSize Width def\n";
    *outfile << "	/YSize " << plot_height << " XZoom mul def\n";
    *outfile << "	/Scale XZoom def\n";
    *outfile << "} ifelse\n";

    *outfile << "Width  XSize sub 2 div\n";
    *outfile << "Height YSize sub 2 div translate\n";
    *outfile << "Scale Scale scale\n";
    *outfile << "\n";
  };

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

    *outfile << "% Draw background...\n";
    *outfile << b_rgba[0] << " " << b_rgba[1] << " " << b_rgba[2] << " C\n";
    *outfile << "newpath\n";
    *outfile << plot_x1 << "  " << plot_y1 << " moveto\n";
    *outfile << "	" << plot_x2 << " " << plot_y1 << " lineto\n";
    *outfile << "	" << plot_x2 << " " << plot_y2 << " lineto\n";
    *outfile << "	" << plot_x1 << " " << plot_y2 << " lineto\n";
    *outfile << "closepath fill\n";
    *outfile << "\n";
  };

  //   if (nimages>0)
  //     *outfile << "%-- Images first\n";
  //   for (int inum=0; inum<nimages; inum++){
  //     *outfile << ps_image(&(images[inum]));
  //   }


  *outfile << "% Start primitives...\n";

  // test reversing boundingboxes
  //   GLPbbox *lastbbox= bboxes; // last bounding box
  //   while (lastbbox->next != NULL)
  //     lastbbox= lastbbox->next;

  //   for (bbox = lastbbox; bbox != NULL; bbox = bbox->prev)

  for (bbox = bboxes; bbox != NULL; bbox = bbox->next) {
#ifdef DEBUG // bounding boxes
    cout << "EndPage() - writing bbox (" << bbox->min[0] << ","
    << bbox->min[1] << "," << bbox->min[2] << ") to (" << bbox->max[0] << ","
    << bbox->max[1] << "," << bbox->max[2] << ").\n" << flush;
#endif /* DEBUG */

    for (prim = bbox->primitives; prim != NULL; prim = prim->next) {
#ifdef DEBUG  // primitives
      cout << "EndPage() - primitive";
      for (i = 0; i < prim->num_verts; i ++)
      cout << " (" << prim->verts[i].xyz[0] << ", "
      << prim->verts[i].xyz[1] << ", "
      << prim->verts[i].xyz[2] << ")";
      cout << "\n";
#endif /* DEBUG */

      switch (prim->num_verts) {
      case 0: /* Custom */
        if (inaline) { // finish off previous line
          *outfile << "S\n";
          inaline = false;
        }
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
        if (inaline) { // finish off previous line
          *outfile << "S\n";
          inaline = false;
        }

        if (prim->primtype == GL_POINT_TOKEN) {
          // set colour
          *outfile << ps_colour(prim);
          // draw one point
          *outfile << prim->verts[0].xyz[0] << " " << prim->verts[0].xyz[1]
              << " " << prim->state.pointsize * pointscale << " " << " P\n";

        } else if (prim->primtype == GL_DRAW_PIXEL_TOKEN) {
          // 		if (imagenum<nimages){
          // 		  *outfile << ps_image(&(images[imagenum]));
          // 		  imagenum++;
          // 		} else {
          // 		  cerr << "---- Illegal imagenum!:" << imagenum << endl;
          // 		}

        } else if (prim->primtype == GL_IMAGE_TOKEN) {
          if (imagenum < nimages) {
            *outfile << ps_image(&(images[imagenum]));
            imagenum++;
          } else {
            cerr << "---- Illegal imagenum!:" << imagenum << endl;
          }

        } else if (prim->primtype == GL_TEXT_TOKEN) {
          if (textlist.size() > 0) {
            // set colour
            *outfile << ps_colour(prim);
            GLPtext gpt = textlist[0];
            textlist.pop_front();
            // set font
            *outfile << ps_setfont(gpt);
            // write text
            ostringstream ost;
            ost << -gpt.angle << " " << "(" << gpt.s << ") " << gpt.angle
                << " " << gpt.x << " " << gpt.y << " MS";
            *outfile << ost.str() << endl;
          }

        } else if (prim->primtype == GL_RCLIP_TOKEN) {
          *outfile << ps_remove_clippath();

        } else if (prim->primtype == GL_CLIP_TOKEN) {
          if (cliplist.size() > 0) {
            GLPclippath gcp = cliplist[0];
            cliplist.pop_front();

            *outfile << ps_clippath(gcp);
          }
        }
        break;

      case 2: /* Line */

        // NB NB NB NB NB NB NB NB NB NB NB NB
        // new Mesa sends only ONE GL_LINE_RESET_TOKEN
        // BRUTAL LINE SEGMENT CODE
        // 	      *outfile << ps_line_properties(prim);
        // 	      *outfile << ps_colour(prim);
        // 	      *outfile << prim->verts[1].xyz[0] << " "
        // 		       << prim->verts[1].xyz[1] << " "
        // 		       << prim->verts[0].xyz[0] << " "
        // 		       << prim->verts[0].xyz[1] << " "
        // 		       << " FL\n";
        // 	      break;
        // NB NB NB NB NB NB NB NB NB NB NB NB

        if (prim->state.shade) { // Shaded line
          // first set line properties
          string lines = ps_line_properties(prim);
          *outfile << lines;
          *outfile << prim->verts[1].xyz[0] << " " << prim->verts[1].xyz[1]
              << " " << prim->verts[1].rgba[0] << " " << prim->verts[1].rgba[1]
              << " " << prim->verts[1].rgba[2] << " " << prim->verts[0].xyz[0]
              << " " << prim->verts[0].xyz[1] << " " << prim->verts[0].rgba[0]
              << " " << prim->verts[0].rgba[1] << " " << prim->verts[0].rgba[2]
              << " SL\n";
          curr_colinuse = false;

        } else { // Flat-shaded line

          // first get line and colour properties
          string lines = ps_line_properties(prim);
          string colos = ps_colour(prim);

          // check if this is part of previous line
          // by comparing vertices and colour/line-properties
          if (lines.length() > 0 || colos.length() > 0 || prevx
              != prim->verts[0].xyz[0] || prevy != prim->verts[0].xyz[1]) {
            if (inaline) {
              *outfile << "S\n";
            }
            inaline = false;
          }
          if (!inaline) {
            inaline = true;
            // set line properties
            *outfile << lines;
            // set line colour
            *outfile << colos;
            // moveto first point
            *outfile << prim->verts[0].xyz[0] << " " << prim->verts[0].xyz[1]
                << " M ";
          }
          // draw new line-segment
          *outfile << prim->verts[1].xyz[0] << " " << prim->verts[1].xyz[1]
              << " L ";

          // remember last vertex in segment
          prevx = prim->verts[1].xyz[0];
          prevy = prim->verts[1].xyz[1];

        }
        break;

        // 	      if (prim->primtype==GL_LINE_RESET_TOKEN){
        // 		if (inaline){ // finish off previous line
        // 		  *outfile << "S\n";
        // 		  inaline= false;
        // 		}
        // 		*outfile << ps_line_properties(prim);

        // 		if (!prim->state.shade){
        // 		  inaline= true;
        // 		  // set line colour
        // 		  *outfile << ps_colour(prim);
        // 		  // moveto first point
        // 		  *outfile << prim->verts[0].xyz[0] << " "
        // 			   << prim->verts[0].xyz[1] << " M " ;
        // 		}
        // 	      }

        // 	      if (prim->state.shade)
        // 		{
        // 		  *outfile <<
        // 		    prim->verts[1].xyz[0] << " " <<
        // 		    prim->verts[1].xyz[1] << " " <<
        // 		    prim->verts[1].rgba[0] << " " <<
        // 		    prim->verts[1].rgba[1] << " " <<
        // 		    prim->verts[1].rgba[2] << " " <<
        // 		    prim->verts[0].xyz[0] << " " <<
        // 		    prim->verts[0].xyz[1] << " " <<
        // 		    prim->verts[0].rgba[0] << " " <<
        // 		    prim->verts[0].rgba[1] << " " <<
        // 		    prim->verts[0].rgba[2] << " SL\n";
        // 		  curr_colinuse= false;
        // 		}
        // 	      else
        // 		{

        // 		  if (inaline) {
        // 		    *outfile << prim->verts[1].xyz[0] << " "
        // 			     << prim->verts[1].xyz[1] << " L ";
        // 		  } else {
        // 		    // shouldn't really get here..
        // 		    *outfile << ps_line_properties(prim);
        // 		    *outfile << ps_colour(prim);
        // 		    *outfile << prim->verts[1].xyz[0] << " "
        // 			     << prim->verts[1].xyz[1] << " "
        // 			     << prim->verts[0].xyz[0] << " "
        // 			     << prim->verts[0].xyz[1] << " "
        // 			     << " FL\n";
        // 		  }
        // 		}
        // 	      break;

      case 3: /* Triangle */
        if (inaline) { // finish off previous line
          *outfile << "S\n";
          inaline = false;
        }

        *outfile << ps_triangle(prim, viewport);
        break;
      };
    };
  };

  if (inaline) { // finish off last line
    *outfile << "S\n";
    inaline = false;
  }

  *outfile << "grestore\n";
  *outfile << "showpage\n";
  *outfile << "%%EndPage\n";
  *outfile << flush;

  // Delete everything from the bounding box and primitive lists...

  delete_all();

#ifdef DEBUG
  cout << "EndPage() - done.\n" << flush;
#endif /* DEBUG */

  return (GLP_SUCCESS);
}

void GLPfile::Reset()
{
  currfont = "";
  fontsize = -10;
  curr_colinuse = false;
  currlw = -1;
}

//
// End of "$Id: file.cc 4306 2013-12-17 12:55:57Z juergens $".
//
