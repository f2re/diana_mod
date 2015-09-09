//
// "$Id: context.cc 4306 2013-12-17 12:55:57Z juergens $"
//
//   GLPcontext class functions for the GLP library, an OpenGL printing
//   toolkit.
//
//   The GLP library is distributed under the terms of the GNU Library
//   General Public License which is described in the file "COPYING.LIB".
//   If you use this library in your program, please include a line reading
//   "OpenGL Printing Toolkit by Michael Sweet" in your version or copyright
//   output.
//
// Contents:
//
//   ~GLPcontext, StartPage, UpdatePage, EndPage,
//   add_primitive, sort_primitive, get_vertex, delete_all
//
// Revision History:
//
//   $Log$
//   Revision 1.2  2005/02/21 10:59:15  audunc
//   Added support for resetting state variables for multiple plots per page
//
//   Revision 1.1  2004/09/08 12:45:16  juergens
//   *** empty log message ***
//
//   Revision 1.2  1996/07/13  12:52:02  mike
//   Fixed delete_all() - was not setting bboxes list pointer to NULL at end.
//
//   Revision 1.1  1996/06/27  00:58:11  mike
//   Initial revision
//

//
// Include necessary headers.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "GLP.h"

#include <cmath>
#include <cstring>
#include <iostream>

using namespace std;

//#define DEBUG


#define min(a,b)	((a) < (b) ? (a) : (b))
#define max(a,b)	((a) > (b) ? (a) : (b))

const float TEXT_TOKEN     = 1005;
const float IMAGE_GL_TOKEN = 1015;
const float IMAGE_P_TOKEN  = 1025;
const float CLIP_REM_TOKEN = 1995;
const float CLIP_TOKEN     = 2000;

//
// GLPcontext destructor; like the constructor, this just does the basics.
// You'll want to implement your own...
//

GLPcontext :: ~GLPcontext()
{
  // Free any memory we've allocated...

  delete_all();

  if (feedback != NULL)
    delete feedback;
  if (colormap != NULL)
    delete colormap;
}


//
// 'Reset' function; this should perform any necessary
// page-state variable initialisation/reset
//
//
void
GLPcontext :: Reset()
{
}



//
// 'StartPage' function for RGBA windows; this should perform any necessary
// output initialization (e.g. send 'start of page' commands, etc) and enable
// feedback mode...
//

int
GLPcontext :: StartPage(int mode)
{
  // Initialize feedback mode...

  feedmode = mode;

#ifdef DEBUG
  cout << "RGBA feedmode = " << mode << "\n" << flush;
#endif /* DEBUG */

  glGetIntegerv(GL_VIEWPORT, viewport);

  glFeedbackBuffer(feedsize, GL_3D_COLOR, feedback);
  glRenderMode(GL_FEEDBACK);

  // You'd put any other 'start page' things here...
  zval= 0.0;
  nimages= 0;
  textcounter= 0;
  imagecounter=0;
  clipcounter=0;

  return (0);
}


//
// 'StartPage' function for color index windows; this does the same thing as
// the RGBA start page function, and also copies the given colormap into our
// class colormap structure for later use...
//

int
GLPcontext :: StartPage(int     mode,
                        int     size,
                        GLPrgba *rgba)
{
  // Initialize feedback mode...

  feedmode = mode;

#ifdef DEBUG
  cout << "Index feedmode = " << mode << "\n" << flush;
#endif /* DEBUG */

  glGetIntegerv(GL_VIEWPORT, viewport);

  glFeedbackBuffer(feedsize, GL_3D_COLOR, feedback);
  glRenderMode(GL_FEEDBACK);

  // Copy the colormap over, removing the old one as necessary...

  if (colormap != NULL)
    delete colormap;

  colorsize = size;
  colormap  = new GLPrgba[size];
  memcpy(colormap, rgba, size * sizeof(GLPrgba));

  // You'd put any other 'start page' things here...
  zval= 0.0;
  nimages= 0;
  textcounter= 0;
  imagecounter=0;
  clipcounter=0;

  return (0);
}


// force OpenGL viewport
// These values are normally pulled from OpenGL context
// with 'glGetIntegerv(GL_VIEWPORT, viewport);' in StartPage...
// Use this function to force specific values
void
GLPcontext :: setViewport(int x, int y, int width, int height)
{
  if (width > 0 && height>0){
    viewport[0] = x;
    viewport[1] = y;
    viewport[2] = width;
    viewport[3] = height;
  }
}

