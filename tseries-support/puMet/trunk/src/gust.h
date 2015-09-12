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


// reads gust factors from file and give functions to access them.
// all the functions to access the factors are in Class tafGF at
// the end of this File
// juergen schulze, june 1998


#ifndef _gust_h
#define _gust_h

#include <string>
#include <vector>

// pass the wind direction into a sector and the
// wind speed into a range (minimum and maximum)

class sector {
private:
  float a1, a2;          // sector minimum and maximum
public:
  sector(){}
  sector(const float i1, const float i2) : a1(i1), a2(i2) {}
  bool isinside(const float a) const { return (a>=a1 && a<a2); }
};


// stores the gustfactor and the sector information

class gustfactor {
private:
  sector  DDsec;         // sector (DD depended)
  std::vector<float> gf;      // gustfactor for different wind speeds
public:
  gustfactor(){}
  gustfactor(const sector& s, const std::vector<float>& g)
    : DDsec(s) , gf(g) {}

  bool isinside(const float a) const { return DDsec.isinside(a); }
  float value(int i){return gf[i] ;}
};


// stores all the factors of one station in a vector

class station {

public:
  std::string name;              // station name (i.e. ENAN)
  std::vector<gustfactor> gust;    // vector with all gf-vectors/sort.by DD
  std::vector<sector> FFsec;       // the FF sectors (station depended)
  bool getSTgust(float DD,float FF, float& gf );
};



// stores all stations in a vector + gf access functions
// readIn has to be called once, to initialize the fields.
// Call it with the whole filename as a miString.
// getGust and getGustFact returns true for founded
// stations and factors. If no GF is found, the GF is set to 1
// by default. The Gust factor 1 is impossible in real
// nature, so if you got a maximum wind, which is like
// the average wind, an error has occured
// (1st guess station doesnt exist in our gf list.....)
// getGust computes gust (gf=factor*FF)
// getGustFact computes  gust factor (gf = factor)
// readIn reads all the factors in from file and initalize the fields
// more information about readIn is written in gust.cc

class tafGF {
private:
  std::vector<station> stations;   // vector with all stations
public:
  tafGF() {}
  bool readIn(const std::string& fname);
  bool getGustFact(const std::string& name, float DD,float FF, float& gf);
  bool getGust(const std::string& Name, float DD,float FF, float& gf);

};

#endif
