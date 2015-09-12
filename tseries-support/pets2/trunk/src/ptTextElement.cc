/*
  libpets2 - presentation and editing of time series

  $Id: ptTextElement.cc 4354 2014-01-02 13:07:57Z juergens $

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



// ptTextElement.cc : Definitions for TextElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptTextElement.h>
#include <ptPatterns.h>
#include <iostream>
#include <string.h>

using namespace miutil;

TextElement::TextElement(const miString& pText,
			 const std::map<miString,miString>& keymap,
			 const ptVertFieldf& field,
			 const Layout& layout,
			 XAxisInfo* xtime)
  : PlotElement(layout, field, xtime), bcolor(layout.color2),
    fillstyle(layout.fillstyle), drawbackground(layout.drawbackground)
{
#ifdef DEBUG
  cout << "Inside TextElement's constructor" << endl;
  cout << "font: " << fontSize << " align: " << align <<
    " startY: " << startY << endl;
#endif
  type=TEXT;

  int n= xtime->xcoord.size();
  startX= stopX= 0;
  if (n>0){
    startX= xtime->xcoord[startT];
    stopX = xtime->xcoord[stopT];
  }

  text= pText;

  if (keymap.size()>0) {
    std::map<miString,miString>::const_iterator p;
    for (p=keymap.begin(); p!=keymap.end(); p++){
      text.replace(p->first, p->second);
    }
  }
}


void TextElement::plot()
{
  if(enabled && text.length() && visible) {
#ifdef DEBUG
    cout << "TextElement::plot(): " << text << endl;
#endif
    std::vector<float> vth, vtw;
    float th, tw, allth=0;
    float x;
    _prepFont();

    std::vector<miString> vs;
    if (text.contains("\n")){
      vs= text.split("\n");
      reverse(vs.begin(),vs.end());
    } else
      vs.push_back(text);

    int n= vs.size();
    for (int i=0; i<n; i++){
      _getStringSize(vs[i],tw,th);
      vtw.push_back(tw);
      vth.push_back(th);
      allth+= th;
    }

    if (drawbackground){
      startX= globalWindow.x1;
      stopX = globalWindow.x2;
      if (fillstyle != SOLID){
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(fillPattern(fillstyle));
      }
      _setColor(bcolor);
      glRectf(startX,startY,stopX,stopY);
      _updatePrinting();
      glDisable(GL_POLYGON_STIPPLE);
    }

    float deltaY= (stopY - startY)/n;

    for (int i=0; i<n; i++){
      switch(align) {
      case LEFT :
	x= 20;
	break;
      case RIGHT :
	x = globalWindow.x2-20-vtw[i];
	break;
      case CENTER :
	x = ((globalWindow.x2 - globalWindow.x1) - vtw[i])/2.0;
	break;
      }

      _setColor(color);
      _printString(vs[i],x,startY+deltaY*i+(deltaY-vth[i]*0.75)/2.0);
      _updatePrinting();
    }
  }
}