//
// 'UpdatePage' function; this does most of the dirty work, adding feedback
// data to the current primitive list.
//
// If the 'more' argument is TRUE then the current rendering mode is put back
// into 'GL_FEEDBACK' mode...
//
// Normally you won't redefine this function...
//

int
GLPcontext :: UpdatePage(GLboolean more)
{
  int		i, used, count, v;
  int           primtype;
  GLfloat	*current;
  GLPvertex	vertices[3];
  GLboolean	depth;
  GLint		shademodel;
  GLPstate      state;
  GLboolean     nexttext=  false;
  GLboolean     nextimage= false;
  GLboolean     nextclip=  false;
  GLint         clipsize;

#ifdef DEBUG
  cout << "UpdatePage(" << (more ? "GL_TRUE" : "GL_FALSE") << ")\n" << flush;
#endif /* DEBUG */

  zval += 0.5;

  // Get the current depth test state and shade model...

  //depth = glIsEnabled(GL_DEPTH_TEST);
  depth= false;

  glGetIntegerv(GL_SHADE_MODEL, &shademodel);
  state.shade = (shademodel == GL_SMOOTH);

  glGetFloatv(GL_POINT_SIZE, &state.pointsize);
  glGetFloatv(GL_LINE_WIDTH, &state.linewidth);
  state.linedash= glIsEnabled(GL_LINE_STIPPLE);
  glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &state.dashpattern);
  glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &state.dashrepeat);

  state.polstiple= glIsEnabled(GL_POLYGON_STIPPLE);
  state.stippat= fakepattern;
  state.stipscale= stipplescale;
  if (state.polstiple){
    glGetPolygonStipple(state.stipmask);
  }

  // Figure out how many feedback entries there are and put the current
  // OpenGL context back in feedback mode if 'more' is true...

  used = glRenderMode(more ? GL_FEEDBACK : GL_RENDER);
  if (used == 0)
    return (GLP_NO_FEEDBACK);
//   cerr << "USED=" << used << " FEEDSIZE=" << feedsize << endl;

#ifdef DEBUG
  cout << "glGetError() after glRenderMode returned " <<
    glGetError() << "\n" << flush;
  cout << "First: used = " << used << ", feedback[0] = " <<
    feedback[0] << "\n" << flush;
