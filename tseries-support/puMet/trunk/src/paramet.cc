/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  $Id: paramet.cc 4344 2014-01-02 09:24:27Z juergens $

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


// returns the termin specific value of the requested parameter

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "paramet.h"

using namespace std;
using namespace miutil;

float paramet::DUMMY=-999;


float paramet::value( miTime termin){
  return (mapVal.count(termin) > 0 ? mapVal[termin] : DUMMY );
};

void paramet::erase(){
  mapVal.clear();
}

void paramet::push(miTime termin, float value){
  mapVal[termin] = value;
};

miTime paramet::getTime(int i){
  map<miTime,float>::iterator itr = mapVal.begin();
  for( ; i>0; --i)
    ++itr;
  return itr->first;
}

