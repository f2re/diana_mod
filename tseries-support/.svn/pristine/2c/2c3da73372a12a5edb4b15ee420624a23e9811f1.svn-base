/*
  libpets2 - presentation and editing of time series

  $Id$

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


// ptPatterns.h : Definitions of polygon and line patterns

#ifndef _patterns_h
#define _patterns_h

#include <puTools/miString.h>
#include <GL/gl.h>
#include <vector>

enum ptFillStyle {
  NONE,
  DIAGRIGHT,
  DIAGLEFT,
  DIAGCROSS,
  HORIZONTAL,
  VERTICAL,
  SHORIZONTAL,
  SVERTICAL,
  SQUARE,
  SOLID
};


enum ptLineStyle {
  FULL,
  DASHED,
  DASHDOTTED,
  DASHDASHDOTTED,
  DOTTED
};

const unsigned int LineStyle[][2] = {
  {1,0xFFFF},
  {1,0x00FF},
  {1,0x0C0F},
  {1,0x1C47},
  {2,0xAAAA}
};

extern const GLubyte* fillPattern(ptFillStyle);

extern ptFillStyle Str2FillStyle(miutil::miString&);
extern ptLineStyle Str2LineStyle(miutil::miString&);
extern std::vector<ptFillStyle> Str2FillStyleList(miutil::miString&);
extern std::vector<ptLineStyle> Str2LineStyleList(miutil::miString&);

extern miutil::miString FillStyle2Str(ptFillStyle);
extern miutil::miString LineStyle2Str(ptLineStyle);
extern miutil::miString FillStyleList2Str(std::vector<ptFillStyle>&);
extern miutil::miString LineStyleList2Str(std::vector<ptLineStyle>&);

#endif
