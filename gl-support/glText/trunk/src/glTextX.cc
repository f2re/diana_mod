/*
 libglText - OpenGL Text Rendering Library

 $Id: glTextX.cc 4314 2013-12-18 08:25:30Z juergens $

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

// glTextX.cc : Class definition for GL-rendered X-fonts

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "glTextX.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <glp/GLP.h>
#include <glp/glpfile.h>
#include <X11/Xatom.h>

#include <stdlib.h>
#include <string.h>

using namespace std;

struct font_info {
  string fndry;
  string fmly;
  string wght;
  string slant;
  string sWdth;
  string adstyl;
  string pxlsz;
  string ptSz;
  string resx;
  string resy;
  string spc;
  string avgWdth;
  string rgstry;
  string encdng;
};

bool parse_fontname(const string& fn, font_info& fi)
{
  int splitnumber = 0;
  string s = "-";
  vector<string> vec;
  int len = fn.length();
  if (len == 0)
    return false;

  int start = 1;
  while (start >= 0 && start < len) {
    int stop = fn.find_first_of(s, start);
    if (stop < 0 || stop > len)
      stop = len;
    vec.push_back(fn.substr(start, stop - start));

    start = stop + 1;
  }

  if (vec.size() < 14)
    return false;

  fi.fndry = vec[0]; /* foundary */
  fi.fmly = vec[1]; /* family */
  fi.wght = vec[2]; /* Weight */
  fi.slant = vec[3]; /* Slant */
  fi.sWdth = vec[4]; /* sWdth */
  fi.adstyl = vec[5]; /* adstyl */
  fi.pxlsz = vec[6]; /* pxlsz */
  fi.ptSz = vec[7]; /* ptSz */
  fi.resx = vec[8]; /* resx */
  fi.resy = vec[9]; /* resy */
  fi.spc = vec[10]; /* spc */
  fi.avgWdth = vec[11];/* avgWdth */
  fi.rgstry = vec[12]; /* rgstry */
  fi.encdng = vec[13]; /* encdng */

  return true;
}

glTextX::glTextX() :
  glText(), dpy(0)
{
  dpy = XOpenDisplay(0); // use environment-variable DISPLAY
  if (!dpy)
    cerr << "glTextX error: Could not open display" << endl;
  else
    initialised = true;
}

glTextX::glTextX(string display_name) :
  glText(), dpy(0)
{
  if (display_name.length() > 0) // use named DISPLAY
    dpy = XOpenDisplay(const_cast<char*> (display_name.c_str()));
  else
    dpy = XOpenDisplay(0);
  if (!dpy)
    cerr << "glTextX error: Could not open display:" << display_name << endl;
  else
    initialised = true;
}

glTextX::glTextX(Display* display) :
  glText(), dpy(display)
{
  initialised = true;
}

glTextX::~glTextX()
{
}

void glTextX::initialise(Display* display)
{
  if (display) {
    dpy = display;
    initialised = true;
  }
}

bool glTextX::defineFonts(const string pattern, const string family,
    const string psname)
{
//  cerr << "  glTextX::defineFonts, pattern:" << pattern
//      << " family:" << family << " psname:" << psname << endl;

  if(!initialised) {
     return false;
  }

  const int max_names = 100;
  char ** fontnamelist;
  int actual_count_return;

  fontnamelist = XListFonts(dpy, pattern.c_str(), max_names,
      &actual_count_return);

  if (fontnamelist != NULL) {
    for (int i = 0; i < actual_count_return; i++) {

      font_info fi;
      bool b = parse_fontname(fontnamelist[i], fi);
      if (!b)
        continue;

      FontFace ff = F_NORMAL;
      bool isbold = (fi.wght == "bold" || fi.wght == "Bold");
      bool isital = (fi.slant == "o" || fi.slant == "i" || fi.slant == "O"
          || fi.slant == "I");
      if (isbold && isital)
        ff = F_BOLD_ITALIC;
      else if (isbold)
        ff = F_BOLD;
      else if (isital)
        ff = F_ITALIC;

      b = defineFont(family, fontnamelist[i], ff, atoi(fi.pxlsz.c_str()),
          psname);
      if (!b) {
        cerr << "ERROR, can not define font " << fontnamelist[i]
            << " to family " << family << " and face " << ff << endl;
      }
    }

    XFreeFontNames(fontnamelist);
  } else {
    //     cerr << "EMPTY fontnamelist" << endl;
  }

  return true;
}