#endif /* DEBUG */

  // Parse the feedback buffer...

  current = feedback;
  while (used > 0)
    {
#ifdef DEBUG
      cout << "Loop: used = " << used << "\n" << flush;
#endif /* DEBUG */

      primtype= (int)*current;

      // OBS: fix missing text-point-tokens etc..
      if (nexttext && primtype!=GL_POINT_TOKEN){
	if (textlist.size() > textcounter){
	  textlist.erase(textlist.begin()+textcounter);
	}
	nexttext= false;
      } else if (nextimage && primtype!=GL_POINT_TOKEN){
	if (nimages > imagecounter){
	  images[imagecounter].clear();
	  nimages--;
	}
	nextimage= false;
      } else if (nextclip && primtype!=GL_POINT_TOKEN){
	if (cliplist.size() > clipcounter){
	  cliplist.erase(cliplist.begin()+clipcounter);
	}
	nextclip= false;
      }

      switch (primtype)
	{
	case GL_POINT_TOKEN :
	  //cout << "GL_POINT_TOKEN" << endl;
#ifdef DEBUG
	  cout << "GL_POINT_TOKEN" << endl;
#endif
          current ++;
          used --;
          i = get_vertex(vertices + 0, current);
          current += i;
          used    -= i;
	  if (nexttext){ // add text primitive
	    if (textcounter<textlist.size()){
	      add_primitive(GL_TEXT_TOKEN, depth, state, 1, vertices);
	      textlist[textcounter].x= vertices[0].xyz[0];
	      textlist[textcounter].y= vertices[0].xyz[1];
	      textcounter++;
	    }
	    nexttext= false;

	  } else if (nextimage){ // add image primitive
	    if (imagecounter<nimages){
	      add_primitive(GL_IMAGE_TOKEN, depth, state, 1, vertices);
	      images[imagecounter].x= vertices[0].xyz[0];
	      images[imagecounter].y= vertices[0].xyz[1];
	      imagecounter++;
	    }
	    nextimage= false;

	  } else if (nextclip){ // add clippath primitive
	    if (clipcounter<cliplist.size()){
	      if (cliplist[clipcounter].v.size()==0)
		add_primitive(GL_CLIP_TOKEN, depth, state, 1, vertices);
	      if (clipsize > 0){
		GLPpoint xyz;
		xyz.x= vertices[0].xyz[0];
		xyz.y= vertices[0].xyz[1];
		xyz.z= 0;
		cliplist[clipcounter].v.push_back(xyz);
// 		cerr << "Added CLIP RASTER point:" << xyz.x << " "
// 		     << xyz.y << endl;
		clipsize--;
		if (clipsize==0){
		  nextclip= false;
		  clipcounter++;
		}
	      }
	    }

	  } else
	    add_primitive(primtype, depth, state, 1, vertices);
          break;
	case GL_LINE_RESET_TOKEN :
	  //cout << "GL_LINE_RESET_TOKEN" << endl;
#ifdef DEBUG
	  cout << "GL_LINE_RESET_TOKEN" << endl;
#endif
	case GL_LINE_TOKEN :
#ifdef DEBUG
	  cout << "GL_LINE_TOKEN" << endl;
#endif
          current ++;
          used --;
          i = get_vertex(vertices + 0, current);
          current += i;
          used    -= i;
          i = get_vertex(vertices + 1, current);
          current += i;
          used    -= i;
          add_primitive(primtype, depth, state, 2, vertices);
          break;

	case GL_POLYGON_TOKEN :
	  //cout << "GL_POLYGON_TOKEN" << endl;
#ifdef DEBUG
	  cout << "GL_POLYGON_TOKEN" << endl;
#endif
          // Get the number of vertices...

          count = (int)current[1];

          current += 2;
          used -= 2;

          // Loop through and add a series of triangles...

          v = 0;
          while (count > 0 && used > 0)
          {
            i = get_vertex(vertices + v, current);
            current += i;
            used    -= i;
            count --;

            // Add a triangle if we have 3 vertices...

            if (v == 2)
            {
              add_primitive(primtype, depth, state, 3, vertices);
              vertices[1] = vertices[2];
            }
            else
              v ++;
          };
          break;

      case GL_BITMAP_TOKEN :
	//cout << "GL_BITMAP_TOKEN" << endl;
#ifdef DEBUG
	  cout << "GL_BITMAP_TOKEN" << endl;
#endif
      case GL_DRAW_PIXEL_TOKEN :
#ifdef DEBUG
	  cout << "GL_DRAW_PIXEL_TOKEN" << endl;
#endif
      case GL_COPY_PIXEL_TOKEN :
#ifdef DEBUG
	  cout << "GL_COPY_PIXEL_TOKEN" << endl;
#endif
          current ++;
          used --;
          i = get_vertex(vertices + 0, current);
          current += i;
          used    -= i;
          add_primitive(primtype, depth, state, 1, vertices);
          break;

      case GL_PASS_THROUGH_TOKEN :
#ifdef DEBUG
          cout << "UpdatePage: Passthrough token " << current[1]
	       << "...\n" << flush;
#endif /* DEBUG */
	  // maybe next token is a text-token..
	  if (current[1]>TEXT_TOKEN-1 && current[1]<TEXT_TOKEN+1){
	    nexttext= true;
	  }
	  // ..or is it an image (in GL coordinates)
	  else if (current[1]>IMAGE_GL_TOKEN-1 && current[1]<IMAGE_GL_TOKEN+1){
	    nextimage= true;
	  }
	  // ..or is it an image (in pixel coordinates)
	  else if (current[1]>IMAGE_P_TOKEN-1 && current[1]<IMAGE_P_TOKEN+1){
	    add_primitive(GL_IMAGE_TOKEN, depth, state, 1, vertices);
	    imagecounter++;
	  }
	  // ..or is it removal of clipping
	  else if (current[1]>CLIP_REM_TOKEN-1 && current[1]<CLIP_REM_TOKEN+1){
	    add_primitive(GL_RCLIP_TOKEN, depth, state, 1, vertices);
	  }
	  // ..or is it a clippath (in pixel coordinates)
	  else if (current[1]>CLIP_TOKEN-1 && current[1]<CLIP_TOKEN+1){
	    add_primitive(GL_CLIP_TOKEN, depth, state, 1, vertices);
	    clipcounter++;
	  }
	  // ..or is it a clippath (in GL coordinates)
	  else if (current[1]>CLIP_TOKEN+0.5){
	    nextclip= true;
	    clipsize= static_cast<int>(rint(current[1]-CLIP_TOKEN));
// 	    cerr << "FOUND clippath...size:" << clipsize << endl;
	  }

          current += 2;
          used -= 2;
          break;

      default :
          cout << "UpdatePage: Ignoring unknown token " << current[0] << "...\n" << flush;
          current ++;
          used --;
          break;
    };
  };

  // Fix missing text-point-token
  if (nexttext){
    if (textlist.size() > textcounter){
      textlist.erase(textlist.begin()+textcounter);
    }
  }
  // Fix missing image-point-token
  if (nextimage){
    if (nimages > imagecounter){
      images[imagecounter].clear();
      nimages--;
    }
  }
  // Fix missing clippath-token
  if (nextclip){
    if (cliplist.size() > clipcounter){
      cliplist.erase(cliplist.begin()+clipcounter);
    }
  }

  return (GLP_SUCCESS);
}


