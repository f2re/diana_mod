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
#ifndef diProjection_h
#define diProjection_h

#include "diRectangle.h"
#include <proj_api.h>
#include <boost/shared_ptr.hpp>
#include <iosfwd>
#include <map>
#include <string>

//**********************************************************************
//   libmi projetions:
//
//   Spherical (rotated) grid, gridtype=2,3:
//
//      gs[0] - western boundary (degrees)
//      gs[1] - southern boundary (degrees)
//      gs[2] - longitude increment (degrees)
//      gs[3] - latitude increment (degrees)
//      gs[4] - xcen: longitude position of rotated equator (degrees)
//      gs[5] - ycen: latitude  position of rotated equator (degrees)
//              (lamda,theta)=(xcen,ycen) at (lamda',theta')=(0,0),
//              where (lamda,theta) are usual spherical coord. and
//              (lamda',theta') are rotated spherical coord.
//              xcen = ycen = 0 for usual spherical coord.
//
//      Note: if the coordinates are geographic longitude(x),latitude(y)
//             set gridtype=2 and gs[6]= { 1.,1.,1.,1.,0.,0. }
//
//   Polar stereographic grid, gridtype=1,4:
//
//      gs[0] - x-position of north pole
//      gs[1] - y-position of north pole
//      gs[2] - number of grid distances between pole and equator
//      gs[3] - rotation angle of the grid (degrees)
//      gs[4] - projection latitude (degrees)
//              (60 degrees north for gridtype=1)
//      gs[5] - 0. (not used)
//
//   Mercator (unrotated) grid, gridtype=5:
//
//      gs[0] - western boundary (longitude for x=1) (degrees)
//      gs[1] - southern boundary (latitude for y=1) (degrees)
//      gs[2] - x (longitude) increment (km)
//      gs[3] - y (latitude)  increment (km)
//      gs[4] - reference (construction) latitude (degrees)
//      gs[5] - 0.  (not used)
//
//   Lambert (tangent,non-oblique) grid, gridtype=6:
//
//      gs[0] - western boundary (longitude for x=1,y=1) (degrees)
//      gs[1] - southern boundary (latitude for x=1,y=1) (degrees)
//      gs[2] - x (easting) increment (km)
//      gs[3] - y (northing) increment (km)
//      gs[4] - reference longitude (degrees)
//      gs[5] - reference (tangent) latitude (degrees)
//
//**********************************************************************


/**

  \brief Map Projection

  Specification of map projection
    - using proj4

*/


class Projection {

public:
  enum pType {
    undefined_projection = 0,
    polarstereographic_60= 1,
    geographic=            2,
    spherical_rotated=     3,
    polarstereographic=    4,
    mercator=              5,
    lambert=               6,
    generic=             100
  };
  enum { speclen= 6 };

private:
  int gridtype;
  bool defined;
  float gridspecstd[speclen]; ///> grid specification (fortran-indexed!)
  float gridspec[speclen];    ///> grid specification, origin moved 1 unit east/north


  bool usingLatLonValues;

  /// PROJ Specification
  std::string projDefinition;
  // Grid Resolution
  /// Grid Resolution in the X direction
  double gridResolutionX;
  /// Grid Resolution in the Y direction
  double gridResolutionY;
  double earthRadius;

  ///proj object of this projection
#if !defined(PROJECTS_H)
    typedef void PJ;
#endif

  boost::shared_ptr<PJ> projObject;
  // Constructor members
  void createProjDefinition(bool addDatum=true);
  // Copy members
  void memberCopy(const Projection& rhs);

  // conversion methods
  bool convert_from_grib(const std::map<std::string,std::string>& definitions, double resolutionX, double resolutionY);


private:
  // return grid type
  int Gridtype() const {return gridtype; }
  /*
    Fill array with C-style grid specification
    Used internally in Diana et al
  */
  void Gridspec(float gs[speclen]) const
       { for (int i=0; i<speclen; i++) gs[i]= gridspec[i]; }
  /*
    Fill array with Fortran-style grid specification
    Used when writing fields to file (MetnoFieldFile, etc)
  */
  void Gridspecstd(float gs[speclen]) const
       { for (int i=0; i<speclen; i++) gs[i]= gridspecstd[i]; }


public:
  // Constructors
  Projection();
  Projection(const std::string& projStr, const double projResX, const double projResY);
  Projection(const std::map<std::string,std::string>& definitions, double resolutionX, double resolutionY);
  Projection(const Projection &rhs);

  // Destructor
  ~Projection();

