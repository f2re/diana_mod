/*
  libpets2 - presentation and editing of time series

  $Id: ptVectorElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptVectorElement.cc : Definitions for VectorElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptVectorElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace miutil;

VectorElement::VectorElement(const DataSpec cds,
			     const ptVertFieldf& field,
			     const Layout& layout,
			     XAxisInfo* xtime)
  : dataPlotElement(cds,layout,field,xtime)
  , boxColor(layout.color2)
  , deltaY(field.y2-startY)
  , lineWidth(layout.lineWidth)
  , reverse(layout.reverse)
  , label(layout.label)
  , arrow(layout.vectorArrow)
  , useTimes(layout.useTimes)
{
#ifdef DEBUG
  cout << "Inside VectorElement's constructor" << endl;
#endif
  type=VECTOR;
  polar = datapolar();
  if (label){ // use layout.text if text is specified
    text = layout.text;
    vtext= text.split("|");
  }
}


void VectorElement::plot()
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "VectorElement::plot()" <<endl;
#endif
    _prepFont();

    const float YLEN=0.7*deltaY/2;
    const float XLEN=YLEN*pixWidth/pixHeight;
    const float AFAC= -0.5;
    const float SFAC= AFAC*0.5;

    const float BOXDELTA = 2.0;
    const float MINVAL = 0.01;
    float y0=startY + deltaY/2.0, x0, left, right;
    float boxTop=y0+BOXDELTA, boxBottom=y0-BOXDELTA;
    int i,j = -1;
    float ff, dd, uu, vv, uff, vff;
    float alpha;

    //plot quadratic boxes
    _setColor(boxColor);
    glLineWidth(lineWidth);
    for (i=startT;i<=stopT;i++) {
      if (valid(i)) {
	// maybe skip timepoints
	if (useTimes && (((i-startT) % useTimes) != 0))
	  continue;
	left = xtime->xcoord[i]-BOXDELTA;
	right = left+2*BOXDELTA;
	glRectf(left,boxBottom,right,boxTop);
      }
    }
    _updatePrinting();

    _setColor(color);
    j = datastart();

    //plotting of vector
    if (datadimension() == 1)// use scalar value as an angle (degrees)
      for (i=startT;i<=stopT;i++) {
	if (valid(i)) {
	  if (!reverse)
	    alpha = M_PI_2 - M_PI*dval(j++,0)/180;
	  else // angle rotated 180 degrees
	    alpha = 3*M_PI_2 - M_PI*dval(j++,0)/180;
	  // maybe skip timepoints
	  if (useTimes && (((i-startT) % useTimes) != 0))
	    continue;

	  uff = XLEN*cos(alpha);  // translate to radians
	  vff = YLEN*sin(alpha);
	  x0 = xtime->xcoord[i];
	  // draw main arrow
	  _glBegin(GL_LINES,0);
	  glVertex2f(x0,y0);
	  glVertex2f(x0+uff,y0+vff);
	  _glEnd();
	  // draw arrowhead
	  if (arrow){
	    _glBegin(GL_TRIANGLES);
	    glVertex2f(x0+uff,y0+vff);
	    glVertex2f(x0+uff+uff*AFAC+vff*SFAC,
		       y0+vff+vff*AFAC-uff*SFAC);
	    glVertex2f(x0+uff+uff*AFAC-vff*SFAC,
		       y0+vff+vff*AFAC+uff*SFAC);
	    _glEnd();
	  }
	}
      }
    else                    // use vector
      for (i=startT;i<=stopT;i++) {
	if (valid(i)) {

	  if (polar) {
	    ff = dval(j,0);   // force
	    dd = dval(j++,1); // direction
	    uu = -cos(M_PI_2 - M_PI_2*dd/90.0)*ff;
	    vv = -sin(M_PI_2 - M_PI_2*dd/90.0)*ff;
	  } else {
	    uu = dval(j,0);     // x-component of vector
	    vv = dval(j++,1);   // y-component of vector
	  }

	  if ((ff=sqrt(uu*uu+vv*vv)) < MINVAL) // absolute value of vector
	    continue;          // don't plot if absolute value is to small
	  // maybe skip timepoints
	  if (useTimes && (((i-startT) % useTimes) != 0))
	    continue;

	  uff = XLEN*uu/ff;         //scaled x-component in screen coordinates
	  vff = YLEN*vv/ff;         // scaled y-component in screen coordinates

	  x0 = xtime->xcoord[i];
	  // draw main arrow
	  _glBegin(GL_LINES,0);
	  glVertex2f(x0,y0);
	  glVertex2f(x0+uff,y0+vff);
	  _glEnd();
	  // draw arrowhead
	  if (arrow){
	    _glBegin(GL_TRIANGLES);
	    glVertex2f(x0+uff,y0+vff);
	    glVertex2f(x0+uff*AFAC+vff*SFAC,
		       y0+vff*AFAC-uff*SFAC);
	    glVertex2f(x0+uff*AFAC-vff*SFAC,
		       y0+vff*AFAC+uff*SFAC);
	    _glEnd();
	  }
	}
      }
    _updatePrinting();

    // print label
    if (label) {
      int n= vtext.size();
      float tw,th,tth;
      _getCharSize('0',tw,th);
      tth= th*n/2;
      for (int i=0; i<n; i++){
	_printString(vtext[i],20,y0+tth-(i+1)*th);
      }
      _updatePrinting();
    }
  }
}


