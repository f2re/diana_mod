/*
  libpets2 - presentation and editing of time series

  $Id: ptFontManager.h 4354 2014-01-02 13:07:57Z juergens $

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


// ptFontManager.h: definition of font manager

#ifndef _fontmanager_h
#define _fontmanager_h

#include "ptGlobals.h"
#include "ptFontDefs.h"
#include "ptColor.h"

#include <puTools/miString.h>
#include <glp/GLP.h>
#include <glText/glText.h>

const int MAXFONTCOLL=10;

extern const char XFONTSET[];
extern const char GLTTFONTSET[];
extern const char QTFONTSET[];

class FontManager {
private:
  struct fontcoll {
    miutil::miString name;
    glText* fontc;
  };
private:
  int curFC; // current fontcollection
  int numFC; // number of fontcollections
  fontcoll FC[MAXFONTCOLL]; // list of..
  static miutil::miString display_name_;

public:
  FontManager();
  ~FontManager();

  // for use in batch - force different display
  static void set_display_name(const miutil::miString name)
  {display_name_= name;}
  static miutil::miString display_name() { return display_name_; }

  bool addFontCollection(glText*, const miutil::miString);
  void removeFontCollection(const miutil::miString);
  bool setFontColl(const miutil::miString);

  // return info
  miutil::miString getCurrentFontCollection();
  int getNumFonts();
  ptFontFace getFontFace();
  ptFontSize getFontSize();
  ptColor getFontColor();
  miutil::miString getFontName(const int index);

  // choose font, size, face and colour
  int setFont(const int index);
  int setFontFace(const ptFontFace face);
  int setFontSize(const ptFontSize size);
  int setFontColor(const ptColor color);
  int setPrintMode(const ptPrintMode mode);
  void setGlSize(float width, float height);
  void setVpSize(int width, int height);

  // printing commands
  int printChar(const int c, const float x, const float y);
  int printStr(const char* s, const float x, const float y);
  // Metric commands
  int getCharSize(const int c, float& w, float& h);
  int getMaxCharSize(float& w, float& h);
  int getStringSize(const char* s, float& w, float& h);
  // hardcopy
  void startHardcopy(GLPcontext*);
  void endHardcopy();
};

extern FontManager FM;

#endif
