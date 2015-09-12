/*
  libpets2 - presentation and editing of time series

  $Id: ptStyle.h 4354 2014-01-02 13:07:57Z juergens $

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


// ptStyle.h: declarations for ptStyle class
#ifndef _ptstyle_h
#define _ptstyle_h

#include <puTools/miString.h>

#include "ptGlobals.h"
#include "ptStyleOrder.h"
#include <tsData/ptPrimitiveType.h>
#include "ptLayout.h"

const int MAXAXES = 5;     // max allowed axes in one diagram
const int MAXYAXIS = 4;    // max allowed y-axes in one axes
const int MAXLINES_IN_ONE = 4; // max allowed lines in one y-axis
const int MAXUNSPECIFIED_LINES = 10; // max allowed unspecified lines
const int MAXFONT = 5;    //max number of allowed user defined fonts

enum stConversion {
  NO_CONVERSION,
  AS_VECTOR,
  AS_VECTOR_ONLY,
  AS_LINE,
  AS_LINE_ONLY
};

enum stMother {
  AXES1,
  AXES2,
  AXES3,
  AXES4,
  AXES5,
  DIAGRAM,
  NO_MOTHER
};

struct Out {
  stMother mother;
  int order;
  int zorder;
  StyleOrder prim;
};

struct Primitive {
  ptPrimitiveType type;
  int order;
  bool enabled;
  ParId id;
  int nr;
  Layout layout;
  stConversion conv;
  stMother mother;
  bool plotAll;
  int component; // which datacomponent to use

  Primitive(ptPrimitiveType t=DUM_PRIMITIVE);
  void print() const;
};

class ptStyle {
  float topMargin_, bottomMargin_, leftMargin_, rightMargin_;
  float leftOffset_;      // time offset left side, see XAXIS
  float rightOffset_;     // time offset right side, see XAXIS
  ptColor bgColor;
  int nprimF, nprim;
  std::vector<Primitive> pprimF, pprim;
  int nAxes;
  bool localTime;
  int timeZone;

  int _findPrimF(const ptPrimitiveType) const;
  int _findPrimF(const ParId, const int from=-1) const;
  void _enablePrimitive(const bool, const int);
  void _disablePrimitive(const bool, const int);
  ptAxis _findFirstFreeAxis(bool usedAxes[][MAXLINES_IN_ONE],
			    const stMother mother);
  void _sortArray(Out***, const int&, const stMother&, const int*);
public:
  ptStyle();
  ptStyle(const ptStyle&);
  ~ptStyle();

  ptStyle& operator=(const ptStyle&);
  const float& leftMargin()  { return leftMargin_; }
  const float& rightMargin() { return rightMargin_; }
  const float& topMargin()   { return topMargin_; }
  const float& bottomMargin(){ return bottomMargin_; }
  void setMargins(const float lm, const float rm,
		  const float tm, const float bm)
  {
    leftMargin_= lm; rightMargin_ = rm;
    topMargin_ = tm; bottomMargin_= bm;
  }

  const float& leftOffset() { return leftOffset_; }
  const float& rightOffset() { return rightOffset_; }

  void clear();

  bool organize(StyleOrder*, int*, StyleOrder*,	int);
  void enablePrimitive(const bool, const ptPrimitiveType);
  void enablePrimitive(const ParId,
		       const ptPrimitiveType type = DUM_PRIMITIVE);
  void enablePrimitive(const bool, const int);
  void enableAllPrimitives(const bool);
  void disablePrimitive(const bool, const ptPrimitiveType);
  void disablePrimitive(const ParId,
			const ptPrimitiveType type = DUM_PRIMITIVE);
  void disablePrimitive(const bool, const int);
  void disableAllPrimitives(const bool);
  void print() const;
  bool readStyle(const miutil::miString filename, bool verbose= false);

  bool addPrimitive(const Primitive& p);

  void backgroundColor(const ptColor c) {bgColor = c;}
  ptColor backgroundColor() const { return bgColor; }
  void getTimeSetting(bool& lc, int& tz) const
  { lc= localTime; tz= timeZone; }
};

extern stMother Str2Mother(miutil::miString);
extern miutil::miString Mother2Str(stMother);

#endif
