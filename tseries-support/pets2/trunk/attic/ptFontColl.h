/*
  libpets2 - presentation and editing of time series

  $Id: ptFontColl.h 2937 2009-12-09 16:39:35Z dages $

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


// ptFontColl.h: definition of font collection

#ifndef _fontcoll_h
#define _fontcoll_h

#include "ptGlobals.h"
#include "ptColor.h"
#include "ptFontDefs.h"

#include <puTools/miString.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

class FontCollection {
protected:
  miutil::miString FamilyName[MAXFONTS];
  int numFonts; // number of defined fonts
  int FontIndex;    // current font index
  ptFontFace Face;  // current font face
  ptFontSize Size;  // current font size
  ptColor Color;    // current font colour
  ptPrintMode Mode; // current print mode
  int charSizes[MAXFONTS][MAXFONTSIZES][2]; // typical char width and height
  float pixWidth, pixHeight;
public:
  FontCollection()
    : numFonts(0), FontIndex(-1), Face(F_NORMAL),
      Size(NORMAL)
    {
      for (int i=0; i<MAXFONTS;i++)
	for (int j=0; j<MAXFONTSIZES; j++)
	  charSizes[i][j][0] = charSizes[i][j][1] = 0;
    }
  ~FontCollection() {}

  virtual int makeFont(const miutil::miString family) = 0;
  virtual int defineFontSize(const int index, const ptFontSize,
			     const float size)= 0;
  // choose font, size, face and colour
  virtual int setFont(const int index) = 0;
  virtual int setFontFace(const ptFontFace face) = 0;
  virtual int setFontSize(const ptFontSize size) = 0;
  virtual int setFontColor(const ptColor color) = 0;
  virtual int setPrintMode(const ptPrintMode mode) = 0;
  virtual int setPixSize(const float pw, const float ph)
    { pixWidth=pw; pixHeight=ph; return(1);}
  // printing commands
  virtual int printChar(const int c, const float x, const float y) = 0;
  virtual int printStr(const char* s, const float x, const float y) = 0;
  // Metric commands
  virtual int getCharSize(const int c, float& w, float& h) = 0;
  virtual int getMaxCharSize(float& w, float& h) = 0;
  virtual int getStringSize(const char* s, float& w, float& h) = 0;
  // return info
  int getNumFonts()
    {return numFonts;}
  ptFontFace getFontFace()
    {return Face;}
  ptFontSize getFontSize()
    {return Size;}
  ptColor getFontColor()
    {return Color;}
  miutil::miString getFontName(const int index)
    {
      return (index >=0 && index <numFonts) ?
	FamilyName[index] : miutil::miString("");
    }
};




class FontCollectionGlc : public FontCollection {
private:
  GLuint contextID;
  GLuint fontIDs[MAXFONTS];
  GLboolean dl; // direct or draw list
  GLenum drawStyle; // Bitmap, Triangle, line etc
  GLint facesIndex[MAXFONTS], facesTotal[MAXFONTS];
  float fScale; // scaling of font
  float fontScales[MAXFONTSIZES];

  void _FontMap_AdobeStandard(GLint f);
  void _FontMap_AdobeDingbats(GLint f);
  void _FontMap_AdobeSymbol(GLint f);
  void _FontMap_Adobe(GLint f);
  int  _Init();
public:
  FontCollectionGlc();
  ~FontCollectionGlc();

  int makeFont(const miutil::miString family);
  int defineFontSize(const int index, const ptFontSize,
			     const float size);
  // choose font, size, face and colour
#if defined(linux)
  int setFont(const int index)
  {FontIndex=index; return(1);}
#else
  int setFont(const int index)
  {FontIndex=index; glcFont(fontIDs[FontIndex]); return(1);}
#endif
  int setFontFace(const ptFontFace face)
  {return(1);}
  int setFontSize(const ptFontSize size)
  {Size = size; fScale= fontScales[int(Size)]; return(1);}
  int setFontColor(const ptColor color)
  {Color = color; return(1);}
  int setPrintMode(const ptPrintMode mode);
  //int setPixSize(const float pw, const float ph);

  // printing commands
  int printChar(const int c, const float x, const float y);
  int printStr(const char* s, const float x, const float y);
  // Metric commands
  int getCharSize(const int c, float& w, float& h);
  int getMaxCharSize(float& w, float& h);
  int getStringSize(const char* s, float& w, float& h);
};

class FontCollectionX : public FontCollection {
private:
  Display *dpy;
  int baseList[MAXFONTSIZES]; //index to display lists for fonts
  bool fontList[MAXFONTSIZES];  //list of fonts currently in use
  XFontStruct* fontInfo[MAXFONTSIZES];
  int base;                          //display list index for this object

  void _makeRasterFont(const ptFontSize);

public:
  FontCollectionX(Display* display);
  ~FontCollectionX();

  int makeFont(const miutil::miString family){return 1;}
  int defineFontSize(const int index,
		     const ptFontSize,
		     const float size){return 1;}
  // choose font, size, face and colour
  int setFont(const int index) {return 1;}
  int setFontFace(const ptFontFace face) {return 1;}
  int setFontSize(const ptFontSize size)
    {
      Size = size;
      if (fontList[Size])
	base = baseList[Size];
      else
	_makeRasterFont(Size);
      return 1;
    }
  int setFontColor(const ptColor color) { return 1;}
  int setPrintMode(const ptPrintMode mode) { return 1;}
  // printing commands
  int printChar(const int c, const float x, const float y);
  int printStr(const char* s, const float x, const float y);
  // Metric commands
  int getCharSize(const int c, float& w, float& h);
  int getMaxCharSize(float& w, float& h);
  int getStringSize(const char* s, float& w, float& h);
};


#ifdef WITH_GLP
class FontCollectionGLPR : public FontCollection {
private:
  float fscale; // scaling of font
  int   fontface;
  float fontScales[MAXFONTSIZES];
  int   fontFamily[MAXFONTS];
  int   fontFaces[MAXFONTFACES];

public:
  FontCollectionGLPR();
  ~FontCollectionGLPR() {}

  int makeFont(const miutil::miString family){return 1;}
  int defineFontSize(const int index,
		     const ptFontSize,
		     const float size){return 1;}
  int setFontColor(const ptColor color) { return 1;}
  int setPrintMode(const ptPrintMode mode) { return 1;}
  // choose font, size, face
  int setFont(const int index)
    {FontIndex=index; return(1);}
  int setFontFace(const ptFontFace face)
    {fontface = fontFaces[int(face)]; return(1);}
  int setFontSize(const ptFontSize size)
    {Size = size; fscale= fontScales[int(Size)]; return(1);}

  // printing commands
  int printChar(const int c, const float x, const float y);
  int printStr(const char* s, const float x, const float y);
  // Metric commands
  int getCharSize(const int c, float& w, float& h);
  int getMaxCharSize(float& w, float& h);
  int getStringSize(const char* s, float& w, float& h);
};

#endif


#endif
