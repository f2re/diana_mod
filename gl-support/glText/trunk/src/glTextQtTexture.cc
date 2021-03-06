/*
 libglText - OpenGL Text Rendering Library

 $Id: glTextQtTexture.cc 4314 2013-12-18 08:25:30Z juergens $

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

// glTextQtTexture.cc : Class definition for GL-rendered qt-fonts

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fstream>
#include "glTextQtTexture.h"
#include "textrenderer.h"
#include <iostream>
#include <math.h>
#include <QFont>

#include <glp/GLP.h>
#include <glp/glpfile.h>

using namespace std;

glTextQtTexture::qtfont::qtfont() :
  created(false), qfont(0), pfont(0)
{
}

glTextQtTexture::qtfont::~qtfont()
{
  delete qfont;
  delete pfont;
}

glTextQtTexture::glTextQtTexture() :
  glText(), scalex(1.0), scaley(1.0), xscale(1.0)
{
#ifdef DEBUGPRINT
  cerr << "glTextQtTexture::constructor" << endl;
#endif
  initialised = true;
}

glTextQtTexture::~glTextQtTexture()
{
}

bool glTextQtTexture::testDefineFonts(string path)
{
  string famn;
  string fontn;

  famn = "Arial";
  fontn = "Arial";
  defineFont(famn, fontn, glText::F_NORMAL, 10, "Arial");
  // italic
  defineFont(famn, fontn, glText::F_ITALIC, 10, "Arial-Oblique");
  // bold
  defineFont(famn, fontn, glText::F_BOLD, 10, "Arial-Bold");
  // bold-italic
  defineFont(famn, fontn, glText::F_BOLD_ITALIC, 10, "Arial-BoldOblique");

  return true;
}

bool glTextQtTexture::defineFont(const string family, const string fname,
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
bool glTextQtTexture::set(const string fam, const FontFace face,
    const float size)
{
  int fidx;
  if (_findFamily(fam, fidx)) {
    FontIndex = fidx;
    Face = face;
    reqSize = size;
    if (_calcScaling()) {
      if (hardcopy)
        output->setFont(fonts[FontIndex][Face][0].psname,
            (wysiwyg ? getFontSize() : reqSize * 1.3));
      return true;
    }
  }
  cerr << "glTextQtTexture::set " << fam << " " << size << " returned false"
      << endl;
  return false;
}

bool glTextQtTexture::setFont(const string family)
{
  int fidx;
  if (_findFamily(family, fidx)) {
    FontIndex = fidx;
    if (_calcScaling()) {
      if (hardcopy)
        output->setFont(fonts[FontIndex][Face][0].psname,
            (wysiwyg ? getFontSize() : reqSize * 1.3));
      return true;
    }
  }
  return false;
}

bool glTextQtTexture::setFontFace(const FontFace face)
{
  Face = face;
  if (_calcScaling()) {
    if (hardcopy)
      output->setFont(fonts[FontIndex][Face][0].psname,
          (wysiwyg ? getFontSize() : reqSize * 1.3));
    return true;
  }
  return false;
}

bool glTextQtTexture::setFontSize(const float size)
{
  reqSize = size;
  if (_calcScaling()) {
    if (hardcopy)
      output->setFont(fonts[FontIndex][Face][0].psname,
          (wysiwyg ? getFontSize() : reqSize * 1.3));
    return true;
  }
  return false;
}

void glTextQtTexture::setScalingType(const FontScaling fs)
{
  scaletype = fs;
  _calcScaling();
}

bool glTextQtTexture::_calcScaling()
{
  if (numFonts == 0)
    return false;

  pixWidth = glWidth / vpWidth;
  pixHeight = glHeight / vpHeight;

  if (scaletype == S_FIXEDSIZE) {
    scalex = pixWidth;
    scaley = pixHeight;
  } else if (scaletype == S_VIEWPORTSCALED) {
    scalex = glWidth / 1000.0;
    scaley = scalex * pixHeight / pixWidth;
  }

  int size = static_cast<int> (roundf(reqSize));
  if (size < 1)
    size = 1;

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

void glTextQtTexture::setGlSize(const float glw, const float glh)
{
  glWidth = glw;
  glHeight = glh;

  _calcScaling();
}

/*void glTextQtTexture::setGlSize(const float x1, const float x2, const float y1, const float y2)
 {
 glx1 = x1;
 glx2 = x2;
 gly1 = y1;
 gly2 = y2;
 glWidth = glx2 - glx1;
 glHeight = gly2 - gly1;

 _calcScaling();
 }*/

