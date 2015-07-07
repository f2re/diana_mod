/*
  Diana - A Free Meteorological Visualisation Tool

  $Id: diArea.cc 3309 2010-06-29 11:37:34Z lisbethb $

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This file is part of Diana

  Diana is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Diana is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Diana; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "FieldSpecTranslation.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#define MILOGGER_CATEGORY "diField.FieldSpecTranslation"
#include "miLogger/miLogging.h"

using namespace std;

FieldSpecTranslation::FieldSpecTranslation(){
}

std::string FieldSpecTranslation::getNewFieldString(std::string pin, bool decodeModel)
{
  std::ostringstream ost;

  vector<string> tokens;
  boost::tokenizer<boost::escaped_list_separator<char> > t(pin, boost::escaped_list_separator<char>("\\", " ", "\""));
  BOOST_FOREACH(string s,t){
    if ( !s.empty() )
      tokens.push_back(s);
  }

  size_t n = tokens.size();

  std::string modelString;
  std::string parameterString;

  if ( n > 2 && tokens[0] == "FIELD") {
    //FIELD <modelName> <plotName>
    ost <<tokens[0];
    modelString = tokens[1];
    parameterString = tokens[2];
  } else if ( n > 1) {
    // <modelName> <plotName>
    modelString = tokens[0];
    parameterString = tokens[1];
  } else {
    return ost.str();
  }


  //model
  if (decodeModel ) {
    ost << getModelString( modelString);
  } else {
    ost << " model="<<modelString;
  }

  //parameter
  ost <<" plot="<< parameterString;

  std::vector<std::string> vtoken;
  std::string str, key;

  for (size_t k = 2; k < n; k++) {
    boost::algorithm::split(vtoken, tokens[k], boost::algorithm::is_any_of("="));
    if (vtoken.size() >= 2 ) {
      if (  vtoken[1] == "0m"&& decodeModel) {
        continue; //vertical level 3, level 0m from felt should have been no vertical coordinate
      }
      key = boost::algorithm::to_lower_copy(vtoken[0]);
      if (key == "level" ) {
        ost <<" vcoord="<<getVcoorFromLevel(vtoken[1])<<" vlevel="<<vtoken[1];
      } else if (key == "idnum") {
        ost << " ecoord=eps elevel=" <<vtoken[1];
      } else {
        ost << " " <<tokens[k];
      }
    }
  }

  return ost.str();
}

std::string FieldSpecTranslation::getModelString(std::string model)
{
  std::ostringstream ost;
  //  boost::algorithm::to_lower(model);

  std::string name = model;
  bool hasGrid = false;
  std::string grid;
  std::string refhour;
  std::string refoffset;

  // split model and grid
  const char* optionFirst= "([{<";
  const char* optionLast=  ")]}>";
  std::string::size_type l= name.length()-1;
  size_t i= 0;

  while (i<4 && name[l]!=optionLast[i]) i++;
  if (i<4) {
    if (((l=name.find_last_of(optionFirst[i]))!=std::string::npos) && l>0) {
      int nn = name.length()-2-l;
      grid = name.substr(l+1, nn);
      name = name.substr(0,l);
      hasGrid = true;
    }
  }

  //find reference time offset model.00-1 -> refoffset = -1
  METLIBS_LOG_DEBUG(name.find_last_of("-")<<" : "<< name.length());
  if ( name.find_last_of("-") == name.length()-2 ) {
    refoffset = name.substr(name.find_last_of("-"));
    name = name.erase(name.find_last_of("-"));
  }
  //find hour of reference time  model.00-1 -> refhour = 00
  size_t index = name.find_last_of(".");
  if ( index == name.length()-3 ) {
    std::string hour = name.substr(index+1,2);
    if(all(hour,boost::is_digit()) ){
      refhour = hour;
      name = name.erase(index);
    }
  }

  // Old syntax: HIRLAM.12KM.00-2(12) -> newsyntax HIRLAM.12KM(12).00-2
  if ( name == "HIRLAM.12KM" ) {
    ost <<" model="<<name;
    if ( hasGrid ) {
      ost <<"("<<grid<<")";
    }

    if( !refhour.empty()) {
      ost <<"."<<refhour;
    }

    if( !refoffset.empty()) {
      ost <<refoffset;
    }

  } else {
    ost <<" model="<<name;
    if ( hasGrid ) {
      ost <<"("<<grid<<")";
    }

    if( !refhour.empty()) {
      ost <<" refhour="<<refhour;
    }

    if( !refoffset.empty()) {
      ost <<" refoffset="<<refoffset;
    }
  }

  return ost.str();


}

std::string FieldSpecTranslation::getVcoorFromLevel(std::string level)
{

 // boost::algorithm::to_lower(level);
  if(level.find("hPa") != std::string::npos) {
    return "pressure";
  }
  if(level.find("E.") != std::string::npos) {
    return "k";
  }
  if(level.find("L.") != std::string::npos) {
    return "model";
  }
  if(level.find("FL") != std::string::npos) {
    return "flightlevel";
  }
  if(level.find("k") != std::string::npos) {
    return "theta";
  }
  if(level.find("pvu") != std::string::npos) {
    return "pvu";
  }
  if(level.find("C") != std::string::npos) {
    return "temperature";
  }
  if(level.find("m") != std::string::npos) {
    return "depth";
  }
  return "";
}

std::string FieldSpecTranslation::getVlevelFromLevel(std::string level)
{

  //boost::algorithm::to_lower(level);
  if(level.find("hPa") != std::string::npos) {
    return level.erase(level.find("hPa"));
  }
  if(level.find("E.") != std::string::npos) {
    return level.erase(level.find("e."),2);
  }
  if(level.find("L.") != std::string::npos) {
    return level.erase(level.find("l."),2);
  }
  if(level.find("FL") != std::string::npos) {
    return level.erase(level.find("fl"),2);
  }
  if(level.find("K") != std::string::npos) {
    return level.erase(level.find("k"));
  }
  if(level.find("pvu") != std::string::npos) {
    return level.erase(level.find("pvu"));
  }
  if(level.find("C") != std::string::npos) {
    return level.erase(level.find("c"));
  }
  if(level.find("m") != std::string::npos) {
    return level.erase(level.find("m"));
  }
  return level;
}

std::string FieldSpecTranslation::getOldModel(const std::string& model, int refHour, int refOffset, const std::string& grid )
{

  std::ostringstream ost;

  ost <<model;

  if(refHour > -1) {
    ost <<"."<< std::setfill('0') << std::setw(2)<<refHour;
  }

  if(refOffset > 0) {
    ost <<"-"<<refOffset;
  }

  if(!grid.empty()) {
    ost <<"("<<grid<<")";
  }
  return ost.str();

}

std::string FieldSpecTranslation::getOldLevel(const std::string& vcoord, const std::string& vlevel)
{

  std::ostringstream ost;
  if (vcoord == "sigma") {
    ost <<vlevel<<"hPa";
  }
  else if (vcoord == "pressure") {
    ost <<vlevel<<"hPa";
  }
  else if (vcoord == "flightlevel") {
    ost <<"FL"<<vlevel;
  }
  else if (vcoord == "model") {
    ost <<"L."<<vlevel;
  }
  else if (vcoord == "hybrid") {
    ost <<"E."<<vlevel;
  }
  else if (vcoord == "k") {
    ost <<"E."<<vlevel;
  }
  else if (vcoord == "isentropic") {
    ost <<vlevel<<"K";
  }
  else if (vcoord == "pvu") {
    ost <<vlevel<<"pvu";
  }
  else if (vcoord == "temperature") {
    ost <<vlevel<<"C";
  }
  else if (vcoord == "depth") {
    ost <<vlevel<<"m";
  }
  else
    ost <<vlevel;

  return ost.str();
}