  // Operation Methods
  /// Convert Points to the Projection
  int convertPoints(const Projection& srcProj, int npos, float * x, float * y,
      bool silent = false) const;
  /// Convert Vectors to the Projection
  int convertVectors(const Projection& srcProj, int nvec, const float * to_x,
      const float * to_y, float * from_u, float * from_v) const;
  ///
  int calculateVectorRotationElements(const Projection& srcProj, int nvec,
          const float * to_x, const float * to_y, float * from_u,
          float * from_v) const;

  /// Get the Scale Factor on the X axis
  float getGridResolutionX() const;
  /// Get the Scale Factor on the Y axis
  float getGridResolutionY() const;

  // return grid type
  int metno_Gridtype() const {return gridtype; }

  /*
    Fill array with Fortran-style grid specification
    Used when writing fields to file (MetnoFieldFile, etc)
  */
  void metno_Gridspecstd(float gs[speclen]) const
       { for (int i=0; i<speclen; i++) gs[i]= gridspecstd[i]; }

  // Assignment operator
  Projection& operator=(const Projection &rhs);
  // Equality operator
  bool operator==(const Projection &rhs) const;
  // Inequality operator
  bool operator!=(const Projection &rhs) const;
  // ostream operator
  friend std::ostream& operator<<(std::ostream& output, const Projection& p);

  /// set mi grid-parameters
  void set_mi_gridspec(const int gt, const float gs[speclen],
      double& resolutionX, double& resolutionY, bool addDatum=true);

  /// set proj4 definitions
  bool set_proj_definition(const std::string& projStr, const double projResX=1.0, const double projResY=1.0);
  std::string getProjDefinition() const { return projDefinition;}

  /// return true if projection is defined
  bool isDefined() const {return defined;}

  /// set projection from string (gridtype:gridspec:gridspec....)
  bool setProjection(const std::string& projectionString,
      double& resolutionX, double& resolutionY);
  ///set area from setup/log string (name=name proj=gt grid=gridspec:gridspec.. )
  bool setProjectionFromLog(const std::string& logtext,
      double& resolutionX, double& resolutionY);
  /// get string (gridtype:gridspec:gridspec....)
  std::string toString(bool fortranStyle=true) const;
  /// get string (name=name proj=gt grid=gridspec:gridspec.. )
  std::string toLogString() const;

  /**
   * Bounding box of 2D data field. West boundary.
   * @return West boundary x-coordinate (degrees)
   */
  float boundaryWest() const;

  /**
   * Bounding box of 2D data field. South boundary.
   * @return South boundary y-coordinate (degrees)
   */
  float boundarySouth() const;

  /**
   * Step size in x-direction in degrees.
   */
  float dx() const;

  /**
   * Step size in y-direction in degrees.
   */
  float dy() const;

  /**
   * Check if position (geographic) is legal/non-singular in this projection
   */
  bool isLegal(float lon, float lat) const;

  /**
   * Check if projection p is equal to this projection with the exception of x_0 and y_0
   */
  bool isAlmostEqual(const Projection& p) const;

  /*
   * Convert point data to geographic values
   */
  int convertToGeographic(int n, float * tx, float * ty) const;
  int convertToGeographic(int n, float* x, float* y, const Projection& geoProj) const;

  /*
   * Convert geographic points to this projection
   */
  int convertFromGeographic(int n, float* x, float* y) const;
  int convertFromGeographic(int n, float* x, float* y, const Projection& geoProj) const;

  /**
   * get max legal jump-distance for a line on the map
   */
  float getMapLinesJumpLimit() const;

  /**
   * calculate geographic extension
   */
  bool calculateLatLonBoundingBox(const Rectangle & maprect,
      float & lonmin, float & lonmax, float & latmin, float & latmax) const;
  /**
   * return adjusted geographic extension
   */
  bool adjustedLatLonBoundingBox(const Rectangle & maprect,
      float & lonmin, float & lonmax, float & latmin, float & latmax) const;

  /**
   * Geogrid specific calculations
   */
  bool calculateGeogridParameters(float * xylim, float & lonmin,
      float & lonmax, float & latmin, float & latmax, float & jumplimit) const;

  /**
   *
   * filledMap specific
   */
  void filledMapCutparameters(bool & cutnorth, bool & cutsouth) const;

  void setUsingLatLonValues(bool latlon){usingLatLonValues = latlon;}
  bool getUsingLatLonValues() const {return usingLatLonValues;}

  /**
   * Return true if geographic projection
   */
  bool isGeographic() const;

  void setDefault();

  void setGeographic();

  bool useRotationElements() const {return true;}

  /// Calculate mapratios and coriolis factors
  int getMapRatios(int imaprc, int icoric, int nx, int ny,
      float* xmapr,float* ymapr, float* coriolis,
      float &dxgrid,float &dygrid) const;

  bool getLatLonIncrement(const float lat, const float lon, float& dlat, float& dlon) const;
};

#endif
