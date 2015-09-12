/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

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


#ifndef _paramet_h
#define _paramet_h

#include "miSymbol.h"

#include <puTools/miString.h>
#include <puTools/miTime.h>

#include <vector>
#include <map>

// 2nd hold different time steps and a parameter number

class paramet {
  friend class symbolMaker;
private:
  static float DUMMY;
  int paraNO;                  //  Parameter Number i.e. 31 for t2m
  float glat;                  //  latitude of the station for lightstatus
  std::map<miutil::miTime,float> mapVal;
public:
  paramet(){}
  paramet(const int& n)
    : paraNO(n){}
  paramet(const int& n,const std::map<miutil::miTime,float>& a )
    : paraNO(n), mapVal(a) {}

  float lat() const {return glat;}
  float value(miutil::miTime);
  miutil::miTime getTime(int);
  int size() const {return mapVal.size();};
  void  push(miutil::miTime, float);
  void  erase();
  void  AddPara(const int& n,const std::map<miutil::miTime,float>& a,const float& gl = 60){
    paraNO = n;
    glat = gl;
    mapVal = a;
  }


};

#endif



