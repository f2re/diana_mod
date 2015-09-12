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


#ifndef _lineelement_h
#define _lineelement_h

#include "ptGlobals.h"
#include "ptPlotElement.h"
#include "ptYaxisElement.h"

class LineElement : public AxisChildElement
{
  ptMarker marker;
  ptColor markercolor;
  ptColor arrowcolor;
  ptFillStyle markerfill;
  float markersize;
  float barsize;
  bool shadow;
  bool lineBar;
  bool lineStep;
  bool lineArrows;
  float arrowLength;
  float arrowSize;
  ptFillStyle arrowfill;
  bool arrow;
  bool reverse;
  bool colorbyvalue;
  std::vector<ptColor> colorlist;
  std::vector<float> datalimits;
  bool smoothing;
  int smoothdiv;
  bool wrapdegrees;
  float wraplimit;
  bool isMultiLine;

public:
  LineElement(yAxisElement* ya,
	      const DataSpec cds,
	      const ptVertFieldf& field, 
	      const Layout& layout, 
	      XAxisInfo* xtime,
	      bool ismulti=false);
  void plot();
  bool needsData() { return true; }
  void dataInfo(float &min, float &max);
};

#endif
