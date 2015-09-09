/*
 libglText - OpenGL Text Rendering Library

 $Id: glText.h 4726 2015-02-12 08:16:59Z alexanderb $

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

// glText.h

#ifndef _gltext_h
#define _gltext_h

#include <string>

class GLPcontext;

class glText {
public:
  enum FontScaling {
    S_FIXEDSIZE, S_VIEWPORTSCALED
  };
  enum {
    MAXFONTFACES = 4, MAXFONTS = 10, MAXFONTSIZES = 40
  };
  enum FontFace {
    F_NORMAL = 0, F_BOLD = 1, F_ITALIC = 2, F_BOLD_ITALIC = 3
  };
protected:
  std::string FamilyName[MAXFONTS];
  int Sizes[MAXFONTSIZES];
  int numFonts; // number of defined fonts
  int numSizes; // number of defined fontsizes

  int FontIndex; // current font index
  FontFace Face; // current font face
  int SizeIndex; // current fontsize index
  float reqSize; // last requested font size
  FontScaling scaletype; // current scaling behaviour
  bool wysiwyg; // hardcopy size = screen size
  float vpWidth, vpHeight; // viewport size in pixels
  float glWidth, glHeight; // viewport size in gl-coord.
  float pixWidth, pixHeight; // size of pixel in gl-coord.
  bool initialised;
  int size_add; // user-controlled size modification
  bool hardcopy; // Hardcopy production
  GLPcontext* output; // OpenGL Hardcopy Module

  bool _addFamily(const std::string, int&);
  bool _addSize(const int, int&);
  bool _findSize(const int, int&, const bool = false);
  bool _findFamily(const std::string, int&);
public:
  glText();
  virtual ~glText()
  {
  }

  // fill fontpack for testing
  virtual bool testDefineFonts(std::string path = "fonts")= 0;
  // define all fonts matching pattern with family, face and ps-equiv
  virtual bool defineFonts(const std::string pattern, const std::string family,
      const std::string psname = "")= 0;
  // define new font (family,name,face,size, ps-equiv, ps-xscale, ps-yscale)
  virtual bool defineFont(const std::string, const std::string, const FontFace,
      const int, const std::string = "", const float = 1.0, const float = 1.0)= 0;
  // choose font, size and face
  virtual bool set(const std::string, const FontFace, const float) = 0;
  virtual bool setFont(const std::string) = 0;
  virtual bool setFontFace(const FontFace) = 0;
  virtual bool setFontSize(const float) = 0;
  // printing commands
  virtual bool drawChar(const int c, const float x, const float y,
      const float a = 0) = 0;
  virtual bool drawStr(const char* s, const float x, const float y,
      const float a = 0) = 0;
  // Metric commands
  virtual void adjustSize(const int sa)
  {
    size_add = sa;
  }
  virtual void setScalingType(const FontScaling fs)
  {
    scaletype = fs;
  }
  // set viewport size in GL coordinates
  virtual void setGlSize(const float glw, const float glh)
  {
    glWidth = glw;
    glHeight = glh;
    pixWidth = glWidth / vpWidth;
    pixHeight = glHeight / vpHeight;
  }
  // set viewport size in physical coordinates (pixels)
  virtual void setVpSize(const float vpw, const float vph)
  {
    vpWidth = vpw;
    vpHeight = vph;
    pixWidth = glWidth / vpWidth;
    pixHeight = glHeight / vpHeight;
  }
  virtual void setPixSize(const float pw, const float ph)
  {
    pixWidth = pw;
    pixHeight = ph;
  }
  virtual bool getCharSize(const int c, float& w, float& h) = 0;
  virtual bool getMaxCharSize(float& w, float& h)= 0;
  virtual bool getStringSize(const char* s, float& w, float& h) = 0;
  // return info
  FontScaling getFontScaletype()
  {
    return scaletype;
  }
  int getNumFonts()
  {
    return numFonts;
  }
  int getNumSizes()
  {
    return numSizes;
  }
  FontFace getFontFace()
  {
    return Face;
  }
  virtual float getFontSize()
  {
    if (numSizes)
      return Sizes[SizeIndex];
    else
      return 0;
  }
  int getFontSizeIndex()
  {
    return SizeIndex;
  }
  std::string getFontName(const int index)
  {
    return (index >= 0 && index < numFonts) ? FamilyName[index] : std::string("");
  }
  // PostScript equivalent name
  virtual std::string getPsName() const
  {
    return "";
  }
  // for harcopy production (postscript)
  virtual void startHardcopy(GLPcontext*);
  virtual void endHardcopy();
  virtual void setWysiwyg(const bool);
  // requested-size/actual-size
  virtual float getSizeDiv();
};

#endif
