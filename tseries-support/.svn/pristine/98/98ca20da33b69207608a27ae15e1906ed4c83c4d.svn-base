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


#ifndef _yaxiselement_h
#define _yaxiselement_h

#include "ptGlobals.h"
#include "ptPlotElement.h"

#define MAX_YAXIS_CHILDREN 32

class AxisChildElement;

class yAxisElement : public PlotElement
{
protected:
  int numChild;
  int id;
  AxisChildElement *child[MAX_YAXIS_CHILDREN]; // the childelements
  ptColor childCol[MAX_YAXIS_CHILDREN]; // child color
  ptLineStyle childStyle[MAX_YAXIS_CHILDREN]; // child line style
  miutil::miString Labels[MAX_YAXIS_CHILDREN+1]; // axistexts
  float labelY[MAX_YAXIS_CHILDREN+1]; // Plot-start
  float labelX[MAX_YAXIS_CHILDREN+1]; // --- " ---
  float labelW[MAX_YAXIS_CHILDREN+1]; // label widths
  float labelH[MAX_YAXIS_CHILDREN+1]; // label heights
  float lineSt[MAX_YAXIS_CHILDREN+1]; // children linesample starts
  float zerox, tickX; // xvalue of axisline and end of tickmark
  float charHeight, charWidth, charHeight_2;
  bool firstPlot;
  std::vector<float> minorticks;
  std::vector<float> majorticks;
  std::vector<float> majorphys;

  float axeStopY;
  float tickLen;
  float lineWidth, axisLineWidth, tickWidth;
  float interval, minRange, delta, minMargin;
  ptLineStyle style;
  ptAxis axis;
  bool minIsSet, maxIsSet;
  float minValue, maxValue;
  float minPlotY, maxPlotY, plotRange; // Physical values to childelements
  float deltaY; // GL value to childelements
  bool recalcDims; // recalculate plot-interval etc.
  bool userlabels;
  std::vector<std::string> userLabels;
  std::vector<std::string> userValueLabels;

  int sign;
  int lsign;
  bool fittopage;
  float linlen; // length of linesample
  bool plotlegends; // plot legends
  bool horLabels;
  float horLabelOffset;
  bool legendlineinside;

  // optional gridline attributes
  bool axisgrid;
  float gridwidth;
  ptColor gridcolor;
  ptLineStyle gridstyle;

  bool useMinMax;

  void plotAxis();
  void plotGrid();
  void plotLabels();
  virtual void calcPlotVal();
  virtual void calcDims();
public:
  yAxisElement(const ptVertFieldf& field,
	       const Layout& layout,
	       XAxisInfo* xtime);

  virtual void plot();
  virtual bool needsData() { return true; }
  virtual void setTimeInterval(const int start, const int stop);
  // called from yaxis childelements to report their presence..
  // return legal range
  bool callinn(AxisChildElement*, float&, float&);
  // returns deltaY(gl),minPlotY and plotRange(physical)
  void dataInfo(float&, float&, float&);
  int Id(){return id; }
  void setLabels(const std::vector<std::string>& labels)
    {userLabels= labels; userlabels= true;}
  void resetLabels()
    {userlabels= false;}
  bool hasUserLabels() const { return userlabels; }
  std::string userValueLabel(const float value);
};

class staticYaxisElement : public yAxisElement
{
private:
  int numTickMajor;
  int numTickMinor;
  float labelSpace; // vertical space for labels
protected:
  void calcPlotVal();
  void calcDims();
public:
  staticYaxisElement(const ptVertFieldf& field,
		     const Layout& layout,
		     XAxisInfo* xtime);
  ~staticYaxisElement();

  virtual void setTimeInterval(const int start, const int stop);
};


// Baseclass for LineElement and EditLineElement
class AxisChildElement : public dataPlotElement
{
protected:
  yAxisElement* Yaxis;
  float lineWidth;
  ptLineStyle style;
  bool quantized;
  float quantum;
  bool labelOnLine;
  float lineLabelPos;
  ptVerSides labelside;
  std::string axisText;
  std::string labelText;
  float minPlotY, plotRange;
  float deltaY;
  float minLegal, maxLegal;
  bool keepinaxis;


  bool _informYaxis()
    {
      if (Yaxis)
	return Yaxis->callinn(this,minLegal,maxLegal);
      else return false;
    }
  bool _legalValue(float val)
    { return (val >= minLegal && val <= maxLegal);}
  void _prePlot()
    { if (Yaxis) Yaxis->dataInfo(deltaY,minPlotY,plotRange);}
  float yval(const int& time)
    { return startY + deltaY*((dval(time) - minPlotY)/plotRange);}
  float yval(const int& time, const int& comp)
    { return startY + deltaY*((dval(time,comp) - minPlotY)/plotRange);}
  bool crossPoint(const float x1, const float y1,
		  const float x2, const float y2,
		  float& cx, float& cy);
  bool dataCrossPoint(const float x1, const float y1,
		      const float x2, const float y2,
		      const float d,
		      float& cx, float& cy);
public:
  AxisChildElement(yAxisElement* ya,
		   const DataSpec cds,
		   const ptVertFieldf& field,
		   const Layout& layout,
		   XAxisInfo* xtime);
  ~AxisChildElement();
  virtual void plot() = 0;
  virtual bool needsData() { return true; }
  virtual void dataInfo(float &min, float &max)
  {
    min   = datamin();
    max   = datamax();
  }
  void plotInterval(float& pmin, float& pmax) const
  { pmin= startY; pmax= pmin+deltaY; }
  std::string& labelName()
  { return axisText; }
  ptColor& Color()
  { return color; }
  ptLineStyle& lStyle()
  { return style; }
  void setMinMax(float min, float max)
  {minLegal= min; maxLegal= max; }
};

#endif
