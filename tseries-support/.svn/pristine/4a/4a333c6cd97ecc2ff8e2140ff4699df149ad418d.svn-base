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


#ifndef _histogramelement_h
#define _histogramelement_h

#include "ptGlobals.h"
#include "ptPlotElement.h"


class HistogramElement : public dataPlotElement 
{
  float deltaY;
  float lineWidth;
  ptFillStyle style;
  bool unit;
  float max;
  int *deltaT;
  enum {YSPACE = 5};
  int hstart;
  int hstop;
  bool drawlabel;
public:
  HistogramElement(const DataSpec cds,
		   const ptVertFieldf& field, 
		   const Layout& layout, 
		   XAxisInfo* xtime,
		   std::vector<miutil::miTime>& timeLine);
  ~HistogramElement();
  void plot();
  void tst_print();
  bool needsData() { return true; }
  //void switchData(void* pData) {data = (WeatherParameter*)pData;}
};

#endif
