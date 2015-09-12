/*
  libpets2 - presentation and editing of time series

  $Id: ptYaxisElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptYaxisElement.cc : Definitions for yAxisElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptYaxisElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace miutil;

yAxisElement::yAxisElement(const ptVertFieldf& field,
			   const Layout& layout,
			   XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , numChild(0)
  , id(layout.yaid)
  , firstPlot(true)
  , axeStopY(field.y2)
  , tickLen(layout.tickLen)
  , lineWidth(layout.lineWidth)
  , axisLineWidth(layout.axisWidth)
  , tickWidth(layout.tickWidth)
  , interval(layout.interval)
  , minRange(layout.minRange)
  , delta(layout.delta)
  , minMargin(layout.minMargin)
  , style(layout.linePattern)
  , axis(layout.axis)
  , minIsSet(layout.minIsSet)
  , maxIsSet(layout.maxIsSet)
  , minValue(layout.minValue)
  , maxValue(layout.maxValue)
  , recalcDims(true)
  , userlabels(false)
  , userLabels(layout.textlabels)
  , userValueLabels(layout.valuetextlabels)
  , fittopage(layout.fittopage)
  , linlen(48)
  , plotlegends(layout.label)
  , horLabels(layout.horLabels)
  , horLabelOffset(layout.horLabelOffset)
  , legendlineinside(layout.legendlineinside)
  , axisgrid(layout.axisgrid)
  , gridwidth(layout.gridwidth)
  , gridcolor(layout.gridcolor)
  , gridstyle(layout.gridstyle)
  , useMinMax(layout.useMinMax)
{
#ifdef DEBUG
  cout << "Inside yAxisElement's constructor" << endl;
#endif
  type = YAXIS;

  for (int i=0; i<MAX_YAXIS_CHILDREN; i++) child[i]=0;

  //axisText = (layout.text.length()) ? layout.text : data->axisText;
  Labels[0] = layout.text;

  userlabels= (userLabels.size() > 0);
  if ( userlabels && userValueLabels.size() == 0 )
    userValueLabels = userLabels;
}

void yAxisElement::plotAxis()
{
  if (!numChild || !visible ) return;
#ifdef DEBUG
    cout << "yAxisElement::plotAxis()" << endl;
#endif
  float txtX, tmpX, tmpY;
  const float SMALLSPACE = 5;
  char txt[20];

  _prepFont();

  txtX= tickX + sign*SMALLSPACE;
  tmpX= txtX;

  // plot y-axis
  glDisable(GL_LINE_STIPPLE);
  _setColor(color);
  glLineWidth(axisLineWidth);
  _glBegin(GL_LINES,2);
  glVertex2f(zerox,startY); glVertex2f(zerox,startY+deltaY);
  _glEnd();
  _updatePrinting();

  // plot tickmarks for each interval step, and write value for each delta
  glLineWidth(tickWidth);
  size_t i,n= minorticks.size();
  for (i=0; i<n; i++){
    _glBegin(GL_LINES,2);
    glVertex2f(tickX,minorticks[i]);
    glVertex2f(zerox,minorticks[i]);  // minor tick mark
    _glEnd();
  }
  _updatePrinting();
  n= majorticks.size();
  float swid, shei;
  float val;
  for (i=0; i<n; i++){
    _glBegin(GL_LINES,2);
    glVertex2f(tickX,majorticks[i]);
    glVertex2f(zerox,majorticks[i]);  // major tick mark
    _glEnd();
    _updatePrinting();
    if (!userlabels){
      // print value
      val= majorphys[i];
      if ( delta >= 5 && fmodf(delta,1.0)==0)
	snprintf(txt,sizeof(txt),"%1.0f",val);
      else if (delta >= 0.1 ){
	snprintf(txt,sizeof(txt),"%1.1f",val);
      } else {
	snprintf(txt,sizeof(txt),"%1.2f",val);
      }
      _getStringSize(txt,swid,shei);

      if (align==OVER) tmpY=majorticks[i];
      else if (align==BELOW) tmpY=majorticks[i]-shei;
      else tmpY= majorticks[i]-shei/2.0;

      if (sign==-1) tmpX= txtX-swid;
      _printString(txt,tmpX,tmpY);
      _updatePrinting();
    } else{
      if (i<userLabels.size()){
	_getStringSize(userLabels[i], swid, shei);

	if (align==OVER) tmpY=majorticks[i];
	else if (align==BELOW) tmpY=majorticks[i]-shei;
	else tmpY= majorticks[i]-shei/2.0;

	if (sign==-1) tmpX= txtX-swid;
	_printString(userLabels[i], tmpX, tmpY);
	_updatePrinting();
      }
    }
  }
  plotLabels();
}


void yAxisElement::plotLabels()
{
  // write axis label
  _printString(Labels[0],labelX[0],labelY[0]);
  _updatePrinting();

  if (!plotlegends) return;
  glLineWidth(lineWidth);
  glPointSize(lineWidth);
  bool fakestipple=false;
  if ((!useColour)||pInColour) {
    if (!useFakeStipple)
      glEnable(GL_LINE_STIPPLE);
    else
      fakestipple=true;
  }
  for (int j=1; j<numChild+1; j++) {
    if (child[j-1]->isEnabled()) {
      _setColor(childCol[j-1]);
      _printString(Labels[j],labelX[j],labelY[j]);
      _updatePrinting();
      if ((!useColour) || pInColour){
	if (fakestipple) {
	  _glBegin(GL_POINTS,1000);
	  lineSegment(lineSt[j],
		      labelY[j],
		      lineSt[j]+lsign*linlen,
		      labelY[j],
		      LineStyle[childStyle[j-1]][0],
		      LineStyle[childStyle[j-1]][1]);
	  _glEnd();
	} else {
	  glLineStipple(LineStyle[childStyle[j-1]][0],
			LineStyle[childStyle[j-1]][1]);
	  _glBegin(GL_LINES,2);
	  glVertex2f(lineSt[j],labelY[j]);
	  glVertex2f(lineSt[j]+lsign*linlen,labelY[j]);
	  _glEnd();
	}
	_updatePrinting();
      }
    }
  }
  glLineStipple(1,0xFFFF);
  glDisable(GL_LINE_STIPPLE);
}

void yAxisElement::plotGrid()
{
  if (!axisgrid) return;
  int n= majorticks.size();
  bool fakestipple = false;
  if (!useFakeStipple) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(LineStyle[gridstyle][0],LineStyle[gridstyle][1]);
  } else fakestipple = true;

  _setColor(gridcolor);
  glLineWidth(gridwidth);
  glPointSize(gridwidth/2.0);
  float fy;
  for (int i=0; i<n; i++){
    fy= majorticks[i];
    if (fakestipple){
      _glBegin(GL_POINTS,1000);
      lineSegment(xtime->x1,fy,
		  xtime->x2,fy,
		  LineStyle[gridstyle][0],
		  LineStyle[gridstyle][1],
		  true);
      _glEnd();
    } else {
      _glBegin(GL_LINES,2);
      glVertex2f(xtime->x1,fy);
      glVertex2f(xtime->x2,fy);
      _glEnd();
    }
  }
  glLineStipple(1,0xFFFF);
  glDisable(GL_LINE_STIPPLE);
}

void yAxisElement::plot()
{
  if(enabled && numChild) {
#ifdef DEBUG
    cout << "yAxisElement::plot()" << endl;
#endif
//     if (firstPlot) calcPlotVal();
    calcPlotVal();
    firstPlot = false;
    if (recalcDims) {
      calcDims();
      plotGrid();
    }
    recalcDims= true;
    plotAxis();
  }
}

void yAxisElement::dataInfo(float &dY, float & minPY, float &pR)
{
  if (firstPlot) calcPlotVal();
  firstPlot = false;
  if (recalcDims) {
    calcDims();
    plotGrid();
  }

  dY = deltaY;
  minPY = minPlotY;
  pR = plotRange;
}


void yAxisElement::calcPlotVal()
{
  const float SMALLSPACE = 5;
  float maxwid = 0, lx;
  int i;

  _prepFont();
  _getMaxCharSize(charWidth,charHeight);
  charHeight_2 = charHeight/2.0;

  for (i=0; i<numChild+1; i++) {
    _getStringSize(Labels[i], labelW[i], labelH[i]);
    if (labelW[i] > maxwid) maxwid = labelW[i];
  }
  labelY[numChild] = axeStopY - labelH[numChild];
  for (i=numChild-1; i>=0; i--) {
    labelY[i] = labelY[i+1] - labelH[i]*1.5;
  }
  deltaY = labelY[0]-charHeight-startY;

  switch(axis) {
  case LEFTLEFT:
    sign=-1;
    zerox = xtime->x1;
    tickX = zerox - tickLen;
    lx = (zerox > maxwid+SMALLSPACE) ?
	(zerox-maxwid-SMALLSPACE) : SMALLSPACE;
    if (fittopage && zerox < (maxwid+linlen))
      lx = linlen;
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx;// - SMALLSPACE;
    }
    break;
  case RIGHTRIGHT:
    sign=1;
    zerox = xtime->x2;
    tickX = zerox + tickLen;
    lx = zerox + SMALLSPACE;
    if (fittopage && (zerox+maxwid+linlen)>globalWindow.x2)
      lx = globalWindow.x2 - (maxwid+linlen);
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx + labelW[i];// + SMALLSPACE;
    }
    break;
  case LEFTRIGHT:
    sign=1;
    zerox = xtime->x1;
    tickX = zerox + tickLen;
    lx = zerox+SMALLSPACE;
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx + labelW[i] + SMALLSPACE;
    }
    break;
  case RIGHTLEFT:
    sign=-1;
    zerox = xtime->x2;
    tickX = zerox - tickLen;
    lx = zerox-maxwid-SMALLSPACE;
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx - SMALLSPACE;
    }
    break;
  }

}

bool yAxisElement::callinn(AxisChildElement* pc,
			   float& minlegal,
			   float& maxlegal)
{
  if (numChild < MAX_YAXIS_CHILDREN) {
    child[numChild] = pc;
    numChild++;
    // save child label and color
    Labels[numChild] = pc->labelName();
    childCol[numChild-1] = pc->Color();
    childStyle[numChild-1] = pc->lStyle();
    minlegal = (minIsSet) ? minValue : -FLT_MAX;
    maxlegal = (maxIsSet) ? maxValue :  FLT_MAX;
    return true;
  }
  return false;
}

void yAxisElement::setTimeInterval(const int start, const int stop)
{
  PlotElement::setTimeInterval(start,stop);
}

void yAxisElement::calcDims()
{
  int j;
  // new : get min,max,delta each time
  //const float eps = 0.0001;
  float cMax,cMin,Max=-100000,Min=100000,Delta;
  for (j=0; j<numChild; j++) {
    if (child[j]->isEnabled()) {
      child[j]->dataInfo(cMin,cMax);
      if (cMax > Max) Max = cMax;
      if (cMin < Min) Min = cMin;
    }
  }

  if ( useMinMax ){
    if ( Min < minValue ) Min = minValue;
    if ( Max > maxValue ) Max = maxValue;
  }

  Delta = Max-Min;

  float newDelta;

  if (!minIsSet && !maxIsSet){
    plotRange = (Delta < minRange) ? minRange + 2*minMargin
      : Delta + 2*minMargin;
    minPlotY = Min + (Delta-plotRange)/2;
    if (minValue<minPlotY) {
      plotRange += (minPlotY-minValue);
      minPlotY = minValue;
    }
    maxPlotY = minPlotY + plotRange;
    if (maxValue>maxPlotY) {
      plotRange += (maxValue-maxPlotY);
      maxPlotY = maxValue;
    }
  } else if (minIsSet && maxIsSet) {
    minPlotY = minValue;
    maxPlotY = maxValue;
    plotRange = maxPlotY - minPlotY;
  } else if (minIsSet) {
    minPlotY = minValue;
    newDelta = Max - minPlotY;
    plotRange = (newDelta < minRange) ? minRange + minMargin
      : newDelta + minMargin;
    maxPlotY = minPlotY + plotRange;
    if (maxValue>maxPlotY) {
      plotRange += (maxValue-maxPlotY);
      maxPlotY = maxValue;
    }
  } else { // maxIsSet
    maxPlotY = maxValue;
    newDelta = maxPlotY - Min;
    plotRange = (newDelta < minRange) ? minRange + minMargin
      : newDelta + minMargin;
    minPlotY = maxPlotY - plotRange;
    if (minValue<minPlotY) {
      plotRange += (minPlotY-minValue);
      minPlotY = minValue;
    }
  }

  minorticks.erase(minorticks.begin(),minorticks.end());
  majorticks.erase(majorticks.begin(),majorticks.end());
  majorphys.erase(majorphys.begin(),majorphys.end());
  float y;
  const float eps2 = 0.01;

  float interv= interval;
  float delt= delta;
  const int maxticks= 30;
  while ((maxPlotY-minPlotY)/interv>maxticks){
    interv *= 2;
    delt   *= 2;
  }
  float starti = (minPlotY < 0) ? minPlotY-interv-fmod(minPlotY,interv)
    : minPlotY+interv-fmod(minPlotY,interv);
  if (starti < 0) starti+=interv;

  for(float i = starti; i <= maxPlotY; i+=interv) {
    y = startY + (deltaY*(i-minPlotY))/plotRange;
    if(fabs(fmod(i,delt)) < eps2 || delt-fabs(fmod(i,delt)) < eps2 ) {
      majorticks.push_back(y);
      majorphys.push_back(i);
    } else {
      minorticks.push_back(y);
    }
  }

  recalcDims= false;
}

std::string yAxisElement::userValueLabel(const float value)
{
  std::string s;

  int n = majorticks.size();

  for (int i=0; i<n; i++) {
    if (value <= majorphys[i] && i < userValueLabels.size()) {
      s = userValueLabels[i];
      break;
    }
  }

  return s;
}

