/*
  libpets2 - presentation and editing of time series
  
  $Id: ptGridlineElement.h 4354 2014-01-02 13:07:57Z juergens $

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


#ifndef _gridlineelement_h
#define _gridlineelement_h

#include "ptGlobals.h"
#include "ptPlotElement.h"


class GridlineElement : public PlotElement
{
  int numTickMajor;
  int numTickMinor;
  float axeStopY;
  float labelSpace;
  float deltaY;
  float lineWidth;
  ptLineStyle style;
  bool gridxonly;
  bool plotDayPattern;
  int useTimes;
  ptColor daycolor;
  std::vector<miutil::miTime> timeLine;
  std::vector<int> daychanges;
public:
  GridlineElement(const std::vector<miutil::miTime> tline,
		  const ptVertFieldf& field, 
		  const Layout& layout, 
		  XAxisInfo* xtime);
  void plot();
  bool needsData() { return false; }
};



#endif
