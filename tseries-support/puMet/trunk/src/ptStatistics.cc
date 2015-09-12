/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
  $Id: ptStatistics.cc 4344 2014-01-02 09:24:27Z juergens $

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

#include "ptStatistics.h"

using namespace std;

// privates

void ptStatistics::baseInit(const vector<float>& a, int di){
 dimension = di;
 allTimes = a;
 if(dimension)
   tSteps = allTimes.size()/dimension;
 else
   tSteps = 0;
};



void ptStatistics::init(const vector<float>& a, int di, float de){
  baseInit(a,di);
  decils.erase(decils.begin(),decils.end());
  decils.push_back(int(de*di/10));
};


void ptStatistics::init(const vector<float>& a, int di, vector<float> de){
  baseInit(a,di);
  decils.erase(decils.begin(),decils.end());
  for( int i = 0; i< de.size(); i++){
    float f = de[i]/10;  
    decils.push_back( (f < 1 ?  int(f * di) : di-1) );
  }
};

vector<float> ptStatistics::compDecils(){  
  int count;
  vector<float> result(tSteps*decils.size(),0);
  vector<float> OneTime;


  for ( int time = 0 ; time < tSteps; time++){
    OneTime.erase(OneTime.begin(), OneTime.end());
    for( int dim = 0; dim < dimension; dim++){
      count = tSteps*dim + time;
      OneTime.push_back(allTimes[count]);
    }
    sort(OneTime.begin(),OneTime.end());
    for(int dz = 0; dz < decils.size(); dz++)
      result[dz*tSteps+time]=OneTime[decils[dz]];
  }
  return result;
};



vector<float> ptStatistics::compMean(){
  vector<float> result;
  float mean;
  int count;
  if(dimension){
    for ( int time = 0 ; time < tSteps; time++){
      mean = 0;
      for( int dim = 0; dim < dimension; dim++){
	count = tSteps*dim + time;
	mean +=(allTimes[count]);
      }
      mean /= float(dimension);
      result.push_back(mean);
    }
  }
  return result;
};

// publics


vector<float> ptStatistics::getXdecil( const vector<float>& a, int di, float de){
  init(a,di,de);
  return compDecils();
};


vector<float> ptStatistics::getXdecil( const vector<float>& a, int di,vector<float> de){
  init(a,di,de);
  return compDecils();
};


vector<float> ptStatistics::getMean(const vector<float>& a, int di){
  baseInit(a,di);
  return compMean();
};


vector<float> ptStatistics::getStDev(const vector<float>& a, int di){
  vector<float> result = getVar(a,di);
  for( int i = 0; i < result.size(); i++)
    result[i] = sqrt(result[i]);
  return result;
};


vector<float> ptStatistics::getVar(const vector<float>& a, int di){
  baseInit(a,di);
  vector<float> mean = compMean();
  vector<float> result;
  int count;
  float variance;

  if( dimension > 1){
     for ( int time = 0 ; time < tSteps; time++){
      variance = 0;
      for( int dim = 0; dim < dimension; dim++){
	count = tSteps*dim + time;
	variance +=pow((allTimes[count] - mean[time]),2);
      }
      variance /= (dimension-1);
      result.push_back(variance);
     }
  }
  return result;
};

