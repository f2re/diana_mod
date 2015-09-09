/*
 libglText - OpenGL Text Rendering Library

 $Id: glTextTTPixmap.h 3021 2010-01-20 16:52:55Z dages $

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

// glTextTTPixmap.h

#ifndef _glTextTTPixmap_h
#define _glTextTTPixmap_h

#include "glText.h"
#include <string>
#include <miFTGL/FTFont.h>

class glTextTTPixmap: public glText {
private:
  struct ttfont {
    std::string fontname; // font filename
    std::string psname; // PostScript equivalent
    float psxscale; // Postscript X-scale
    float psyscale; // Postscript Y-scale
    bool created; // font created
    FTFont *pfont;
    ttfont() :
      created(false), pfont(0)
    {
    }
    ~ttfont()
    {
      delete pfont;
    }
  };
private:
  float scalex, scaley; // font scaling
  float xscale; // finetuning
  ttfont fonts[MAXFONTS][MAXFONTFACES][MAXFONTSIZES]; // tt fonts

  bool _checkFont();
  bool _calcScaling();
public:
  glTextTTPixmap();
  ~glTextTTPixmap();

  bool testDefineFonts(std::string path = "fonts");
  // define all fonts matching pattern with family, face and ps-equiv
  bool defineFonts(const std::string pattern, const std::string family,
      const std::string psname = "")
  {
    return true;
  }
  // define new font (family,name,face,size, ps-equiv, ps-xscale, ps-yscale)
  bool defineFont(const std::string, const std::string, const FontFace, const int,
      const std::string = "", const float = 1.0, const float = 1.0);
  // choose font, size and face
  bool set(const std::string, const FontFace, const float);
  bool setFont(const std::string);
  bool setFontFace(const FontFace);
  bool setFontSize(const float size);
  // drawing commands
  bool drawChar(const int c, const float x, const float y, const float a = 0);
  bool drawStr(const char* s, const float x, const float y, const float a = 0);
  // Metric commands
  void setScalingType(const FontScaling fs);
  void setGlSize(const float, const float);
  void setVpSize(const float, const float);
  bool getCharSize(const int c, float& w, float& h);
  bool getMaxCharSize(float& w, float& h);
  bool getStringSize(const char* s, float& w, float& h);
  // PostScript Equivalent
  std::string getPsName() const;
};

#endif