//
// 'EndPage' function; this does nothing except parse the bounding boxes
// and output any remaining primitives.  It then frees the bounding box
// and primitive lists...
//

int
GLPcontext :: EndPage()
{
  GLPbbox	*bbox;				// Current bounding box
  GLPprimitive	*prim;				// Current primitive


#ifdef DEBUG
  cout << "EndPage()\n" << flush;
#endif /* DEBUG */

  // Stop doing feedback...

  UpdatePage(GL_FALSE);

  if (bboxes == NULL)
    return (GLP_NO_FEEDBACK);

  // Loop through all bounding boxes and primitives...

  for (bbox = bboxes; bbox != NULL; bbox = bbox->next)
  {
    for (prim = bbox->primitives; prim != NULL; prim = prim->next)
    {
    };
  };

  // Delete everything from the bounding box and primitive lists...

  delete_all();

#ifdef DEBUG
  cout << "EndPage() - done.\n" << flush;
#endif /* DEBUG */

  return (GLP_SUCCESS);
}


//
// 'SetOptions' function; this just sets the 'options' member to whatever is
// passed in.
//

void
GLPcontext :: SetOptions(int print_options)	// I - New printing options
{
  options = print_options;
}


//
// 'add_primitive' function; add a primitive to the list of primitives and
// bounding boxes for the current context.
//

