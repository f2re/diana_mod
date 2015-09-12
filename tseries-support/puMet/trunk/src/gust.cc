/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  $Id: gust.cc 4344 2014-01-02 09:24:27Z juergens $

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


// functions to compute a gust factor or direct gust
// from the wind speed and the  wind direction for several
// airports in Norway

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gust.h"

#include <puTools/miStringFunctions.h>
#include <fstream>

using namespace std;

// functions to access a gust factor
//    1st step : search the station name
//       computes only the gust factor

bool tafGF::getGustFact(const std::string& Name, float DD,float FF, float& gf){
  for (int i=0; i<stations.size();i++){
    if(Name == stations[i].name){
      return(stations[i].getSTgust(DD,FF,gf));
    }
  }
  gf =1;
  return(false);
};


//       like getGustFact, but computes gust

bool tafGF::getGust(const std::string& Name, float DD,float FF, float& gf){
  if(!getGustFact(Name,DD,FF,gf)){
    gf = FF;
    return(false);
  }
  gf = gf*FF;
  return(true);
};


//     2nd step : search the sector and the main speed range

bool station::getSTgust( float DD,float FF, float& gf){
  for (int i=0; i<gust.size(); i++){
    if (gust[i].isinside(DD)){
      for(int spd=0; spd<FFsec.size(); spd++){
	if (FFsec[spd].isinside(FF)){
	  gf = gust[i].value(spd);
	  return(true);
	}
      }
    }
  }
  gf=1;
  return(false);
};


// function to read in all gust factors from file

bool tafGF::readIn(const std::string& fname){
  gustfactor *gtemp;           //  stores all
  sector *stemp;               //  stores a DD-sector
  sector *ftemp;               //  stores a FF-sector
  float sectMin, sectMax;      //  DD-sector Borders
  float FFmin  , FFmax;        //  FF-sector Borders
  float gustInp;               //  The gust factor from file
  string statMi= "";           //  Name of the Station ENAN...
  ifstream gfFile;             //  the input file
  int NOofGF;                  //  Number of Gust factors / DD-sector



  gfFile.open(fname.c_str(),ios::in);
  if(!gfFile)
    return(false);


  while(!gfFile.eof()){

    // reads strings until {

    gfFile>>statMi;
    miutil::trim(statMi);

    if(statMi =="{"){
      station temp;

      // searches for the name of the station

      while(temp.name == "")
	gfFile >> temp.name;


      // reads the Number of GF per sector

      gfFile>>NOofGF;

      // reads the Gf borders

      for (int FFgrp=0; FFgrp< NOofGF; FFgrp++){
	gfFile>>FFmin>>FFmax;
	ftemp = new sector(FFmin,FFmax);
	temp.FFsec.push_back(*ftemp);
      }

      // reads the gustfactors and sector borders
      // jumps to the next station if illegal format occurs
      // (worst case) and ignores the following sectors
      // (no mismatch between sectors and factors)
      // if a whole sector is gone, the sector will be ignored
      // and set to gf 1. (the other sectors will be readed normal)
      // if everything is ok, the station will be readed until
      // a sectMax 360 occurs, or a string (here = " }")

      while(sectMax < 360&& !gfFile.bad()){
	gfFile>>sectMin>>sectMax;
	vector<float>  gusttemp;

	for ( int FFrange=0; FFrange<NOofGF; FFrange++){
	  gfFile>>gustInp;
	  gusttemp.push_back(gustInp);
	}

	stemp = new sector(sectMin,sectMax);
	gtemp = new gustfactor(*stemp,gusttemp);
	temp.gust.push_back(*gtemp);
      }
      gfFile.clear();
      sectMax = 0;
      stations.push_back(temp);
    }
  }
  gfFile.close();
  return(true);
}
