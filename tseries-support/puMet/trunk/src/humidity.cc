/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
  $Id: humidity.cc 4344 2014-01-02 09:24:27Z juergens $

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

#include "humidity.h"

const double Rw = 461.51;
//const double Rl = 287.05; 

double rHumid(double t, double dew ){
  
  double e,eSat,rh;

  e = magnus(dew);
  eSat = magnus(t);
  
  rh = (e/eSat) * 100;
  return rh;  
};


double absHumid(double t, double dew ){
  double rho,e;
  e = magnus(dew);
  
  if ( t < 150 )
    t+=273.15;
  
  rho = e / ( Rw * t ) * 1000;

  return rho;
};

double magnus(double t ){

  double C1 = 6.10780;
  double C2 = 17.08085;
  double C3 = 234.175;
  double e, ex;

  if ( t > 150 )
    t-=273.15;

  if( t <= 0){
    C2 = 17.84362;
    C3 = 245.425;
  }
  
  ex = ( C2 * t ) / ( C3 + t );
  
  e = C1 * exp(ex);

  return e;
};