void
GLPcontext :: add_primitive(int primtype,       // primitive type
			    GLboolean depth,	// I - Depth testing enabled?
                            GLPstate  state,    // state variables
                            int       num_verts,// I - Number of vertices
                            GLPvertex *verts)	// I - Vertices
{
  int		i,				// Looping var
		count;				// Count of intersections
  GLfloat	min[3],				// Minimum (x,y) coords
		max[3];				// Maximum (x,y) coords
  GLPprimitive	*newprim;			// New primitive
  GLPbbox	*bbox,				// Current bounding box
		*joinbbox,			// Bounding box to join with
		*nextbbox;			// Next bounding box...


#ifdef DEBUG
  cout << "add_primitive " << primtype << " ("
       << (depth ? "GL_TRUE" : "GL_FALSE") << ", "
       << (state.shade ? "GL_TRUE" : "GL_FALSE") << ", "
       << (state.linedash ? "GL_TRUE" : "GL_FALSE") << ", "
       << num_verts << ", "
       << (long int)verts << ")\n" << flush;
#endif /* DEBUG */

  // First create the new primitive and compute the bounding box for it...

  newprim = new GLPprimitive;

  newprim->primtype  = primtype;
  newprim->state     = state;
  newprim->num_verts = num_verts;
  if (num_verts>0)
    memcpy(newprim->verts, verts, sizeof(GLPvertex) * num_verts);

  if (num_verts > 0){
    min[0] = min[1] = min[2] = 100000.0;
    max[0] = max[1] = max[2] = -100000.0;
  } else {
    min[0] = min[1] = min[2] = 0.0;
    max[0] = max[1] = max[2] = 0.0;
  }

  for (i = 0; i < num_verts; i ++)
  {
    if (verts[i].xyz[0] < min[0])
      min[0] = verts[i].xyz[0];
    if (verts[i].xyz[1] < min[1])
      min[1] = verts[i].xyz[1];
    if (verts[i].xyz[2] < min[2])
      min[2] = verts[i].xyz[1];
    if (verts[i].xyz[0] > max[0])
      max[0] = verts[i].xyz[0];
    if (verts[i].xyz[1] > max[1])
      max[1] = verts[i].xyz[1];
    if (verts[i].xyz[2] > max[2])
      max[2] = verts[i].xyz[2];
  };

  newprim->zmin    = min[2];
  newprim->zmax    = max[2];

  // Stretch the bbox out to the nearest 64 pixels to improve performance...
  min[0] = floor(min[0] * 0.015625) * 64.0;
  min[1] = floor(min[1] * 0.015625) * 64.0;
  max[0] = ceil(max[0] * 0.015625) * 64.0;
  max[1] = ceil(max[1] * 0.015625) * 64.0;

  // Now search the current bounding box list to see if this primitive lies
  // inside an existing bbox, partially inside, or completely outside...
  //
  // The 'count' variable counts the number of corners that lie inside the
  // current bounding box.  If 'count' is 0, then the bbox is completely
  // outside the current bbox.  Values between 1 and 3 indicate a partially
  // inside primitive.  A value of 4 means that the primitive is completely
  // inside the bbox.
  //
  // If the primitive lies completely outside any bboxes that are out there
  // already, then a new bbox is created with the primitive in it.
  //
  // If the primitive lies partially inside the bbox, the bbox is expanded to
  // include the primitive, and a 'join' operation is performed with any
  // neighboring bboxes that intersect with the expanded bbox.  Finall, the
  // primitive is added to the bbox using the 'sort_primitive()' function
  // (this handles depth buffering if enabled).
  //
  // If the primitive lies completely inside the bbox, it is added with
  // 'sort_primitive()'.

  for (bbox = bboxes; bbox != NULL; bbox = bbox->next){
    count = 0;
    if (min[0] > bbox->min[0] && min[0] < bbox->max[0])
      count ++;
    if (max[0] > bbox->min[0] && max[0] < bbox->max[0])
      count ++;
    if (min[1] > bbox->min[1] && min[1] < bbox->max[1])
      count ++;
    if (max[1] > bbox->min[1] && max[1] < bbox->max[1])
      count ++;

    // NB NB NB
    // test: only one bounding-box
    count=4;

    if (count > 0)
      break;
  };

  if (bbox == NULL){
    // New bbox...
//     cerr << "Making new bounding box" << endl;

    bbox = new GLPbbox;

    bbox->prev = NULL;
    bbox->next = bboxes;
    if (bboxes != NULL)
      bboxes->prev = bbox;
    bboxes = bbox;

    bbox->min[0]     = min[0];
    bbox->max[0]     = max[0];
    bbox->min[1]     = min[1];
    bbox->max[1]     = max[1];
    bbox->min[2]     = min[2];
    bbox->max[2]     = max[2];
    bbox->primitives = newprim;
    bbox->lastprim   = newprim;
    newprim->next    = NULL;
    newprim->prev    = NULL;
  } else if (count < 4) {
    // Partially inside...
//     cerr << "Partially inside a bounding box" << endl;

    if (min[0] < bbox->min[0])
      bbox->min[0] = min[0];
    if (max[0] > bbox->max[0])
      bbox->max[0] = max[0];
    if (min[1] < bbox->min[1])
      bbox->min[1] = min[1];
    if (max[1] > bbox->max[1])
      bbox->max[1] = max[1];

    // Incrementally join bounding boxes until no more boxes are joined...

    do {
      count = 0;
      for (joinbbox = bboxes; joinbbox != NULL; joinbbox = nextbbox) {
        nextbbox = joinbbox->next;

        if (joinbbox == bbox)
          continue;
        else if ((bbox->min[0] > joinbbox->min[0] &&
		  bbox->min[0] < joinbbox->max[0]) ||
        	 (bbox->max[0] > joinbbox->min[0] &&
		  bbox->max[0] < joinbbox->max[0]) ||
        	 (bbox->min[1] > joinbbox->min[1] &&
		  bbox->min[1] < joinbbox->max[1]) ||
        	 (bbox->max[1] > joinbbox->min[1] &&
		  bbox->max[1] < joinbbox->max[1]))
	  {
	    // Join this one...

	    count ++;

	    // Add this bbox's primitives to the end of the current bbox...

	    (bbox->lastprim->next)       = joinbbox->primitives;
	    (joinbbox->primitives)->prev = bbox->lastprim;
	    bbox->lastprim               = joinbbox->lastprim;

	    if (joinbbox->prev == NULL)
	      bboxes = joinbbox->next;
	    else
	      (joinbbox->prev)->next = joinbbox->next;

	    if (nextbbox != NULL)
	      nextbbox->prev = joinbbox->prev;

	    for (i = 0; i < 3; i ++){
	      if (joinbbox->min[i] < bbox->min[i])
		bbox->min[i] = joinbbox->min[i];
	      if (joinbbox->max[i] > bbox->max[i])
		bbox->max[i] = joinbbox->max[i];
	    };

	    delete joinbbox;
	  };
      };
    }
    while (count > 0);

    // Add the primitive to this bbox...

    sort_primitive(depth, bbox, newprim);
  }
  else {
    // Primitive lies completely inside the bbox, so just add it...
//     cerr << "Completely inside another bounding box" << endl;

    sort_primitive(depth, bbox, newprim);
  };
}


