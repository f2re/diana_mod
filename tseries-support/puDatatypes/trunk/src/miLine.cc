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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miLine.h"

miLine::miLine(const miCoordinates& start, const miCoordinates& stop)
{
  set(start,stop);
}


miCoordinates miLine::middle()
{
  float x12 = (X1 + X2) / 2;
  float y12 = (Y1 + Y2) / 2;

  return  miCoordinates(x12,y12);
}

void miLine::set(const miCoordinates& beg_, const miCoordinates& end_)
{
  X1 = beg_.dLon();
  Y1 = beg_.dLat();
  X2 = end_.dLon();
  Y2 = end_.dLat();

  is_v = ( beg_.iLon() == end_.iLon() );
  is_h = ( beg_.iLat() == end_.iLat() );
  
  A = (Y2 - Y1) / ( X2 - X1 );
  B =  Y2 -  A*X2;
}
  

bool miLine::out_of_range(const float& a, const float& x1, const float& x2) const
{  
  if ((( a - x1 ) > 0.0001 ) && (( a - x2 ) > 0.0001 ) )  return true;
  if ((( x1 - a ) > 0.0001 ) && (( x2 - a ) > 0.0001 ) )  return true;
  return false;
}

bool miLine::cross(const miLine& tc) const
{
  float x12,y12;

  if(tc.is_v && is_v) return (X1 == tc.X1); // lines are identical or parallel   
  if(tc.is_h && is_h) return (Y1 == tc.Y1); // lines are identical or parallel 
  if(tc.A    == A   ) return false;         // lines are parallel

  if(tc.is_v){
    x12 = tc.X1;
    y12 = A*x12 + B;
  }
  else if(is_v){
    x12 = X1;
    y12 = tc.A*x12 + tc.B;
  }
  else if (is_h){
    y12 = Y1;
    x12 = (y12 - tc.B) / tc.A;
  }
  else if (tc.is_h ){
    y12 = tc.Y1;
    x12 = (y12 - B ) / A;
  }
  else {
    y12 = ( (A * tc.B) - (tc.A * B) ) / (A - tc.A);  
    x12 = (tc.B - B) / (A - tc.A);
  }

  if ( out_of_range(x12,X1,X2) )
    return false;
  if ( out_of_range(x12,tc.X1,tc.X2) )
    return false;
  if ( out_of_range(y12,Y1,Y2 ) )
    return false;
  if ( out_of_range(y12,tc.Y1,tc.Y2) )
    return false;

  return true;
}
  
  
bool miLine::crossingPoint(const miLine& tc, miCoordinates& mic) const
{
  float x12,y12;

  if( tc.is_v && is_v ) return false; // lines are identical or parallel 
  if( tc.is_h && is_h ) return false; // lines are identical or parallel
  if( tc.A    == A    ) return false; // lines are parallel

  if(tc.is_v){
    x12 = tc.X1;
    y12 = A*x12 + B;
  }
  else if(is_v){
    x12 = X1;
    y12 = tc.A*x12 + tc.B;
  }
  else if (is_h){
    y12 = Y1;
    x12 = (y12 - tc.B) / tc.A;
  }
  else if (tc.is_h ){
    y12 = tc.Y1;
    x12 = (y12 - B ) / A;
  }
  else {
    y12 = ( (A * tc.B) - (tc.A * B) ) / (A - tc.A);  
    x12 = (tc.B - B) / (A - tc.A);
  }

  if ( out_of_range(x12,X1,X2) )
    return false;
  if ( out_of_range(x12,tc.X1,tc.X2) )
    return false;
  if ( out_of_range(y12,Y1,Y2 ) )
    return false;
  if ( out_of_range(y12,tc.Y1,tc.Y2) )
    return false;

  mic = miCoordinates(x12,y12);
  
  return true;
}
  
  
