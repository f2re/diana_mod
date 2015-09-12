/*
  libpets2 - presentation and editing of time series

  $Id: ptFontCollGLPR.cc 2937 2009-12-09 16:39:35Z dages $

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


//

#include "ptFontColl.h"

using namespace miutil;

#ifdef WITH_GLP
#include <glp.h>

static GLPenum GLPerr;
#define GLPERR \
if((GLPerr = glpGetError()) != GLP_NONE) {cout<<">>>>\tGLP error " << GLPerr << " (" << glpGetErrorMsg(GLPerr) << ") at line "<< __LINE__ << " in " << __FILE__ << endl;}


FontCollectionGLPR::FontCollectionGLPR()
  : FontCollection(),
    fscale(1.0)
{
  const float defFontScales[MAXFONTSIZES] = {
    1.0,
    20.0,
    15.0,
    12.0,
    10.0,
     8.0,
    20.0,
    20.0,
    20.0,
    20.0,
    20.0
  };
  for (int i=0; i<MAXFONTSIZES; i++) fontScales[i] = defFontScales[i];

  fontFamily[0] = GLP_FONTFAMILY_HELVETICA;
  fontFamily[1] = GLP_FONTFAMILY_TIMES;
  fontFamily[2] = GLP_FONTFAMILY_COURIER;
  fontFamily[3] = GLP_FONTFAMILY_SYMBOL;
  //fontFamily[4] = GLP_FONTFAMILY_STICK;
  numFonts = 4;

  fontFaces[0] = GLP_FONTFACE_REGULAR;
  fontFaces[1] = GLP_FONTFACE_BOLD;
  fontFaces[2] = GLP_FONTFACE_ITALIC;

  setFont(0);
  setFontFace(F_NORMAL);
  setFontSize(NORMAL);
};






int FontCollectionGLPR::printChar(const int c, const float x, const float y)
{
  char s[2];
  glpBegin(GLP_2D_COLOR,100); GLPERR;
  glpSetStyle(GL_TRUE,GL_TRUE,GL_TRUE,1.0,GL_TRUE); GLPERR;
  glpSetFont(fontFamily[FontIndex], fontface, fscale); GLPERR;
  glRasterPos2f(x,y);
  s[0] = c; s[1] = '\0';
  glpText(s,0); GLPERR;
  glpEnd(); GLPERR;
  return(1);
}


int FontCollectionGLPR::printStr(const char* s, const float x, const float y)
{
  if (!s) return(0);
//   cout << "Skriver ut :" << s << " med fontFamily:" << FontIndex
//        << " fontface:" << fontface << " fontsize:" << fscale << endl;
  glpBegin(GLP_2D_COLOR,50*strlen(s)); GLPERR;
  glpSetStyle(GL_TRUE,GL_TRUE,GL_TRUE,1.0,GL_TRUE); GLPERR;
  glpSetFont(fontFamily[FontIndex], fontface, fscale); GLPERR;
  glRasterPos2f(x,y);
  glpText((char*)s,0); GLPERR;
  glpEnd(); GLPERR;
  return(1);
}


int FontCollectionGLPR::getCharSize(const int c, float& w, float& h)
{
  w=fscale;
  h=fscale*1.5;
//   float vec[8],xscale,yscale;
//   glPushMatrix();
//   if (Mode == M_BITMAP) {
//     //glcScale(1.0,1.0);
//     xscale=yscale=fScale;
//     //if (pixWidth>0) xscale = fScale*pixWidth;
//     //if (pixHeight>0) yscale = fScale*pixHeight;
//     glcLoadIdentity();
//     glcGetCharMetric(c,GLC_BOUNDS,vec); // Feilsjekk!!!
//     w = int((vec[2] - vec[0])*xscale);
//     h = int((vec[7] - vec[1])*yscale);
//   } else {
//     //glScalef(fScale,fScale,1.0);
//     glcGetCharMetric(c,GLC_BOUNDS,vec); // Feilsjekk!!!
//     w = int((vec[2] - vec[0])*fScale);
//     h = int((vec[7] - vec[1])*fScale);
//   }
//   glPopMatrix();
#ifdef DEBUG
  cout << "getCharSize: w=" << w << "  h=" << h << endl;
#endif
  return(1);
}

int FontCollectionGLPR::getMaxCharSize(float& w, float& h)
{
  w=fscale;
  h=fscale*1.5;
//   if (!charSizes[FontIndex][Size][0]) {
//     int ch1,cw1,ch2,cw2;
//     getCharSize('M',cw1, ch1);
//     getCharSize('f',cw2, ch2);
//     if (cw1 > cw2) cw2 = cw1;
//     if (ch1 > ch2) ch2 = ch1;
//     charSizes[FontIndex][Size][0] = cw2;
//     charSizes[FontIndex][Size][1] = ch2;
//   }
//   w = charSizes[FontIndex][Size][0];
//   h = charSizes[FontIndex][Size][1];
#ifdef DEBUG
  cout << "Current font sizes:" << w << " " << h << endl;
  cout << "Current font scaling:" << fscale << endl;
#endif
  return(1);
}

int FontCollectionGLPR::getStringSize(const char* s, float& w, float& h)
{
  w = h = 0;
  if (!s) return(0);
  h=fscale*1.5;
  w=strlen(s)*fscale;
//   float vec[8],xscale,yscale;
//   w = h = 0;
//   glPushMatrix();
//   if (Mode == M_BITMAP) {
//     xscale=yscale=fScale;
//     glcLoadIdentity();
//     if (glcMeasureString(false, s) != 0) {
//       glcGetStringMetric(GLC_BOUNDS, vec);
//     } else {
//       glPopMatrix();
//       return(0);
//     }
//     w = int((vec[2] - vec[0])*xscale*1.2);
//     h = int((vec[7] - vec[1])*yscale);
//   } else {
//     //glScalef(fScale,fScale,1.0);
//     if (glcMeasureString(false, s) != 0) {
//       glcGetStringMetric(GLC_BOUNDS, vec);
//     } else {
//       glPopMatrix();
//       return(0);
//     }
//     w = int((vec[2] - vec[0])*fScale);
//     h = int((vec[7] - vec[1])*fScale);
//   }
//   glPopMatrix();
#ifdef DEBUG
  cout << "getStringSize:" << s <<  " w=" << w << "  h=" << h << endl;
#endif
  return(1);
}

#endif

