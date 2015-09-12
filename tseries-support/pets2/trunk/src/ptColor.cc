/*
  libpets2 - presentation and editing of time series

  $Id: ptColor.cc 4354 2014-01-02 13:07:57Z juergens $

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

#include <puTools/miString.h>
#include "ptColor.h"
#include <map>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


using namespace std;


namespace petsColorTools 
{

map<std::string,ptColor> colorlist;

void initColorlist()
{
  map<std::string,ptColor> tmpList;

  tmpList["BLACK"         ]=ptColor("BLACK"        , 0.00, 0.00, 0.00, 1.00);
  tmpList["BLUE"          ]=ptColor("BLUE"         , 0.00, 0.00, 1.00, 1.00);
  tmpList["GREEN"         ]=ptColor("GREEN"        , 0.00, 1.00, 0.00, 1.00);
  tmpList["CYAN"          ]=ptColor("CYAN"         , 0.00, 1.00, 1.00, 1.00);
  tmpList["RED"           ]=ptColor("RED"          , 1.00, 0.00, 0.00, 1.00);
  tmpList["MAGENTA"       ]=ptColor("MAGENTA"      , 1.00, 0.00, 1.00, 1.00);
  tmpList["YELLOW"        ]=ptColor("YELLOW"       , 1.00, 1.00, 0.00, 1.00);
  tmpList["WHITE"         ]=ptColor("WHITE"        , 1.00, 1.00, 1.00, 1.00);
  tmpList["GREY25"        ]=ptColor("GREY25"       , 0.25, 0.25, 0.25, 1.00);
  tmpList["GREY40"        ]=ptColor("GREY40"       , 0.40, 0.40, 0.40, 1.00);
  tmpList["GREY45"        ]=ptColor("GREY45"       , 0.45, 0.45, 0.45, 1.00);
  tmpList["GREY50"        ]=ptColor("GREY50"       , 0.50, 0.50, 0.50, 1.00);
  tmpList["GREY55"        ]=ptColor("GREY55"       , 0.55, 0.55, 0.55, 1.00);
  tmpList["GREY60"        ]=ptColor("GREY60"       , 0.60, 0.60, 0.60, 1.00);
  tmpList["GREY65"        ]=ptColor("GREY65"       , 0.65, 0.65, 0.65, 1.00);
  tmpList["GREY70"        ]=ptColor("GREY70"       , 0.70, 0.70, 0.70, 1.00);
  tmpList["GREY75"        ]=ptColor("GREY75"       , 0.75, 0.75, 0.75, 1.00);
  tmpList["GREY80"        ]=ptColor("GREY80"       , 0.80, 0.80, 0.80, 1.00);
  tmpList["GREY85"        ]=ptColor("GREY85"       , 0.85, 0.85, 0.85, 1.00);
  tmpList["GREY90"        ]=ptColor("GREY90"       , 0.90, 0.90, 0.90, 1.00);
  tmpList["GREY95"        ]=ptColor("GREY95"       , 0.95, 0.95, 0.95, 1.00);
  tmpList["MIST_RED"      ]=ptColor("MIST_RED"     , 0.94, 0.86, 0.86, 1.00);
  tmpList["MIST_GREEN"    ]=ptColor("MIST_GREEN"   , 0.86, 0.94, 0.86, 1.00);
  tmpList["MIST_BLUE"     ]=ptColor("MIST_BLUE"    , 0.86, 0.94, 0.94, 1.00);
  tmpList["DARK_GREEN"    ]=ptColor("DARK_GREEN"   , 0.00, 0.50, 0.50, 1.00);
  tmpList["BROWN"         ]=ptColor("BROWN"        , 0.70, 0.20, 0.00, 1.00);
  tmpList["ORANGE"        ]=ptColor("ORANGE"       , 1.00, 0.35, 0.00, 1.00);
  tmpList["PURPLE"        ]=ptColor("PURPLE"       , 0.30, 0.00, 0.30, 1.00);
  tmpList["LIGHT_BLUE"    ]=ptColor("LIGHT_BLUE"   , 0.20, 0.20, 1.00, 1.00);
  tmpList["DARK_YELLOW"   ]=ptColor("DARK_YELLOW"  , 0.70, 0.70, 0.00, 1.00);
  tmpList["DARK_RED"      ]=ptColor("DARK_RED"     , 0.50, 0.00, 0.00, 1.00);
  tmpList["DARK_BLUE"     ]=ptColor("DARK_BLUE"    , 0.00, 0.00, 0.50, 1.00);
  tmpList["DARK_CYAN"     ]=ptColor("DARK_CYAN"    , 0.00, 0.50, 0.50, 1.00);
  tmpList["DARK_MAGENTA"  ]=ptColor("DARK_MAGENTA" , 0.50, 0.00, 0.50, 1.00);
  tmpList["MIDNIGHT_BLUE" ]=ptColor("MIDNIGHT_BLUE", 0.10, 0.10, 0.43, 1.00);
  tmpList["DNMI_GREEN"    ]=ptColor("DNMI_GREEN"   , 0.17, 0.47, 0.14, 1.00);
  tmpList["DNMI_BLUE"     ]=ptColor("DNMI_BLUE"    , 0.00, 0.21, 0.49, 1.00);
  tmpList["RUST_RED"      ]=ptColor("RUST_RED"     , 0.67, 0.15, 0.11, 1.00);
  tmpList["DAWN_RED"      ]=ptColor("DAWN_RED"     , 1.00, 0.20, 0.00, 1.00);
  tmpList["SUN_YELLOW"    ]=ptColor("SUN_YELLOW"   , 1.00, 0.91, 0.20, 1.00);
  tmpList["SPRING_GREEN"  ]=ptColor("SPRING_GREEN" , 0.71, 0.83, 0.28, 1.00);
  tmpList["IRR_GREEN"     ]=ptColor("IRR_GREEN"    , 0.67, 0.75, 0.03, 1.00);
  tmpList["MOSS_GREEN"    ]=ptColor("MOSS_GREEN"   , 0.60, 0.60, 0.00, 1.00);
  tmpList["GRASS_GREEN"   ]=ptColor("GRASS_GREEN"  , 0.00, 0.40, 0.21, 1.00);
  tmpList["THUNDER_GREY"  ]=ptColor("THUNDER_GREY" , 0.12, 0.10, 0.29, 1.00);
  tmpList["SEA_BLUE"      ]=ptColor("SEA_BLUE"     , 0.00, 0.20, 0.40, 1.00);
  tmpList["SKY_BLUE"      ]=ptColor("SKY_BLUE"     , 0.40, 0.60, 0.80, 1.00);
  tmpList["ICE_BLUE"      ]=ptColor("ICE_BLUE"     , 0.60, 0.80, 0.80, 1.00);
  tmpList["GLACIER_WHITE" ]=ptColor("GLACIER_WHITE", 0.95, 0.95, 0.80, 1.00);
  tmpList["RAIN_GREY"     ]=ptColor("RAIN_GREY"    , 0.56, 0.52, 0.46, 1.00);
  tmpList["IVORY"         ]=ptColor("IVORY"        , 0.93, 0.93, 0.88, 1.00);
  tmpList["DARK_IVORY"    ]=ptColor("DARK_IVORY"   , 0.88, 0.89, 0.84, 1.00);
  tmpList["BLUEGREY"      ]=ptColor("BLUEGREY"     , 0.788,0.796,0.810,1.00);
  tmpList["GREYBROWN"     ]=ptColor("GREYBROWN"    , 0.705,0.660,0.600,1.00);
  tmpList["OLIVE"         ]=ptColor("OLIVE"        , 0.705,0.540,0.660,1.00);
  tmpList["MUDDYGREEN"    ]=ptColor("MUDDYGREEN"   , 0.545,0.557,0.407,1.00);
  tmpList["GREYBLUE"      ]=ptColor("GREYBLUE"     , 0.370,0.640,0.815,1.00);
  tmpList["DARK_BLUEGREY" ]=ptColor("DARK_BLUEGREY", 0.560,0.619,0.705,1.00);
  tmpList["DARKOLIVE"     ]=ptColor("DARKOLIVE"    , 0.540,0.396,0.490,1.00);
  tmpList["RED_YELLOW"    ]=ptColor("RED_YELLOW"   , 1.000,0.870,0.376,1.00);
  tmpList["YELLOW_RED"    ]=ptColor("YELLOW_RED"   , 0.920,0.176,0.180,1.00);

  colorlist=tmpList;
}


ptColor getColorFromList(const std::string& col)
{
  if ( colorlist.empty() )
    initColorlist();

  if(!col.empty() && colorlist.count(col))
    return colorlist[col];

  ptColor dummy;
  return dummy;
}
}


ptColor::ptColor()
{
  name = "BLACK";
  colTable[0] = 0.0;
  colTable[1] = 0.0;
  colTable[2] = 0.0;
  colTable[3] = 1.0;
}


ptColor::ptColor(const std::string& n)
{
  fromStr(n);
}




ptColor::ptColor(const std::string& n, float r, float g, float b, float a)
{
  name=n;
  colTable[0] = r;
  colTable[1] = g;
  colTable[2] = b;
  colTable[3] = a;
}



void ptColor::fromStr(const std::string& icol)
{
  string col = icol,alpha;
  
  vector<string> vs;

  boost::split(vs, col, boost::algorithm::is_any_of(":"));

  if (vs.size() == 5){         // full registration
    float r= atof(vs[1].c_str());
    float g= atof(vs[2].c_str());
    float b= atof(vs[3].c_str());
    float a= atof(vs[4].c_str());
    *this = ptColor(vs[0],r,g,b,a);
    return;

  } else if (vs.size() == 4 &&  !isdigit(vs[0][0])){// registration, no alpha
    float r= atof(vs[1].c_str());
    float g= atof(vs[2].c_str());
    float b= atof(vs[3].c_str());
    *this = ptColor(vs[0],r,g,b,1.0);
    return;

  } else if (vs.size() == 4){  // colourcomponents + alpha
    float r= atof(vs[0].c_str());
    float g= atof(vs[1].c_str());
    float b= atof(vs[2].c_str());
    float a= atof(vs[3].c_str());
    *this = ptColor("UNKNOWN",r,g,b,a);
    return;

  } else if (vs.size() == 3){  // only colourcomponents
    float r= atof(vs[0].c_str());
    float g= atof(vs[1].c_str());
    float b= atof(vs[2].c_str());
    *this = ptColor("UNKNOWN",r,g,b,1.0);
    return;

  } else if (vs.size() == 2){  // name + alpha
    col=   vs[0];
    alpha= vs[1];

  } else if (vs.size() == 1){  // only name
    col=   vs[0];
  }

  *this = petsColorTools::getColorFromList(col);

  if (alpha.length() > 0){
    float a= atof(alpha.c_str());
    colTable[3] = a;
  }
}

std::string ptColor::Color2Str() const
{
  return name;
}

vector<ptColor> ptColor::Str2Colorlist(const std::string& s)
{
  vector<ptColor> list;
  const vector<std::string> slist= miutil::split(s, ",");

  int n= slist.size();
  for (int i=0; i<n; i++){
    ptColor c;
    c.fromStr(slist[i]);
    list.push_back(c);
  }

  return list;
}

std::string ptColor::Colorlist2Str(const vector<ptColor>& list)
{
  std::string buf;
  int n= list.size();
  for (int i=0; i<n; i++){
    if (i>0) buf+= ",";
    buf+= list[i].Color2Str();
  }

  return buf;
}

