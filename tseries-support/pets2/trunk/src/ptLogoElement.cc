/*
  libpets2 - presentation and editing of time series

  $Id: ptLogoElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptLogoElement.cc : Definitions for LogoElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptLogoElement.h>
#include <iostream>
#include <stdio.h>

using namespace miutil;

LogoElement::LogoElement(const ptVertFieldf& field,
			 const Layout& layout,
 			 XAxisInfo* xtime)

  : PlotElement(layout, field, xtime),
    pheight(layout.trueheight), pxposition(layout.position)
{
#ifdef DEBUG
  cout << "Inside LogoElement's constructor" << endl;
#endif
  type=LOGO;
}


void LogoElement::plot()
{
  const int mainline[][2]={
    {26,123},{37,109},{46,98},{52,86},{56,75},{57,64},{56,53},{51,44},
    {47,39},{42,35},{49,33},{65,31},{77,34},{84,37},{92,43},{102,51},
    {118,67},{129,77},{141,84}};
  const int line1[][2]={
    {52,137},{60,125},{72,113},{90,99},{111,89},{134,81},{139,77},{142,67}
  };
  const int line2[][2]={
    {4,91},{11,99},{25,108},{37,109},{55,100},{72,89},{91,76},{108,69},
    {119,68},{125,50},{125,33},{118,18}
  };
  const int line3[][2]={
    {4,50},{5,62},{12,73},{23,81},{37,85},{52,87},{65,72},{81,60},
    {104,52},{108,36},{104,21},{96,10},{82,1}
  };
  const int line4[][2]={
    {18,24},{14,33},{14,46},{19,54},{30,60},{44,65},{58,66},{66,52},
    {77,41},{85,38},{85,27},{77,15},{64,7},{51,6},{43,6}
  };
  const int line5[][2]={
    {52,46},{45,48},{33,43},{29,36},{32,28},{40,22},{52,21},{60,24},
    {64,31},{56,36},{52,46}
  };

  const int numhcp= 61;
  const float halfcirc[numhcp][2]= {{0,-1},{0.256038,-0.966667},{0.359011,-0.933333},{0.43589,-0.9},{0.498888,-0.866667},{0.552771,-0.833333},{0.6,-0.8},{0.642046,-0.766666},{0.67987,-0.733333},{0.714143,-0.7},{0.745356,-0.666666},{0.773879,-0.633333},{0.8,-0.6},{0.823947,-0.566666},{0.845905,-0.533333},{0.866026,-0.5},{0.884433,-0.466666},{0.901234,-0.433333},{0.916515,-0.4},{0.930353,-0.366666},{0.942809,-0.333333},{0.953939,-0.3},{0.963789,-0.266666},{0.972397,-0.233333},{0.979796,-0.2},{0.986013,-0.166666},{0.991071,-0.133333},{0.994987,-0.0999996},{0.997775,-0.0666663},{0.999444,-0.033333},{1,3.65078e-07},{0.999444,0.0333337},{0.997775,0.066667},{0.994987,0.1},{0.991071,0.133334},{0.986013,0.166667},{0.979796,0.2},{0.972397,0.233334},{0.963789,0.266667},{0.953939,0.3},{0.942809,0.333334},{0.930352,0.366667},{0.916515,0.4},{0.901234,0.433334},{0.884433,0.466667},{0.866025,0.5},{0.845905,0.533334},{0.823947,0.566667},{0.8,0.6},{0.773879,0.633334},{0.745356,0.666667},{0.714142,0.700001},{0.679869,0.733334},{0.642045,0.766667},{0.599999,0.800001},{0.55277,0.833334},{0.498886,0.866667},{0.435888,0.900001},{0.359009,0.933334},{0.256035,0.966667},{0,1}};

  const miString dnmi= "DNMI";

  if (enabled) {
#ifdef DEBUG
    cout << "LogoElement::plot()" << endl;
#endif
    float diaheight, diawidth;
    float texth, textmargin;
    float xposition,height,my1,my2;
    float th,tw;
    int i;
    float globeh, globew;
    ptColor c1("DNMI_BLUE"),c2("DNMI_GREEN"),cl("WHITE");
    _prepFont();
    _getStringSize(dnmi,tw,th);

    diaheight= globalWindow.y2-globalWindow.y1;
    diawidth = globalWindow.x2-globalWindow.x1;
    if (pheight > 0)
      height= pheight*diaheight/1000.0;
    else height = stopY-startY;
    xposition= pxposition*diawidth/1000.0;
    textmargin= 0.01*height;
    texth= th + 2*textmargin;
    globeh= (height - texth)/2.0;
    globew= globeh*pixWidth/pixHeight;

    if (align==BELOW){
      my1= startY-height;
      my2= startY;
    } else {
      my1= startY;
      my2= startY+height;
    }
    // -- draw logo --
    // first half-circle (blue)
    _setColor(c1);
    glLineWidth(1);
    _glBegin(GL_POLYGON,numhcp);
    for (i=0; i<numhcp; i++){
      glVertex2f(float(xposition) - globew*halfcirc[i][0],
		 float(my2) - globeh - globeh*halfcirc[i][1]);
    }
    _glEnd();
    _updatePrinting();

    // second half-circle (green)
    _setColor(c2);
    _glBegin(GL_POLYGON,numhcp);
    for (i=0; i<numhcp; i++){
      glVertex2f(float(xposition) + globew*halfcirc[i][0],
		 float(my2) - globeh - globeh*halfcirc[i][1]);
    }
    _glEnd();
    _updatePrinting();

    float yscale= globeh/71.0;
    float xscale= globew/71.0;
    _setColor(cl);

    // Thick line (wave)
    glLineWidth(2);
    _glBegin(GL_LINE_STRIP,19);
    for (i=0; i<19; i++){
      glVertex2f(xposition-globew + xscale*mainline[i][0],
		 my2 - yscale*mainline[i][1]);
    }
    _glEnd();
    _updatePrinting();

    // Thin lines (isolines)
    glLineWidth(1);
    _glBegin(GL_LINE_STRIP,8);
    for (i=0; i<8; i++){
      glVertex2f(xposition-globew + xscale*line1[i][0],
		 my2 - yscale*line1[i][1]);
    }
    _glEnd();

    _glBegin(GL_LINE_STRIP,12);
    for (i=0; i<12; i++){
      glVertex2f(xposition-globew + xscale*line2[i][0],
		 my2 - yscale*line2[i][1]);
    }
    _glEnd();
    _glBegin(GL_LINE_STRIP,13);
    for (i=0; i<13; i++){
      glVertex2f(xposition-globew + xscale*line3[i][0],
		 my2 - yscale*line3[i][1]);
    }
    _glEnd();
    _glBegin(GL_LINE_STRIP,15);
    for (i=0; i<15; i++){
      glVertex2f(xposition-globew + xscale*line4[i][0],
		 my2 - yscale*line4[i][1]);
    }
    _glEnd();
    _glBegin(GL_LINE_STRIP,11);
    for (i=0; i<11; i++){
      glVertex2f(xposition-globew + xscale*line5[i][0],
		 my2 - yscale*line5[i][1]);
    }
    _glEnd();
    _updatePrinting();

    // print DNMI
    _setColor(color);
    _printString(dnmi,float(xposition)- tw/2.0, my1+textmargin);
    _updatePrinting();
  }
}
