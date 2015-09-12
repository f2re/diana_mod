/*
  libpets2 - presentation and editing of time series

  $Id: ptQBoxElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptQBoxElement.cc : Definitions for QBoxElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptQBoxElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace miutil;

QBoxElement::QBoxElement(yAxisElement* ya,
			 const DataSpec cds,
			 const ptVertFieldf& field,
			 const Layout& layout, XAxisInfo* xtime)
  : AxisChildElement(ya,cds,field,layout,xtime)
  , lineWidth(layout.lineWidth)
  , hstart(layout.histStart), hstop(layout.histStop)
  , tickLen(layout.tickLen)
  , linestyle(layout.linePattern)
  , linecolor(layout.color)
  , boxcolor(layout.color2)
  , boxfill(layout.fillstyle)
{
#ifdef DEBUG
  cout << "Inside QBOXElement's constructor" << endl;
#endif
  type=QBOX;
}


void QBoxElement::dataInfo(float &min, float &max)
{
  float mi= FLT_MAX, ma= -FLT_MAX;
  int comp, ndim= datadimension();
  for (comp=0; comp<ndim; comp++){
    min = datamin(comp);
    max = datamax(comp);
    if (min<mi) mi= min;
    if (max>ma) ma= max;
  }
  min= mi;
  max= ma;
}

void QBoxElement::plot()
{
  if(enabled && Yaxis) {
#ifdef DEBUG
    cout << "QBoxElement::plot()" << endl;
#endif

    _prePlot();

    glLineStipple(LineStyle[linestyle][0],LineStyle[linestyle][1]);
    glLineWidth(lineWidth);

    if (boxfill != NONE) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      if (boxfill != SOLID){
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(fillPattern(boxfill));
      }
    } else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float prevx = xtime->x1; //xval(startT);
    float newx;
    int i, j = datastart();

    for (i=startT; i<=stopT; i++) {
      if (valid(i)) {
	newx= xval(i);
	float del= (newx-prevx)/10.0;
	float x1 = prevx+hstart*del;
	float x2 = prevx+hstop*del;
	float xm = (x1 + x2)/2.0;
	float tx1= xm- (xm-x1)*tickLen;
	float tx2= xm+ (x2-xm)*tickLen;

	float y0 = yval(j,0);
	float y1 = yval(j,1);
	float y2 = yval(j,2);
	float y3 = yval(j,3);
	float y4 = yval(j,4);

	_setColor(linecolor);
	// draw vertical lines
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2f(xm,y0);
	glVertex2f(xm,y1);
	glVertex2f(xm,y3);
	glVertex2f(xm,y4);
	glEnd();
	_updatePrinting();
	glDisable(GL_LINE_STIPPLE);
	// draw horisontal lines
	glBegin(GL_LINES);
	glVertex2f(tx1,y0);
	glVertex2f(tx2,y0);
	glVertex2f(tx1,y4);
	glVertex2f(tx2,y4);
	glEnd();
	_updatePrinting();
	// draw box
	_setColor(boxcolor);
	glRectf(x1,y1,x2,y3);
	_updatePrinting();
	// draw box frame
	_setColor(linecolor);
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1,y1);
	glVertex2f(x1,y3);
	glVertex2f(x2,y3);
	glVertex2f(x2,y1);
	glVertex2f(x1,y1);
	glEnd();
	_updatePrinting();
	// draw 50-line
	_setColor(linecolor);
	glBegin(GL_LINES);
	glVertex2f(x1,y2);
	glVertex2f(x2,y2);
	glEnd();
	_updatePrinting();

	prevx= newx;
	j++;
      }
    }
    _updatePrinting();
    glDisable(GL_LINE_STIPPLE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_STIPPLE);

#ifdef DEBUG
    cout << "QBoxElement::plot() finished" << endl;
#endif
  }
}
