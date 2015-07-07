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
#ifndef diGridConverter_h
#define diGridConverter_h

#include "diArea.h"

#include "puTools/miRing.h"

/**
 \brief Array of points for grid conversion
 */
struct Points {
  Area area; ///< input (field) projection
  Area map_area; ///< output (map) projection
  Rectangle maprect; ///< current map rect.
  int npos; ///< number of points/vectors
  bool gridboxes; ///< gridpoints(false) gridbox corners(true)
  float *x; ///< position x
  float *y; ///< position y
  double gridResolutionX;
  double gridResolutionY;
  int ixmin, ixmax, iymin, iymax; ///< index range to cover current map rect.
  Points() :
    npos(0), gridboxes(false), x(0), y(0), ixmin(0), ixmax(0), iymin(0), iymax(0)
  {
  }
  ~Points()
  {
    delete[] x;
    delete[] y;
  }
  Points& operator=(const Points& rhs);
};

/**
 \brief Data on fields for grid conversion
 */
struct MapFields {
  Area area; ///< input (field) projection
  int npos; ///< number of points/vectors
  int imapr; ///< 0=no mapratio  1=mapratio  2,3..=mapratrio/(imapr-1)
  int icori; ///< 0=no coriolis  1=coriolis exists
  float *xmapr; ///< map ratio x-direction
  float *ymapr; ///< map ratio y-direction
  float *coriolis; ///< coriolis parameter
  float dxgrid; ///< gridresolution in x-dir. (m) where xmapr=1.
  float dygrid; ///< gridresolution in y-dir. (m) where ymapr=1.
  MapFields() :
    npos(0), imapr(0), icori(0), xmapr(NULL),ymapr(NULL),coriolis(NULL),
    dxgrid(0.0), dygrid(0.0)
  {
  }
  ~MapFields()
  {
    delete[] xmapr;
    delete[] ymapr;
    delete[] coriolis;
  }
  MapFields& operator=(const Points& rhs);
};

/**
 \brief Projection conversion of gridpositions

 The GridConverter manages caches of calculated gridvalues for different projections
 - single x/y values for grids
 - map ratio and/or coriolis parameter fields

 */
class GridConverter {
private:
  enum {
    defringsize = 10
  };
  enum {
    defringsize_angles = 10
  };
  enum {
    defringsize_mapfields = 4
  };

  /// get vector rotation elements from angle cache
  bool getVectorRotationElements(const Area& data_area, const Area& map_area,
      const int nvec, const float *x, const float *y, float ** cosx, float ** sinx);

public:
  GridConverter();
  GridConverter(const int s, const int smf);
  GridConverter(const int s);
  ~GridConverter();

  /// set size of cache for lists of x/y values
  void setBufferSize(const int);
  /// set size of cache for lists of rotation/angle values
  void setAngleBufferSize(const int);
  /// set size of cache for map fields data
  void setBufferSizeMapFields(const int);

  /// assume regular grid...keep calculations in ringbuffer
  bool getGridPoints(const Area&,
      const double& gridResolutionX,
      const double& gridResolutionY,
      const Area&, const Rectangle& maprect,
      bool gridboxes,
      int nx, int ny,
      float**, float**,
      int& ix1, int& ix2,
      int& iy1, int& iy2, bool use_cached = true);
  /// convert arbitrary set of points
  bool getPoints(const Area&, const Area&, int, float*, float*) const;
  /// convert arbitrary set of points
  bool getPoints(const Projection&, const Projection&, int, float*, float*) const;
  /// convert u,v vector coordinates for points x,y - obsolete syntax, to be removed
  bool getVectors(const Area&, const Area&, int,
      const float*, const float*, float*, float*);
  /// convert true north direction and velocity (dd,ff) to u,v vector coordinates for points x,y
  bool getDirectionVectors(const Area&, const bool, int,
      const float*, const float*, float*, float*);
  /// convert true north direction and velocity (dd,ff) to u,v vector coordinates for one point
  /// Specific point given by index
  bool getDirectionVector(const Area&, const bool, int,
      const float *, const float *, int index, float &, float &);
  /// convert from geo to xy
  bool geo2xy(const Area&, int, float*, float*);
  /// convert from xy to geo
  bool xy2geo(const Area&, int, float*, float*);
  /// convert geographical u,v vector coordinates for points x,y
  bool geov2xy(const Area&, int, const float*, const float*, float*, float*);
  /// convert xy vector coordinates for points x,y to geographical
  /// for an entire field
  bool xyv2geo(const Area&, int, int, float*, float*);

  /// map ratio and/or coriolis parameter fields
  bool getMapFields(const Area& area, const int imapr, const int icori,
      int nv, int ny, float**xmapr, float**ymapr, float**coriolis, float& dxgrid,
      float& dygrid);

private:
  ring<Points> *pointbuffer;
  ring<Points> *anglebuffer;
  ring<MapFields> *mapfieldsbuffer;
  float cosx_buffer;
  float sinx_buffer;
  Projection geo_proj;
};

#endif
