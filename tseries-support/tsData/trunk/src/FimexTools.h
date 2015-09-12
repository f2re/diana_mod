/*
 * FimexTools.h
 *
 *  Created on: Aug 8, 2013
 *      Author: juergens
 */

#ifndef FIMEXTOOLS_H_
#define FIMEXTOOLS_H_

/*
 $Id$

 Copyright (C) 2006 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: diana@met.no

 This file is part of generated by met.no

 This is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Tseries; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string>
#include <vector>
#include <map>
#include "PetsExceptions.h"
#include "ptParameterDefinition.h"
#include "ptWeatherParameter.h"
#include "ptTimeLine.h"

namespace pets {

class FimexPoslist {
private:
  std::vector<double> lat;
  std::vector<double> lon;
  std::map<std::string,unsigned int> pos;
public:
  FimexPoslist() {}
  void addFile(std::string filename);
  void addEntry(std::string entry);
  void addEntry(std::string posname, float lat, float lon);
  void clear();

  std::vector<double>  getLat() const { return lat;}
  std::vector<double>  getLon() const { return lon;}
  int getPos(std::string posname, float lat, float lon);
  int getNumPos() const { return lat.size(); }
  int size() const { return lat.size();}


};

struct FimexDimension {
  std::string name;
  unsigned int start;
  int size; // negative means all
  void setFromString(std::string);
};

struct FimexParameter {
  std::string parametername;
  std::vector<FimexDimension> dimensions;
  std::string petsname;
  std::string streamtype;
  std::string unit;
  ParId       parid;

  void setFromString(std::string);
};

struct FimexPetsCache {
  std::vector<WeatherParameter> parameters;
  TimeLine timeLines;
  void getOutpars(std::vector<ParId>& inpar, std::vector<ParId>& outpar);
  void getExtrapars(std::vector<ParId>& inpar, std::vector<ParId>& extrapar);

  std::vector<float> tmp_values;
  std::vector<miutil::miTime> tmp_times;
  void process(ParId pid);
  void clear_tmp();
};




}; /* namespace pets */
#endif /* FIMEXTOOLS_H_ */