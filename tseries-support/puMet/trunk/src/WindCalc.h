
#ifndef WINDCALC_H
#define WINDCALC_H

/*
  $Id: WindCalc.h 4344 2014-01-02 09:24:27Z juergens $

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

#include <puTools/miString.h>
#include <map>

class WindCalc {
private:
  //  height   stability
  std::map<int, std::map<int, float> > gust_factors;
  //  height   stability
  std::map<int, std::map<int, float> > wind_factors;


public:
  WindCalc() {}

  bool readFactors(const miutil::miString& filename);
  float meanWind(float wind10m, int stability, int height);
  float gust(float wind10m, int stability, int height);
  static int beaufort(float wind10m, miutil::miString & text);

};

#endif
