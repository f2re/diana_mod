/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
  $Id: iceAccretion.cc 4344 2014-01-02 09:24:27Z juergens $

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

#include <math.h>


double iceAccretion(const double& ta,   //air temperature in deg celcius
		    const double& tw,   //sea surface temperature in deg celcius
		    const double& wspd) //wind speed in meters/sec
{

  const double a = 2.73E-02;
  const double b = 2.91E-04;
  const double c = 1.84E-06;

  double pr = wspd*(-1.7-ta)/(1.+0.4*(tw+1.7));
  double ia = a*pr+b*pow(pr,2)+c*pow(pr,3);

  return fmax(ia,0.0);
}    



