/*
 libpets2 - presentation and editing of time series

 $Id: ptAxisHistogramElement.cc 4354 2014-01-02 13:07:57Z juergens $

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

// ptAxisHistogramElement.cc : Definitions for AxisHistogramElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptAxisHistogramElement.h>
#include <puTools/miTime.h>
#include <iostream>
#include <stdio.h>
#include "ptPatterns.h"
#include <float.h>

using namespace miutil;

AxisHistogramElement::AxisHistogramElement(yAxisElement* ya,
    const DataSpec cds, const ptVertFieldf& field, const Layout& layout,
    XAxisInfo* xtime)
    : AxisChildElement(ya, cds, field, layout, xtime)
    , lineWidth(layout.lineWidth)
    , fstyle(layout.fillstyle)
    , hstart(layout.histStart)
    , hstop(layout.histStop)
    , drawlabel(layout.label)
{
#ifdef DEBUG
  cout << "Inside AxisHistogramElement's constructor" << endl;
#endif
  type = AXISHIST;
}

void AxisHistogramElement::plot()
{
  if (enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "AxisHistogramElement::plot()" <<endl;
#endif
    float th, tw;
    int i, j;
    _prePlot();
    _prepFont();
    _getCharSize('0', tw, th);
    float y;
    char text[16];
    _setColor(color);
    glLineWidth(lineWidth);
    if (fstyle != SOLID) {
      glEnable(GL_POLYGON_STIPPLE);
      glPolygonStipple(fillPattern(fstyle));
    }

    float oldx, newx;
    j = datastart();

    oldx = xtime->xcoord[startT];
    for (i = startT; i <= stopT; i++)
      if (valid(i)) {
        newx = xtime->xcoord[i];
        if (dval(j) >= 0.05) { // don't draw if is ignorable
          y = yval(j);
          float del = (newx - oldx) / 10.0;
          float x1 = oldx + hstart * del;
          float x2 = oldx + hstop * del;
          // enclosing box
          _initPrinting(10);
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          glBegin(GL_LINE_STRIP);
          glVertex2f(x1, startY);
          glVertex2f(x1, y);
          glVertex2f(x2, y);
          glVertex2f(x2, startY);
          glEnd();
          glRectf(x1, startY, x2, y);
          _updatePrinting();
          if (drawlabel) {
            glDisable(GL_POLYGON_STIPPLE);
            snprintf(text, sizeof(text), "%2.1f", dval(j));
            _printString(text, x1 + (x2 - x1) / 2 - tw, y + YSPACE);
            _updatePrinting();
            glEnable(GL_POLYGON_STIPPLE);
          }
        }
        oldx = newx;
        j++;
      }
    glDisable(GL_POLYGON_STIPPLE);
  }
}

