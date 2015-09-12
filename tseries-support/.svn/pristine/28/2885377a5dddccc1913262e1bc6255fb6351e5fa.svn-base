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


// ptLineElement.cc : Definitions for LineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptLineElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace miutil;

LineElement::LineElement(yAxisElement* ya,
    const DataSpec cds,
    const ptVertFieldf& field,
    const Layout& layout, XAxisInfo* xtime, bool ismulti)
  : AxisChildElement(ya,cds,field,layout,xtime)
  , marker(layout.marker)
  , markercolor(layout.color2)
  , arrowcolor(layout.color2)
  , markerfill(layout.markerFill)
  , markersize(layout.size)
  , barsize(layout.delta)
  , shadow(layout.shadow)
  , lineBar(layout.lineBar)
  , lineStep(layout.lineStep)
  , lineArrows(layout.lineArrows)
  , arrowLength(layout.arrowLength)
  , arrowSize(layout.arrowSize)
  , arrowfill(layout.fillstyle)
  , arrow(layout.vectorArrow)
  , reverse(layout.reverse)
  , colorbyvalue(layout.colorbyvalue)
  , colorlist(layout.colorlist)
  , smoothing(layout.smoothing)
  , smoothdiv(layout.smoothdiv)
  , wrapdegrees(layout.wrapdegrees)
  , wraplimit(layout.wraplimit)
  , isMultiLine(ismulti)

{
#ifdef DEBUG
  cout << "Inside LineElement's constructor" << endl;
#endif
  type=LINE;
}


