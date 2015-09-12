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


#ifndef _editlineelement_h
#define _editlineelement_h

#include "ptGlobals.h"
#include "ptPlotElement.h"
#include "ptYaxisElement.h"
#include "ptEditModes.h"
#include <puTools/miRing.h>

#include <vector>

class EditLineElement : public AxisChildElement
{
  std::vector<float> accuvalues;  // accumulated delta-y for quantization;
  std::vector<bool> markedPoints; // datapoints marked for moving
  ring< std::vector<float> > *undobuffer; // ringbuffer with saved datapoints
  int activePoint; // pointer to last grabbed point (used for moving)
  ptLineEditMode editMode;
  ptLineEditMarker markerType;
  float markerSize;
  ptColor backcolor;
  ptColor histocolor;
  bool printValue; // print value of active point
  bool printValueLock; // lock printValue flag
  bool modified;
  bool activenodes;
  float actPx, actPy; // position of active point
  miutil::miString pformat; // numerical format of small value-tip
  int priority;     // priority for editing
  bool wrapdegrees;
  float wraplimit;

  ptEditStyle editstyle; // type of graphical editing
  ptFillStyle fstyle;    // fillstyle for editing elements
  ptAlign pointAlign;    // alignment of edit-markers

  void _resetAccu(); // reset accuvalues
  bool _accumulate(const int,float&); //change accuvalues; return new diff
public:
  EditLineElement(yAxisElement* ya,
		  const DataSpec cds,
		  const ptVertFieldf& field,
		  const Layout& layout,
		  XAxisInfo* xtime);
  ~EditLineElement();
  void plot();
  bool needsData() { return true; }

  bool pMarked; // true if any points are marked for edit
  bool grabPoint(float x, float y,// return true if x,y near any datapoint
		 bool mark = false,
		 bool fillInterval = false);
  bool grabPoint(int  step,
		 bool mark,
		 bool fillInterval);
  void movePoints(float dy); // move marked points dy
  void movePointsQuant(int dy); // move marked points dy quants
  void replaceDataValues(const WeatherParameter & source);
  void releasePoints(); // unmark all points
  void releaseButton(); // notify buttonrelease
  void zero(); // try to set marked points to zero..
  void undo(); // undo previous editing
  void saveundo(); // put datavalues in undobuffer
  void setEditMode(ptLineEditMode em); // set type of editing
  void setMarkerType(ptLineEditMarker em)
    { markerType = em; }
  void setValuePrinting(const bool flag); // change printValue
  void lockValuePrinting(const bool flag); // lock/unlock printValue
  bool isModified(){return modified;}
  void setActive(const bool b){activenodes= b; }
  void getActivePos(float& px, float& py){ px= actPx; py= actPy;}
  int  Priority() const { return priority;}
};


#endif
