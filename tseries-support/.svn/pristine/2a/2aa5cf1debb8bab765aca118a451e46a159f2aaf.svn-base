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


// ptDateElement.cc : Definitions for DateElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptDateElement.h>
#include <iostream>
#include <stdio.h>
#include <list>

using namespace miutil;
using namespace std;

DateElement::DateElement(const vector<miTime> tline,
			 const ptVertFieldf& field,
			 const Layout& layout,
			 XAxisInfo* xtime)

  : PlotElement(layout, field, xtime)
  , timeLine(tline)
  , labeltext(layout.text)
  , language(layout.language)
  , asNumber(layout.asNumber)
  , datestyle(layout.datestyle)
  , plotlabel(layout.label)
  , minSkipX(layout.minSkipX)
{
#ifdef DEBUG
  cout << "Inside DateElement's constructor" << endl;
#endif
  type=DATE;
  lang= (language=="NO" ? miDate::Norwegian :  miDate::English);

  if (plotlabel && !labeltext.exists()){
    if (datestyle == DS_DATE)
      labeltext= (lang==miDate::English ? "Date" : "Dato");
    else if (datestyle == DS_WEEK)
      labeltext= (lang==miDate::English ? "Week" : "Uke");
    else if (datestyle == DS_MONTH)
      labeltext= (lang==miDate::English ? "Month" : "Måned");
    else if (datestyle == DS_DAY)
      labeltext= (lang==miDate::English ? "Day" : "Dag");
    else if (datestyle == DS_YEAR)
      labeltext= (lang==miDate::English ? "Year" : "År");
  }
}

miString DateElement::dataAsString(const miDate& date)
{
  miString txt;
  if (datestyle == DS_DATE){
    txt= miString(date.day())+"/"+miString(date.month());
  } else if (datestyle == DS_WEEK){
    txt= miString(date.weekNo());
  } else if (datestyle == DS_MONTH){
    if (asNumber)
      txt= miString(date.month());
    else
      txt= date.shortmonthname(lang);
  } else if (datestyle == DS_DAY){
    if (asNumber)
      txt= miString(date.dayOfYear());
    else
      txt= date.shortweekday(lang);
  } else if (datestyle == DS_YEAR){
    txt= miString(date.year());
  }
  return txt;
}

void DateElement::plot()
{
  miString txt;
  miDate curDate = timeLine[startT].date();
  float offset,th;
  float prevf; // end of previous text

  if (enabled && visible) {
#ifdef DEBUG
    cout << "DateElement::plot()" << endl;
#endif
    _prepFont();
    _setColor(color);

    // print label on left margin
    prevf= 0;
    if (plotlabel){
      txt= labeltext;
      if (labeltext=="$YEAR")
	txt= miString(curDate.year());
      _printString(txt,20,startY);
      _updatePrinting();
      _getStringSize(txt,offset,th);
      prevf= 20+offset;
    }

    // print date for each new day/week/month/year
    // (centered on each day - or preferably under 12-hour)
    bool active= false;
    float startx=startT, stopx;
    list<int> li12;

    for (int i=startT; i<=stopT; i++){
      if (timeLine[i].hour() == 12){
	li12.push_back(i);
      }

      if (i!=startT){
	if (timeLine[i].date()==timeLine[i-1].date())
	  continue;
	if (datestyle==DS_WEEK){
	  li12.clear();
	  if (timeLine[i].weekNo()==timeLine[i-1].weekNo() &&
	      timeLine[i].year()==timeLine[i-1].year())
	    continue;
	} else if (datestyle==DS_MONTH){
	  li12.clear();
	  if (timeLine[i].month()==timeLine[i-1].month() &&
	      timeLine[i].year()==timeLine[i-1].year())
	    continue;
	} else if (datestyle==DS_YEAR){
	  li12.clear();
	  if (timeLine[i].year()==timeLine[i-1].year())
	    continue;
	}
      }

      if (active){
	// print previous day/week/month/year
	float sx;
	if (li12.size() > 0){
	  int i12= *(li12.begin());
	  li12.pop_front();
	  sx= xtime->xcoord[i12] - offset;
	} else {
	  stopx= xtime->xcoord[i];
	  sx= (stopx+startx)/2.0 - offset;
	}
	// print if room
	if (sx >= prevf){
	  _printString(txt,sx,startY);
	  _updatePrinting();
	  prevf= sx+2*offset;
	}
      }

      curDate = timeLine[i].date();
      txt= dataAsString(curDate);
      _getStringSize(txt,offset,th);
      startx= xtime->xcoord[i];
      offset = offset/2.0;
      active=true;
    }

    // print last date also
    if (active){
      // print previous day...
      float sx;
      if (li12.size() > 0){
	int i12= *(li12.begin());
	sx= xtime->xcoord[i12] - offset;
      } else {
	stopx= xtime->xcoord[stopT];
	sx= (stopx+startx)/2.0 - offset;
      }
      // print if room
      if (sx >= prevf){
	_printString(txt,sx,startY);
	_updatePrinting();
      }
    }

  }
}
