/*
  libpets2 - presentation and editing of time series

  $Id: ptWindVectorElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptWindVectorElement.cc : Definitions for WindVectorElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptWindVectorElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace miutil;

WindVectorElement::WindVectorElement(const DataSpec cds,
				     const ptVertFieldf& field,
				     const Layout& layout,
				     XAxisInfo* xtime)
  : dataPlotElement(cds, layout, field, xtime)
  , boxColor(layout.color2)
  , deltaY(field.y2-startY)
  , lineWidth(layout.lineWidth)
  , label(layout.label)
  , center(layout.centerVector)
  , useTimes(layout.useTimes)
  , datainknots(layout.datainknots)
{
#ifdef DEBUG
  cout << "Inside WindVectorElement's constructor" << endl;
#endif
  type=WIND_VECTOR;
  FF_DD = datapolar();
  if (label) {
    text = layout.text;
    vtext= text.split("|");
  }
}


void WindVectorElement::plot()
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "WindVectorElement::plot()" <<endl;
#endif
    _prepFont();

    const float TICK05X=0.6, TICK05Y=0.6;
    const float BOXDELTA = 2;
    const float YLEN=0.9*deltaY/2;
    const float DELTAY=0.15*YLEN;
    const float FLAGY=3*DELTAY;
    const float mskt = float(3600.0/1847.0);

    float fsc = ( datainknots ? 1.0 : mskt );

    float xscale = pixWidth/pixHeight;
    float XLEN=YLEN*xscale;
    float DELTAX=0.15*XLEN;
    float FLAGX=3*DELTAX;
    float y0=startY + deltaY/2, left, right;
    float boxTop=y0+BOXDELTA, boxBottom=y0-BOXDELTA;
    int i, j, k = -1, k50, k10, k05;
    float ff, dd, uu, vv, uff, vff, deltaU, deltaV, u1, v1, u2, v2;
    ptPointf arrow[3];
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(lineWidth);



    //plot quadratic boxes (if not arrow centered)
    if (!center){
      _setColor(boxColor);
      for (i=startT;i<=stopT;i++) {
	if (valid(i)) {
	  left = xtime->xcoord[i]-BOXDELTA;
	  right = left+2*BOXDELTA;
	  _glBegin(GL_LINE_LOOP,4);
	  glVertex2f(left,boxBottom); glVertex2f(right,boxBottom);
	  glVertex2f(right,boxTop); glVertex2f(left,boxTop);
	  _glEnd();
// 	  glRectf(left,boxBottom,right,boxTop);
	}
      }
    }
    _updatePrinting();

    _setColor(color);
    //plotting of wind arrows
    arrow[0].y = y0;
    k = datastart();
    for (i=startT;i<=stopT;i++) {
      if (valid(i)) {
	if (FF_DD) {
	  ff = dval(k,0)*fsc;// wind force
	  dd = dval(k++,1);  // wind direction
	  uu = -cos(M_PI_2 - M_PI_2*dd/90.0)*ff;
	  vv = -sin(M_PI_2 - M_PI_2*dd/90.0)*ff;
	} else {
	  uu = dval(k,0)*fsc;   // x-component of wind
	  vv = dval(k++,1)*fsc; // y-component of wind
	  ff = sqrt(uu*uu+vv*vv); // absolute value of wind
	}
	if (ff < 0.1)
	  continue;     // don't plot arrow if absolute value is too small
	// maybe skip timepoints
	if (useTimes && (((i-startT) % useTimes) != 0))
	  continue;

	uff = uu/ff;            // scaled x-component
	vff = vv/ff;            // scaled y-component
	deltaU = DELTAX*uff;    // x-spacing between tick flags on arrow
	deltaV = DELTAY*vff;    // y-spacing between tick flags on arrow

	// first, count number of 5, 10 and 50 knot flags
	if (ff < 182.4) {
	  k05 = (int)(ff/5);    // Round off to the nearest 5
	  if ( ff-k05*5 >= 2.5)
	    k05++;
	  k50 = k05/10;
	  k05 -= 10*k50;
	  k10 = k05/2;
	  k05 -= 2*k10;
	}
	else if (ff < 190) {
	  k50 = 3; k10 = 3; k05 = 0;
	}
	else if (ff < 205) {
	  k50 = 4; k10 = 0; k05 = 0;
	}
	else {
	  k50 = 5; k10 = 0; k05 = 0;
	}
	// set coordinates of main arrow
	arrow[0].x = xtime->xcoord[i];
	if (center){
	  arrow[1].x = arrow[0].x-(XLEN*uff/2.0);
	  arrow[1].y = arrow[0].y-(YLEN*vff/2.0);
	  arrow[2].x = arrow[0].x+(XLEN*uff/2.0);
	  arrow[2].y = arrow[0].y+(YLEN*vff/2.0);
	} else {
	  arrow[1].x = arrow[0].x-(XLEN*uff);
	  arrow[1].y = arrow[0].y-(YLEN*vff);
	}
	// current location on arrow
	u1 = arrow[1].x;
	v1 = arrow[1].y;
	u2 = u1 - FLAGX*vff - deltaU;
	v2 = v1 + FLAGY*uff - deltaV;
	// draw 50 knot flag(s)
	if(k50) {
	  _glBegin(GL_TRIANGLES,k50*3);
	  for (j=0;j<k50;j++) {
	    glVertex2f(u1,v1);
	    glVertex2f((u2+2*deltaU),(v2+2*deltaV));
	    glVertex2f((u1+2*deltaU),(v1+2*deltaV));
	    u1 += 2*deltaU;
	    v1 += 2*deltaV;
	    u2 += 2*deltaU;
	    v2 += 2*deltaV;
	  }
	  _glEnd();
	  u1 += deltaU;
	  v1 += deltaV;
	  u2 += deltaU;
	  v2 += deltaV;
	}
	_glBegin(GL_LINES,k10*2+k05*2+2);
	// draw 10 knot flag(s)
	for (j=0;j<k10;j++) {
	  glVertex2f(u1,v1);
	  glVertex2f(u2,v2);
	  u1 += deltaU;
	  v1 += deltaV;
	  u2 += deltaU;
	  v2 += deltaV;
	}
	// draw 5 knot flag
	if (k05) {
	  if ((k50+k10)==0) {
	    u1 += deltaU;
	    v1 += deltaV;
	    u2 += deltaU;
	    v2 += deltaV;
	  }
	  glVertex2f(u1,v1);
	  glVertex2f((u1+(u2-u1)*TICK05X),(v1+(v2-v1)*TICK05Y));
	}
	// draw main arrow
	glVertex2f(arrow[1].x,arrow[1].y);
	if (center)
	  glVertex2f(arrow[2].x,arrow[2].y);
	else
	  glVertex2f(arrow[0].x,arrow[0].y);
	_glEnd();

	// if centering, draw box at end of arrow
	if (center) {
	  //_setColor(boxColor);
	  float bdx = BOXDELTA*xscale;
// 	  glRectf(arrow[2].x-bdx,
// 		  arrow[2].y+BOXDELTA,
// 		  arrow[2].x+bdx,
// 		  arrow[2].y-BOXDELTA);
	  _glBegin(GL_LINE_LOOP,4);
	  glVertex2f(arrow[2].x-bdx,
		     arrow[2].y+BOXDELTA);
	  glVertex2f(arrow[2].x+bdx,
		     arrow[2].y+BOXDELTA);
	  glVertex2f(arrow[2].x+bdx,
		     arrow[2].y-BOXDELTA);
	  glVertex2f(arrow[2].x-bdx,
		     arrow[2].y-BOXDELTA);
	  _glEnd();
	}
      }
    }
    _updatePrinting();
    // print label
    if (label) {
      int n= vtext.size();
      float tw,th, tth;
      _getCharSize('0',tw,th);
      tth= th*n/2;
      for (int i=0; i<n; i++){
	_printString(vtext[i],20,y0+tth-(i+1)*th);
      }
      _updatePrinting();
    }
  }
}

