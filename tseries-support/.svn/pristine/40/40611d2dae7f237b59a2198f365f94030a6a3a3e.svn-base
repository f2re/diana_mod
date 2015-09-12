/*
  libpets2 - presentation and editing of time series

  Copyright (C) 2013 met.no

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


// ptUTCElement.cc : Definitions for UTCElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptPlotElement.h"
#include "ptUTCElement.h"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace miutil;

UTCElement::UTCElement(const std::vector<miTime> tline,
		       const ptVertFieldf& field,
		       const Layout& layout,
		       XAxisInfo* xtime)

  :PlotElement(layout, field, xtime),
   minSkipX(layout.minSkipX), label(layout.label), text(layout.text),
   modhours(layout.modhours)
{
#ifdef DEBUG
  cout << "Inside UTCElement's constructor" << endl;
#endif
  type=UTC;
  timeLine = tline;
  if (label && !text.exists()) text= "UTC";
  if (modhours.size()==0){
    modhours.push_back(12);
    modhours.push_back(6);
    modhours.push_back(3);
    modhours.push_back(1);
  }
}


float UTCElement::plottime(const miTime& t, const int i,
			   const bool minute, const float cwid)
{
  std::ostringstream ost;
  float offset;
  if (minute){
    ost << std::setw(2) << std::setfill('0') << t.hour() << ":" << std::setw(2) << t.min();
    offset= cwid*2.5;
  } else {
    ost << std::setw(2) << std::setfill('0') << t.hour();
    offset= cwid;
  }
  _printString(ost.str().c_str(),xtime->xcoord[i]-offset,startY);
  return (xtime->xcoord[i]+offset);
}

void UTCElement::plot()
{
  if (enabled && visible) {
    float th,tw;
    _prepFont();
    _getCharSize('0',tw,th);

    int interv= miTime::minDiff(timeLine[stopT],timeLine[startT]);
    bool plotmin= (interv < 3*60);
    bool plotallmin= (interv < 3*60 && (stopT-startT) < 20);
    float prev;
    float minw= pixWidth;//minSkipX*pixWidth;
    float guess= tw*(plotmin ? 2.5 : 1.0);

    _setColor(color);
    bool *taken= new bool[stopT+2];
    for (int i=0; i<stopT+2; i++) taken[i]= false;

    for ( int k=0; k<modhours.size(); k++){
      prev= -1000;
      for (int i=startT; i<=stopT; i++) {
	if (!taken[i]){
	  miTime t= timeLine[i];
	  if (t.hour() % modhours[k] == 0){
	    if (!plotallmin)
	      if (t.min()!=0)
		continue;
	    if ((xtime->xcoord[i]-guess-prev) < minw)
	      continue;
	    if ( k!=0 ){
	      // find next taken
	      int nextt=i+1;
	      for (int j=i+1; j<stopT; j++){
		if (taken[j]){
		  nextt=j;
		  break;
		}
	      }
	      if (taken[nextt])
		if (((xtime->xcoord[nextt]-guess)-(xtime->xcoord[i]+guess)) < minw)
		  continue;
	    }
	    prev= plottime(t,i,plotmin,tw);
	    taken[i]= true;
	  }
	} else
	  prev= xtime->xcoord[i] + guess;
      }
    }

//     // plot 0,12
//     prev= -1000;
//     for (int i=startT; i<=stopT; i++) {
//       miTime t= timeLine[i];
//       if (t.hour() % 12 == 0){
// 	if (!plotallmin)
// 	  if (t.min()!=0)
// 	    continue;
// 	if ((xtime->xcoord[i]-guess-prev) < minw)
// 	  continue;
// 	prev= plottime(t,i,plotmin,tw);
// 	taken[i]= true;
//       }
//     }
//     // plot 6,18
//     prev= -1000;
//     for (int i=startT; i<=stopT; i++) {
//       if (!taken[i]){
// 	miTime t= timeLine[i];
// 	if (t.hour() % 6 == 0){
// 	  if (!plotallmin)
// 	    if (t.min()!=0)
// 	      continue;
// 	  if ((xtime->xcoord[i]-guess-prev) < minw)
// 	    continue;
// 	  if (taken[i+1])
// 	    if (((xtime->xcoord[i+1]-guess)-(xtime->xcoord[i]+guess)) < minw)
// 	      continue;
// 	  prev= plottime(t,i,plotmin,tw);
// 	  taken[i]= true;
// 	}
//       } else
// 	prev= xtime->xcoord[i] + guess;
//     }
//     // plot 3,9,15,21
//     prev= -1000;
//     for (int i=startT; i<=stopT; i++) {
//       if (!taken[i]){
// 	miTime t= timeLine[i];
// 	if (t.hour() % 3 == 0){
// 	  if (!plotallmin)
// 	    if (t.min()!=0)
// 	      continue;
// 	  if ((xtime->xcoord[i]-guess-prev) < minw)
// 	    continue;
// 	  if (taken[i+1])
// 	    if (((xtime->xcoord[i+1]-guess)-(xtime->xcoord[i]+guess)) < minw)
// 	      continue;
// 	  prev= plottime(t,i,plotmin,tw);
// 	  taken[i]= true;
// 	}
//       } else
// 	prev= xtime->xcoord[i] + guess;
//     }
//     // plot the rest
//     prev= -1000;
//     for (int i=startT; i<=stopT; i++) {
//       if (!taken[i]){
// 	miTime t= timeLine[i];
// 	if (!plotallmin)
// 	  if (t.min()!=0)
// 	    continue;
// 	if ((xtime->xcoord[i]-guess-prev) < minw)
// 	  continue;
// 	if (taken[i+1])
// 	  if (((xtime->xcoord[i+1]-guess)-(xtime->xcoord[i]+guess)) < minw)
// 	    continue;
// 	prev= plottime(t,i,plotmin,tw);
// 	taken[i]= true;
//       } else
// 	prev= xtime->xcoord[i] + guess;
//     }

    delete[] taken;
    _printString(text,20,startY);
    _updatePrinting();
  }
}

