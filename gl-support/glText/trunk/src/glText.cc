/*
 libglText - OpenGL Text Rendering Library

 $Id: glText.cc 4314 2013-12-18 08:25:30Z juergens $

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

// glText.cc : Class definition for abstract GL-rendered fonts

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "glText.h"
#include <iostream>
#include <stdlib.h>

using std::string;

glText::glText()
    : numFonts(0)
    , numSizes(0)
    , FontIndex(0)
    , Face(F_NORMAL)
    , SizeIndex(0)
    , reqSize(1.0)
    , scaletype(S_FIXEDSIZE)
    , wysiwyg(false)
    , vpWidth(1.0), vpHeight(1.0)
    , glWidth(1.0), glHeight(1.0)
    , pixWidth(1.0), pixHeight(1.0)
    , initialised(false)
    , size_add(0)
    , hardcopy(false)
    , output(0)
{
#ifdef DEBUGPRINT
  cerr << "glText::constructor" << endl;
#endif
}

bool glText::_findSize(const int size, int& index, const bool exact)
{
  if (!numSizes)
    return false;
  int diff = 100000, cdiff;
  for (int i = 0; i < numSizes; i++) {
    cdiff = abs(size - Sizes[i]);
    if (cdiff < diff) {
      diff = cdiff;
      index = i;
    }
  }
  return (exact ? diff == 0 : (diff < 1000));
}

bool glText::_addSize(const int size, int &index)
{
  if (numSizes == MAXFONTSIZES)
    return false;
  index = numSizes;
  Sizes[index] = size;
  numSizes++;
  return true;
}

bool glText::_addFamily(const string fam, int &index)
{
  if (numFonts == MAXFONTS)
    return false;
  index = numFonts;
  FamilyName[index] = fam;
  numFonts++;
  return true;
}

bool glText::_findFamily(const string fam, int& fidx)
{
  for (fidx = 0; fidx < numFonts; fidx++)
    if (FamilyName[fidx] == fam)
      return true;
  return false;
}

void glText::startHardcopy(GLPcontext* con)
{
  if (con && !hardcopy) {
    output = con;
    hardcopy = true;
  }
}

void glText::endHardcopy()
{
  output = 0;
  hardcopy = false;
}

void glText::setWysiwyg(const bool b)
{
  wysiwyg = b;
}

float glText::getSizeDiv()
{
  if (SizeIndex >= 0 && SizeIndex < MAXFONTSIZES) {
    int s = Sizes[SizeIndex];
    if (s > 0) {
      return reqSize / float(s);
    }
  }
  return 1.0;
}
