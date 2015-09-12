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


// ptLineElement.cc : Definitions for LineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptDoubleLineElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace miutil;
using namespace std;

DoubleLineElement::DoubleLineElement(yAxisElement* ya,
				     const DataSpec cds,
				     const ptVertFieldf& field,
				     const Layout& layout, XAxisInfo* xtime)
  :AxisChildElement(ya,cds,field,layout,xtime),
   fillColor(layout.color2)
{
#ifdef DEBUG
  cout << "Inside DoubleLineElement's constructor" << endl;
#endif
  type=DOUBLE_LINE;
}

void DoubleLineElement::dataInfo(float &min, float &max)
{
  float mi= FLT_MAX, ma= -FLT_MAX;
  int comp1=0, comp2=1;
  min = datamin(comp1);
  max = datamax(comp1);
  mi  = datamin(comp2);
  ma  = datamax(comp2);
  if (mi<min) min= mi;
  if (ma>max) max= ma;
}

void DoubleLineElement::plot()
{
  if(enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "DoubleLineElement::plot()" << endl;
#endif
    _prePlot();
    _setColor(color);

    // plot curve
    bool fakestipple=false;
    if ((!useColour) || pInColour){
      if (!useFakeStipple) {
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(LineStyle[style][0],LineStyle[style][1]);
      } else fakestipple=true;
    }
    glLineWidth(lineWidth);
    glPointSize(lineWidth);

    // find x-startcoordinate for line-label
    float labelx, labely1, labely2;
    if (labelOnLine)
      labelx=lineLabelPos*float((stopT-startT)/100.0);

    vector<float> Xval, Y1val, Y2val;
    int j,i;
    j = datastart();
    for (i=startT; i<=stopT; i++) {
      if (valid(i)) {
	// find correct position for line-label
	if (labelOnLine)
	  if (i>=labelx) {
	    labelx = xval(i);
	    labely1 = yval(j,0)*1.04;
	    labely2 = yval(j,1)*1.04;
	  }

	Xval.push_back(xval(i));
	Y1val.push_back(yval(j,0));
	Y2val.push_back(yval(j,1));
	j++;
      }
    }

    // fill the area between the two lines
    _setColor(fillColor);
    glBegin(GL_TRIANGLE_STRIP);
    for (i=0; i<Xval.size();i++){
      glVertex2f(Xval[i],Y2val[i]);
      glVertex2f(Xval[i],Y1val[i]);
    }
    _glEnd();
    _updatePrinting();

    _setColor(color);

    // the first line..
    bool firstsegment;

    if (fakestipple)
      glBegin(GL_POINTS);
    else
      glBegin(GL_LINE_STRIP);

    firstsegment = true;
    for (i=0; i<Xval.size();i++){
      if (fakestipple){
	if (i>0){
	  lineSegment(Xval[i-1],Y1val[i-1],Xval[i],Y1val[i],
		      LineStyle[style][0],
		      LineStyle[style][1],
		      firstsegment);
	  firstsegment=false;
	}
      } else {
	glVertex2f(Xval[i],Y1val[i]);
      }
    }
    _glEnd();
    _updatePrinting();

    // ..then the second line
    if (fakestipple)
      _glBegin(GL_POINTS,1000);
    else
      _glBegin(GL_LINE_STRIP,Xval.size()+1);

    firstsegment = true;
    for (i=0; i<Xval.size();i++){
      if (fakestipple){
	if (i>0){
	  lineSegment(Xval[i-1],Y2val[i-1],Xval[i],Y2val[i],
		      LineStyle[style][0],
		      LineStyle[style][1],
		      firstsegment);
	  firstsegment=false;
	}
      } else {
	glVertex2f(Xval[i],Y2val[i]);
      }
    }
    _glEnd();
    _updatePrinting();

    glDisable(GL_LINE_STIPPLE);


    if (labelOnLine){
      _prepFont();
      _setColor(ptColor("WHITE"));
      float bx1, bx2, by1, by2;
      float th, tw, mw;
      _getStringSize(labelText, tw, th);
      _getMaxCharSize(mw, th);
      float marg= th*0.15;
      float labely= (labely1>labely2)?labely1:labely2;
      bx1= labelx - marg;
      bx2= bx1 + 2*marg + tw;
      by1= labely - marg - th*0.3;
      by2= by1 + th + 2*marg;
      _glBegin(GL_POLYGON, 4);
      glVertex2f(bx1,by1);
      glVertex2f(bx2,by1);
      glVertex2f(bx2,by2);
      glVertex2f(bx1,by2);
      _glEnd();
      _updatePrinting();

      _setColor(color);
      _printString(labelText,labelx,labely);
      _updatePrinting();
    }
#ifdef DEBUG
    cout << "DoubleLineElement::plot() finished" << endl;
#endif
  }
}