//
// 'sort_primitive' function; add a primitive to the specified bounding
// box.  If depth buffering is disabled then we just add the primitive to
// the end of the primitive list.  Otherwise we go through a complicated
// depth sorting and clipping algorithm to add the visible pieces of the
// primitive and remove anything we cover...
//

void
GLPcontext :: sort_primitive(GLboolean    depth,	// I - Depth test enabled?
                             GLPbbox      *bbox,	// I - Bounding box
                             GLPprimitive *newprim)	// I - New primitive
{
  GLPprimitive	*prim;


  if (!depth)
  {
    // Add the primitive to the end of the list...
//     cerr << "Simply add primitive to list" << endl;

    bbox->lastprim->next = newprim;
    newprim->prev        = bbox->lastprim;
    newprim->next        = NULL;
    bbox->lastprim       = newprim;
  }
  else
  {
    if (newprim->zmax < bbox->min[2])
    {
      // Add the primitive to the end of the list...

      bbox->lastprim->next = newprim;
      newprim->prev        = bbox->lastprim;
      newprim->next        = NULL;
      bbox->lastprim       = newprim;
    }
    else if (newprim->zmin > bbox->max[2])
    {
      // Add the primitive to the start of the list...

      bbox->primitives->prev = newprim;
      newprim->next          = bbox->primitives;
      newprim->prev          = NULL;
      bbox->primitives       = newprim;
    }
    else
    {
      // Sort the primitive by depth...

      for (prim = bbox->lastprim; prim != NULL; prim = prim->prev)
        if (newprim->zmax < prim->zmin)
        {
          // Insert the primitive after this one...

	  newprim->prev      = prim;
	  newprim->next      = prim->next;
	  (prim->next)->prev = newprim;
	  prim->next         = newprim;
        }
        else if (newprim->zmax < prim->zmax)
        {
          // Break polys up...

	  newprim->next      = prim;
	  newprim->prev      = prim->prev;
	  (prim->prev)->next = newprim;
	  prim->prev         = newprim;
        };
    };
  };

  if (newprim->zmin < bbox->min[2])
    bbox->min[2] = newprim->zmin;
  if (newprim->zmax < bbox->max[2])
    bbox->max[2] = newprim->zmax;
}


//
// 'get_vertex' function; get a vertex from the feedback buffer...
//

int
GLPcontext :: get_vertex(GLPvertex *v,	// O - Vertex pointer
                         GLfloat   *p)	// I - Data pointer
{
  int	i;				// Color index


  v->xyz[0] = p[0];
  v->xyz[1] = p[1];
//   v->xyz[2] = p[2];
  v->xyz[2] = zval;

#ifdef DEBUG
//   cout << "{ " << p[0] << ", " << p[1] << ", " << p[2] << "}, " << flush;
  cout << "{ " << p[0] << ", " << p[1] << ", " << zval << "}, " << flush;
#endif /* DEBUG */

  if (feedmode == GL_COLOR_INDEX && colorsize > 0)
  {
    // Color index value...
    i = (int)(p[3] + 0.5);

    v->rgba[0] = colormap[i][0];
    v->rgba[1] = colormap[i][1];
    v->rgba[2] = colormap[i][2];
    v->rgba[3] = colormap[i][3];

    return (4);
  }
  else
  {
    // RGBA value...

    v->rgba[0] = p[3];
    v->rgba[1] = p[4];
    v->rgba[2] = p[5];
    v->rgba[3] = p[6];

    return (7);
  };
}


//
// 'delete_all' function; delete all bounding boxes and primitives from
// the current context.
//

void
GLPcontext :: delete_all()
{
  GLPbbox	*bbox,				// Current bounding box
		*nextbbox;			// Next bounding box
  GLPprimitive	*prim,				// Current primitive
		*nextprim;			// Next primitive


#ifdef DEBUG
  cout << "delete_all()\n" << flush;
#endif /* DEBUG */

  for (bbox = bboxes, nextbbox = NULL; bbox != NULL; bbox = nextbbox)
  {
    for (prim = bbox->primitives, nextprim = NULL; prim != NULL; prim = nextprim)
    {
      nextprim = prim->next;
      delete prim;
    };

    nextbbox = bbox->next;
    delete bbox;
  };

  bboxes = NULL;

  // delete all images
  if (nimages){
    for (int i=0; i<nimages; i++){
      delete[] images[i].pixels;
      images[i].pixels= 0;
    }
    nimages= 0;
  }
#ifdef DEBUG
  cout << "delete_all(): done.\n" << flush;
#endif /* DEBUG */
}


