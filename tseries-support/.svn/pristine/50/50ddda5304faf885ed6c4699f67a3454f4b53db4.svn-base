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


// ptCloudElement.cc : Definitions for CloudElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptCloudElement.h>
#include <iostream>

using namespace miutil;

CloudElement::CloudElement(const DataSpec cds,
			   const ptVertFieldf& field,
			   const Layout& layout,
			   XAxisInfo* xtime)
  : dataPlotElement(cds, layout, field, xtime),
    style(layout.fillstyle), width(layout.lineWidth),
    text(layout.text)
{
#ifdef DEBUG
  cout << "Inside CloudElement's constructor" << endl;
#endif
  type = CLOUDBOX;
}

// Plotting of cloud boxes could probably be made more effective
void CloudElement::plot()
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "CloudElement::plot()" <<endl;
#endif
    float th=0,tw=0;
    _prepFont();
    if (text.length() > 0)
      _getStringSize(text,tw,th);

    float x, left, right;
    glLineWidth(width);
    _setColor(color);
    //glPolygonStipple(.....style.....)

    // find minimum delta T for calculation of cloud box width
    int i,j=-1,k= startT;
    float testx, minx=10000;
    for (i=startT+1;i<=stopT;i++) {
      if (valid(i)) {
	testx = xtime->xcoord[i]-xtime->xcoord[k];
	if (testx < minx) minx = testx;
	k=i;
      }
    }
    deltaX = minx/3.0;

    float mostleft= 10000;

    j = datastart();

    for (i=startT;i<=stopT;i++) {
      if (valid(i)) {
	x     = xtime->xcoord[i];
	left  = x - deltaX;
	right = x + deltaX;
	// draw rectangular box
	glBegin(GL_LINE_LOOP);
	glVertex2f(right,startY); glVertex2f(left,startY);
	glVertex2f(left,stopY); glVertex2f(right,stopY);
	glEnd();
	// draw each cloud box
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glRectf(left,startY,left+(2.0*deltaX*dval(j)/100.0),stopY);
	j++;
	if (left < mostleft) mostleft= left;
      }
    }
    _updatePrinting();
    // print text
    if (tw > 0){
      //glRasterPos2i(OFFSET,startY);
      _printString(text,mostleft-tw-2,startY);
    }
    _updatePrinting();
  }
}