bool glTextX::testDefineFonts(string path)
{

  defineFonts("*-arial-medium-r*UTF-8", "Arial", "Arial");
  defineFonts("*-arial-bold-r*UTF-8", "Arial", "Arial-Bold");
  defineFonts("*-arial-medium-o*UTF-8", "Arial",
      "Arial-Oblique");
  defineFonts("*-arial-bold-o*UTF-8", "Arial",
      "Arial-BoldOblique");

  defineFonts("*-courier-medium-r*iso8859-1", "Courier", "Courier");
  defineFonts("*-courier-bold-r*iso8859-1", "Courier", "Courier-Bold");
  defineFonts("*-courier-medium-o*iso8859-1", "Courier", "Courier-Oblique");
  defineFonts("*-courier-bold-o*iso8859-1", "Courier", "Courier-BoldOblique");

  return true;
}

bool glTextX::defineFont(const string family, const string fname,
    const FontFace face, const int size, const string psname,
    const float psxscale, const float psyscale)
{
  const string facenames[MAXFONTFACES] =
    { "Normal", "Bold", "Italic", "Bold_Italic" };

  //   cerr << " defineFont, family=" << family
  //        << " fname=" << fname
  //        << " size=" << size
  //        << " psname=" << psname
  //        << endl;

  int fidx, sidx, i, j;
  if (!_findFamily(family, fidx))
    if (!_addFamily(family, fidx))
      return false;
  //   cerr << "fidx:" << fidx << endl;

  if (!_findSize(size, sidx, true))
    if (!_addSize(size, sidx))
      return false;
  //   cerr << "sidx:" << sidx << endl;

  for (i = 0; i < MAXFONTS; i++)
    for (j = 0; j < MAXFONTFACES; j++)
      fonts[i][j][sidx].size = size;

  fonts[fidx][face][sidx].fontname = fname;
  fonts[fidx][face][sidx].psname = psname;
  fonts[fidx][face][sidx].psxscale = psxscale;
  fonts[fidx][face][sidx].psyscale = psyscale;

  //    cerr << " Defined font from (" << fname
  //         << "), Family:" << family
  //         << " Face:" << facenames[face]
  //         << " Size:" << size
  //         << " Psname:" << psname << endl;

  return true;
}

bool glTextX::set(const string fam, const FontFace face, const float size)
{

  int fidx, sidx;
  if (_findFamily(fam, fidx)) {
    FontIndex = fidx;
    Face = face;
    if (_findDefinedSize(int(size + 0.5), sidx, false)) {
      SizeIndex = sidx;
      reqSize = size;
      if (_checkFont()) {
        if (hardcopy)
          output->setFont(fonts[FontIndex][Face][SizeIndex].psname,
              (wysiwyg ? getFontSize() : reqSize));
        return true;
      }
    }
  }
  return false;
}

// choose font, size and face
bool glTextX::setFont(const string family)
{
  int fidx;
  if (_findFamily(family, fidx)) {
    FontIndex = fidx;
    if (_checkFont()) {
      if (hardcopy)
        output->setFont(fonts[FontIndex][Face][SizeIndex].psname,
            (wysiwyg ? getFontSize() : reqSize));
      return true;
    }
  }
  return false;
}

bool glTextX::setFontFace(const FontFace face)
{
  if (fonts[FontIndex][face][SizeIndex].fontname.size()) {
    Face = face;
    if (_checkFont()) {
      if (hardcopy)
        output->setFont(fonts[FontIndex][Face][SizeIndex].psname,
            (wysiwyg ? getFontSize() : reqSize));
      return true;
    }
  }
  return false;
}

bool glTextX::setFontSize(const float size)
{
  int idx;
  if (_findDefinedSize(int(size + 0.5), idx, false)) {
    SizeIndex = idx;
    reqSize = size;
    if (_checkFont()) {
      if (hardcopy)
        output->setFont(fonts[FontIndex][Face][SizeIndex].psname,
            (wysiwyg ? getFontSize() : reqSize));
      return true;
    }
  }
  return false;
}

