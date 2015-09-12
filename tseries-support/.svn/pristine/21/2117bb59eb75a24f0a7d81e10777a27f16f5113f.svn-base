/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  $Id$

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



// this little program is ment to be as an translation from the
// vision index you get from miSymbol. The translation can after
// intitialising direct be used by miSymbol....

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vision.h"

using namespace std;
using namespace miutil;


vision::vision(){
/*
 * // As used in RIGG - removed 26.march 2009
    AddToList( 1,"0.5", "0-1",   "0-1"               );
    AddToList( 2,"0.5", "0-1",   "@{atTimes} 0-1"    );
    AddToList( 3,"1",   "0.5-2", "0.5-2"             );
    AddToList( 4,"1",   "0.5-2", "0.5-2 @{inShowers}");
    AddToList( 5,"2",   "1-4",   "1-4"               );
    AddToList( 6,"2",   "1-4",   "1-4 @{inShowers}"  );// new class!
    AddToList( 7,"7",   "4-10",  "4-10"              );
    AddToList( 8,"7",   "4-10",  "4-10 @{inShowers}" );
    AddToList( 9,"10",  "10",    "@{mainly} 10"      );
    AddToList(10,"10+", "10+",   "10+"               );
*/
    AddToList( 1,"0.5", "0-1",   "0-1"               );
    AddToList( 2,"1",   "0-4",   "0-4"               );
    AddToList( 3,"1",   "0.5-2", "0.5-2"             );
    AddToList( 4,"1",   "0.5-2", "0.5-2 @{inShowers}");
    AddToList( 5,"2",   "1-4",   "1-4"               );
    AddToList( 6,"2",   "1-4",   "1-4 @{inShowers}"  );
    AddToList( 7,"6",   "3-8",   "3-8"               );
    AddToList( 8,"7",   "4-10",  "4-10"              );
    AddToList( 9,"7",   "4-10",  "4-10 @{inShowers}" );
    AddToList(10,"8",   "6-10",  "6-10"              );
    AddToList(11,"10+", "10+",   "10+"               );


};


miString vision::singleRange(int inp){
  return textOut(inp,0);
};

miString vision::range(int inp){
  return textOut(inp,1);
};
miString vision::rangeAndtext(int inp){
  return textOut(inp,2);
};

bool vision::signChange(int custom1, int custom2){
  if ((custom1==custom2) || (custom1*custom2== (9*10)) )
    return false;
  return true;
};

int vision::findVision(int inp, int xweath){
  int vis = mapToVis(xweath);
  return ((vis < inp) ? vis : inp);
};

int vision::mapToVis(int inp){
  if(inp == 1 )
    return 2;
  if(inp == 2)
    return 5;
  return 11;
};
















