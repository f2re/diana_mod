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


//ptPlotElement.h class declaration for PETS PlotElement class

#ifndef _plotelement_h
#define _plotelement_h

#include "ptGlobals.h"
#include <tsData/ptParameterDefinition.h>
#include <tsData/ptDiagramData.h>
#include <tsData/ptWeatherParameter.h>
#include "ptXAxisInfo.h"
#include "ptLayout.h"
#include "ptColor.h"
#include "ptFontManager.h"

#include <puTools/miString.h>
#include <puTools/miTime.h>

#include <glp/glpfile.h>

#include <GL/gl.h>

#include <vector>

struct DataSpec {
  ptDiagramData *dData;// diagramData
  int index;           // index to weatherparameter vector
  int comp;            // weatherparameter component
};

class PlotElement
{
protected:
  ptPrimitiveType type;       // Type of plotelement
  miutil::miString name;              // label or name of element
  bool enabled;               // plotElement is enabled
  bool visible;               // plotElement is visible
  float startY;               // vertical startposition
  float stopY;                // vertical stopposition
  ptColor color;              // base colour
  XAxisInfo *xtime;           // x-coordinates of timepoints
  ptFontSize fontSize;        // Font size
  int fontidx;                // font index
  ptAlign align;              // Alignment
  float glwidth, glheight;    // current GL width and height
  int scwidth, scheight;      // current screen width and height (in pixels)
  float pixWidth;             // current screenpixel width
  float pixHeight;            // current screenpixel height
  int startT;                 // start of timeinterval to plot
  int stopT;                  // end of timeinterval to plot
  bool AllTimesAxisScale;     // Axis scale calculated from all time points
  bool pInColour;             // use patterns in colour
  GLuint circle_list;         // display-list for circle
  std::vector<float> datalimits;
  std::vector<ptColor> colorlist;


  static bool useFakeStipple; // imitate linestippling
  static bool useColour; // colour or black/white
  static bool printing; // postscript plotting
  static GLPcontext* psoutput; // document factory module

  void _printString(const miutil::miString& text, const float x, const float y)
  {
    FM.printStr(text.c_str(), x, y);
  }
  void _setColor(const ptColor& col)
  {
    if (useColour)
      glColor4fv(col.colTable);
    else
      glColor4f(0.0, 0.0, 0.0, 1.0);
  }
  void _getCharSize(const char& c, float& w, float& h)
  {
    FM.getCharSize(c, w, h);
  }
  void _getMaxCharSize(float& w, float& h)
  {
    FM.getMaxCharSize(w, h);
  }
  void _getStringSize(const miutil::miString& c, float& w, float& h)
  {
    FM.getStringSize(c.c_str(), w, h);
  }
  void _prepFont()
  {
    FM.setGlSize(glwidth, glheight);
    FM.setVpSize(scwidth, scheight);
    FM.setFontSize(fontSize);
  }
  void _initPrinting(int /*size*/ = 500)
  {
  }
  void _updatePrinting()
  {
    if (printing && psoutput){
      psoutput->UpdatePage(true);
    }
  }
  void _endPrinting()
  {
    if (printing && psoutput){
      psoutput->UpdatePage(true);
    }
  }
  void _glBegin(const GLenum mode, int /*size*/ = 0)
  {
    glBegin(mode);
  }
  void _glEnd()
  {
    glEnd();
  }
  void lineSegment(const float &x1, const float &y1, const float &x2,
      const float &y2, const unsigned int &repeat, const unsigned int &bitmask,
      const bool &reset = true);
  void
      ellipse(const float& x, const float& y, const float& rx, const float& ry);
  float& xval(const int& i) // return xvalue (time)
  {
    return xtime->xcoord[i];
  }