void glTextQtTexture::setVpSize(const float vpw, const float vph)
{
  vpWidth = vpw;
  vpHeight = vph;

  _calcScaling();
}

bool glTextQtTexture::_checkFont()
{
  const string facenames[MAXFONTFACES] =
    { "Normal", "Bold", "Italic", "Bold_Italic" };

  qtfont *tf = &(fonts[FontIndex][Face][SizeIndex]);
  tf->fontname = fonts[FontIndex][Face][0].fontname;

  if (!tf->fontname.size()) {
    //cerr << "glTextQtTexture: zero fontname" << endl;
    return false;
  }
  if (!(tf->created)) {
    cerr << "glTextQtTexture: Creating font for Family:"
        << FamilyName[FontIndex] << " Face:" << facenames[Face] << " Size:"
        << Sizes[SizeIndex] << endl;

    tf->pfont = new TextRenderer();
    tf->qfont = new QFont(tf->fontname.c_str()/*"Helvetica"*/);
    tf->qfont->setPointSize(Sizes[SizeIndex]);
    tf->qfont->setBold(Face == F_BOLD || Face == F_BOLD_ITALIC);
    tf->qfont->setItalic(Face == F_ITALIC || Face == F_BOLD_ITALIC);
    tf->pfont->set_geometry(vpWidth, vpHeight, glWidth, glHeight);
    tf->pfont->setup(*(tf->qfont));
    tf->created = true;
  }

  tf->pfont->set_geometry(vpWidth, vpHeight, glWidth, glHeight);

  return true;
}

bool glTextQtTexture::drawChar(const int c, const float x, const float y,
    const float a)
{
  char s[2];
  s[0] = (char) c;
  s[1] = '\0';

  return drawStr(s, x, y, a);
}

bool glTextQtTexture::drawStr(const char* s, const float x, const float y,
    const float a)
{
  if (!s)
    return false;
  if (hardcopy) {
    output->addStr(s, x, y, a);
  } else {
    QString string(s);
    //float wx = (x - glx1) / pixWidth;
    //float wy = (y - gly1) / pixHeight;
//    METLIBS_LOG_WARN("obsData " << text );
    fonts[FontIndex][Face][SizeIndex].pfont->print(x, y, a, string);

    /*
     // draw the bounding box (left,bottom corner is x,y)
     float w, h;
     getStringSize(s,w,h);
     float glx1=x, gly1=y, glx2=x+w, gly2=y+h;
     //cerr << glx1 << " " << gly1 << " " << glx2 << " " << gly2 << " " << endl;
     glLineWidth(1);
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     glBegin(GL_LINE_LOOP);
     glVertex2f(glx1, gly1);
     glVertex2f(glx2, gly1);
     glVertex2f(glx2, gly2);
     glVertex2f(glx1, gly2);
     glEnd();
     */

  }

  return true;
}

bool glTextQtTexture::getStringSize(const char* s, float& w, float& h)
{
  w = h = 0;
  if (!s) {
    return false;
  }
  qtfont *tf = &fonts[FontIndex][Face][SizeIndex];

  if (tf->created) {
    QFontMetrics fm(*(tf->qfont));

    w = fm.width(s) * scalex * xscale;
    h = fm.height() * scaley * xscale * 0.75;

    return true;
  }
  return false;
}

bool glTextQtTexture::getCharSize(const int c, float& w, float& h)
{
  char s[2];
  s[0] = (char) c;
  s[1] = '\0';

  return getStringSize(s, w, h);
}

bool glTextQtTexture::getMaxCharSize(float& w, float& h)
{
  getCharSize('M', w, h);
  return true;
}

string glTextQtTexture::getPsName() const
{
  return fonts[FontIndex][Face][SizeIndex].psname;
}
