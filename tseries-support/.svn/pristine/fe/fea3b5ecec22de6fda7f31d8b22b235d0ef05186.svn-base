/*
  libpets2 - presentation and editing of time series

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


// ptAxesElement.cc : Definitions for AxesElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptAxesElement.h>
#include <iostream>

using namespace miutil;

AxesElement::AxesElement(const ptVertFieldf& field,
			 const Layout& layout,
			 const bool& hor,
			 XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , y1(field.y1+layout.tickLen), y2(field.y2-layout.labelSpace)
  , axisWidth(layout.axisWidth), tickLineWidth(layout.tickWidth)
  , tickColor(layout.color2)
  , rectangle(layout.axisRectangle)
  , horizontal(hor)
{
#ifdef DEBUG
  cout << "Inside AxesElement's constructor" << endl;
#endif
  type=XAXIS;
}


void AxesElement::plot()
{
  if(enabled) {

#ifdef DEBUG
    cout << "AxesElement::plot()" <<endl;
#endif
    if (horizontal){
      x1 = xtime->x1;
      x2 = xtime->x2;
    }
    _setColor(color);
    glLineWidth(axisWidth);

    // draw a rectangle
    if (rectangle) {
      _glBegin(GL_LINE_STRIP,4);
      glVertex2f(x1,y1); glVertex2f(x1,y2);
      glVertex2f(x2,y2); glVertex2f(x2,y1);
      _glEnd();
    }
    _updatePrinting();

    if (horizontal) {
      // draw the x-axis with tick marks
      _glBegin(GL_LINES,2);
      glVertex2f(x1,y1); glVertex2f(x2,y1);   // axis
      _glEnd();
      _updatePrinting();

      glLineWidth(tickLineWidth);
      _setColor(tickColor);
      _glBegin(GL_LINES,2*(stopT-startT+1));
      for(int i=startT; i<=stopT; i++) {            // tick marks
 	glVertex2f(xtime->xcoord[i],startY);
	glVertex2f(xtime->xcoord[i],y1);
      }
      _glEnd();
      _updatePrinting();
    }
  }
}
