/*
 libglText - OpenGL Text Rendering Library

 $Id: glTextTTBitmap.cc 3117 2010-03-02 12:57:17Z dages $

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

// glTextTTBitmap.cc : Class definition for GL-rendered truetype-fonts

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fstream>
#include "glTextTTBitmap.h"
#include <iostream>
#include <miFTGL/FTGLBitmapFont.h>
#include <GL/gl.h>
#include <glp/GLP.h>
#include <glp/glpfile.h>
#include <math.h>

using namespace std;

glTextTTBitmap::glTextTTBitmap() :
  glText(), scalex(1.0), scaley(1.0), xscale(1.0)
{
#ifdef DEBUGPRINT
  cerr << "glTextTTBitmap::constructor" << endl;
#endif
  initialised = true;
}

glTextTTBitmap::~glTextTTBitmap()
{
}

bool glTextTTBitmap::testDefineFonts(string path)
{
  string famn;
  string fontn;
  if (path.length() == 0)
    path = "fonts";

  famn = "Arial";
  if (fontn.length() == 0)
    fontn = path + "/Arialn.ttf";
  defineFont(famn, fontn, glText::F_NORMAL, 16);
  // italic
  fontn = path + "/Arialni.ttf";
  defineFont(famn, fontn, glText::F_ITALIC, 20);
  // bold
  fontn = path + "/Arialnb.ttf";
  defineFont(famn, fontn, glText::F_BOLD, 20);

  // different weather symbols
  famn = "Symbol";
  fontn = path + "/dnmimet.ttf";
  defineFont(famn, fontn, glText::F_NORMAL, 20);

  return true;
}

bool glTextTTBitmap::defineFont(const string family, const string fname,
    const FontFace face, const int size, const string psname,
    const float psxscale, const float psyscale)
{
  int fidx;
  if (!_findFamily(family, fidx))
    if (!_addFamily(family, fidx))
      return false;

  fonts[fidx][face][0].fontname = fname;
  fonts[fidx][face][0].psname = psname;
  fonts[fidx][face][0].psxscale = psxscale;
  fonts[fidx][face][0].psyscale = psyscale;

  return true;
}

// choose font, size and face
bool glTextTTBitmap::set(const string fam, const FontFace face,
    const float size)
{

  int fidx;
  if (_findFamily(fam, fidx)) {
    FontIndex = fidx;
    Face = face;
    reqSize = size * 1.3;
    bool b = _calcScaling();
    if (hardcopy)
      output->setFont(fonts[FontIndex][Face][0].psname,
          (wysiwyg ? getFontSize() : reqSize));
    return b;
  }
  return false;
}

bool glTextTTBitmap::setFont(const string family)
{
  int fidx;
  if (_findFamily(family, fidx)) {
    FontIndex = fidx;
    bool b = _calcScaling();
    if (hardcopy)
      output->setFont(fonts[FontIndex][Face][0].psname,
          (wysiwyg ? getFontSize() : reqSize));
    return b;
  }
  return false;
}

bool glTextTTBitmap::setFontFace(const FontFace face)
{
  Face = face;
  bool b = _calcScaling();
  if (hardcopy)
    output->setFont(fonts[FontIndex][Face][0].psname, (wysiwyg ? getFontSize()
        : reqSize));
  return b;
}

bool glTextTTBitmap::setFontSize(const float size)
{
  reqSize = size * 1.3;
  bool b = _calcScaling();
  if (hardcopy)
    output->setFont(fonts[FontIndex][Face][0].psname, (wysiwyg ? getFontSize()
        : reqSize));
  return b;
}

void glTextTTBitmap::setScalingType(const FontScaling fs)
{
  scaletype = fs;
  _calcScaling();
}

bool glTextTTBitmap::_calcScaling()
{
  pixWidth = glWidth / vpWidth;
  pixHeight = glHeight / vpHeight;

  scalex = scaley = 1.0;

  float truesize = reqSize * scalex;
  int size = static_cast<int> (roundf(reqSize));
  if (size < 1)
    size = 1;

  float qx = truesize / (1.0 * size);

  scalex = qx;
  scaley = qx;

  //   cerr << "PixWidth:"      << pixWidth << " PixHeight:" << pixHeight
  //        << " reqSize:"      << reqSize
  //        << " trueSize:"     << truesize
  //        << " size:"         << size
  //        << " Scaling(x,y):" << scalex << ", " << scaley
  //        << endl;

  // find correct sizeindex
  if (!_findSize(size, SizeIndex, true)) {
    if (!_addSize(size, SizeIndex)) {
      if (!_findSize(size, SizeIndex, false)) {
        SizeIndex = 0;
      }
    }
  }

  // create font if necessary
  return _checkFont();
}

void glTextTTBitmap::setGlSize(const float glw, const float glh)
{
  glWidth = glw;
  glHeight = glh;

  _calcScaling();
}

void glTextTTBitmap::setVpSize(const float vpw, const float vph)
{
  vpWidth = vpw;
  vpHeight = vph;

  _calcScaling();
}

bool glTextTTBitmap::_checkFont()
{
  const string facenames[MAXFONTFACES] =
    { "Normal", "Bold", "Italic", "Bold_Italic" };

  ttfont *tf = &(fonts[FontIndex][Face][SizeIndex]);
  tf->fontname = fonts[FontIndex][Face][0].fontname;
  if (!tf->fontname.size()) {
    //cerr << "glTextTTBitmap: zero fontname" << endl;
    return false;
  }
  if (!(tf->created)) {
    cerr << "glTextTTBitmap: Creating font for Family:"
        << FamilyName[FontIndex] << " Face:" << facenames[Face] << " Size:"
        << Sizes[SizeIndex] << endl;
    tf->pfont = new FTGLBitmapFont(tf->fontname.c_str());
    FT_Error error = tf->pfont->Error();
    if (error != 0) {
      cerr << "glTextTTBitmap:ERROR creating font from:" << tf->fontname
          << endl;
      delete tf->pfont;
      tf->pfont = 0;
      return false;
    }
    // create the actual glyph
    tf->pfont->FaceSize(Sizes[SizeIndex], 85);

    tf->created = true;
  }

  return true;
}

bool glTextTTBitmap::drawChar(const int c, const float x, const float y,
    const float a)
{
  char s[2];
  s[0] = (char) c;
  s[1] = '\0';

  return drawStr(s, x, y, a);
}

bool glTextTTBitmap::drawStr(const char* s, const float x, const float y,
    const float a)
{
  if (!s)
    return false;

  if (hardcopy) {
    output->addStr(s, x, y, a);
  } else {
    glPushMatrix();

    glRasterPos2f(x, y);

    if (fonts[FontIndex][Face][SizeIndex].pfont) {
      fonts[FontIndex][Face][SizeIndex].pfont->Render(s);
    } else {
      cerr << "Bad pfont" << endl;
    }
    glPopMatrix();

  }

  return true;
}

bool glTextTTBitmap::getStringSize(const char* s, float& w, float& h)
{
  if (!s) {
    w = 0;
    h = 0;
    return false;
  }
  ttfont *tf = &fonts[FontIndex][Face][SizeIndex];

  float psxscale = 1.0;
  float psyscale = 1.0;
  if (hardcopy) {
    psxscale = fonts[FontIndex][Face][0].psxscale;
    psyscale = fonts[FontIndex][Face][0].psyscale;
  }
  w = h = 0;
  if (tf->pfont) {
    float llx, lly, llz, urx, ury, urz;
    tf->pfont->BBox(s, llx, lly, llz, urx, ury, urz);
    w = (urx - llx) * scalex * xscale * pixWidth * psxscale;
    h = (ury - lly) * scaley * xscale * pixHeight * psyscale;
  }
  return true;
}

bool glTextTTBitmap::getCharSize(const int c, float& w, float& h)
{
  char s[2];
  s[0] = (char) c;
  s[1] = '\0';

  return getStringSize(s, w, h);
}

bool glTextTTBitmap::getMaxCharSize(float& w, float& h)
{
  getCharSize('M', w, h);
  return true;
}

string glTextTTBitmap::getPsName() const
{
  return fonts[FontIndex][Face][0].psname;
}