// add image - x/y in pixel coordinates
bool GLPcontext::AddImage(const GLvoid* pix,GLint size,GLint w,GLint h,
			  GLfloat x,GLfloat y,GLfloat sx,GLfloat sy,
			  GLint ix1,GLint iy1,GLint ix2,GLint iy2,
			  GLenum format,GLenum type,GLboolean blend){

  if (nimages==MAXIMAGES) return false;
  if (size == 0) return false;

  images[nimages].size= size;
  images[nimages].w= w;
  images[nimages].h= h;
  images[nimages].x= x;
  images[nimages].y= y;
  images[nimages].sx= sx;
  images[nimages].sy= sy;
  images[nimages].ix1= ix1;
  images[nimages].iy1= iy1;
  images[nimages].ix2= ix2;
  images[nimages].iy2= iy2;
  images[nimages].format= format;
  images[nimages].type= type;
  images[nimages].blend= blend;
  images[nimages].pixels= 0;
  if (type==GL_UNSIGNED_BYTE){
    images[nimages].pixels= new unsigned char[size];
    memcpy(images[nimages].pixels,pix,size);
  }
  nimages++;

  // add marker in feedback buffer
  glPassThrough(IMAGE_P_TOKEN);

  return true;
}

// add image - x/y in GL coordinates
bool GLPcontext::AddImageGl(const GLvoid* pix,GLint size,GLint w,GLint h,
			    GLfloat x,GLfloat y,GLfloat sx,GLfloat sy,
			    GLint ix1,GLint iy1,GLint ix2,GLint iy2,
			    GLenum format,GLenum type,GLboolean blend){

  if (nimages==MAXIMAGES) return false;
  if (size == 0) return false;

  images[nimages].size= size;
  images[nimages].w= w;
  images[nimages].h= h;
  images[nimages].x= x;
  images[nimages].y= y;
  images[nimages].sx= sx;
  images[nimages].sy= sy;
  images[nimages].ix1= ix1;
  images[nimages].iy1= iy1;
  images[nimages].ix2= ix2;
  images[nimages].iy2= iy2;
  images[nimages].format= format;
  images[nimages].type= type;
  images[nimages].blend= blend;
  images[nimages].pixels= 0;
  if (type==GL_UNSIGNED_BYTE){
    images[nimages].pixels= new unsigned char[size];
    memcpy(images[nimages].pixels,pix,size);
  }
  nimages++;

  // add marker in feedback buffer
  glPassThrough(IMAGE_GL_TOKEN);
  glBegin(GL_POINTS);
  glVertex2f(x,y);
  glEnd();

  return true;
}

// grab image from framebuffer
bool GLPcontext::grabImage(GLint x,GLint y,GLint w,GLint h,
			   GLenum format,GLenum type){

  if (nimages==MAXIMAGES) return false;

  // grab image
  int npixels = w*h;
  int nchannels = (format==GL_RGBA ? 4 : 3);
  images[nimages].pixels = new unsigned char[npixels*nchannels];
  cerr << "grabbing image: x:" << x << " y:" << y << "w:"
       << w << " h:" << h << " size:" << npixels*nchannels << endl;
  glReadBuffer(GL_BACK);
  //glFlush();
  glReadPixels(x,y,w,h,
	       format, type,
	       images[nimages].pixels);
  //glFlush();

  cerr << "grabbing done" << endl;
  images[nimages].size= npixels*nchannels;
  images[nimages].w= w;
  images[nimages].h= h;
  images[nimages].x= x;
  images[nimages].y= y;
  images[nimages].sx= 1.0;
  images[nimages].sy= 1.0;
  images[nimages].ix1= 0;
  images[nimages].iy1= 0;
  images[nimages].ix2= w-1;
  images[nimages].iy2= h-1;
  images[nimages].format= format;
  images[nimages].type= type;
  images[nimages].blend= false;
  nimages++;

  // add marker in feedback buffer
  glPassThrough(IMAGE_P_TOKEN);

  return true;
}

void GLPcontext::setFont(const string fn, const float size)
{
  currfont= fn;
  fontsize= size;

  // maintain complete list of used fonts
  deffonts.insert(fn);
}

bool GLPcontext::addStr(string s,
			const float x, const float y,
			const float a)
{
  GLPtext t;
  t.s= s;
  t.font= currfont;
  t.size= fontsize;
  t.angle= a;
  textlist.push_back(t);

  // add marker in feedback buffer
  glPassThrough(TEXT_TOKEN);
  glBegin(GL_POINTS);
  glVertex2f(x,y);
  glEnd();

  return true;
}

