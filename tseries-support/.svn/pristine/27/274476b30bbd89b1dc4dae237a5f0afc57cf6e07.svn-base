/*
  libpets2 - presentation and editing of time series

  $Id: ptFontCollGlc.cc 2937 2009-12-09 16:39:35Z dages $

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

FontCollectionGlc::FontCollectionGlc()
  : FontCollection(),
    drawStyle(GL_BITMAP), dl(GL_FALSE), contextID(0),
    fScale(1.0)
{
  const float defFontScales[MAXFONTSIZES] = {
    1.0,
    40.0,//35.0,//40.0
    30.0,//15.0,//30.0
    25.0,//15.0,//25.0
    20.0,//20.0
    15.0,//15.0
    20.0,
    20.0,
    20.0,
    20.0,
    20.0
  };
  for (int i=0; i<MAXFONTSIZES; i++) fontScales[i] = defFontScales[i];
  _Init();
};

FontCollectionGlc::~FontCollectionGlc()
{
#if defined(linux)
#else
  // release the context from the thread
  glcContext(0);
  // delete GLC fonts
  glcDeleteFont(fontIDs[FontIndex]);
  // delete GLC context
  glcDeleteContext(contextID);
#endif
};


//

int FontCollectionGlc::_Init() {
  //int i, index;

#if defined(linux)
#else
  if ((contextID = glcGenContext()) == 0) {
    cout << "FONTCOLLECTIONGLC: Died at glcGenContext" << endl;
    return 0;
  }
  if (glcIsContext(contextID) == GL_FALSE) {
    cout << "FONTCOLLECTIONGLC: Died at glcIsContext" << endl;
    return 0;
  }
  glcContext(contextID);
  if (glcGetError() != GL_NO_ERROR) {
    cout << "FONTCOLLECTIONGLC: Died at glcContext" << endl;
    return 0;
  }
//   glcPrependCatalog("/usr/lib/X11/fonts/100dpi/fonts.dir");
//   if (glcGetError() != GL_NO_ERROR) {
//     cout << "FONTCOLLECTIONGLC: Died at glcPrependCatalog" << endl;
//     return 0;
//   }
  //glcRemoveCatalog(1);

//   numFonts = glcGeti(GLC_MASTER_COUNT);
//   if (glcGetError() != GL_NO_ERROR) {
//     cout << "FONTCOLLECTIONGLC: Died at glcGeti" << endl;
//     return 0;
//   }
//   if (numFonts > MAXFONTS) {
//     numFonts = MAXFONTS;
//   }
// #ifdef DEBUG
//   cout << "FONTCOLLECTIONGLC: Number of font masters:" << numFonts << endl;
// #endif

//   index = 0;
//   for (i = 0; i < numFonts; i++) {
//     FamilyName[i] = (char*)glcGetMasterc(i, GLC_FAMILY);
// #ifdef DEBUG
//     cout << "Family: " << FamilyName[i] << endl;
// #endif
//     fontIDs[index] = glcGenFontID();
//     if (glcNewFontFromMaster(fontIDs[index], i) != fontIDs[index]) {
//       cout << "FONTCOLLECTIONGLC: Died at glcNewFontFromMaster" << endl;
//       return 0;
//     }
//     facesIndex[index] = 0;
//     facesTotal[index] = glcGetFonti(fontIDs[index], GLC_FACE_COUNT);
//     index++;
//   }

  numFonts = 1;
  fontIDs[0] = glcGenFontID();
  if (glcNewFontFromFamily(fontIDs[0], "ITC Avant Garde Gothic") != fontIDs[0]) {
    cout << "Died at glcNewFontFromFamily" << endl; //Courier
    return 0;
  }

  setPrintMode(M_BITMAP);
  //setPrintMode(M_LINE);
  setFont(0);
  setFontSize(NORMAL);
  //_FontMap_Adobe(fontIDs[FontIndex]);
  //glcFontMap(fontIDs[FontIndex], 0x20, "SPACE");
  //glcFontMap(fontIDs[FontIndex], 0xF8, "LATIN SMALL LETTER O WITH STROKE");

  //  GLint f = fontIDs[FontIndex];
//   if (0) {
//   glcFontMap(f, 0x61, "LATIN CAPITAL LIGATURE AE");
//   glcFontMap(f, 0x62, "FEMININE ORDINAL INDICATOR");
//   glcFontMap(f, 0x63, "LATIN CAPITAL LETTER L WITH STROKE");
//   glcFontMap(f, 0x64, "LATIN CAPITAL LETTER O WITH STROKE");
//   glcFontMap(f, 0x65, "LATIN CAPITAL LIGATURE OE");
//   glcFontMap(f, 0x66, "MASCULINE ORDINAL INDICATOR");
//   glcFontMap(f, 0x67, "LATIN SMALL LIGATURE AE");
//   glcFontMap(f, 0x68, "LATIN SMALL LETTER DOTLESS I");
//   glcFontMap(f, 0x69, "LATIN SMALL LETTER L WITH STROKE");
//   glcFontMap(f, 0x70, "LATIN SMALL LETTER O WITH STROKE");
//   glcFontMap(f, 0x71, "LATIN SMALL LIGATURE OE");
//   glcFontMap(f, 0x72, "LATIN SMALL LETTER SHARP S");
//   }
#endif
  return(1);
}


int FontCollectionGlc::setPrintMode(const ptPrintMode mode)
{
#if defined(linux)
#else
  Mode = mode;
  if (Mode==M_BITMAP) {
    drawStyle = GLC_BITMAP;
  } else {
    drawStyle = GLC_TRIANGLE;
  }
  glcRenderStyle(drawStyle);
#endif
  return(1);
}

int FontCollectionGlc::makeFont(const miString family)
{
  return(1);
}

int FontCollectionGlc::defineFontSize(const int index,
				      const ptFontSize,
				      const float size)
{
  return(1);
}


int FontCollectionGlc::printChar(const int c, const float x, const float y)
{
#if defined(linux)
#else
  float xscale,yscale;
  if (drawStyle==GLC_BITMAP) {
    glcLoadIdentity();
    xscale=yscale=fScale;
    if (pixWidth>0) xscale = fScale/pixWidth;
    if (pixHeight>0) yscale = fScale/pixHeight;
    glcScale(xscale,yscale);
    glRasterPos2f(x,y);
    glcRenderChar(c);
  } else {
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(fScale,fScale,0.0);
    glcRenderChar(c);
    glPopMatrix();
  }
#endif
  return(1);
}

int FontCollectionGlc::printStr(const char* s, const float x, const float y)
{
#if defined(linux)
#else
  float xscale,yscale;
  if (!s) return(0);
  if (drawStyle==GLC_BITMAP) {
    glcLoadIdentity();
    xscale=yscale=fScale;
    if (pixWidth>0) xscale = fScale/pixWidth;
    //if (xscale>fScale) xscale = fScale;
    if (pixHeight>0) yscale = fScale/pixHeight;
    //if (yscale>fScale) yscale = fScale;
    glcScale(xscale,yscale);
    glRasterPos2f(x,y);
    glcRenderString(s);
  } else {
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(fScale,fScale,0.0);
    glcRenderString(s);
//     for (int i=0; i<strlen(s); i++)
//       cout << "Character:" << s[i] << " =" << int(s[i]) << endl;
    glPopMatrix();
  }
#endif
  return(1);
}


int FontCollectionGlc::getCharSize(const int c, float& w, float& h)
{
#if defined(linux)
#else
  float vec[8],xscale,yscale;
  glPushMatrix();
  if (Mode == M_BITMAP) {
    //glcScale(1.0,1.0);
    xscale=yscale=fScale;
    //if (pixWidth>0) xscale = fScale*pixWidth;
    //if (pixHeight>0) yscale = fScale*pixHeight;
    glcLoadIdentity();
    glcGetCharMetric(c,GLC_BOUNDS,vec); // Feilsjekk!!!
    w = int((vec[2] - vec[0])*xscale);
    h = int((vec[7] - vec[1])*yscale);
  } else {
    //glScalef(fScale,fScale,1.0);
    glcGetCharMetric(c,GLC_BOUNDS,vec); // Feilsjekk!!!
    w = (vec[2] - vec[0])*fScale;
    h = (vec[7] - vec[1])*fScale;
  }
  glPopMatrix();
#endif
#ifdef DEBUG
  cout << "getCharSize: w=" << w << "  h=" << h << endl;
#endif
  return(1);
}

int FontCollectionGlc::getMaxCharSize(float& w, float& h)
{
#if defined(linux)
#else
  if (!charSizes[FontIndex][Size][0]) {
    float ch1,cw1,ch2,cw2;
    getCharSize('M',cw1, ch1);
    getCharSize('f',cw2, ch2);
    if (cw1 > cw2) cw2 = cw1;
    if (ch1 > ch2) ch2 = ch1;
    charSizes[FontIndex][Size][0] = cw2;
    charSizes[FontIndex][Size][1] = ch2;
  }
  w = charSizes[FontIndex][Size][0];
  h = charSizes[FontIndex][Size][1];
#ifdef DEBUG
  cout << "Current font sizes:" << w << " " << h << endl;
  cout << "Current font scaling:" << fScale << endl;
#endif
#endif
  return(1);
}

int FontCollectionGlc::getStringSize(const char* s, float& w, float& h)
{
#if defined(linux)
#else
  float vec[8],xscale,yscale;
  w = h = 0;
  glPushMatrix();
  if (Mode == M_BITMAP) {
    //glcScale(fScale,fScale);
    xscale=yscale=fScale;
    //if (pixWidth>0) xscale = fScale*pixWidth;
    //if (pixHeight>0) yscale = fScale*pixHeight;
    glcLoadIdentity();
    if (glcMeasureString(false, s) != 0) {
      glcGetStringMetric(GLC_BOUNDS, vec);
    } else {
      glPopMatrix();
      return(0);
    }
    w = (vec[2] - vec[0])*xscale*1.2;
    h = (vec[7] - vec[1])*yscale;
  } else {
    //glScalef(fScale,fScale,1.0);
    if (glcMeasureString(false, s) != 0) {
      glcGetStringMetric(GLC_BOUNDS, vec);
    } else {
      glPopMatrix();
      return(0);
    }
    w = (vec[2] - vec[0])*fScale;
    h = (vec[7] - vec[1])*fScale;
  }
  glPopMatrix();
#endif
#ifdef DEBUG
  cout << "getStringSize:" << s <<  " w=" << w << "  h=" << h << endl;
#endif
  return(1);
}


void FontCollectionGlc::_FontMap_Adobe(GLint f) {
#if defined(linux)
#else
    const GLCenum stringType = glcGeti(GLC_STRING_TYPE);
    //const GLCchar *family;
    const char *family;

    glcStringType(GLC_UCS1);
    family = (const char*)glcGetFontc(f, GLC_FAMILY);
    //cout << "Family name:" << family << endl;
    if (!strcmp(family, "ITC Zapf Dingbats")) {
        _FontMap_AdobeDingbats(f);
    } else if (!strcmp(family, "Symbol")) {
        _FontMap_AdobeSymbol(f);
    } else {
        _FontMap_AdobeStandard(f);
    }
    glcStringType(stringType);
#endif
}


void FontCollectionGlc::_FontMap_AdobeStandard(GLint f) {
#if defined(linux)
#else
    GLint i;
    const GLint minMapped = glcGetFonti(f, GLC_MIN_MAPPED_CODE);
    const GLint maxMapped = glcGetFonti(f, GLC_MAX_MAPPED_CODE);
    const GLCenum stringType = glcGeti(GLC_STRING_TYPE);
    //cout << "minMapped/maxMapped:" << minMapped << " / " << maxMapped << endl;

    glcStringType(GLC_UCS1);
    for (i = minMapped ; i <= maxMapped ; ++i) {
      //cout << "FONTMAP[i]:" << i << endl;
      //glcFontMap(f, i, GLC_NONE);
      glcFontMap(f, i, "PERCENT SIGN");
    }
    glcFontMap(f, 0x20, "SPACE");
    glcFontMap(f, 0x21, "EXCLAMATION MARK");
    glcFontMap(f, 0x22, "QUOTATION MARK");
    glcFontMap(f, 0x23, "NUMBER SIGN");
    glcFontMap(f, 0x24, "DOLLAR SIGN");
    glcFontMap(f, 0x25, "PERCENT SIGN");
    glcFontMap(f, 0x26, "AMPERSAND");
    glcFontMap(f, 0x27, "RIGHT SINGLE QUOTATION MARK");
    glcFontMap(f, 0x28, "LEFT PARENTHESIS");
    glcFontMap(f, 0x29, "RIGHT PARENTHESIS");
    glcFontMap(f, 0x2A, "ASTERISK");
    glcFontMap(f, 0x2B, "PLUS SIGN");
    glcFontMap(f, 0x2C, "COMMA");
    glcFontMap(f, 0x2D, "HYPHEN-MINUS");
    glcFontMap(f, 0x2E, "FULL STOP");
    glcFontMap(f, 0x2F, "SOLIDUS");
    glcFontMap(f, 0x30, "DIGIT ZERO");
    glcFontMap(f, 0x31, "DIGIT ONE");
    glcFontMap(f, 0x32, "DIGIT TWO");
    glcFontMap(f, 0x33, "DIGIT THREE");
    glcFontMap(f, 0x34, "DIGIT FOUR");
    glcFontMap(f, 0x35, "DIGIT FIVE");
    glcFontMap(f, 0x36, "DIGIT SIX");
    glcFontMap(f, 0x37, "DIGIT SEVEN");
    glcFontMap(f, 0x38, "DIGIT EIGHT");
    glcFontMap(f, 0x39, "DIGIT NINE");
    glcFontMap(f, 0x3A, "COLON");
    glcFontMap(f, 0x3B, "SEMICOLON");
    glcFontMap(f, 0x3C, "LESS-THAN SIGN");
    glcFontMap(f, 0x3D, "EQUALS SIGN");
    glcFontMap(f, 0x3E, "GREATER-THAN SIGN");
    glcFontMap(f, 0x3F, "QUESTION MARK");
    glcFontMap(f, 0x40, "COMMERCIAL AT");
    glcFontMap(f, 0x41, "LATIN CAPITAL LETTER A");
    glcFontMap(f, 0x42, "LATIN CAPITAL LETTER B");
    glcFontMap(f, 0x43, "LATIN CAPITAL LETTER C");
    glcFontMap(f, 0x44, "LATIN CAPITAL LETTER D");
    glcFontMap(f, 0x45, "LATIN CAPITAL LETTER E");
    glcFontMap(f, 0x46, "LATIN CAPITAL LETTER F");
    glcFontMap(f, 0x47, "LATIN CAPITAL LETTER G");
    glcFontMap(f, 0x48, "LATIN CAPITAL LETTER H");
    glcFontMap(f, 0x49, "LATIN CAPITAL LETTER I");
    glcFontMap(f, 0x4A, "LATIN CAPITAL LETTER J");
    glcFontMap(f, 0x4B, "LATIN CAPITAL LETTER K");
    glcFontMap(f, 0x4C, "LATIN CAPITAL LETTER L");
    glcFontMap(f, 0x4D, "LATIN CAPITAL LETTER M");
    glcFontMap(f, 0x4E, "LATIN CAPITAL LETTER N");
    glcFontMap(f, 0x4F, "LATIN CAPITAL LETTER O");
    glcFontMap(f, 0x50, "LATIN CAPITAL LETTER P");
    glcFontMap(f, 0x51, "LATIN CAPITAL LETTER Q");
    glcFontMap(f, 0x52, "LATIN CAPITAL LETTER R");
    glcFontMap(f, 0x53, "LATIN CAPITAL LETTER S");
    glcFontMap(f, 0x54, "LATIN CAPITAL LETTER T");
    glcFontMap(f, 0x55, "LATIN CAPITAL LETTER U");
    glcFontMap(f, 0x56, "LATIN CAPITAL LETTER V");
    glcFontMap(f, 0x57, "LATIN CAPITAL LETTER W");
    glcFontMap(f, 0x58, "LATIN CAPITAL LETTER X");
    glcFontMap(f, 0x59, "LATIN CAPITAL LETTER Y");
    glcFontMap(f, 0x5A, "LATIN CAPITAL LETTER Z");
    glcFontMap(f, 0x5B, "LEFT SQUARE BRACKET");
    glcFontMap(f, 0x5C, "REVERSE SOLIDUS");
    glcFontMap(f, 0x5D, "RIGHT SQUARE BRACKET");
    glcFontMap(f, 0x5E, "CIRCUMFLEX ACCENT");
    glcFontMap(f, 0x5F, "LOW LINE");
    glcFontMap(f, 0x60, "LEFT SINGLE QUOTATION MARK");
    glcFontMap(f, 0x61, "LATIN SMALL LETTER A");
    glcFontMap(f, 0x62, "LATIN SMALL LETTER B");
    glcFontMap(f, 0x63, "LATIN SMALL LETTER C");
    glcFontMap(f, 0x64, "LATIN SMALL LETTER D");
    glcFontMap(f, 0x65, "LATIN SMALL LETTER E");
    glcFontMap(f, 0x66, "LATIN SMALL LETTER F");
    glcFontMap(f, 0x67, "LATIN SMALL LETTER G");
    glcFontMap(f, 0x68, "LATIN SMALL LETTER H");
    glcFontMap(f, 0x69, "LATIN SMALL LETTER I");
    glcFontMap(f, 0x6A, "LATIN SMALL LETTER J");
    glcFontMap(f, 0x6B, "LATIN SMALL LETTER K");
    glcFontMap(f, 0x6C, "LATIN SMALL LETTER L");
    glcFontMap(f, 0x6D, "LATIN SMALL LETTER M");
    glcFontMap(f, 0x6E, "LATIN SMALL LETTER N");
    glcFontMap(f, 0x6F, "LATIN SMALL LETTER O");
    glcFontMap(f, 0x70, "LATIN SMALL LETTER P");
    glcFontMap(f, 0x71, "LATIN SMALL LETTER Q");
    glcFontMap(f, 0x72, "LATIN SMALL LETTER R");
    glcFontMap(f, 0x73, "LATIN SMALL LETTER S");
    glcFontMap(f, 0x74, "LATIN SMALL LETTER T");
    glcFontMap(f, 0x75, "LATIN SMALL LETTER U");
    glcFontMap(f, 0x76, "LATIN SMALL LETTER V");
    glcFontMap(f, 0x77, "LATIN SMALL LETTER W");
    glcFontMap(f, 0x78, "LATIN SMALL LETTER X");
    glcFontMap(f, 0x79, "LATIN SMALL LETTER Y");
    glcFontMap(f, 0x7A, "LATIN SMALL LETTER Z");
    glcFontMap(f, 0x7B, "LEFT CURLY BRACKET");
    glcFontMap(f, 0x7C, "VERTICAL LINE");
    glcFontMap(f, 0x7D, "RIGHT CURLY BRACKET");
    glcFontMap(f, 0x7E, "TILDE");
    glcFontMap(f, 0xA1, "INVERTED EXCLAMATION MARK");
    glcFontMap(f, 0xA2, "CENT SIGN");
    glcFontMap(f, 0xA3, "POUND SIGN");
    glcFontMap(f, 0xA4, "FRACTION SLASH");
    glcFontMap(f, 0xA5, "YEN SIGN");
    glcFontMap(f, 0xA6, "LATIN SMALL LETTER F WITH HOOK");
    glcFontMap(f, 0xA7, "SECTION SIGN");
    glcFontMap(f, 0xA8, "CURRENCY SIGN");
    glcFontMap(f, 0xA9, "APOSTROPHE");
    glcFontMap(f, 0xAA, "LEFT DOUBLE QUOTATION MARK");
    glcFontMap(f, 0xAB, "LEFT-POINTING DOUBLE ANGLE QUOTATION MARK");
    glcFontMap(f, 0xAC, "SINGLE LEFT-POINTING ANGLE QUOTATION MARK");
    glcFontMap(f, 0xAD, "SINGLE RIGHT-POINTING ANGLE QUOTATION MARK");
    glcFontMap(f, 0xAE, "fi");
    glcFontMap(f, 0xAF, "fl");
    glcFontMap(f, 0xB1, "EN DASH");
    glcFontMap(f, 0xB2, "DAGGER");
    glcFontMap(f, 0xB3, "DOUBLE DAGGER");
    glcFontMap(f, 0xB4, "MIDDLE DOT");
    glcFontMap(f, 0xB6, "PILCROW SIGN");
    glcFontMap(f, 0xB7, "BULLET");
    glcFontMap(f, 0xB8, "SINGLE LOW-9 QUOTATION MARK");
    glcFontMap(f, 0xB9, "DOUBLE LOW-9 QUOTATION MARK");
    glcFontMap(f, 0xBA, "RIGHT DOUBLE QUOTATION MARK");
    glcFontMap(f, 0xBB, "RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK");
    glcFontMap(f, 0xBC, "HORIZONTAL ELLIPSIS");
    glcFontMap(f, 0xBD, "PER MILLE SIGN");
    glcFontMap(f, 0xBF, "INVERTED QUESTION MARK");
    glcFontMap(f, 0xC1, "GRAVE ACCENT");
    glcFontMap(f, 0xC2, "ACUTE ACCENT");
    glcFontMap(f, 0xC3, "MODIFIER LETTER CIRCUMFLEX ACCENT");
    glcFontMap(f, 0xC4, "SMALL TILDE");
    glcFontMap(f, 0xC5, "MACRON");
    glcFontMap(f, 0xC6, "BREVE");
    glcFontMap(f, 0xC7, "DOT ABOVE");
    glcFontMap(f, 0xC8, "DIAERESIS");
    glcFontMap(f, 0xCA, "RING ABOVE");
    glcFontMap(f, 0xCB, "CEDILLA");
    glcFontMap(f, 0xCD, "DOUBLE ACUTE ACCENT");
    glcFontMap(f, 0xCE, "OGONEK");
    glcFontMap(f, 0xCF, "CARON");
    glcFontMap(f, 0xD0, "EM DASH");
    glcFontMap(f, 0xE1, "LATIN CAPITAL LIGATURE AE");
    glcFontMap(f, 0xE3, "FEMININE ORDINAL INDICATOR");
    glcFontMap(f, 0xE8, "LATIN CAPITAL LETTER L WITH STROKE");
    glcFontMap(f, 0xE9, "LATIN CAPITAL LETTER O WITH STROKE");
    glcFontMap(f, 0xEA, "LATIN CAPITAL LIGATURE OE");
    glcFontMap(f, 0xEB, "MASCULINE ORDINAL INDICATOR");
    glcFontMap(f, 0xF1, "LATIN SMALL LIGATURE AE");
    glcFontMap(f, 0xF5, "LATIN SMALL LETTER DOTLESS I");
    glcFontMap(f, 0xF8, "LATIN SMALL LETTER L WITH STROKE");
    glcFontMap(f, 0xF9, "LATIN SMALL LETTER O WITH STROKE");
    glcFontMap(f, 0xFA, "LATIN SMALL LIGATURE OE");
    glcFontMap(f, 0xFB, "LATIN SMALL LETTER SHARP S");
    glcStringType(stringType);
#endif
}

void FontCollectionGlc::_FontMap_AdobeDingbats(GLint f) {
#if defined(linux)
#else
    GLint i;
    const GLint minMapped = glcGetFonti(f, GLC_MIN_MAPPED_CODE);
    const GLint maxMapped = glcGetFonti(f, GLC_MAX_MAPPED_CODE);
    const GLCenum stringType = glcGeti(GLC_STRING_TYPE);

    glcStringType(GLC_UCS1);
    for (i = minMapped ; i <= maxMapped ; ++i) glcFontMap(f, i, GLC_NONE);
    glcFontMap(f, 0x20, "SPACE");
    glcFontMap(f, 0x21, "UPPER BLADE SCISSORS");
    glcFontMap(f, 0x22, "BLACK SCISSORS");
    glcFontMap(f, 0x23, "LOWER BLADE SCISSORS");
    glcFontMap(f, 0x24, "WHITE SCISSORS");
    glcFontMap(f, 0x25, "BLACK TELEPHONE");
    glcFontMap(f, 0x26, "TELEPHONE LOCATION SIGN");
    glcFontMap(f, 0x27, "TAPE DRIVE");
    glcFontMap(f, 0x28, "AIRPLANE");
    glcFontMap(f, 0x29, "ENVELOPE");
    glcFontMap(f, 0x2A, "BLACK RIGHT POINTING INDEX");
    glcFontMap(f, 0x2B, "WHITE RIGHT POINTING INDEX");
    glcFontMap(f, 0x2C, "VICTORY HAND");
    glcFontMap(f, 0x2D, "WRITING HAND");
    glcFontMap(f, 0x2E, "LOWER RIGHT PENCIL");
    glcFontMap(f, 0x2F, "PENCIL");
    glcFontMap(f, 0x30, "UPPER RIGHT PENCIL");
    glcFontMap(f, 0x31, "WHITE NIB");
    glcFontMap(f, 0x32, "BLACK NIB");
    glcFontMap(f, 0x33, "CHECK MARK");
    glcFontMap(f, 0x34, "HEAVY CHECK MARK");
    glcFontMap(f, 0x35, "MULTIPLICATION X");
    glcFontMap(f, 0x36, "HEAVY MULTIPLICATION X");
    glcFontMap(f, 0x37, "BALLOT X");
    glcFontMap(f, 0x38, "HEAVY BALLOT X");
    glcFontMap(f, 0x39, "OUTLINED GREEK CROSS");
    glcFontMap(f, 0x3A, "HEAVY GREEK CROSS");
    glcFontMap(f, 0x3B, "OPEN CENTRE CROSS");
    glcFontMap(f, 0x3C, "HEAVY OPEN CENTRE CROSS");
    glcFontMap(f, 0x3D, "LATIN CROSS");
    glcFontMap(f, 0x3E, "SHADOWED WHITE LATIN CROSS");
    glcFontMap(f, 0x3F, "OUTLINED LATIN CROSS");
    glcFontMap(f, 0x40, "MALTESE CROSS");
    glcFontMap(f, 0x41, "STAR OF DAVID");
    glcFontMap(f, 0x42, "FOUR TEARDROP-SPOKED ASTERISK");
    glcFontMap(f, 0x43, "FOUR BALLON-SPOKED ASTERISK");
    glcFontMap(f, 0x44, "HEAVY FOUR BALLOON-SPOKED ASTERISK");
    glcFontMap(f, 0x45, "FOUR CLUB-SPOKED ASTERISK");
    glcFontMap(f, 0x46, "BLACK FOUR POINTED STAR");
    glcFontMap(f, 0x47, "WHITE FOUR POINTED STAR");
    glcFontMap(f, 0x48, "BLACK STAR");
    glcFontMap(f, 0x49, "STRESS OUTLINED WHITE STAR");
    glcFontMap(f, 0x4A, "CIRCLED WHITE STAR");
    glcFontMap(f, 0x4B, "OPEN CENTRE BLACK STAR");
    glcFontMap(f, 0x4C, "BLACK CENTRE WHITE STAR");
    glcFontMap(f, 0x4D, "OUTLINED BLACK STAR");
    glcFontMap(f, 0x4E, "HEAVY OUTLINED BLACK STAR");
    glcFontMap(f, 0x4F, "PINWHEEL STAR");
    glcFontMap(f, 0x50, "SHADOWED WHITE STAR");
    glcFontMap(f, 0x51, "HEAVY ASTERISK");
    glcFontMap(f, 0x52, "OPEN CENTRE ASTERISK");
    glcFontMap(f, 0x53, "EIGHT SPOKED ASTERISK");
    glcFontMap(f, 0x54, "EIGHT POINTED BLACK STAR");
    glcFontMap(f, 0x55, "EIGHT POINTED PINWHEEL STAR");
    glcFontMap(f, 0x56, "SIX POINTED BLACK STAR");
    glcFontMap(f, 0x57, "EIGHT POINTED RECTILINEAR BLACK STAR");
    glcFontMap(f, 0x58, "HEAVY EIGHT POINTED RECTILINEAR BLACK STAR");
    glcFontMap(f, 0x59, "TWELVE POINTED BLACK STAR");
    glcFontMap(f, 0x5A, "SIXTEEN POINTED ASTERISK");
    glcFontMap(f, 0x5B, "TEARDROP-SPOKED ASTERISK");
    glcFontMap(f, 0x5C, "OPEN CENTRE TEARDROP-SPOKED ASTERISK");
    glcFontMap(f, 0x5D, "HEAVY TEARDROP-SPOKED ASTERISK");
    glcFontMap(f, 0x5E, "SIX PETALLED BLACK AND WHITE FLORETTE");
    glcFontMap(f, 0x5F, "BLACK FLORETTE");
    glcFontMap(f, 0x60, "WHITE FLORETTE");
    glcFontMap(f, 0x61, "EIGHT PETALLED OUTLINED BLACK FLORETTE");
    glcFontMap(f, 0x62, "CIRCLED OPEN CENTRE EIGHT POINTED STAR");
    glcFontMap(f, 0x63, "HEAVY TEARDROP-SPOKED PINWHEEL ASTERISK");
    glcFontMap(f, 0x64, "SNOWFLAKE");
    glcFontMap(f, 0x65, "TIGHT TRIFOLIATE SNOWFLAKE");
    glcFontMap(f, 0x66, "HEAVY CHEVRON SNOWFLAKE");
    glcFontMap(f, 0x67, "SPARKLE");
    glcFontMap(f, 0x68, "HEAVY SPARKLE");
    glcFontMap(f, 0x69, "BALLOON-SPOKED ASTERISK");
    glcFontMap(f, 0x6A, "EIGHT TEARDROP-SPOKED PROPELLER ASTERISK");
    glcFontMap(f, 0x6B, "HEAVY EIGHT TEARDROP-SPOKED PROPELLER ASTERISK");
    glcFontMap(f, 0x6D, "SHADOWED WHITE CIRCLE");
    glcFontMap(f, 0x6E, "BLACK SQUARE");
    glcFontMap(f, 0x6F, "LOWER RIGHT DROP-SHADOWED WHITE SQUARE");
    glcFontMap(f, 0x70, "UPPER RIGHT DROP-SHADOWED WHITE SQUARE");
    glcFontMap(f, 0x71, "LOWER RIGHT SHADOWED WHITE SQUARE");
    glcFontMap(f, 0x72, "UPPER RIGHT SHADOWED WHITE SQUARE");
    glcFontMap(f, 0x73, "BLACK UP-POINTING TRIANGLE");
    glcFontMap(f, 0x74, "BLACK DOWN-POINTING TRIANGLE");
    glcFontMap(f, 0x75, "BLACK DIAMOND");
    glcFontMap(f, 0x76, "BLACK DIAMOND MINUS WHITE X");
    glcFontMap(f, 0x78, "LIGHT VERTICAL BAR");
    glcFontMap(f, 0x79, "MEDIUM VERTICAL BAR");
    glcFontMap(f, 0x7A, "HEAVY VERTICAL BAR");
    glcFontMap(f, 0x7B, "HEAVY SINGLE TURNED COMMA QUOTATION MARK ORNAMENT");
    glcFontMap(f, 0x7C, "HEAVY SINGLE COMMA QUOTATION MARK ORNAMENT");
    glcFontMap(f, 0x7D, "HEAVY DOUBLE TURNED COMMA QUOTATION MARK ORNAMENT");
    glcFontMap(f, 0x7E, "HEAVY DOUBLE COMMA QUOTATION MARK ORNAMENT");
    glcFontMap(f, 0xA1, "CURVED STEM PARAGRAPH SIGN ORNAMENT");
    glcFontMap(f, 0xA2, "HEAVY EXCLAMATION MARK ORNAMENT");
    glcFontMap(f, 0xA3, "HEAVY HEART EXCLAMATION MARK ORNAMENT");
    glcFontMap(f, 0xA4, "HEAVY BLACK HEART");
    glcFontMap(f, 0xA5, "ROTATED HEAVY BLACK HEART BULLET");
    glcFontMap(f, 0xA6, "FLORAL HEART");
    glcFontMap(f, 0xA7, "ROTATED FLORAL HEART BULLET");
    glcFontMap(f, 0xA8, "BLACK CLUB SUIT");
    glcFontMap(f, 0xA9, "BLACK DIAMOND SUIT");
    glcFontMap(f, 0xAA, "BLACK HEART SUIT");
    glcFontMap(f, 0xAB, "BLACK SPADE SUIT");
    glcFontMap(f, 0xAC, "CIRCLED DIGIT ONE");
    glcFontMap(f, 0xAD, "CIRCLED DIGIT TWO");
    glcFontMap(f, 0xAE, "CIRCLED DIGIT THREE");
    glcFontMap(f, 0xAF, "CIRCLED DIGIT FOUR");
    glcFontMap(f, 0xB0, "CIRCLED DIGIT FIVE");
    glcFontMap(f, 0xB1, "CIRCLED DIGIT SIX");
    glcFontMap(f, 0xB2, "CIRCLED DIGIT SEVEN");
    glcFontMap(f, 0xB3, "CIRCLED DIGIT EIGHT");
    glcFontMap(f, 0xB4, "CIRCLED DIGIT NINE");
    glcFontMap(f, 0xB5, "CIRCLED NUMBER TEN");
    glcFontMap(f, 0xB6, "DINGBAT NEGATIVE CIRCLED DIGIT ONE");
    glcFontMap(f, 0xB7, "DINGBAT NEGATIVE CIRCLED DIGIT TWO");
    glcFontMap(f, 0xB8, "DINGBAT NEGATIVE CIRCLED DIGIT THREE");
    glcFontMap(f, 0xB9, "DINGBAT NEGATIVE CIRCLED DIGIT FOUR");
    glcFontMap(f, 0xBA, "DINGBAT NEGATIVE CIRCLED DIGIT FIVE");
    glcFontMap(f, 0xBB, "DINGBAT NEGATIVE CIRCLED DIGIT SIX");
    glcFontMap(f, 0xBC, "DINGBAT NEGATIVE CIRCLED DIGIT SEVEN");
    glcFontMap(f, 0xBD, "DINGBAT NEGATIVE CIRCLED DIGIT EIGHT");
    glcFontMap(f, 0xBE, "DINGBAT NEGATIVE CIRCLED DIGIT NINE");
    glcFontMap(f, 0xBF, "DINGBAT NEGATIVE CIRCLED NUMBER TEN");
    glcFontMap(f, 0xC0, "DINGBAT CIRCLED SANS-SERIF DIGIT ONE");
    glcFontMap(f, 0xC1, "DINGBAT CIRCLED SANS-SERIF DIGIT TWO");
    glcFontMap(f, 0xC2, "DINGBAT CIRCLED SANS-SERIF DIGIT THREE");
    glcFontMap(f, 0xC3, "DINGBAT CIRCLED SANS-SERIF DIGIT FOUR");
    glcFontMap(f, 0xC4, "DINGBAT CIRCLED SANS-SERIF DIGIT FIVE");
    glcFontMap(f, 0xC5, "DINGBAT CIRCLED SANS-SERIF DIGIT SIX");
    glcFontMap(f, 0xC6, "DINGBAT CIRCLED SANS-SERIF DIGIT SEVEN");
    glcFontMap(f, 0xC7, "DINGBAT CIRCLED SANS-SERIF DIGIT EIGHT");
    glcFontMap(f, 0xC8, "DINGBAT CIRCLED SANS-SERIF DIGIT NINE");
    glcFontMap(f, 0xC9, "DINGBAT CIRCLED SANS-SERIF NUMBER TEN");
    glcFontMap(f, 0xCA, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT ONE");
    glcFontMap(f, 0xCB, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT TWO");
    glcFontMap(f, 0xCC, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT THREE");
    glcFontMap(f, 0xCD, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FOUR");
    glcFontMap(f, 0xCE, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FIVE");
    glcFontMap(f, 0xCF, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SIX");
    glcFontMap(f, 0xD0, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SEVEN");
    glcFontMap(f, 0xD1, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT EIGHT");
    glcFontMap(f, 0xD2, "DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT NINE");
    glcFontMap(f, 0xD3, "DINGBAT NEGATIVE CIRCLED SANS-SERIF NUMBER TEN");
    glcFontMap(f, 0xD4, "HEAVY WIDE-HEADED RIGHTWARDS ARROW");
    glcFontMap(f, 0xD5, "RIGHTWARDS ARROW");
    glcFontMap(f, 0xD6, "LEFT RIGHT ARROW");
    glcFontMap(f, 0xD7, "UP DOWN ARROW");
    glcFontMap(f, 0xD8, "HEAVY SOUTH EAST ARROW");
    glcFontMap(f, 0xD9, "HEAVY RIGHTWARDS ARROW");
    glcFontMap(f, 0xDA, "HEAVY NORTH EAST ARROW");
    glcFontMap(f, 0xDB, "DRAFTING POINT RIGHTWARDS ARROW");
    glcFontMap(f, 0xDC, "HEAVY ROUND-TIPPED RIGHTWARDS ARROW");
    glcFontMap(f, 0xDD, "TRIANGLE-HEADED RIGHTWARDS ARROW");
    glcFontMap(f, 0xDE, "HEAVY TRIANGLE-HEADED RIGHTWARDS ARROW");
    glcFontMap(f, 0xDF, "DASHED TRIANGLE-HEADED RIGHTWARDS ARROW");
    glcFontMap(f, 0xE0, "HEAVY DASHED TRIANGLE-HEADED RIGHTWARDS ARROW");
    glcFontMap(f, 0xE1, "BLACK RIGHTWARDS ARROW");
    glcFontMap(f, 0xE2, "THREE-D TOP-LIGHTED RIGHTWARDS ARROWHEAD");
    glcFontMap(f, 0xE3, "THREE-D BOTTOM-LIGHTED RIGHTWARDS ARROWHEAD");
    glcFontMap(f, 0xE4, "BLACK RIGHTWARDS ARROWHEAD");
    glcFontMap(f, 0xE5, "HEAVY BLACK CURVED DOWNWARDS AND RIGHTWARDS ARROW");
    glcFontMap(f, 0xE6, "HEAVY BLACK CURVED UPWARDS AND RIGHTWARDS ARROW");
    glcFontMap(f, 0xE7, "SQUAT BLACK RIGHTWARDS ARROW");
    glcFontMap(f, 0xE8, "HEAVY CONCAVE-POINTED BLACK RIGHTWARDS ARROW");
    glcFontMap(f, 0xE9, "RIGHT-SHADED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xEA, "LEFT-SHADED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xEB, "BACK-TILTED SHADOWED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xEC, "FRONT-TILTED SHADOWED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xED, "HEAVY LOWER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xEE, "HEAVY UPPER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xEF, "NOTCHED LOWER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xF1, "NOTCHED UPPER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xF2, "CIRCLED HEAVY WHITE RIGHTWARDS ARROW");
    glcFontMap(f, 0xF3, "WHITE-FEATHERED RIGHTWARDS ARROW");
    glcFontMap(f, 0xF4, "BLACK-FEATHERED SOUTH EAST ARROW");
    glcFontMap(f, 0xF5, "BLACK-FEATHERED RIGHTWARDS ARROW");
    glcFontMap(f, 0xF6, "BLACK-FEATHERED NORTH EAST ARROW");
    glcFontMap(f, 0xF7, "HEAVY BLACK-FEATHERED SOUTH EAST ARROW");
    glcFontMap(f, 0xF8, "HEAVY BLACK-FEATHERED RIGHTWARDS ARROW");
    glcFontMap(f, 0xF9, "HEAVY BLACK-FEATHERED NORTH EAST ARROW");
    glcFontMap(f, 0xFA, "TEARDROP-BARBED RIGHTWARDS ARROW");
    glcFontMap(f, 0xFB, "HEAVY TEARDROP-SHANKED RIGHTWARDS ARROW");
    glcFontMap(f, 0xFC, "WEDGE-TAILED RIGHTWARDS ARROW");
    glcFontMap(f, 0xFD, "HEAVY WEDGE-TAILED RIGHTWARDS ARROW");
    glcFontMap(f, 0xFE, "OPEN-OUTLINED RIGHTWARDS ARROW");
    glcStringType(stringType);
#endif
}

void FontCollectionGlc::_FontMap_AdobeSymbol(GLint f) {
#if defined(linux)
#else
    GLint i;
    const GLint minMapped = glcGetFonti(f, GLC_MIN_MAPPED_CODE);
    const GLint maxMapped = glcGetFonti(f, GLC_MAX_MAPPED_CODE);
    const GLCenum stringType = glcGeti(GLC_STRING_TYPE);

    glcStringType(GLC_UCS1);
    for (i = minMapped ; i <= maxMapped ; ++i) glcFontMap(f, i, GLC_NONE);
    glcFontMap(f, 0x20, "SPACE");
    glcFontMap(f, 0x21, "EXCLAMATION MARK");
    glcFontMap(f, 0x22, "FOR ALL");
    glcFontMap(f, 0x23, "NUMBER SIGN");
    glcFontMap(f, 0x24, "THERE EXISTS");
    glcFontMap(f, 0x25, "PERCENT SIGN");
    glcFontMap(f, 0x26, "AMPERSAND");
    glcFontMap(f, 0x27, "CONTAINS AS MEMBER");
    glcFontMap(f, 0x28, "LEFT PARENTHESIS");
    glcFontMap(f, 0x29, "RIGHT PARENTHESIS");
    glcFontMap(f, 0x2A, "ASTERISK OPERATOR");
    glcFontMap(f, 0x2B, "PLUS SIGN");
    glcFontMap(f, 0x2C, "COMMA");
    glcFontMap(f, 0x2D, "MINUS SIGN");
    glcFontMap(f, 0x2E, "FULL STOP");
    glcFontMap(f, 0x2F, "SOLIDUS");
    glcFontMap(f, 0x30, "DIGIT ZERO");
    glcFontMap(f, 0x31, "DIGIT ONE");
    glcFontMap(f, 0x32, "DIGIT TWO");
    glcFontMap(f, 0x33, "DIGIT THREE");
    glcFontMap(f, 0x34, "DIGIT FOUR");
    glcFontMap(f, 0x35, "DIGIT FIVE");
    glcFontMap(f, 0x36, "DIGIT SIX");
    glcFontMap(f, 0x37, "DIGIT SEVEN");
    glcFontMap(f, 0x38, "DIGIT EIGHT");
    glcFontMap(f, 0x39, "DIGIT NINE");
    glcFontMap(f, 0x3A, "COLON");
    glcFontMap(f, 0x3B, "SEMICOLON");
    glcFontMap(f, 0x3C, "LESS-THAN SIGN");
    glcFontMap(f, 0x3D, "EQUALS SIGN");
    glcFontMap(f, 0x3E, "GREATER-THAN SIGN");
    glcFontMap(f, 0x3F, "QUESTION MARK");
    glcFontMap(f, 0x40, "APPROXIMATELY EQUAL TO");
    glcFontMap(f, 0x41, "GREEK CAPITAL LETTER ALPHA");
    glcFontMap(f, 0x42, "GREEK CAPITAL LETTER BETA");
    glcFontMap(f, 0x43, "GREEK CAPITAL LETTER CHI");
    glcFontMap(f, 0x44, "GREEK CAPITAL LETTER DELTA");
    glcFontMap(f, 0x45, "GREEK CAPITAL LETTER EPSILON");
    glcFontMap(f, 0x46, "GREEK CAPITAL LETTER PHI");
    glcFontMap(f, 0x47, "GREEK CAPITAL LETTER GAMMA");
    glcFontMap(f, 0x48, "GREEK CAPITAL LETTER ETA");
    glcFontMap(f, 0x49, "GREEK CAPITAL LETTER IOTA");
    glcFontMap(f, 0x4A, "GREEK THETA SYMBOL");
    glcFontMap(f, 0x4B, "GREEK CAPITAL LETTER KAPPA");
    glcFontMap(f, 0x4C, "GREEK CAPITAL LETTER LAMDA");
    glcFontMap(f, 0x4D, "GREEK CAPITAL LETTER MU");
    glcFontMap(f, 0x4E, "GREEK CAPITAL LETTER NU");
    glcFontMap(f, 0x4F, "GREEK CAPITAL LETTER OMICRON");
    glcFontMap(f, 0x50, "GREEK CAPITAL LETTER PI");
    glcFontMap(f, 0x51, "GREEK CAPITAL LETTER THETA");
    glcFontMap(f, 0x52, "GREEK CAPITAL LETTER RHO");
    glcFontMap(f, 0x53, "GREEK CAPITAL LETTER SIGMA");
    glcFontMap(f, 0x54, "GREEK CAPITAL LETTER TAU");
    glcFontMap(f, 0x55, "GREEK CAPITAL LETTER UPSILON");
    glcFontMap(f, 0x56, "GREEK SMALL LETTER FINAL SIGMA");
    glcFontMap(f, 0x57, "GREEK CAPITAL LETTER OMEGA");
    glcFontMap(f, 0x58, "GREEK CAPITAL LETTER XI");
    glcFontMap(f, 0x59, "GREEK CAPITAL LETTER PSI");
    glcFontMap(f, 0x5A, "GREEK CAPITAL LETTER ZETA");
    glcFontMap(f, 0x5B, "LEFT SQUARE BRACKET");
    glcFontMap(f, 0x5C, "THEREFORE");
    glcFontMap(f, 0x5D, "RIGHT SQUARE BRACKET");
    glcFontMap(f, 0x5E, "UP TACK");
    glcFontMap(f, 0x5F, "LOW LINE");
    glcFontMap(f, 0x60, "OVERLINE");
    glcFontMap(f, 0x61, "GREEK SMALL LETTER ALPHA");
    glcFontMap(f, 0x62, "GREEK SMALL LETTER BETA");
    glcFontMap(f, 0x63, "GREEK SMALL LETTER CHI");
    glcFontMap(f, 0x64, "GREEK SMALL LETTER DELTA");
    glcFontMap(f, 0x65, "GREEK SMALL LETTER EPSILON");
    glcFontMap(f, 0x66, "GREEK SMALL LETTER PHI");
    glcFontMap(f, 0x67, "GREEK SMALL LETTER GAMMA");
    glcFontMap(f, 0x68, "GREEK SMALL LETTER ETA");
    glcFontMap(f, 0x69, "GREEK SMALL LETTER IOTA");
    glcFontMap(f, 0x6A, "GREEK PHI SYMBOL");
    glcFontMap(f, 0x6B, "GREEK SMALL LETTER KAPPA");
    glcFontMap(f, 0x6C, "GREEK SMALL LETTER LAMDA");
    glcFontMap(f, 0x6D, "GREEK SMALL LETTER MU");
    glcFontMap(f, 0x6E, "GREEK SMALL LETTER NU");
    glcFontMap(f, 0x6F, "GREEK SMALL LETTER OMICRON");
    glcFontMap(f, 0x70, "GREEK SMALL LETTER PI");
    glcFontMap(f, 0x71, "GREEK SMALL LETTER THETA");
    glcFontMap(f, 0x72, "GREEK SMALL LETTER RHO");
    glcFontMap(f, 0x73, "GREEK SMALL LETTER SIGMA");
    glcFontMap(f, 0x74, "GREEK SMALL LETTER TAU");
    glcFontMap(f, 0x75, "GREEK SMALL LETTER UPSILON");
    glcFontMap(f, 0x76, "GREEK PI SYMBOL");
    glcFontMap(f, 0x77, "GREEK SMALL LETTER OMEGA");
    glcFontMap(f, 0x78, "GREEK SMALL LETTER XI");
    glcFontMap(f, 0x79, "GREEK SMALL LETTER PSI");
    glcFontMap(f, 0x7A, "GREEK SMALL LETTER ZETA");
    glcFontMap(f, 0x7B, "LEFT CURLY BRACKET");
    glcFontMap(f, 0x7C, "VERTICAL LINE");
    glcFontMap(f, 0x7D, "RIGHT CURLY BRACKET");
    glcFontMap(f, 0x7E, "TILDE OPERATOR");
    glcFontMap(f, 0xA1, "GREEK UPSILON WITH HOOK SYMBOL");
    glcFontMap(f, 0xA2, "PRIME");
    glcFontMap(f, 0xA3, "LESS-THAN OR EQUAL TO");
    glcFontMap(f, 0xA4, "FRACTION SLASH");
    glcFontMap(f, 0xA5, "INFINITY");
    glcFontMap(f, 0xA6, "LATIN SMALL LETTER F WITH HOOK");
    glcFontMap(f, 0xA7, "BLACK CLUB SUIT");
    glcFontMap(f, 0xA8, "BLACK DIAMOND SUIT");
    glcFontMap(f, 0xA9, "BLACK HEART SUIT");
    glcFontMap(f, 0xAA, "BLACK SPADE SUIT");
    glcFontMap(f, 0xAB, "LEFT RIGHT ARROW");
    glcFontMap(f, 0xAC, "LEFTWARDS ARROW");
    glcFontMap(f, 0xAD, "UPWARDS ARROW");
    glcFontMap(f, 0xAE, "RIGHTWARDS ARROW");
    glcFontMap(f, 0xAF, "DOWNWARDS ARROW");
    glcFontMap(f, 0xB0, "DEGREE SIGN");
    glcFontMap(f, 0xB1, "PLUS-MINUS SIGN");
    glcFontMap(f, 0xB2, "DOUBLE PRIME");
    glcFontMap(f, 0xB3, "GREATER-THAN OR EQUAL TO");
    glcFontMap(f, 0xB4, "MULTIPLICATION SIGN");
    glcFontMap(f, 0xB5, "PROPORTIONAL TO");
    glcFontMap(f, 0xB6, "PARTIAL DIFFERENTIAL");
    glcFontMap(f, 0xB7, "BULLET");
    glcFontMap(f, 0xB8, "DIVISION SIGN");
    glcFontMap(f, 0xB9, "NOT EQUAL TO");
    glcFontMap(f, 0xBA, "IDENTICAL TO");
    glcFontMap(f, 0xBB, "ALMOST EQUAL TO");
    glcFontMap(f, 0xBC, "HORIZONTAL ELLIPSIS");
    glcFontMap(f, 0xBD, "arrowvertex");
    glcFontMap(f, 0xBE, "arrowhorizex");
    glcFontMap(f, 0xBF, "DOWNWARDS ARROW WITH CORNER LEFTWARDS");
    glcFontMap(f, 0xC0, "ALEF SYMBOL");
    glcFontMap(f, 0xC1, "BLACK-LETTER CAPITAL I");
    glcFontMap(f, 0xC2, "BLACK-LETTER CAPITAL R");
    glcFontMap(f, 0xC3, "SCRIPT CAPITAL P");
    glcFontMap(f, 0xC4, "CIRCLED TIMES");
    glcFontMap(f, 0xC5, "CIRCLED PLUS");
    glcFontMap(f, 0xC6, "EMPTY SET");
    glcFontMap(f, 0xC7, "INTERSECTION");
    glcFontMap(f, 0xC8, "UNION");
    glcFontMap(f, 0xC9, "SUPERSET OF");
    glcFontMap(f, 0xCA, "SUPERSET OF OR EQUAL TO");
    glcFontMap(f, 0xCB, "NOT A SUBSET OF");
    glcFontMap(f, 0xCC, "SUBSET OF");
    glcFontMap(f, 0xCD, "SUBSET OF OR EQUAL TO");
    glcFontMap(f, 0xCE, "ELEMENT OF");
    glcFontMap(f, 0xCF, "NOT AN ELEMENT OF");
    glcFontMap(f, 0xD0, "ANGLE");
    glcFontMap(f, 0xD1, "NABLA");
    glcFontMap(f, 0xD2, "REGISTERED SIGN");
    glcFontMap(f, 0xD3, "COPYRIGHT SIGN");
    glcFontMap(f, 0xD4, "TRADE MARK SIGN");
    glcFontMap(f, 0xD5, "N-ARY PRODUCT");
    glcFontMap(f, 0xD6, "SQUARE ROOT");
    glcFontMap(f, 0xD7, "DOT OPERATOR");
    glcFontMap(f, 0xD8, "NOT SIGN");
    glcFontMap(f, 0xD9, "LOGICAL AND");
    glcFontMap(f, 0xDA, "LOGICAL OR");
    glcFontMap(f, 0xDB, "LEFT RIGHT DOUBLE ARROW");
    glcFontMap(f, 0xDC, "LEFTWARDS DOUBLE ARROW");
    glcFontMap(f, 0xDD, "UPWARDS DOUBLE ARROW");
    glcFontMap(f, 0xDE, "RIGHTWARDS DOUBLE ARROW");
    glcFontMap(f, 0xDF, "DOWNWARDS DOUBLE ARROW");
    glcFontMap(f, 0xE0, "LOZENGE");
    glcFontMap(f, 0xE1, "LEFT-POINTING ANGLE BRACKET");
    glcFontMap(f, 0xE5, "N-ARY SUMMATION");
    glcFontMap(f, 0xE6, "parenlefttp");
    glcFontMap(f, 0xE7, "parenleftex");
    glcFontMap(f, 0xE8, "parenleftbt");
    glcFontMap(f, 0xE9, "bracketlefttp");
    glcFontMap(f, 0xEA, "bracketleftex");
    glcFontMap(f, 0xEB, "bracketleftbt");
    glcFontMap(f, 0xEC, "bracelefttp");
    glcFontMap(f, 0xED, "braceleftmid");
    glcFontMap(f, 0xEE, "braceleftbt");
    glcFontMap(f, 0xEF, "braceex");
    glcFontMap(f, 0xF1, "RIGHT-POINTING ANGLE BRACKET");
    glcFontMap(f, 0xF2, "INTEGRAL");
    glcFontMap(f, 0xF3, "TOP HALF INTEGRAL");
    glcFontMap(f, 0xF4, "integralx");
    glcFontMap(f, 0xF5, "BOTTOM HALF INTEGRAL");
    glcFontMap(f, 0xF6, "parenrighttp");
    glcFontMap(f, 0xF7, "parenrightex");
    glcFontMap(f, 0xF8, "parenrightbt");
    glcFontMap(f, 0xF9, "bracketrighttp");
    glcFontMap(f, 0xFA, "bracketrightex");
    glcFontMap(f, 0xFB, "bracketrightbt");
    glcFontMap(f, 0xFC, "bracerighttp");
    glcFontMap(f, 0xFD, "bracerightmid");
    glcFontMap(f, 0xFE, "bracerightbt");
    glcStringType(stringType);
#endif
}
