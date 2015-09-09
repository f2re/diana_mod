/*
 libglText - OpenGL Text Rendering Library

 $Id: glTextQtTexture.h 4314 2013-12-18 08:25:30Z juergens $

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

// glTextQtTexture.h

#ifndef _gltextqttexture_h
#define _gltextqttexture_h

#include "glText.h"
#include <string>

class TextRenderer;
class QFont;

class glTextQtTexture: public glText {
private:
  struct qtfont {
    std::string fontname; // font filename
    std::string psname;
    float psxscale; // Postscript X-scale
    float psyscale; // Postscript Y-scale
    bool created; // font created
    QFont * qfont;
    TextRenderer * pfont;
    qtfont();
    ~qtfont();
  };
private:
  float scalex, scaley; // font scaling
  float xscale; // finetuning
  qtfont fonts[MAXFONTS][MAXFONTFACES][MAXFONTSIZES]; // qt fonts
  float glx1;
  float glx2;
  float gly1;
  float gly2;

  bool _checkFont();
  bool _calcScaling();
public:
  glTextQtTexture();
  ~glTextQtTexture();

  bool testDefineFonts(std::string path = "fonts");
  // define all fonts matching pattern with family, face and ps-equiv
  bool defineFonts(const std::string /*pattern*/, const std::string /*family*/,
      const std::string /*psname*/ = "")
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
  //void setGlSize(const float, const float, const float, const float);
  void setVpSize(const float, const float);
  bool getCharSize(const int c, float& w, float& h);
  bool getMaxCharSize(float& w, float& h);
  bool getStringSize(const char* s, float& w, float& h);
  // PostScript Equivalent
  std::string getPsName() const;
};

#endif