// void GLPcontext::printStr(string s, const float x,
// 			  const float y, const float a)
// {
//   GLPtext t;
//   t.s= s;
//   t.font= currfont;
//   t.size= fontsize;
//   t.x= x;
//   t.y= y;
//   t.angle= a;
//   textlist.push_back(t);
//   // add primitive
//   GLPvertex	vert;
//   GLboolean	depth;
//   GLint		shademodel;
//   GLPstate      state;
//   GLfloat       curcol[4];

//   int primtype= GL_TEXT_TOKEN;

//   glGetFloatv(GL_CURRENT_COLOR, curcol);

//   depth = glIsEnabled(GL_DEPTH_TEST);

//   glGetIntegerv(GL_SHADE_MODEL, &shademodel);
//   state.shade = (shademodel == GL_SMOOTH);

//   glGetFloatv(GL_POINT_SIZE, &state.pointsize);
//   glGetFloatv(GL_LINE_WIDTH, &state.linewidth);
//   state.linedash= glIsEnabled(GL_LINE_STIPPLE);
//   glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &state.dashpattern);
//   glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &state.dashrepeat);

//   state.polstiple= glIsEnabled(GL_POLYGON_STIPPLE);
//   state.stippat= fakepattern;
//   state.stipscale= stipplescale;
//   if (state.polstiple){
//     glGetPolygonStipple(state.stipmask);
//   }

//   vert.xyz[0]= x;
//   vert.xyz[1]= y;
//   vert.xyz[2]= 0.0;
//   vert.rgba[0]= curcol[0];
//   vert.rgba[1]= curcol[1];
//   vert.rgba[2]= curcol[2];
//   vert.rgba[3]= curcol[3];

// //   add_primitive(primtype, depth, state, 1, &vert);
// }

void GLPcontext::addCustom(const string s)
{
  customcoms.push_back(s);
  // add primitive
  bool depth = glIsEnabled(GL_DEPTH_TEST);
  int primtype= GL_CUSTOM_TOKEN;
  GLPstate state;
  add_primitive(primtype, depth, state, 0, (GLPvertex*)(0));
}

bool GLPcontext::addClipPath(const int& size,
			     const float* x, const float* y,
			     const bool rect)
{
  if (size < 3 || !x || !y) return false;
  GLPclippath c;
  c.rectangle= rect;
  cliplist.push_back(c);

  // add marker in feedback buffer
  glPassThrough(CLIP_TOKEN+size);
  glBegin(GL_POINTS);
  for (int i=0; i<size; i++){
    glVertex2f(x[i],y[i]);
//     cerr << "Added CLIP point:" << x[i] << " " << y[i] << endl;
  }
  glEnd();

  return true;
}

void GLPcontext::addStencil(const int& size, const float* x, const float* y)
{
  addClipPath(size,x,y,false);
}

void GLPcontext::addScissor(const double x0, const double y0,
			    const double  w, const double  h)
{
  float x[3], y[3];
  x[0]= x0;
  y[0]= y0;
  x[2]= x0+w;
  y[2]= y0+h;
  x[1]= (x[0]+x[2])/2.0;
  y[1]= (y[0]+y[2])/2.0;

  addClipPath(3,x,y,true);
}

void GLPcontext::addScissor(const int x0, const int y0,
			    const int  w, const int  h)
{
  GLPclippath c;
  c.rectangle= true;

  float x[3], y[3];
  x[0]= x0;
  y[0]= y0;
  x[2]= x0+w;
  y[2]= y0+h;
  x[1]= (x[0]+x[2])/2.0;
  y[1]= (y[0]+y[2])/2.0;

  for (int i=0; i<3; i++){
    GLPpoint p;
    p.x= x[i];
    p.y= y[i];
    p.z= 0;
    c.v.push_back(p);
//     cerr << "Added RASTER CLIP point:" << x[i] << " " << y[i] << endl;
  }
  cliplist.push_back(c);

  // add marker in feedback buffer
  glPassThrough(CLIP_TOKEN);
}

bool GLPcontext::removeClipping()
{
  // add marker in feedback buffer
  glPassThrough(CLIP_REM_TOKEN);

  return true;
}


void GLPcontext::addReset()
{
  // add primitive
  bool depth = glIsEnabled(GL_DEPTH_TEST);
  int primtype= GL_RESET_TOKEN;
  GLPstate state;
  add_primitive(primtype, depth, state, 0, (GLPvertex*)(0));
}


//
// End of "$Id: context.cc 4306 2013-12-17 12:55:57Z juergens $".
//
