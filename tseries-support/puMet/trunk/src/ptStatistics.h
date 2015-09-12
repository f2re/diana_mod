/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
  $Id: ptStatistics.h 4344 2014-01-02 09:24:27Z juergens $

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


// program to compute div. statistical parameters for
// display with PETS.
// input is always a vector<float>, which is sorted
// first by parameter, then by time : var1tim1, var1tim2,...var1timN,var2tim1.....varNtimN 
// Pu-JS 1/99

#ifndef _ptStatistics_h
#define _ptStatistics_h

#include <math.h>

#include <vector>
#include <algorithm>

class ptStatistics {
private:
  std::vector<float> allTimes;       // all dimensions all times
  std::vector<int> decils;           // requested decils (place in the dim vect)
  int dimension;                // number of dimensions
  int tSteps;                   // time steps (vectorElements/dimensions)
  float decil;                  // Input decil ( 0.1 decil = 1 percentil)
  void baseInit(const std::vector<float>&, int);
  void init(const std::vector<float>&, int, float);
  void init(const std::vector<float>&, int, std::vector<float>);
  std::vector<float> compDecils();
  std::vector<float> compMean();
public:
  ptStatistics(){}
  std::vector<float> getXdecil(const std::vector<float>&, int, float);
  std::vector<float> getXdecil(const std::vector<float>&, int, std::vector<float>);
  std::vector<float> getMedian(const std::vector<float>& a, int d)
    {return getXdecil(a,d,5);}

  std::vector<float> getMean(const std::vector<float>&, int);
  std::vector<float> getStDev(const std::vector<float>&, int);
  std::vector<float> getVar(const std::vector<float>&, int);
};

#endif




