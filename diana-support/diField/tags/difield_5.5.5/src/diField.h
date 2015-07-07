// -*- c++ -*-
/*
  Diana - A Free Meteorological Visualisation Tool

  Copyright (C) 2013 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This file is part of Diana

  Diana is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Diana is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Diana; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef diField_h
#define diField_h

#include "diArea.h"
#include "VcrossData.h"

#include <puTools/miTime.h>
#include <iosfwd>

const float fieldUndef= 1.e+35;

/**

  \brief Field data and information

  Contains one scalar field and information about size, projection, time
  and vertical coordinate values (pressure etc. as needed for computations).
  May also contain names and text for annotations. When plotting wind etc.
  using more than one scalar field, only the first contains annotation info.
*/
class Field {
  friend class FieldCacheEntity;
  friend class FieldCache;
public:
  //.................... set when reading: ..........................................
  float *data;
  int nx,ny;
  Area area;
  /// Grid Resolution in the X direction
  double gridResolutionX;
  /// Grid Resolution in the Y direction
  double gridResolutionY;

  bool allDefined;       // true if none undefined/missing values in the field
  int  producer;         // WMO producer number (88=met.no, 98=ECMWF,...)
  int  gridnum;          // a grid or model number
  int  dtype;            // datatype (1=analysis 2=forecast 3=init.ana 4=timeindependent)
  int  vcoord;           // met.no vertical coordinate number (should be more general)
  int  level;            // vertical level
  int  idnum;            // level2, used for EPS clusters and members
  int  forecastHour;
  float storageScaling;  // fieldValue= storedValue*storageScaling (rarely used)
  float storageOffset;   // fieldValue= storedValue+storageOffset (rarely used)
  miutil::miTime validFieldTime;
  miutil::miTime analysisTime;   // time of model analysis

  // set if applicable (used in computations, and to show obs. in same level)
  float pressureLevel;  // pressure in hPa
  float aHybrid;        // pressure[] = aHybrid + bHybrid * pSurface[]  ("eta levels")
  float bHybrid;        //
  float oceanDepth;     // depth in meters (positive), if fixed depth
  std::string unit;     //unit of field (celsius, kelvin, m, mm, ...?)
  bool discontinuous;    //data values are discontinuous/classes
  vcross::Values_p palette; //colour palette from file
  //.................................................................................

  int numSmoothed;
  bool gridChanged;
  bool difference;
  bool modifiedByProfet;
  bool turnWaveDirection; //In some fields wave direction need to be turned

  std::string name;
  std::string text;
  std::string fulltext;
  std::string modelName;
  std::string paramName;
  std::string fieldText;
  std::string leveltext;
  std::string idnumtext;
  std::string progtext;
  std::string timetext;
  std::string suffix;

  // estimated size in the memory of this field.....
  long bytesize();


  /**
   * Return nearest point value
   */
  float nearestPoint(const float & lat, const float & lon, int & index);

  /**
   * Copies all members, but performs a shallow copy on the actual data.
   * @param rhs Field to copy
   */
  void shallowMemberCopy(const Field& rhs);

  /**
   * Bounding box of 2D data field. West boundary.
   * @return West boundary x-coordinate (degrees)
   */
  float boundaryWest(){return area.P().boundaryWest();}


  /**
   * Bounding box of 2D data field. South boundary.
   * @return South boundary y-coordinate (degrees)
   */
  float boundarySouth(){return area.P().boundarySouth();}

  /**
   * Step size in x-direction in degrees.
   */
  float dx(){return area.P().dx();}

  /**
   * Step size in y-direction in degrees.
   */
  float dy(){return area.P().dy();}

private:
  // Copy members
  void memberCopy(const Field& rhs);

  // this member is set by its friends diFieldCache(Entity) and noone else...
  bool isPartOfCache;
  // this member is set by its friends diFieldCache(Entity) and noone else...
  miutil::miTime lastAccessed;


public:
  // Constructors
  Field();
  Field(const Field &rhs);
  // Destructor
  ~Field();

  bool isInCache() const { return isPartOfCache;}

  // Assignment operator
  Field& operator=(const Field &rhs);
  // Equality operator (only checking nx,ny)
  bool operator==(const Field &rhs) const;
  // Inequality operator (only checking nx,ny)
  bool operator!=(const Field &rhs) const;

  /// Delete the data
  void cleanup();
  /// Add dataspace for a xdim*ydim field
  void reserve(int xdim, int ydim);

  /// Add a field
  bool add(const Field &rhs);
  /// Subtract a field
  bool subtract(const Field &rhs);
  /// Multiply by a field
  bool multiply(const Field &rhs);
  /// Divide by a field (division by 0 = fieldUndef)
  bool divide(const Field &rhs);
  /// Difference between another field and this
  bool differ(const Field &rhs);
  /// Negate (the negative)
  void negate();
  /// Set value
  void setValue(float value);
  /// Set all values undefined
  void setUndefined();

  /// smooth the field in nsmooth iterations
  bool smooth(int nsmooth);
  /// smooth the field in nsmooth iterations
  bool smooth(int nsmooth, float *work,
	      float *worku1, float *worku2);
  /// interpolation in the field to selected positions
  bool interpolate(int npos, float *xpos, float *ypos, float *fpos,
		   int interpoltype);
  /// interpolate to another grid
  bool changeGrid(const Area& anew,
      const double& gridResolutionX_new,
      const double& gridResolutionY_new,
      const std::string& demands,
      int gridnumber);

  /// Return x,y in proj-coord
  void convertFromGrid(int npos, float* xpos, float* ypos);
  /// Return x,y in grid-coord
  void convertToGrid(int npos, float* xpos, float* ypos);
  /// Return grid in proj-coord
  void getXY(float* xpos, float* ypos);

  // ostream operator
  friend std::ostream& operator<<(std::ostream& output, const Field& f);
};

#endif
