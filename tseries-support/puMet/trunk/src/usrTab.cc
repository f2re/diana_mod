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

#include "usrTab.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace miutil;


bool usrTab::create(miString infile){

  ifstream inp;
  miString tmpStr;
  vector<miString> tmpVec;


  inp.open(infile.c_str(),ios::in);
  if(!infile.exists()){
    cerr<<"No setup file specified!"<<endl;
    return false;
  }
  if( !inp ){
    cerr<<infile<<" not found .... "<<endl;
    return false;
  }
  entity.clear();

  while(inp){
    tmpStr = mNull;
    getline(inp,tmpStr);

    if(tmpStr.contains("#"))
      continue;

    if(tmpStr == mNull)
      continue;

    if(tmpStr.contains("=")){
      tmpVec = tmpStr.split('=');
      entity[tmpVec[0]] = tmpVec[1];
    }
  }
  inp.close();
  return true;
};


miTime usrTab::dates(miString index){

  miTime tmpTime = miTime::nowTime();
  miString tmpStr;

  tmpStr = entity[index];


  if(tmpStr.exists()){
    const std::vector<miString> tmpVec = tmpStr.split();

    if (tmpVec.size() > 2) {
        const int year = atoi(tmpVec[0].c_str());
        const int month = atoi(tmpVec[1].c_str());
        
        int hour;
        if (tmpVec.size() > 3)
            hour = atoi(tmpVec[3].c_str());
        else
            hour = 12;
        
        const int day = atoi(tmpVec[2].c_str());
        tmpTime.setTime(year, month, day, hour);
    }
  }

  return tmpTime;
};


map<miString,miString> usrTab::selected(const vector<miString>& sel){
  map<miString,miString> tmp;
  miString s;
  for ( int i = 0; i < sel.size(); i++ ){
    s = sel[i];
    if( bool(entity.count(s) ) )
      tmp[s] = entity[s];
  }
  return tmp;
};

