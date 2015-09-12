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


// ptMultiLineElement.cc : Definitions for MultiLineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptMultiLineElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace miutil;

MultiLineElement::MultiLineElement(yAxisElement* ya,
    const DataSpec cds,
    const ptVertFieldf& field,
    const Layout& layout, XAxisInfo* xtime)
:AxisChildElement(ya,cds,field,layout,xtime)
{
#ifdef DEBUG
  cout << "Inside MultiLineElement's constructor" << endl;
#endif
  type=MULTI_LINE;
}


void MultiLineElement::dataInfo(float &min, float &max)
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

void MultiLineElement::plot()
{
  if(enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "MultiLineElement::plot()" << endl;
#endif
    _prePlot();
    _setColor(color);

    // plot curves
    bool fakestipple=false;
    if ((!useColour) || pInColour){
      if (!useFakeStipple) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(LineStyle[style][0],LineStyle[style][1]);
      } else fakestipple=true;
    }
    glLineWidth(lineWidth);
    glPointSize(lineWidth);

    std::vector<float> Xval;
    std::vector<int>   Yidx;
    int j,i;
    j = datastart();
    for (i=startT; i<=stopT; i++) {
      if (valid(i)) {
        Xval.push_back(xval(i));
        Yidx.push_back(j);
        j++;
      }
    }

    _setColor(color);

    // the lines..
    bool firstsegment;
    int comp, ndim= datadimension();
    for (comp=0; comp<ndim; comp++){
      if (fakestipple)
        _glBegin(GL_POINTS,1000);
      else
        _glBegin(GL_LINE_STRIP,Xval.size()+1);

      firstsegment = true;
      for (i=0; i<Xval.size();i++){
        if (fakestipple){
          if (i>0){
            lineSegment(Xval[i-1],yval(Yidx[i-1],comp),
                Xval[i],yval(Yidx[i],comp),
                LineStyle[style][0],
                LineStyle[style][1],
                firstsegment);
            firstsegment=false;
          }
        } else {
          glVertex2f(Xval[i],yval(Yidx[i],comp));
        }
      }
      _glEnd();
      _updatePrinting();
    }

    glDisable(GL_LINE_STIPPLE);

#ifdef DEBUG
    cout << "MultiLineElement::plot() finished" << endl;
#endif
  }
}
