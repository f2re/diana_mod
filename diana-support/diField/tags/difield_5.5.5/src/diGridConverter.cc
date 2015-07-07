/*
  Diana - A Free Meteorological Visualisation Tool

  $Id: diGridConverter.cc 4730 2015-02-19 10:34:58Z alexanderb $

  Copyright (C) 2006 met.no

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "diGridConverter.h"

#include <cmath>

#define MILOGGER_CATEGORY "diField.GridConverter"
#include "miLogger/miLogging.h"

Points& Points::operator=(const Points& rhs)
{
  if (this != &rhs) {
    area = rhs.area;
    map_area = rhs.map_area;

    npos = rhs.npos;
    delete[] x;
    delete[] y;
    if (npos) {
      x = new float[npos];
      y = new float[npos];
      for (int i = 0; i < npos; i++) {
        x[i] = rhs.x[i];
        y[i] = rhs.y[i];
      }
    }
  }
  return *this;
}


GridConverter::GridConverter() :
  pointbuffer(0), anglebuffer(0), mapfieldsbuffer(0)
{
  geo_proj.setGeographic();
}

GridConverter::GridConverter(const int s, const int smf) :
  pointbuffer(0), anglebuffer(0), mapfieldsbuffer(0)
{
  geo_proj.setGeographic();
  setBufferSize(s);
  setAngleBufferSize(s);
  setBufferSizeMapFields(smf);
}

GridConverter::GridConverter(const int s) :
  pointbuffer(0), anglebuffer(0), mapfieldsbuffer(0)
{
  geo_proj.setGeographic();
  setBufferSize(s);
  setAngleBufferSize(s);
}

GridConverter::~GridConverter()
{
  delete pointbuffer;
  delete anglebuffer;
  delete mapfieldsbuffer;
}

void GridConverter::setBufferSize(const int s)
{
  delete pointbuffer;
  pointbuffer = new ring<Points> (s);
}

void GridConverter::setAngleBufferSize(const int s)
{
  delete anglebuffer;
  anglebuffer = new ring<Points> (s);
}

void GridConverter::setBufferSizeMapFields(const int s)
{
  delete mapfieldsbuffer;
  mapfieldsbuffer = new ring<MapFields> (s);
}

bool GridConverter::getGridPoints(const Area& area,
    const double& gridResolutionX,
    const double& gridResolutionY,
    const Area& map_area,
    const Rectangle& maprect, bool gridboxes, int nx, int ny, float**x, float**y,
    int& ix1, int& ix2, int& iy1, int& iy2, bool use_cached)

{

  if (!pointbuffer)
    setBufferSize(defringsize);
  // search buffer for existing gridpoint-set
  int n = pointbuffer->size();
  METLIBS_LOG_DEBUG("Ringbuffer's size is now: " << n);

  bool skiplimits = (maprect.width() == 0. || maprect.height() == 0.);

  bool found = false;
  int iy, ix, ipb=0;
  int i = -1;

  if (gridboxes) {
    nx++;
    ny++;
  }

  int npos = nx * ny;

  if (use_cached) {
    while (!found && i < n - 1) {
      i++;
      found = ((*pointbuffer)[i].area == area) &&
          ((*pointbuffer)[i].map_area.P() == map_area.P()) &&
          ((*pointbuffer)[i].npos == npos) &&
          ((*pointbuffer)[i].gridResolutionX == gridResolutionX) &&
    ((*pointbuffer)[i].gridResolutionY == gridResolutionY) &&
    ((*pointbuffer)[i].gridboxes == gridboxes);
    }

    if (found) {
      *x = (*pointbuffer)[i].x;
      *y = (*pointbuffer)[i].y;
      // check if correct map rectangle
      if (skiplimits) {
        ix1 = 0;
        ix2 = 0;
        iy1 = 0;
        iy2 = 0;
        return true;
      } else if ((*pointbuffer)[i].maprect == maprect) {
        ix1 = (*pointbuffer)[i].ixmin;
        ix2 = (*pointbuffer)[i].ixmax;
        iy1 = (*pointbuffer)[i].iymin;
        iy2 = (*pointbuffer)[i].iymax;
        return true;
      }
      ipb = i;
    }
  }


  const Projection& pa = area.P();
  const Projection& pr = map_area.P();

  if (!found) {
    METLIBS_LOG_DEBUG("++++++ Hmmm..need to calculate new gridpoints:");

    // calculate new gridpointcoordinates
    Points p;
    pointbuffer->push(p); // push a new points structure on the ring
    (*pointbuffer)[0].area = area;
    (*pointbuffer)[0].map_area = map_area;
    (*pointbuffer)[0].gridboxes = gridboxes;
    (*pointbuffer)[0].npos = npos;
    (*pointbuffer)[0].x = new float[npos];
    (*pointbuffer)[0].y = new float[npos];
    (*pointbuffer)[0].gridResolutionX = gridResolutionX;
    (*pointbuffer)[0].gridResolutionY = gridResolutionY;
    if (!gridboxes) {
      // gridpoints
      // init x,y arrays
      i = -1;
      for (iy = 0; iy < ny; iy++) {
        for (ix = 0; ix < (nx-1); ix++) {
          i++;
          (*pointbuffer)[0].x[i] = ix*gridResolutionX;
          (*pointbuffer)[0].y[i] = iy*gridResolutionY;
        }
//        //todo: Bug:  x[i]=nx-1 converts to x[i]=0 when transforming between geo-projections
        i++;
        (*pointbuffer)[0].x[i] = (nx-1.1)*gridResolutionX;
        (*pointbuffer)[0].y[i] = iy*gridResolutionY;
      }
    } else {
      // grid boxes
      i = -1;
      for (iy = 0; iy < ny; iy++)
        // init x,y arrays
        for (ix = 0; ix < nx; ix++) {
          i++;
          (*pointbuffer)[0].x[i] = (ix - 0.5)*gridResolutionX;
          (*pointbuffer)[0].y[i] = (iy - 0.5)*gridResolutionY;
        }
    }
    // Convert Points
    if (pr.convertPoints(pa, npos, (*pointbuffer)[0].x, (*pointbuffer)[0].y) != 0) {
      // error
      pointbuffer->pop();
      return false;
    }

    *x = (*pointbuffer)[0].x;
    *y = (*pointbuffer)[0].y;
    ipb = 0;

  }

  // find needed (field) area to cover map are
  int i1, i2, j1, j2;

  if (skiplimits) {

    i1 = 0;
    i2 = 0;
    j1 = 0;
    j2 = 0;

  } else {

    // check field corners
    i1 = nx;
    i2 = -1;
    j1 = ny;
    j2 = -1;
    int numinside = 0;
    for (iy = 0; iy < ny; iy += ny - 1) {
      for (ix = 0; ix < nx; ix += nx - 1) {
        i = iy * nx + ix;
        if (maprect.isinside((*pointbuffer)[ipb].x[i], (*pointbuffer)[ipb].y[i]))
          numinside++;
      }
    }
    if (numinside >= 3) {
      i1 = 0;
      i2 = nx - 1;
      j1 = 0;
      j2 = ny - 1;
    } else {
      for (iy = 0; iy < ny; iy++) {
        bool was_inside = maprect.isinside((*pointbuffer)[ipb].x[iy * nx], (*pointbuffer)[ipb].y[iy * nx]);
        int left = -1;
        int right = -1;
        
        for (ix = 1; ix < nx; ix++) {
          i = iy * nx + ix;

          bool inside = maprect.isinside((*pointbuffer)[ipb].x[i], (*pointbuffer)[ipb].y[i]);

          if (inside) {
            if (iy < j1)
              j1 = iy;
            if (iy > j2)
              j2 = iy;
          }
          if (!was_inside && inside)
            left = ix;
          if (was_inside && !inside)
            right = ix - 1;

          was_inside = inside;
        }
        // Handle the cases where the map and field areas overlap partially or completely.
        if (left == -1 && right != -1)
          left = 0;                     // left edges of the field and map areas may coincide
        if (left != -1 && right == -1)
          right = nx - 1;               // right edges of the field and map areas may coincide
        if (left == -1 && right == -1 && was_inside) {
          left = 0;                     // the whole span coincides with the map area
          right = nx - 1;
        }

        // Expand to the left.
        if (left != -1 && left < i1)
          i1 = left;
        // Expand to the right.
        if (right != -1 && right > i2)
          i2 = right;
      }

      i1-=2;
      i2+=2;
      j1-=2;
      j2+=2;
    }

    if (i1 < 0)
      i1 = 0;
    if (j1 < 0)
      j1 = 0;
    if (gridboxes) {
      if (i2 > nx - 2)
        i2 = nx - 2;
      if (j2 > ny - 2)
        j2 = ny - 2;
    } else {
    if (i2 > nx - 1)
      i2 = nx - 1;
    if (j2 > ny - 1)
      j2 = ny - 1;
    }
  }

  (*pointbuffer)[ipb].maprect = maprect;
  (*pointbuffer)[ipb].ixmin = i1;
  (*pointbuffer)[ipb].ixmax = i2;
  (*pointbuffer)[ipb].iymin = j1;
  (*pointbuffer)[ipb].iymax = j2;

  ix1 = i1;
  ix2 = i2;
  iy1 = j1;
  iy2 = j2;

  return true;
}

// convert set of points
bool
GridConverter::getPoints(const Projection& projection, const Projection& map_projection,
    int npos, float* x, float* y) const
{
  return map_projection.convertPoints(projection, npos, x, y, false) == 0;
}

// convert set of points - obsolete, use the above method
bool
GridConverter::getPoints(const Area& area, const Area& map_area,
    int npos, float* x, float* y) const
{
  return getPoints(area.P(), map_area.P(), npos, x, y);
}

// get arrays of vector rotation elements
bool GridConverter::getVectorRotationElements(const Area& data_area,
    const Area& map_area, const int nvec, const float *x, const float *y,
    float ** cosx, float ** sinx)
{
  if (!anglebuffer)
    setAngleBufferSize(defringsize_angles);

  // search buffer for existing point-set
  int n = anglebuffer->size();
  bool found = false;
  int i = -1;
  while (!found && i < n - 1) {
    i++;
    found = ((*anglebuffer)[i].area == data_area)
        && ((*anglebuffer)[i].map_area.P() == map_area.P()) && (nvec
        == (*anglebuffer)[i].npos);
  }

  if (found) {
    *cosx = (*anglebuffer)[i].x;
    *sinx = (*anglebuffer)[i].y;
  } else {
    // need to calculate new rotation elements
    Points p;
    anglebuffer->push(p); // push a new points structure on the ring
    (*anglebuffer)[0].area = data_area;
    (*anglebuffer)[0].map_area = map_area;
    (*anglebuffer)[0].npos = nvec;
    (*anglebuffer)[0].gridboxes = false;
    (*anglebuffer)[0].x = new float[nvec];
    (*anglebuffer)[0].y = new float[nvec];

    int ierror = map_area.P().calculateVectorRotationElements(data_area.P(),
        nvec, x, y, (*anglebuffer)[0].x, (*anglebuffer)[0].y);
    if (ierror) {
      METLIBS_LOG_ERROR("getVectorRotationElements error :" << ierror);
      anglebuffer->pop();
      return false;
    }
    *cosx = (*anglebuffer)[0].x;
    *sinx = (*anglebuffer)[0].y;
  }
  return true;
}

// convert u,v vector coordinates for points x,y
bool GridConverter::getVectors(const Area& data_area, const Area& map_area,
    int nvec, const float *x, const float *y, float* u, float* v)
{
  int ierror = 0;
  const Projection& pr = map_area.P();

  if (pr.useRotationElements()) {
    float undef = +1.e+35;

    float * cosx = 0;
    float * sinx = 0;
    if (!getVectorRotationElements(data_area, map_area, nvec, x, y, &cosx, &sinx))
      return false;

    for (int i = 0; i < nvec; ++i) {
      if (u[i] != undef && v[i] != undef) {
        float cu = u[i];
        if (cosx[i] == HUGE_VAL || sinx[i] == HUGE_VAL) {
          u[i] = undef;
          v[i] = undef;
        } else {
          u[i] = cosx[i] * cu - sinx[i] * v[i];
          v[i] = sinx[i] * cu + cosx[i] * v[i];
        }
      }
    }

  } else {

    ierror = pr.convertVectors(data_area.P(), nvec, x, y, u, v);

  }

  return (ierror == 0);
}

// convert true north direction and velocity (dd=u,ff=v)
// to u,v vector coordinates for points x,y
bool GridConverter::getDirectionVectors(const Area& map_area, const bool turn,
    int nvec, const float *x, const float *y, float *u, float *v)
{
  float undef= +1.e+35;
  Rectangle r;
  Area geo_area(geo_proj,r);

  // make vector-components (east/west and north/south) in geographic grid,
  // to be rotated to the map grid
  // u,v is dd,ff coming in
  float zturn = 1.;
  if (turn) zturn = -1.;
  float zpir = 2. * asinf(1.) / 180.;
  float dd, ff;
  for (int i=0; i<nvec; ++i) {
    if (u[i]!=undef && v[i]!=undef) {
      dd   = u[i] * zpir;
      ff   = v[i] * zturn;
      u[i] = ff * sinf(dd);
      v[i] = ff * cosf(dd);
    }
  }

  if (map_area.P().useRotationElements()) {

    float * cosx = 0;
    float * sinx = 0;
    if (!getVectorRotationElements(geo_area, map_area, nvec, x, y, &cosx, &sinx)) {
      return false;
    }

    for (int i = 0; i < nvec; ++i) {
      if (u[i]!=undef && v[i]!=undef) {
        float cu = u[i];
        if ( cosx[i] == HUGE_VAL || sinx[i] == HUGE_VAL) {
          u[i] = undef;
          v[i] = undef;
        } else {
          u[i] = cosx[i] * cu - sinx[i] * v[i];
          v[i] = sinx[i] * cu + cosx[i] * v[i];
        }
      }
    }

  } else {

    int ierror = map_area.P().convertVectors(geo_area.P(), nvec, x, y, u, v);
    return (ierror==0);

  }
  return true;
}

// convert true north direction and velocity (dd=u,ff=v)
// to u,v vector coordinates for one point
// Specific point given by index
bool GridConverter::getDirectionVector(const Area& map_area, const bool turn,
    int nvec, const float *x, const float *y, int index, float & u, float & v)
{
  if (index < 0 || index >= nvec)
    return false;
  float undef= +1.e+35;
  Rectangle r;
  Area geo_area(geo_proj,r);

  // make vector-components (east/west and north/south) in geographic grid,
  // to be rotated to the map grid
  // u,v is dd,ff coming in
  float zturn = 1.;
  if (turn)
    zturn = -1.;
  float zpir = 2. * asinf(1.) / 180.;
  if (u != undef && v != undef) {
    float dd = u * zpir;
    float ff = v * zturn;
    u = ff * sinf(dd);
    v = ff * cosf(dd);
  }

  if (map_area.P().useRotationElements()) {

    float * cosx = 0;
    float * sinx = 0;
    if (!getVectorRotationElements(geo_area, map_area, nvec, x, y, &cosx, &sinx)) {
      return false;
    }

    if (u!=undef && v!=undef) {
      float cu = u;
      if ( cosx[index] == HUGE_VAL || sinx[index] == HUGE_VAL) {
        u = undef;
        v = undef;
      } else {
        u = cosx[index] * cu - sinx[index] * v;
        v = sinx[index] * cu + cosx[index] * v;
      }
    }

  } else {

    int npos = 1;
    float px = x[index];
    float py = y[index];
    int ierror = map_area.P().convertVectors(geo_area.P(), npos, &px, &py, &u,
        &v);
    return (ierror==0);

  }
  return true;
}

bool GridConverter::geo2xy(const Area& area, int npos, float* x, float* y)
{
  return area.P().convertFromGeographic(npos, x, y) == 0;
}

bool GridConverter::xy2geo(const Area& area, int npos, float* x, float* y)
{
  return area.P().convertToGeographic(npos, x, y) == 0;
}

bool GridConverter::geov2xy(const Area& area, int npos,
    const float* x, const float* y, float *u, float *v)
{
  return area.P().convertVectors(geo_proj, npos, x, y, u, v) == 0;
}

// converting xv directions to geographic area
bool GridConverter::xyv2geo(const Area& area, int nx, int ny, float *u, float *v)
{
  int npos = nx * ny;

  // geographic projection - entire planet...
  Rectangle geor(-180, -90, 180, 90);
  Area geoarea(geo_proj, geor);

  // create entire grid for the model
  float *x = new float[npos];
  float *y = new float[npos];
  int i = 0;

  for (int yy = 0; yy < ny; yy++)
    for (int xx = 0; xx < nx; xx++) {
      i++;
      x[i] = xx;
      y[i] = yy;
    }

  // transform all model points to geographical grid

  bool ret = false;

  if (getPoints(area, geoarea, npos, x, y))
    if (getVectors(area, geoarea, npos, x, y, u, v)) // convertVectors
      ret = true;

  delete[] x;
  delete[] y;

  return ret;
}



bool GridConverter::getMapFields(const Area& area, const int imapr,
    const int icori, int nx, int ny, float**xmapr, float**ymapr, float**coriolis,
    float& dxgrid, float& dygrid)
{
  if (!mapfieldsbuffer)
    setBufferSizeMapFields(defringsize_mapfields);
  // search buffer for existing data
  int n = mapfieldsbuffer->size();
  METLIBS_LOG_DEBUG("++ Map.Ringbuffer's size is now: " << n);
  bool foundm = false, foundc = false;
  int i=0, ipbm, ipbc;

  int npos = nx * ny;

  if (imapr > 0) {
    i = 0;
    while (i < n && ((*mapfieldsbuffer)[i].area != area
        || (*mapfieldsbuffer)[i].imapr != imapr))
      i++;
    if (i < n) {
      ipbm = i;
      foundm = true;
    } else {
      ipbm = -1;
    }
  } else {
    foundm = true;
    ipbm = -2;
  }

  if (icori > 0) {
    i = 0;
    while (i < n && ((*mapfieldsbuffer)[i].area != area
        || (*mapfieldsbuffer)[i].icori != icori))
      i++;
    if (i < n) {
      ipbc = i;
      foundc = true;
    } else {
      ipbc = -1;
    }
  } else {
    foundc = true;
    ipbc = -2;
  }

  if (!foundm || !foundc) {


    if (ipbm >= 0 && ipbc == -1)
      ipbc = ipbm;
    else if (ipbc >= 0 && ipbm == -1)
      ipbm = ipbc;

    if (ipbm == -1 || ipbc == -1) {
      MapFields p;
      mapfieldsbuffer->push(p); // push a new MapFields structure on the ring
      (*mapfieldsbuffer)[0].area = area;
      (*mapfieldsbuffer)[0].npos = npos;
      (*mapfieldsbuffer)[0].imapr = 0;
      (*mapfieldsbuffer)[0].icori = 0;
      (*mapfieldsbuffer)[0].xmapr = 0;
      (*mapfieldsbuffer)[0].ymapr = 0;
      (*mapfieldsbuffer)[0].coriolis = 0;
      if (imapr > 0)
        ipbm = 0;
      if (icori > 0)
        ipbc = 0;
    }

    int imaprc = 0, icoric = 0;
    if (!foundm) {
      imaprc = imapr;
      i = ipbm;
      if (!(*mapfieldsbuffer)[i].xmapr)
        (*mapfieldsbuffer)[i].xmapr = new float[npos];
      if (!(*mapfieldsbuffer)[i].ymapr)
        (*mapfieldsbuffer)[i].ymapr = new float[npos];
    }
    if (!foundc) {
      icoric = icori;
      i = ipbc;
      if (!(*mapfieldsbuffer)[i].coriolis)
        (*mapfieldsbuffer)[i].coriolis = new float[npos];
    }

    int ierror = area.P().getMapRatios(imaprc, icoric, nx, ny,
        (*mapfieldsbuffer)[i].xmapr, (*mapfieldsbuffer)[i].ymapr,
        (*mapfieldsbuffer)[i].coriolis, dxgrid, dygrid);
    if (ierror) {
      METLIBS_LOG_ERROR("mapfield: " << ierror);
      return false;
    }

    if (imaprc > 0)
      (*mapfieldsbuffer)[i].imapr = imaprc;
    if (icoric > 0)
      (*mapfieldsbuffer)[i].icori = icoric;

  }

  if (imapr > 0) {
    npos = (*mapfieldsbuffer)[ipbm].npos;
    dxgrid = (*mapfieldsbuffer)[ipbm].dxgrid;
    dygrid = (*mapfieldsbuffer)[ipbm].dygrid;
    *xmapr = (*mapfieldsbuffer)[ipbm].xmapr;
    *ymapr = (*mapfieldsbuffer)[ipbm].ymapr;
  }

  if (icori > 0) {
    npos = (*mapfieldsbuffer)[ipbc].npos;
    dxgrid = (*mapfieldsbuffer)[ipbc].dxgrid;
    dygrid = (*mapfieldsbuffer)[ipbc].dygrid;
    *coriolis = (*mapfieldsbuffer)[ipbc].coriolis;
  }
  return true;
}