bool glTextX::_checkFont()
{
  xfont * xf = &fonts[FontIndex][Face][SizeIndex];
  if (!xf->fontname.size())
    return false;
  if (xf->created)
    base = xf->baseList;
  else
    return _makeRasterFont();
  return true;
}

bool glTextX::_findDefinedSize(const int size, int& sidx, const bool exact)
{
  if (!numFonts)
    return false;
  int diff = 100000, cdiff;
  for (int i = 0; i < MAXFONTSIZES; i++) {
    if (fonts[FontIndex][Face][i].fontname.size()) {
      cdiff = abs(size - fonts[FontIndex][Face][i].size);
      if (cdiff < diff) {
        diff = cdiff;
        sidx = i;
      }
    }
  }
  return (exact ? (diff == 0) : (diff < 1000));
}

bool glTextX::_makeRasterFont()
{
  Font id;
  unsigned int first, last;
  XFontStruct *info;
  xfont * xf = &fonts[FontIndex][Face][SizeIndex];

  if (!xf->fontname.size() || xf->created || !initialised)
    return false;

  cerr << "glTextX: Creating font:" << xf->fontname << endl;

  info = XLoadQueryFont(dpy, xf->fontname.c_str());

  if (!info) {
    cerr << "glTextX ERROR: Font " << xf->fontname << " not found" << endl;
    return false;
  }

  id = info->fid;
  first = info->min_char_or_byte2;
  last = info->max_char_or_byte2;

  base = glGenLists((GLuint) last + 1);
  if (base == 0) {
    cerr << "glTextX ERROR: out of display lists" << endl;
    return false;
  }
  xf->baseList = base;
  xf->created = true;
  xf->fontInfo = info;
  glXUseXFont(id, first, last - first + 1, base + first);
  return true;
}

bool glTextX::drawChar(const int c, const float x, const float y, const float a)
{
  char s[2];
  s[0] = (char) c;
  s[1] = '\0';

  if (hardcopy) {
    output->addStr(s, x, y, a);
  } else {
    glRasterPos2f(x, y);
    glPushAttrib(GL_LIST_BIT);
    glListBase((GLuint) base);
    glCallLists(1, GL_UNSIGNED_BYTE, (GLubyte*) s);
    glPopAttrib();
  }
  return true;
}

bool glTextX::drawStr(const char* s, const float x, const float y,
    const float a)
{
  if (!s)
    return 0;

  if (hardcopy) {
    output->addStr(s, x, y, a);
  } else {
    glRasterPos2f(x, y);
    glPushAttrib(GL_LIST_BIT);
    glListBase((GLuint) base);
    glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte*) s);
    glPopAttrib();

    /*
     // draw a rectangle showing the boundary box
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

bool glTextX::getCharSize(const int c, float& w, float& h)
{
  char s[2];
  s[0] = (char) c;
  s[1] = '\0';
  xfont *xf = &fonts[FontIndex][Face][SizeIndex];
  if (xf->fontInfo) {
    w = XTextWidth(xf->fontInfo, s, 1) * pixWidth;
    h = (xf->fontInfo->ascent + xf->fontInfo->descent) * pixHeight;
    //cout << "getCharSize  size:" << Size << " w:" << w << " h:" << h << endl;
    return true;
  } else
    return false;
}

bool glTextX::getMaxCharSize(float& w, float& h)
{
  float mw, mh;
  getCharSize('M', mw, mh);
  getCharSize('f', w, h);
  if (mw > w)
    w = mw;
  if (mh > h)
    h = mh;
  return true;
}

bool glTextX::getStringSize(const char* s, float& w, float& h)
{
  if (!s) {
    w = 0;
    h = 0;
    return 0;
  }
  xfont *xf = &fonts[FontIndex][Face][SizeIndex];
  if (xf->fontInfo) {
    w = XTextWidth(xf->fontInfo, s, strlen(s)) * pixWidth;
    h = (xf->fontInfo->ascent + xf->fontInfo->descent) * pixHeight;
    return true;
  } else
    return false;
}

string glTextX::getPsName() const
{
  return fonts[FontIndex][Face][SizeIndex].psname;
}
