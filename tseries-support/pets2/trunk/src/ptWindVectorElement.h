/*
  libpets2 - presentation and editing of time series
  
  $Id: ptWindVectorElement.h 4354 2014-01-02 13:07:57Z juergens $

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


#ifndef _windvectorelement_h
#define _windvectorelement_h

#include "ptGlobals.h"
#include "ptPlotElement.h"

class WindVectorElement : public dataPlotElement 
{
  ptColor boxColor;
  float deltaY;
  float lineWidth;
  bool label;
  bool FF_DD;
  bool center; // windarrow rotates around center
  int useTimes;
  miutil::miString text;
  std::vector<miutil::miString> vtext;
  bool datainknots;
public:
  WindVectorElement(const DataSpec cds,
		    const ptVertFieldf& field, 
		    const Layout& layout, 
		    XAxisInfo* xtime);
  void plot();
  bool needsData() { return true; }
};




#endif
