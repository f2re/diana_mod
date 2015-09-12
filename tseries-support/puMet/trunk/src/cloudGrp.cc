/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
  $Id: cloudGrp.cc 4344 2014-01-02 09:24:27Z juergens $

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

#include "cloudGrp.h"

cloudGrp::cloudGrp(){
  AddToList(   1, "1-2/3000"     ); 
  AddToList(   2, "3-6/2000"     );
  AddToList(   3, "4-8/1000-2000");
  AddToList(   4, "7-8/1000-1500"); // not used !
  AddToList(   5, "4-8/1000-1500");
  AddToList(   6, "7-8/0500-1000");
  AddToList(   7, "4-8/0700-1500");
  AddToList(   8, "7-8/0700-1500");
  AddToList(   9, "4-7/0800-1500");
  AddToList(  10, "8/0100-0300 @{ifFog}");
};
  
int cloudGrp::findCl(symbolMaker::Symboltype symb,const int& addW){

 if (addW == 1)
     return 10;

  if(symb == symbolMaker::Fog)
    return 9;

  if(symb == symbolMaker::Sleet       ||
     symb == symbolMaker::Rain        ||
     symb == symbolMaker::LightRain)
    return 8;    
 
  if(symb == symbolMaker::SleetSun    ||
     symb == symbolMaker::SnowSun     )
    return 7;
 
  if(symb == symbolMaker::Snow        ||
     symb == symbolMaker::RainThunder ||
     symb == symbolMaker::SnowThunder )
    return 6;

  if(symb == symbolMaker::LightRainSun||
     symb == symbolMaker::LightRainThunderSun)
    return 5;

  if(symb == symbolMaker::Cloud       )
    return 3;
  if(symb == symbolMaker::LightCloud  ||
     symb == symbolMaker::PartlyCloud )
    return 2;
  if(symb == symbolMaker::Sun  ) 
    return 1;
  return 0;
};

bool cloudGrp::signChange(int cloud1,int cloud2)
{
  return (cloud1 != cloud2);
};
 






