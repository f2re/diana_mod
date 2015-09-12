/*
  libpuDatatypes - Diverse datatypes: Regions, coordinates and alike
  
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


#ifndef _miLine_h
#define _miLine_h

#include "miCoordinates.h"

class miLine {
private:
  float X1,Y1;             // begin
  float X2,Y2;             // end
  float A,B;               // y = Ax + B
  bool out_of_range(const float&,const float&, const float&) const;
  bool is_v;
  bool is_h;

public:
  miLine()  : is_v(false), is_h(false) {}
  miLine(   const miCoordinates& beg_, const miCoordinates& end_);
  void set( const miCoordinates& beg_, const miCoordinates& end_);

  bool cross(         const miLine& tc                    ) const;
  bool crossingPoint( const miLine& tc, miCoordinates& mic) const;
  
  miCoordinates middle();
  miCoordinates begin() const { return miCoordinates(X1,Y1); }
  miCoordinates end()   const { return miCoordinates(X2,Y2); }
};

#endif












