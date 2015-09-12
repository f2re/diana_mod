/*
  libpets2 - presentation and editing of time series

  $Id: ptFontCollX.cc 2937 2009-12-09 16:39:35Z dages $

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


// ptFontCollX.cc : Class definition for X-fonts

#include "ptFontColl.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <string.h>

using namespace miutil;


#ifdef linux
// linux fonts
const char HEADER1_FONT[] =
"-*-helvetica-medium-r-*-*-18-*-*-*-p-*-iso8859-1";
const char HEADER2_FONT[] =
"-*-helvetica-medium-r-*-*-17-*-*-*-p-*-iso8859-1";
const char HEADER3_FONT[] =
"-*-helvetica-medium-r-*-*-14-*-*-*-p-*-iso8859-1";
const char NORMAL_FONT[] =
"-*-helvetica-medium-r-*-*-12-*-*-*-p-*-iso8859-1";
const char SUBSCRIPT_FONT[] =
"-*-helvetica-medium-r-*-*-8-*-*-*-p-*-iso8859-1";
#else
// SGI fonts
const char HEADER1_FONT[] =
"-adobe-palatino-medium-r-normal--24-0-100-100-p-100-iso8859-1";
const char HEADER2_FONT[] =
"-adobe-helvetica-medium-r-normal--16-0-100-100-p-100-iso8859-1";
const char HEADER3_FONT[] =
"-adobe-helvetica-medium-r-narrow--0-110-100-100-p-100-iso8859-1";
const char NORMAL_FONT[] =
"-adobe-helvetica-medium-r-narrow--0-90-100-100-p-100-iso8859-1";
const char SUBSCRIPT_FONT[] =
"-adobe-helvetica-medium-r-narrow--0-70-100-100-p-100-iso8859-1";
#endif

//initialization of static members
// int FontCollectionX::baseList[MAXFONTSIZES];  // zero by default
// bool FontCollectionX::fontList[MAXFONTSIZES]; // set to false by default
// XFontStruct* FontCollectionX::fontInfo[MAXFONTSIZES];  // NULL by default

FontCollectionX::FontCollectionX(Display* display)
  : FontCollection(), dpy(display)
{
  for (int i=0; i<MAXFONTSIZES; i++){
    baseList[i]= 0;
    fontList[i]= false;
    fontInfo[i]= 0;
  }
  //_makeRasterFont(Size);
}

void FontCollectionX::_makeRasterFont(const ptFontSize size)
{
    Font id;
    unsigned int first, last;
    XFontStruct *info;

#ifdef DEBUG
    cout << "before XLoadQueryFont" << endl;
#endif
    switch(size) {
    case HEADER1 :
      info = XLoadQueryFont(dpy,HEADER1_FONT);
      break;
    case HEADER2 :
      info = XLoadQueryFont(dpy,HEADER2_FONT);
      break;
    case HEADER3 :
      info = XLoadQueryFont(dpy,HEADER3_FONT);
      break;
    case NORMAL :
      info = XLoadQueryFont(dpy,NORMAL_FONT);
      break;
    case SUBSCRIPT :
      info = XLoadQueryFont(dpy,SUBSCRIPT_FONT);
      break;
    }
    if (info == NULL) {
      cerr << "Fontsize " << size << " not found\n" << endl;
      //exit (0);    //  should not exit from program .....
    }
#ifdef DEBUG
      cout << "after XLoadQueryFont" << endl;
#endif

    id = info->fid;
    first = info->min_char_or_byte2;
    last = info->max_char_or_byte2;

//     cerr << "ptFontCollX making new font display lists for size:"
// 	 << size << endl;

    base = glGenLists((GLuint) last+1);
    if (base == 0) {
      cerr << "out of display lists\n" << endl;
      //      exit (0);   //do something smart (use previous font or something)
    }
    baseList[size] = base;
    fontList[size] = true;
    fontInfo[size] = info;
#ifdef DEBUG
    cout << "before glXUseXFont" << endl;
#endif
    glXUseXFont(id, first, last-first+1, base+first);
#ifdef DEBUG
    cout << "after glXUseXFont" << endl;
#endif
/*    *height = info->ascent + info->descent;
    *width = info->max_bounds.width;  */
}

int FontCollectionX::printChar(const int c, const float x, const float y )
{
  char s[2];
  s[0] = (char)c;
  s[1] = '\0';
  glRasterPos2f(x,y);
  glPushAttrib (GL_LIST_BIT);
  glListBase((GLuint)base);
  glCallLists(1, GL_UNSIGNED_BYTE, (GLubyte*)s);
  glPopAttrib ();
  return 0;
}

int FontCollectionX::printStr(const char* s, const float x, const float y )
{
  if (!s) return 0;
  glRasterPos2f(x,y);
  glPushAttrib (GL_LIST_BIT);
  glListBase((GLuint)base);
  glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte*)s);
  glPopAttrib ();
  return 1;
}

int FontCollectionX::getCharSize(const int c, float& w, float& h)
{
  char s[2];
  s[0] = (char)c;
  s[1] = '\0';
  w = XTextWidth(fontInfo[Size],s,1)*pixWidth;
  h = (fontInfo[Size]->ascent+fontInfo[Size]->descent)*pixHeight;
  //cout << "getCharSize  size:" << Size << " w:" << w << " h:" << h << endl;
  return 1;
}

int FontCollectionX::getMaxCharSize(float& w, float& h)
{
  float mw, mh;
  getCharSize('M',mw,mh);
  getCharSize('f',w,h);
  if (mw>w) w=mw;
  if (mh>h) h=mh;
  return 1;
}

int FontCollectionX::getStringSize(const char* s, float& w, float& h)
{
  if (!s){
    w=0;
    h=0;
    return 0;
  }
//   cout << "pixWidth:" << pixWidth << " pixHeight:" << pixHeight << endl;
  w = XTextWidth(fontInfo[Size],s,strlen(s))*pixWidth;
  h = (fontInfo[Size]->ascent+fontInfo[Size]->descent)*pixHeight;
  return 1;
}

