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

// ptBoxElement.cc : Definitions for BoxElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptBoxElement.h>
#include <iostream>
#include <stdio.h>

using namespace miutil;
using namespace std;

BoxElement::BoxElement(const DataSpec cds, const vector<miTime> tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
    : dataPlotElement(cds, layout, field, xtime)
    , label(layout.text)
    , linewidth(layout.lineWidth)
    , pattern(layout.fillstyle)
    , colorlist(layout.colorlist)
    , patternlist(layout.patternlist)
    , hsides(layout.horsides)
    , vsides(layout.versides)
    , style(layout.linePattern)
    , useTimes(layout.useTimes)
    , timeLine(tline)
{
#ifdef DEBUG
  cout << "Inside BoxElement's constructor" << endl;
#endif
  type = TIMEBOX;

  int nc = colorlist.size();
  int np = patternlist.size();
  // make sure colorlist and patternlist have equal sizes
  if (nc < np) {
    ptColor c;
    if (nc > 0)
      c = colorlist[nc - 1];
    else
      c = ptColor("BLACK");
    for (int i = nc; i <= np; i++)
      colorlist.push_back(c);

  } else if (nc > np) {
    ptFillStyle f;
    if (np > 0)
      f = patternlist[np - 1];
    else
      f = SOLID;
    for (int i = np; i <= nc; i++)
      patternlist.push_back(f);
  }
}

void BoxElement::plot()
{
  if (enabled && visible) {
#ifdef DEBUG
    cout << "BoxElement::plot()" <<endl;
#endif
    int i, j;

    _prepFont();

    glLineWidth(linewidth);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(xtime->xcoord[startT], startY, xtime->xcoord[stopT], stopY);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_STIPPLE);
    j = datastart();

    int ldat = -1, ndat = -1, lidx = 0, nidx = 0;
    for (i = startT; i <= stopT; i++) {
      if (valid(i)) {
        nidx = i;
        ndat = static_cast<int> (dval(j));
        if (ndat != ldat || i == stopT) {
          if (ldat >= 0 && ldat < colorlist.size()) {
            _setColor(colorlist[ldat]);
            glPolygonStipple(fillPattern(patternlist[ldat]));
            glRectf(xtime->xcoord[lidx], startY, xtime->xcoord[nidx], stopY);
          }
          ldat = ndat;
          lidx = nidx;
        }
        j++;
      }
      if (i == stopT && lidx != nidx) {
        if (ldat >= 0 && ldat < colorlist.size()) {
          _setColor(colorlist[ldat]);
          glPolygonStipple(fillPattern(patternlist[ldat]));
          glRectf(xtime->xcoord[lidx], startY, xtime->xcoord[nidx], stopY);
        }
      }
    }
    glDisable(GL_POLYGON_STIPPLE);
    _setColor(color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(xtime->xcoord[startT], startY, xtime->xcoord[stopT], stopY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    _printString(label, 20, startY);

    if (useTimes > 0) {
      // make time-grid
      bool fakestipple = false;
      if (!useFakeStipple) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(LineStyle[style][0], LineStyle[style][1]);
      } else
        fakestipple = true;

      glPointSize(linewidth / 2.0);
      for (i = startT + 1; i < stopT; i++) {
        if ((timeLine[i].hour() % useTimes) == 0 && timeLine[i].min() == 0) {
          if (fakestipple) {
            _glBegin(GL_POINTS, 1000);
            lineSegment(xtime->xcoord[i], startY, xtime->xcoord[i], stopY,
                LineStyle[style][0], LineStyle[style][1], true);
            _glEnd();
          } else {
            _glBegin(GL_LINES, 2);
            glVertex2f(xtime->xcoord[i], startY);
            glVertex2f(xtime->xcoord[i], stopY);
            _glEnd();
          }
        }
      }
    }

  }

}