void LineElement::dataInfo(float &min, float &max)
{
  if(!isMultiLine) {
    AxisChildElement::dataInfo(min,max);
    return;
  }

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






void LineElement::plot()
{
  if(enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "LineElement::plot()" << endl;
#endif
    // vertex points (legal datapoints)
    std::vector<float> vertexx, vertexy;
    // vertex point data index (legal datapoints)
    std::vector<int>   vertexi;

    // marker types
    float mSizeX = markersize*pixWidth;
    float mSizeY = markersize*pixHeight;
    bool noc = (lineWidth < 0);

    _prePlot();
    _setColor(color);

    // plot curve
    if ( !useColour || pInColour ){
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(LineStyle[style][0],LineStyle[style][1]);
    }
    glLineWidth(lineWidth);
    //     glPointSize(lineWidth);

    float prevx=-2000,prevy,newx,newy;
    int previ1=-2000;

    // find x-startcoordinate for line-label
    float labelx, labely;
    if (labelOnLine)
      labelx = lineLabelPos*1.0*(stopT-startT)/100.0;

    float cx, cy; // crossing point
    std::vector<float> plotx, ploty; // plot points
    std::vector<int> ploti; // plot color index (for colorbyvalue)

    int j,i;



    int numOfLines=1;
    if(isMultiLine)
      numOfLines=datadimension();
    int numOfPoints = (stopT-startT);

    for(int n=0;n<numOfLines;n++) {
      previ1=-2000;
      prevx=-2000;
      j = datastart();

      for (i=startT; i<=stopT; i++) {
        if (valid(i)) {
          newx= xval(i);
          if(isMultiLine) {
            newy= yval(j,n);
          } else {
            newy= yval(j,ds.comp);
          }


          bool pointbad=false;

          if (newy > stopY || newy < startY){ // illegal point
            pointbad = true;
          }

          // find correct position for line-label
          if ( labelOnLine )
            if (i>=labelx) {
              labelx = newx;
              labely = newy;
              if ( keepinaxis ){
                if (newy > stopY) labely= stopY-10;
                else if (newy < startY) labely= startY;
              }
            }

          // types of line drawing
          if (lineBar){  // draw horisontal bars
            if (!noc && !(keepinaxis && pointbad)){
              int i1 = dataLimitIndex(dval(j));
              plotx.push_back(newx-barsize);
              ploty.push_back(newy);
              ploti.push_back(i1);
              plotx.push_back(newx+barsize);
              ploty.push_back(newy);
              ploti.push_back(i1);
            }

          } else if (lineStep) { // draw stairs
            // find points
            if (prevx < 0){
              prevx= xtime->x1;
              prevy= newy;
            }
            float d2;
            if (align == LEFT) d2= prevx;
            else if (align == RIGHT) d2= newx;
            else d2= (prevx+newx)/2.0;
            if (!noc){
              if ( colorbyvalue ) {
                int i1 = dataLimitIndex(dval(j));
                if ( previ1 > -1000 ){
                  if ( i1 == previ1 ){
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);
                    plotx.push_back(d2);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);

                    plotx.push_back(d2);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);
                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                  } else {
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(previ1);
                    plotx.push_back(d2);
                    ploty.push_back(prevy);
                    ploti.push_back(previ1);
                    // find crossing points to data limits
                    float cx,cy;
                    int neg = ( previ1 > i1 ? -1 : 1);
                    int ncr = abs(previ1-i1);
                    float xc1=d2, xc2=d2;
                    float yc1=prevy, yc2=newy;
                    for ( int k = 1; k<=ncr; k++ ){
                      int ind1= previ1+neg*(k-1);
                      int ind2= previ1+neg*k;
                      float d = dataLimit( std::max(ind1,ind2) );

                      // find crossing
                      if (dataCrossPoint(d2,yc1,d2,yc2,d,cx,cy)){
                        // make line

                        plotx.push_back(d2);
                        ploty.push_back(yc1);
                        ploti.push_back(ind1);
                        plotx.push_back(d2);
                        ploty.push_back(cy);
                        ploti.push_back(ind2);

                        xc1= d2; xc2= d2;
                        yc1= cy; yc2= newy;
                      }
                    }
                    plotx.push_back(cx);
                    ploty.push_back(cy);
                    ploti.push_back(i1);
                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                  }
                }
                previ1= i1;
              } else { // no colorbyvalue
                if ( prevx > -1000 ){
                  plotx.push_back(prevx);
                  ploty.push_back(prevy);
                  ploti.push_back(0);
                  plotx.push_back(d2);
                  ploty.push_back(prevy);
                  ploti.push_back(0);

                  plotx.push_back(d2);
                  ploty.push_back(prevy);
                  ploti.push_back(0);
                  plotx.push_back(d2);
                  ploty.push_back(newy);
                  ploti.push_back(0);

                  plotx.push_back(d2);
                  ploty.push_back(newy);
                  ploti.push_back(0);
                  plotx.push_back(newx);
                  ploty.push_back(newy);
                  ploti.push_back(0);
                }
              }
            }

          } else { // normal line
            if ( !noc ){
              if ( colorbyvalue ) {
                int i1 = dataLimitIndex(dval(j));
                if ( previ1 > -1000 ){
                  if ( i1 == previ1 ){
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                  } else {
                    // find crossing points to data limits
                    float cx,cy;
                    int neg = ( previ1 > i1 ? -1 : 1);
                    int ncr = abs(previ1-i1);
                    float xc1=prevx, xc2=newx;
                    float yc1=prevy, yc2=newy;
                    for ( int k = 1; k<=ncr; k++ ){
                      int ind1= previ1+neg*(k-1);
                      int ind2= previ1+neg*k;
                      float d = dataLimit( std::max(ind1,ind2) );
                      // find crossing
                      if (dataCrossPoint(xc1,yc1,xc2,yc2,d,cx,cy)){
                        // make line
                        plotx.push_back(xc1);
                        ploty.push_back(yc1);
                        ploti.push_back(ind1);
                        plotx.push_back(cx);
                        ploty.push_back(cy);
                        ploti.push_back(ind2);

                        xc1= cx; xc2= newx;
                        yc1= cy; yc2= newy;
                      }
                    }
                    plotx.push_back(cx);
                    ploty.push_back(cy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                  }
                }
                previ1= i1;
              } else {
                if ( prevx > -1000 ){
                  if ( !wrapdegrees || fabsf(dval(j) - dval(j-1)) < wraplimit ){
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(0);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(0);
                  }
                }
              }
            }
          }

          // keep vertex point
          if ( !(keepinaxis && pointbad) ){
            vertexx.push_back(newx);
            vertexy.push_back(newy);
            vertexi.push_back(j);
          }
          // previous point
          prevx= newx;
          prevy= newy;

          // increase data pointer
          j++;
        } // valid time
      } // time loop
    }// dimension loop

    int npos= plotx.size();

    // spline smoothing of line
    if ( smoothing && npos > 2 ){
      float * x = new float[npos];
      float * y = new float[npos];
      int   * z = new int[npos];
      float * x_s = new float[(npos-1)*smoothdiv+npos];
      float * y_s = new float[(npos-1)*smoothdiv+npos];

      j=0;
      for ( int i=0; i<npos; i++ ){
        if ( j==0 || (plotx[i]!=x[j-1] || ploty[i]!=y[j-1]) ){
          x[j] = plotx[i];
          y[j] = ploty[i];
          z[j] = ploti[i];
          j++;
        }
      }
      npos = j;

      int s_length = smoothline(npos, &x[0], &y[0] , 0, npos-1, smoothdiv, &x_s[0], &y_s[0]);
      if ( s_length > 0 ) { // spline worked..
        plotx.clear(); ploty.clear(); ploti.clear();
        int co=0, tyc=0;
        for ( int i=0; i<s_length; i++, co++ ){
          int index = z[tyc];
          if ( co == smoothdiv ){
            tyc++;
            //cerr << "increasing tyc=" << tyc << " size of z=" << npos << endl;
            co=-1;
          }
          plotx.push_back(x_s[i]);
          ploty.push_back(y_s[i]);
          ploti.push_back(index);
          if ( i!= 0 && i!= s_length-1 ){
            plotx.push_back(x_s[i]);
            ploty.push_back(y_s[i]);
            ploti.push_back(index);
          }
        }
      }
      npos= plotx.size();
      delete[] x;
      delete[] y;
      delete[] z;
      delete[] x_s;
      delete[] y_s;
    }

    int currentPoint=0;

    // plot line segments
    //glBegin(GL_LINES);
    for ( int i=0; i<npos; i+=2 ){

      // for multilines - stop drawing at the end of everly line restart at start
      if(currentPoint==numOfPoints) {
      currentPoint=0;
      glEnd();
      }
      if(!currentPoint)
        glBegin(GL_LINES);
      currentPoint++;
      // end multilines

      if ( colorbyvalue ) _setColor( colorfromindex(ploti[i]) );
      if ( keepinaxis ){
        if ( ( ploty[i] > stopY && ploty[i+1] > stopY ) ||
            ( ploty[i] < startY && ploty[i+1] < startY ) ){ // both outside
          continue;
        } else if ( ploty[i] > stopY   ||
            ploty[i+1] > stopY ||
            ploty[i] < startY  ||
            ploty[i+1] < startY ){ // one outside
          // find crossing
          if ( crossPoint(plotx[i],ploty[i],plotx[i+1],ploty[i+1],cx,cy) ){
            if ( ploty[i] < startY ||  ploty[i] > stopY ){ // first bad
              glVertex2f(cx,cy);
              glVertex2f(plotx[i+1],ploty[i+1]);
            } else if ( ploty[i+1] < startY || ploty[i+1] > stopY ){ // second bad
              glVertex2f(plotx[i],ploty[i]);
              glVertex2f(cx,cy);
            }
          }

        } else { // all inside
          glVertex2f(plotx[i],ploty[i]);
          glVertex2f(plotx[i+1],ploty[i+1]);
        }
      } else { // no keepinaxis checking
        glVertex2f(plotx[i],ploty[i]);
        glVertex2f(plotx[i+1],ploty[i+1]);
      }


    }
    glEnd(); // GL_LINES

    _updatePrinting();
    glDisable(GL_LINE_STIPPLE);

    // draw a marker at each point
    if ( marker != NO_MARKER ){
      int n=vertexx.size();
      _setColor(markercolor);

      if (markerfill != NONE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (markerfill != SOLID){
          glEnable(GL_POLYGON_STIPPLE);
          glPolygonStipple(fillPattern(markerfill));
        }
      } else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      switch (marker) {
      case NO_MARKER :
        break;
      case M_RECTANGLE :
        for (i=0; i<n; i++){
          glRectf(vertexx[i]-mSizeX,vertexy[i]-mSizeY,
              vertexx[i]+mSizeX,vertexy[i]+mSizeY);
          _updatePrinting();
        }
        break;
      case M_TRIANGLE :
        mSizeX*= 1.2;
        mSizeY*= 1.2*0.866;
        _glBegin(GL_TRIANGLES,3*n);
        for (i=0; i<n; i++){
          glVertex2f(vertexx[i]-mSizeX,vertexy[i]+mSizeY);
          glVertex2f(vertexx[i]+mSizeX,vertexy[i]+mSizeY);
          glVertex2f(vertexx[i],vertexy[i]-mSizeY);
        }
        _glEnd();
        break;
      case M_DIAMOND :
        mSizeX*= 1.2;
        mSizeY*= 1.2;
        for (i=0; i<n; i++){
          _glBegin(GL_POLYGON,4);
          glVertex2f(vertexx[i]-mSizeX,vertexy[i]);
          glVertex2f(vertexx[i],vertexy[i]+mSizeY);
          glVertex2f(vertexx[i]+mSizeX,vertexy[i]);
          glVertex2f(vertexx[i],vertexy[i]-mSizeY);
          _glEnd();
        }
        break;
      case M_STAR :
        _glBegin(GL_LINES,8*n);
        for (i=0; i<n; i++){
          glVertex2f(vertexx[i]-mSizeX,vertexy[i]);
          glVertex2f(vertexx[i]+mSizeX,vertexy[i]);

          glVertex2f(vertexx[i],vertexy[i]-mSizeY);
          glVertex2f(vertexx[i],vertexy[i]+mSizeY);

          glVertex2f(vertexx[i]-mSizeX*0.75,vertexy[i]-mSizeY*0.75);
          glVertex2f(vertexx[i]+mSizeX*0.75,vertexy[i]+mSizeY*0.75);

          glVertex2f(vertexx[i]-mSizeX*0.75,vertexy[i]+mSizeY*0.75);
          glVertex2f(vertexx[i]+mSizeX*0.75,vertexy[i]-mSizeY*0.75);
        }
        _glEnd();
        break;
      case M_CIRCLE :
        for (i=0; i<n; i++){
          ellipse(vertexx[i],vertexy[i],mSizeX,mSizeY);
        }
        break;
      default:
        break;
      }
    }
    _updatePrinting();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_POLYGON_STIPPLE);

    // draw an arrow at each point
    if ( datadimension() > 1 && lineArrows ){
      float pixf = pixWidth/pixHeight;
      // arrow types
      float YLEN=arrowLength;
      float XLEN=YLEN;
      float AFAC= -arrowSize;
      float SFAC= AFAC*0.5;

      int n=vertexi.size();
      _setColor(arrowcolor);

      bool closed=false;

      if (arrowfill != NONE) {
        closed = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (arrowfill != SOLID){
          glEnable(GL_POLYGON_STIPPLE);
          glPolygonStipple(fillPattern(arrowfill));
        }
      } else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      float alpha;

      for ( i=0; i<n; i++ ){
        if (!reverse)
          alpha = M_PI_2 - M_PI*dval(vertexi[i],1)/180;
        else // angle rotated 180 degrees
          alpha = 3*M_PI_2 - M_PI*dval(vertexi[i],1)/180;

        float uff = XLEN*cos(alpha);
        float vff = YLEN*sin(alpha);
        float x0 = vertexx[i];
        float y0 = vertexy[i];

        if ( colorbyvalue ) _setColor( colorfromvalue(dval(vertexi[i])) );

        // draw main arrow
        _glBegin(GL_LINES,0);
        glVertex2f(x0,y0);
        glVertex2f(x0+uff*pixf,y0+vff);
        _glEnd();

        // draw arrowhead
        if (arrow){
          if ( closed ){
            _glBegin(GL_TRIANGLES);
            glVertex2f(x0+uff*pixf,y0+vff);
            glVertex2f(x0+(uff+uff*AFAC+vff*SFAC)*pixf,
                y0+vff+vff*AFAC-uff*SFAC);
            glVertex2f(x0+(uff+uff*AFAC-vff*SFAC)*pixf,
                y0+vff+vff*AFAC+uff*SFAC);
            _glEnd();
          } else {
            _glBegin(GL_LINES);
            glVertex2f(x0+uff*pixf,y0+vff);
            glVertex2f(x0+(uff+uff*AFAC+vff*SFAC)*pixf,
                y0+vff+vff*AFAC-uff*SFAC);
            glVertex2f(x0+uff*pixf,y0+vff);
            glVertex2f(x0+(uff+uff*AFAC-vff*SFAC)*pixf,
                y0+vff+vff*AFAC+uff*SFAC);
            _glEnd();
          }
        }
      }
    }
    _updatePrinting();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_STIPPLE);

    if (labelOnLine){
      _prepFont();
      _setColor(ptColor("WHITE"));
      float bx1, bx2, by1, by2;
      float th, tw;
      _getStringSize(labelText, tw, th);
      float marg= th*0.15;
      bx1= labelx - marg;
      bx2= bx1 + 2*marg + tw;
      if (labelside==SV_BOTTOM)
        labely-= (marg + 5 + th);
      else labely += marg+5;
      by1= labely - marg;
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
    cout << "LineElement::plot() finished" << endl;
#endif
  }
}
