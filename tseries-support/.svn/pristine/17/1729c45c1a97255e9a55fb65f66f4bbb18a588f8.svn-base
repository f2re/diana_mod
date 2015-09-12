// -*- c++ -*-
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

// ptDiagram.h : Declarations for the ptDiagram class

#ifndef _diagram_h
#define _diagram_h

#include "ptGlobals.h"
#include <tsData/ptWeatherParameter.h>
#include "ptXAxisInfo.h"
#include <tsData/ptDiagramData.h>
#include "ptPlotElement.h"
#include "ptLayout.h"
#include "ptStyle.h"

#include <puTools/miTime.h>

#include <vector>
#include <map>

class ptDiagram {
private:
  XAxisInfo xtime; // x-coordinates for the timepoints
  std::vector<miutil::miTime> timeLine; // the actual time points
  std::vector<int> timeLineIdx; // indices to complete timeline in DD
  int nPlotElements; // number of plotelements
  PlotElement *first, *last;// pointers to plotelement list
  ptDiagramData *DD; // diagramdata
  ptStyle *Style; // current style
  float glwidth, glheight; // current GL width and height
  int scwidth, scheight; // current screen width and height (in pixels)
  float pixWidth, pixHeight;// current width and height of screenpixel
  miutil::miTime startT, stopT; // current timeinterval
  int startidx, stopidx; // current timeinterval xaxis-indices
  bool colourFlag; // diagram in colour or black/white
  std::map<miutil::miString, miutil::miString> keymap; // keywords for text-strings
  bool localTime; // use localtime
  int timeZone; // which timezone
  bool showGridLines;
  void makeXtime();
  void toLocaltime(miutil::miTime& t);

public:
  ptDiagram(ptStyle*, bool showGlines=true);
  ~ptDiagram();

  // attach a diagramdata to diagram
  bool attachData(ptDiagramData*);
  // make plotelements based on style and DD
  bool makeDefaultPlotElements(ptColor *bgColor);
  // set current viewport both in screen pixels and in GL coordinates
  void setViewport(int sw, int sh, float gw, float gh);

  // turn on scaling of Yaxis based on data from all timepoints
  void setAllTimesAxisScale(bool b);

  // set time interval for plotting
  void setTimeInterval(int, int);
  void setTimeInterval(miutil::miTime, miutil::miTime);
  void setProgInterval(int progstart, int progstop);

  // return current timeinterval, start and stop
  void getTimeInterval(int &start, int &stop);
  void getTimeInterval(miutil::miTime& start, miutil::miTime& stop);
  void getTotalTimeInterval(int &start, int &stop);
  void getTotalTimeInterval(miutil::miTime &start, miutil::miTime &stop);

  PlotElement* findElement(const miutil::miString, PlotElement* start = 0);// search by name
  PlotElement* findElement(ptPrimitiveType, PlotElement* start = 0);// search by type
  PlotElement* findElement(ptPrimitiveType, ParId, PlotElement* start = 0);// search by type and pid
  void addElement(PlotElement*);
  void removeElement(PlotElement*);
  void removeElement(ptPrimitiveType);
  void makeAxesLocation();
  void enableElement(PlotElement*);
  void disableElement(PlotElement*);
  void enableElement(ptPrimitiveType);
  void disableElement(ptPrimitiveType);
  void disableAll();
  void enableAll();
  void plot();
  void tst_print();

  void toggleColour(bool use);
  bool getColour()
  {
    return colourFlag;
  }
  void setPrinting(bool print);
  void setFakeStipple(bool use);
  bool startPSoutput(const std::string& fname, const bool incolour,
      const bool inlandscape, const bool doEPS = false);
  bool startPSnewpage();
  bool endPSoutput();
  // keyword/value pair methods
  void addKeyword(const miutil::miString&, const miutil::miString&);
  void setKeymap(const std::map<miutil::miString, miutil::miString>&);
  void clearKeywords();
};

#endif
