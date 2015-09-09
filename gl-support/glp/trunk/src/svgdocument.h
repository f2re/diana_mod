//
// "$Id: svgdocument.h 3372 2010-09-17 11:43:18Z audunc $"
//
//   Header file for the svgdocument class for the OpenGL printing toolkit.
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
//   Initial revision
//

#ifndef _GL_svgdocument_H_
#define _GL_svgdocument_H_

//
// Include necessary headers.
//

#include "GLP.h"
#include <string>

//
// The SVG document output class for GLP.  Basically, we just
// add file and page count members and define our own constructor,
// destructor, and 'EndPage()' functions.
//

class SvgDocument: public GLPcontext //// svgdocument class
{
protected:
  int page_count; // Page count
  std::fstream *outfile; // Output file
  int clipstack; // number of clippings on stack
  bool inaline; // drawing a polyline

  void Write_Header(); // make svg header
  void Reset();

  std::string svg_dash(unsigned int, int); // svg dash-pattern
  std::string svg_line_properties(GLPprimitive*); // svg linewidth, pattern, opacity etc
  std::string svg_fill_properties(GLPprimitive*); // svg fill-opacity etc
  std::string svg_image(const GLPimage*, float pheight); // svg rgb(a)-image
  std::string svg_masked_triangle(GLPprimitive*, // svg masked triangle
      GLint[]);
  std::string svg_pattern_triangle(GLPprimitive*, // svg patterned triangle
      int, float[], float, float);
  std::string svg_triangle(GLPprimitive*, // generic svg triangle-code
      float height, GLint[]);
  std::string svg_colour(GLPprimitive*); // flat-shading colour routine
  std::string svg_remove_clippath(); // remove a clippath
  std::string svg_clippath(GLPclippath&);// add a clippath
  std::string svg_startpolyline(GLPprimitive*);
  std::string svg_endpolyline();

public:
  SvgDocument(char *print_name, int print_options = GLP_FIT_TO_PAGE
      | GLP_DRAW_BACKGROUND, int print_size = 0, std::map<std::string,
      std::string> *mc = 0);
  ~SvgDocument();

  int EndPage();
};

#endif // !_GL_svgdocument_H_
//
// End of "$Id: svgdocument.h 3372 2010-09-17 11:43:18Z audunc $".
//
