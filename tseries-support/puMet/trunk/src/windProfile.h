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


// windProfile.h  program to compute wind Profiles over water surfaces,
// by using the Louis parametrisation

#ifndef _windProfile_h
#define _windProfile_h

#include <string>
#include <vector>

// stores the single point speed at height z and z itself
// used private by class Uprofile

class values {
  friend class Uprofile;
private:
  double z;
  double speedOut;
public:
  values(){}
  values( double i1, double i2)
    : z(i1),
      speedOut(i2){}
};


// computes the profile and stores it into a vector wind.
// the class works with three overloaded compute functions.
// the constructor deserves the model wind and height, and the
// Richardson Bulk number to initialize the stability.
// after this the class works with three overloaded compute functions.
//   all of them deserves:
//         zref       : the height of the known wind (HIRLAM or edited)
//         speedZref  : the known wind at height zref
//in addition deserves
//   the first case :
//         z          : a single height
//   the second case:
//         z          : the maximum height of the computed profile
//                      (starting at zZero)
//         increm     : an increment between the height steps
//   the third case:
//         vector z   : a vector containing  a couple of heights z
//                      zero values will be transformed into zZero,
//                      the returned profile is sorted by height.
// the compute functions returns nothing, but compute a new vector
// wind, which can be accessed by the functions height() and speed()

class Uprofile {
private:
  std::string errorOut;
  double Ustar ;
  std::vector <values> wind;
  double Ri;
  bool stability(double z, double speedZ);
public:
  // Uprofile(){}

  void setRi(double mslp, double sst, double thetaN,double zref, double speedZref);
  void editRi(double rich);
  float compute(double zref,double speedZref, double z);
  bool  compute(double zref,double speedZref, double z, double increm);
  bool  compute(double zref,double speedZref, std::vector <double> z);

  int length() const {return wind.size();};

  double height(int i) const {return wind[i].z;};
  double speed(int i) const {return wind[i].speedOut;};
  double gust(double z, double spd);
  double gust(int i);
  double getRi() const {return Ri;}
  void error();
};

#endif
