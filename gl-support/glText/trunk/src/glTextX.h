/*
 libglText - OpenGL Text Rendering Library

 $Id: glTextX.h 4314 2013-12-18 08:25:30Z juergens $

 Copyright (C) 2006 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: diana@met.no

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

// glTextX.h

#ifndef _gltextx_h
#define _gltextx_h

#include "glText.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>

class glTextX: public glText {
private:
  struct xfont {
    std::string fontname; // X fontname
    std::string psname; // PostScript equivalent
    float psxscale; // Postscript X-scale
    float psyscale; // Postscript Y-scale
    int size; // size in points
    bool created; // font created
    int baseList; // index to display list
    XFontStruct* fontInfo; // X fontstruct
    xfont() :
      fontname(""), size(0), created(false), baseList(0), fontInfo(0)
    {
    }
    ~xfont()
    {
    }
  };
private:
  Display *dpy; // connection to x-server
  xfont fonts[MAXFONTS][MAXFONTFACES][MAXFONTSIZES]; // X fonts
  int base; // display list index

  bool _findDefinedSize(const int, int&, const bool = false);
  bool _makeRasterFont();
  bool _checkFont();

public:
  glTextX();
  glTextX(std::string display_name); // display_name
  glTextX(Display* display); // display
  ~glTextX();

  void initialise(Display*);
  bool testDefineFonts(std::string path = "fonts");
  // define all fonts matching pattern with family, face and ps-equiv
  bool defineFonts(const std::string pattern, const std::string family,
      const std::string psname = "");
  // define new font (family,name,face,size, ps-equiv, ps-xscale, ps-yscale)
  bool defineFont(const std::string, const std::string, const FontFace, const int,
      const std::string = "", const float = 1.0, const float = 1.0);
  // choose font, size and face
  bool set(const std::string, const FontFace, const float);
  bool setFont(const std::string);
  bool setFontFace(const FontFace);
  bool setFontSize(const float);
  // printing commands
  bool drawChar(const int c, const float x, const float y, const float a = 0);
  bool drawStr(const char* s, const float x, const float y, const float a = 0);
  // Metric commands
  bool getCharSize(const int c, float& w, float& h);
  bool getMaxCharSize(float& w, float& h);
  bool getStringSize(const char* s, float& w, float& h);
  // PostScript Equivalent
  std::string getPsName() const;
};

#endif