  void psAddImage(const GLvoid* data, GLint size, GLint nx, GLint ny,
      GLfloat x, GLfloat y, GLfloat sx, GLfloat sy, GLint x1, GLint y1,
      GLint x2, GLint y2, GLenum format, GLenum type);
  int dataLimitIndex(const float v);
  float dataLimit(const int i)
  {
    if (i < 0 || i >= (int)datalimits.size())
      return 0;
    return datalimits[i];
  }
  ptColor colorfromvalue(const float v);
  ptColor colorfromindex(const int i);
  int smoothline(int npos, float x[], float y[], int nfirst, int nlast,
      int ismooth, float xsmooth[], float ysmooth[]); // B-spline smooth
public:
  PlotElement *next; // next is made public, to let Diagram access it...
  PlotElement(const Layout& layout, const ptVertFieldf& field,
      XAxisInfo *pXtime = 0, const bool& en = true, PlotElement *pNext = 0);
  virtual ~PlotElement();

  void setViewport(int sw, int sh, float gw, float gh);

  const miutil::miString& Name() const
  {
    return name;
  }
  ptPrimitiveType getType() const
  {
    return type;
  }
  virtual ParId getParId() const
  {
    ParId id = ID_UNDEF;
    return id;
  }
  ptColor Color() const
  {
    return color;
  }
  void setEnabled(const bool b)
  {
    enabled = b;
  }
  void enable()
  {
    enabled = true;
  }
  void disable()
  {
    enabled = false;
  }
  bool isEnabled() const
  {
    return enabled;
  }
  virtual void plot()=0;
  virtual bool needsData()
  {
    return false;
  }
  virtual void switchData(void*)
  {
  }
  virtual void setTimeInterval(const int start, const int stop);
  void setAllTimesAxisScale(bool b)
  {
    AllTimesAxisScale = b;
  }
  // for static use
  static void setColour(bool use)
  {
    useColour = use;
  }
  static void setPrinting(bool print)
  {
    printing = print;
  }
  static void setFakeStipple(bool use)
  {
    useFakeStipple = use;
  }

  static bool startPSoutput(const miutil::miString& fname, const bool incolour,
      const bool inlandscape, const bool doEPS = false);
  static bool startPSnewpage();
  static bool endPSoutput();

  void getRectangle(float& x1, float& y1, float& x2, float& y2);
  void getTimeRectangle(float& x1, float& y1, float& x2, float& y2);
  void setVisible(const bool b){visible= b; }

};


class dataPlotElement: public PlotElement
{
  ParId paramId;          // Id from Weatherparameter

protected:
  WeatherParameter *fdata; // datavalues
  DataSpec ds;             // data specification

  bool valid(const int& i) // if valid timepoint
    { return ds.dData->tobeplotted(ds.index,i);}
  int datastart();// find number of points before startT
  int dataend();// find number of points after stopT
  const float& dval(const int& i, const int& comp) // return datavalue
    { return fdata->Data(i,comp);}
  const float& dval(const int& i) // return datavalue, default component
    { return fdata->Data(i,ds.comp);}
  void setdval(const int i, const float val)
    { fdata->setData(i,ds.comp,val); fdata->setDirty(true); }
  void setdval(const int i, const int comp, const float val)
    { fdata->setData(i,comp,val); }
  int datasize() const { return fdata->Npoints(); }
  void calcDataProperties()
    { fdata->calcCompProperties(ds.comp);}
  void calcAllDataProperties()
    { fdata->calcAllProperties();}
  int datadimension() const { return fdata->Ndim(); }
  float datamin(int& comp) const {return fdata->Cmin(comp);}
  float datamin() const {return fdata->Cmin(ds.comp);}
  float datamax(int& comp) const {return fdata->Cmax(comp);}
  float datamax() const {return fdata->Cmax(ds.comp);}
  bool  datapolar() const {return fdata->Polar();}
public:
  dataPlotElement(const DataSpec cds,
		  const Layout& layout,
		  const ptVertFieldf& field,
		  XAxisInfo *pXtime=0,
		  const bool& en=true,
		  PlotElement *pNext=0)
    : PlotElement(layout,field,pXtime,en,pNext),
      paramId((*cds.dData)[cds.index].Id()),
      fdata(&((*cds.dData)[cds.index])), ds(cds)
  {}
  virtual ~dataPlotElement(){}
  virtual void setTimeInterval(const int start, const int stop);
  ParId getParId() const {return paramId;}
  virtual void plot()=0;
  virtual bool needsData()=0;
  DataSpec getDataSpec() const {return ds;}
};


#endif



