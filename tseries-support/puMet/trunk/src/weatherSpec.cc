/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  $Id: weatherSpec.cc 4344 2014-01-02 09:24:27Z juergens $

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

#include "weatherSpec.h"

using namespace std;
using namespace miutil;


void weatherSpec::AddToList( int i, miString txt1,miString txt2,miString txt3){
  mapping tmp;
  tmp.index = i;
  tmp.txt[0] = txt1;
  tmp.txt[1] = txt2;
  tmp.txt[2] = txt3;
  list.push_back(tmp);
};

miString weatherSpec::textOut(int ind,int txtNum){
   for(int count = 0; count < list.size(); count++){
    if(list[count].index == ind)
      return(list[count].txt[txtNum]);
  }
  return("");
};
miString weatherSpec::text(int ind){
  return textOut(ind,0);
};
int weatherSpec::maxv()
{
  int mx = - 100000;
  for(int c = 0; c < list.size(); c++)
    if(list[c].index > mx)
      mx = list[c].index;
  return mx;
}

int weatherSpec::minv()
{
 int mi = 100000;
  for(int c = 0; c < list.size(); c++)
    if(list[c].index < mi)
      mi = list[c].index;
  return mi;

}
