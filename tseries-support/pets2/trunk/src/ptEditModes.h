/*
  libpets2 - presentation and editing of time series
  
  Copyright (C) 2006-2013 met.no

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

#ifndef _ptEditModes_h
#define _ptEditModes_h

#include <string>

const int ptNumLineEditModes = 4;
enum ptLineEditMode {
  LE_SINGLE, LE_MULTI, LE_ROTATE, LE_LINEAR
};

const std::string ptLineEditModeName[ptNumLineEditModes] =
  {"Enkel","Linje","Rotér","Lineær"};
const std::string ptLineEditModeMnemonic[ptNumLineEditModes] =
  {"E","L","R","I"};

enum ptLineEditMarker {
  LE_RECT, LE_CIRC, LE_DIAMOND
};

#endif
