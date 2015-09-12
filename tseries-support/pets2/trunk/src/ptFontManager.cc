/*
 libpets2 - presentation and editing of time series

 $Id: ptFontManager.cc 4354 2014-01-02 13:07:57Z juergens $

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptFontManager.h"

using namespace miutil;
using namespace std;

FontManager FM;

miString FontManager::display_name_;

/*
const float HEADER1_size = 24.0;
const float HEADER2_size = 18.0;
const float HEADER3_size = 14.0;
const float NORMAL_size = 12.0;
const float SUBSCRIPT_size = 8.0;
*/

extern const char XFONTSET[] = "XFONTSET";
extern const char GLTTFONTSET[] = "GLTTFONTSET";
extern const char QTFONTSET[] = "QTFONTSET";

const float HEADER1_size = 20.0;
const float HEADER2_size = 14.0;
const float HEADER3_size = 11.0;
const float NORMAL_size = 9.0;
const float SUBSCRIPT_size = 6.0;

FontManager::FontManager() :
  curFC(-1), numFC(0)
{
  for (int i = 0; i < MAXFONTCOLL; i++)
    FC[i].fontc = 0;
}

FontManager::~FontManager()
{
  for (int i = 0; i < MAXFONTCOLL; i++)
    if (FC[i].fontc)
      delete FC[i].fontc;
}

miString FontManager::getCurrentFontCollection()
{
  if (curFC >= 0)
    return FC[curFC].name;
  else
    return "";
}

int FontManager::getNumFonts()
{
  if (curFC >= 0)
    return FC[curFC].fontc->getNumFonts();
  else
    return 0;
}

bool FontManager::addFontCollection(glText* fc, const miString s)
{
  if (!s.exists() || !fc)
    return false;
  for (int i = 0; i < MAXFONTCOLL; i++)
    if (!FC[i].fontc) {
      FC[i].name = s;
      FC[i].fontc = fc;
      numFC++;
      return true;
    }
  return false;
}

void FontManager::removeFontCollection(const miString s)
{
  for (int i = 0; i < MAXFONTCOLL; i++)
    if (FC[i].name == s) {
      if (FC[i].fontc) {
        delete FC[i].fontc;
        FC[i].fontc = 0;
      }
      FC[i].name = "";
      numFC--;
      if (curFC == i)
        curFC = -1;
    }
}

bool FontManager::setFontColl(const miString s)
{
  for (int i = 0; i < MAXFONTCOLL; i++)
    if (FC[i].name == s)
      if (FC[i].fontc) {
        curFC = i;
        // OBS
        setFont(0);
        //
        return true;
      }
  return false;
}

ptFontFace FontManager::getFontFace()
{
  ptFontFace pff = F_NORMAL;
  if (curFC >= 0) {
    glText::FontFace ff = FC[curFC].fontc->getFontFace();
    if (ff == glText::F_NORMAL)
      pff = F_NORMAL;
    else if (ff == glText::F_BOLD)
      pff = F_BOLD;
    else if (ff == glText::F_ITALIC)
      pff = F_ITALIC;
  }
  return pff;
}

ptFontSize FontManager::getFontSize()
{
  ptFontSize pfs = NORMAL;
  if (curFC >= 0) {
    float fs = FC[curFC].fontc->getFontSize();

    if (fs == HEADER1_size)
      pfs = HEADER1;
    else if (fs == HEADER2_size)
      pfs = HEADER2;
    else if (fs == HEADER3_size)
      pfs = HEADER3;
    else if (fs == NORMAL_size)
      pfs = NORMAL;
    else if (fs == SUBSCRIPT_size)
      pfs = SUBSCRIPT;
  }
  return pfs;
}

ptColor FontManager::getFontColor()
{
  //   if (curFC>=0)
  //     return FC[curFC].fontc->getFontColor();
  //   else return BLACK;
  return ptColor();
}

miString FontManager::getFontName(const int index)
{
  if (curFC >= 0)
    return FC[curFC].fontc->getFontName(index);
  else
    return "";
}

int FontManager::setFont(const int index)
{
  if (curFC >= 0) {
    string fs = getFontName(index);
    return (int) FC[curFC].fontc->setFont(fs);
  } else
    return 0;
}

int FontManager::setFontFace(const ptFontFace face)
{
  if (curFC >= 0) {
    glText::FontFace ff = glText::F_NORMAL;
    if (face == F_NORMAL)
      ff = glText::F_NORMAL;
    else if (face == F_BOLD)
      ff = glText::F_BOLD;
    else if (face == F_ITALIC)
      ff = glText::F_ITALIC;
    return FC[curFC].fontc->setFontFace(ff);
  } else
    return 0;
}

int FontManager::setFontSize(const ptFontSize size)
{
  if (curFC >= 0) {
    float fs = NORMAL_size;

    if (size == HEADER1)
      fs = HEADER1_size;
    else if (size == HEADER2)
      fs = HEADER2_size;
    else if (size == HEADER3)
      fs = HEADER3_size;
    else if (size == NORMAL)
      fs = NORMAL_size;
    else if (size == SUBSCRIPT)
      fs = SUBSCRIPT_size;

    return FC[curFC].fontc->setFontSize(fs);
  } else
    return 0;
}

int FontManager::setFontColor(const ptColor color)
{
  //   if (curFC>=0)
  //     return FC[curFC].fontc->setFontColor(color);
  //   else return 0;
  return 0;
}

int FontManager::setPrintMode(const ptPrintMode mode)
{
  //   if (curFC>=0)
  //     return FC[curFC].fontc->setPrintMode(mode);
  //   else return 0;
  return 0;
}

void FontManager::setGlSize(float width, float height)
{
  if (curFC >= 0) {
    FC[curFC].fontc->setGlSize(width, height);
  }
}

void FontManager::setVpSize(int width, int height){
  if (curFC >= 0) {
    FC[curFC].fontc->setVpSize(width, height);
  }
}


// printing commands
int FontManager::printChar(const int c, const float x, const float y)
{
  if (curFC >= 0)
    return FC[curFC].fontc->drawChar(c, x, y);
  else
    return 0;
}

int FontManager::printStr(const char* s, const float x, const float y)
{
  if (curFC >= 0)
    return FC[curFC].fontc->drawStr(s, x, y);
  else
    return 0;
}

// Metric commands
int FontManager::getCharSize(const int c, float& w, float& h)
{
  if (curFC >= 0)
    return FC[curFC].fontc->getCharSize(c, w, h);
  else
    return 0;
}

int FontManager::getMaxCharSize(float& w, float& h)
{
  if (curFC >= 0)
    return FC[curFC].fontc->getMaxCharSize(w, h);
  else
    return 0;
}

int FontManager::getStringSize(const char* s, float& w, float& h)
{
  if (curFC >= 0)
    return FC[curFC].fontc->getStringSize(s, w, h);
  else
    return 0;
}

// hardcopy
void FontManager::startHardcopy(GLPcontext* gc)
{
  for (int i = 0; i < numFC; i++)
    if (FC[i].fontc) {
      FC[i].fontc->startHardcopy(gc);
    }
}

void FontManager::endHardcopy()
{
  for (int i = 0; i < numFC; i++)
    if (FC[i].fontc) {
      FC[i].fontc->endHardcopy();
    }
}
