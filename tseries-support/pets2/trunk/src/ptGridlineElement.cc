/*
  libpets2 - presentation and editing of time series

  $Id: ptGridlineElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptGridlineElement.cc : Definitions for GridlineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptGridlineElement.h>
#include <iostream>
#include <stdio.h>

using namespace miutil;

GridlineElement::GridlineElement(const std::vector<miTime> tline,
				 const ptVertFieldf& field,
				 const Layout& layout,
				 XAxisInfo* xtime)
  : PlotElement(layout,field,xtime)
  , numTickMajor(layout.numTickMajor)
  , numTickMinor(layout.numTickMinor)
  , axeStopY(field.y2)
  , labelSpace(layout.labelSpace)
  , lineWidth(layout.lineWidth)
  , style(layout.linePattern)
  , gridxonly(layout.gridxonly)
  , plotDayPattern(layout.plotDayPattern)
  , useTimes(layout.useTimes)
  , daycolor(layout.color2)
  , timeLine(tline)
{
#ifdef DEBUG
  cout << "Inside GridlineElement's constructor" << endl;
#endif
  type=GRIDLINE;
  deltaY = axeStopY - startY - labelSpace;

  if (plotDayPattern){
    // find all daychanges in timeline
    int n= timeLine.size();
    daychanges.push_back(0);
    for (int i=1; i<n; i++)
      if (timeLine[i].day()!=timeLine[i-1].day())
	daychanges.push_back(i);
    if (daychanges[daychanges.size()-1]!=n-1)
      daychanges.push_back(n-1);
  }
}


void GridlineElement::plot()
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "GridlineElement::plot()" <<endl;
#endif
    _setColor(color);

    bool plotday= true;
    std::vector<int> daych;
    int i;
    bool fakestipple = false;
    if ((!useColour) || pInColour){
      if (!useFakeStipple) {
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(LineStyle[style][0],LineStyle[style][1]);
      } else fakestipple = true;
    }
    glLineWidth(lineWidth);
    glPointSize(lineWidth/2.0);

    if (!gridxonly){
      float matick = deltaY/(numTickMajor*1.0);
      float fy;

      fy = startY;
      for (i=0; i<numTickMajor;i++){
	fy+= matick;

	if (fakestipple){
	  _glBegin(GL_POINTS,1000);
	  lineSegment(xtime->x1,fy,
		      xtime->x2,fy,
		      LineStyle[style][0],
		      LineStyle[style][1],
		      true);
	  _glEnd();
	} else {
	  _glBegin(GL_LINES,2);
	  glVertex2f(xtime->x1,fy);
	  glVertex2f(xtime->x2,fy);
	  _glEnd();
	}
      }
    }
    _updatePrinting();

    int d,n= daychanges.size();
    int d1, d2;
    if (plotDayPattern)
      for (d=0; d<n && daychanges[d]<=startT; d++)
	plotday= !plotday;

    int i1= (xtime->xcoord[startT] == xtime->x1 ? startT+1 : startT);
    int i2= (xtime->xcoord[stopT]  == xtime->x2 ? stopT-1 : stopT);

    _setColor(daycolor);
    for (i=i1; i<=i2; i++) {
      // plot alternating day-backgrounds
      if (plotDayPattern){
	d2= daychanges[d];
	if (i==d2){
	  if (plotday) {
	    d1= daychanges[d-1];
	    if (d1<startT) d1= startT;
	    glRectf(xtime->xcoord[d1],startY+2,
		    xtime->xcoord[d2],startY+deltaY-2);
	  }
	  plotday= !plotday;
	  d++;
	}
      }
    }
    // plot last day-background
    if (plotDayPattern && plotday){
      d2= stopT;
      if (daychanges[d]<d2)
	d1= daychanges[d];
      else
	d1= daychanges[d-1];
      if (d1<startT) d1= startT;

      glRectf(xtime->xcoord[d1],startY+2,
	      xtime->xcoord[d2],startY+deltaY-2);
    }

    _updatePrinting();

    _setColor(color);
    for (i=i1; i<=i2; i++) {
      // plot lines on hours modulo useTimes
      if (useTimes >= 0)
	if ((timeLine[i].hour() % useTimes)==0 && timeLine[i].min()==0) {
	  if (fakestipple){
	    _glBegin(GL_POINTS,1000);
	    lineSegment(xtime->xcoord[i],startY,
			xtime->xcoord[i],startY+deltaY,
			LineStyle[style][0],
			LineStyle[style][1],
			true);
	    _glEnd();
	  } else {
	    _glBegin(GL_LINES,2);
	    glVertex2f(xtime->xcoord[i],startY);
	    glVertex2f(xtime->xcoord[i],startY+deltaY);
	    _glEnd();
	  }
	}
    }

    _updatePrinting();
    glDisable(GL_LINE_STIPPLE);
  }
}
