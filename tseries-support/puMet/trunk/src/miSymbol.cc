/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

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

#include "miSymbol.h"

#include <algorithm>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <float.h>

using namespace std;
using namespace miutil;


// checks out the lightstatus ( false = darkness) and returns the
// customer requested number

int miSymbol::customNumber(bool lstat)const{
  if (lstat == false)
    return dark.extNum;
  return light.extNum;
};

miString miSymbol::picture(){
  if (lightstat == false)
    return dark.pic;
  return light.pic;
};
miString miSymbol::picture(bool lstat){
  if (lstat == false)
    return dark.pic;
  return light.pic;
};





int miSymbol::customNumber()const{
  if (lightstat == false)
    return dark.extNum;
  return light.extNum;
};


// like customNumber, but for the Name
miString miSymbol::customName()const{
  if (lightstat == false)
    return dark.name;
  return light.name;
};


miutil::miString
miSymbol::customName( bool lightState_ )const
{
   if (lightState_ == false)
     return dark.name;
   return light.name;

}


// visibility
int miSymbol::vis(){
  return light.vision;
};


// changes the occured error string to the  error symbol
void miSymbol::AddErr(miString errMessage){
  light.AddValues(999, errMessage,9);
  dark.AddValues(999, errMessage,9);
}


// function to determine the state of the sun
// at the requested station. If the sun is below the horizon,
// the switch for sun/darkness becomes false else true
void miSymbol::setLightStat(bool setting){
  lightstat = setting;
}



void miSymbol::setLightStat(miTime termin, float glat){

  double lat = glat;
  double pcirc = 66.55;
  double doy = double(termin.dayOfYear())+1.0;

  double senit = 90.0 - lat - (90.0-pcirc)*cos((doy+10.0)/365.0*2*M_PI);

  // atmospheric correction/refraction

  senit = senit +1;

  if( lat <= (pcirc+1))
    lightstat =  true;
  else
    lightstat = (senit >= 0);

};

bool
miSymbol::
getLightStat()const
{
	return lightstat;
}




