/*
 Diana - A Free Meteorological Visualisation Tool

 Copyright (C) 2006-2013 met.no

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

//#define DEBUGPRINT

//#define DEBUG_PROJ

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "diProjection.h"

#include <puDatatypes/miCoordinates.h> // for earth radius
#include <puTools/miString.h>

#include <float.h>
#include <iostream>
#include <cmath>
#include <values.h>

#include <functional>
#include <stdexcept>

#define MILOGGER_CATEGORY "diField.Projection"
#include "miLogger/miLogging.h"

using namespace std;
using namespace miutil;

// Default constructor
Projection::Projection() :
  gridtype(undefined_projection), defined(false), usingLatLonValues(false), gridResolutionX(1.0),
  gridResolutionY(1.0), earthRadius(EARTH_RADIUS_M)
{
  for (int i = 0; i < speclen; i++) {
    gridspecstd[i] = 0.0;
    gridspec[i] = 0.0;
  }
}

Projection::Projection(const std::string& projStr, const double projResX,
    const double projResY) :
                          gridtype(undefined_projection), defined(false), usingLatLonValues(false), gridResolutionX(1.0),
                          gridResolutionY(1.0), earthRadius(EARTH_RADIUS_M)
{
  set_proj_definition(projStr, projResX, projResY);
}

// constructor from set of definitions following a specific grammar
Projection::Projection(const map<std::string, std::string>& definitions,
    double resolutionX, double  resolutionY) :
                          gridtype(undefined_projection), defined(false), usingLatLonValues(false), gridResolutionX(1.0),
                          gridResolutionY(1.0), earthRadius(EARTH_RADIUS_M)
{
  METLIBS_LOG_SCOPE();
  const std::map<std::string, std::string>::const_iterator it = definitions.find("grammar");
  if (it == definitions.end()) {
    METLIBS_LOG_ERROR("missing grammar");
    return;
  }

  if (it->second == "GribFile") {
    // GRIB
    if (!convert_from_grib(definitions, resolutionX, resolutionY)) {
      return;
    }
  } else {
    METLIBS_LOG_ERROR("unknown grammar: '" << it->second << "'");
  }
}

void Projection::createProjDefinition( bool addDatum )
{
  METLIBS_LOG_SCOPE();
  std::ostringstream ost;

  std::string str_earth;
  if ( addDatum ) {
    std::ostringstream ellipsoid;
    ellipsoid << " +ellps=WGS84 +towgs84=0,0,0 +no_defs";
    str_earth = ellipsoid.str();
  } else {
    str_earth = " +R=6371000";
  }

  Projection tmpProj;
  float x;
  float y;
  int npos;

  switch (gridtype) {
  case polarstereographic_60:
  case polarstereographic:
    ost << "+proj=stere";
    /*
     +proj=stere +lat_ts=Latitude at natural origin
     +lat_0=90
     +lon_0=Longitude at natural origin
     +k_0=Scale factor at natural origin (normally 1.0)
     +x_0=False Easting
     +y_0=False Northing
     */
    /*
     gridspec in mi
     g[0] - x-position of pole
     g[1] - y-position of pole
     g[2] - number of grid distances between pole and equator
     g[3] - rotation angle of the grid (degrees)
     g[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     g[5] - 0. (not used)
     */

    // Latitude of true scale
    ost << " +lat_ts=" << gridspec[4];
    // Latitude of origin
    ost << " +lat_0=" << (gridspec[4] > 0 ? "90.0" : "-90.0");
    // Longitude of origin
    ost << " +lon_0=" << gridspec[3];
    // Scale factor at origin
    //ost << " +k_0=" <<  (1.0 + sin(gridspec[4]*DEG_TO_RAD) + 1.0)/2.0;

    // Calculate grid resolution
    if (gridspec[2] > 0.0) {
      /*gridResolutionX = 150000.0 * 79.0 / static_cast<double> (gridspec[2]);*/
      gridResolutionX = (earthRadius * (1.0 + sin(fabs(gridspec[4]) * DEG_TO_RAD))) / static_cast<double> (gridspec[2]);
      gridResolutionY = gridResolutionX;
    } else {
      gridResolutionX = 1.0;
      gridResolutionY = 1.0;
    }
    // False Easting
    ost << " +x_0=" << (gridResolutionX * (gridspec[0]-1));
    // False Northing
    ost << " +y_0=" << (gridResolutionY * (gridspec[1]-1));
    ost << str_earth;
    projDefinition = ost.str();
    break;
  case geographic: // Geographic
    /*
     g(1) - western boundary (degrees)
     g(2) - southern boundary (degrees)
     g(3) - longitude increment (degrees)
     g(4) - latitude increment (degrees)
     g(5) - 0
     g(6) - 0
     */
    if ((gridspec[0] == 1.0) && (gridspec[1] == 1.0) && (gridspec[2] == 1.0)
        && (gridspec[3] == 1.0)) {
      ost << "+proj=latlong";
      // Calculate grid resolution
      gridResolutionX = DEG_TO_RAD;
      gridResolutionY = DEG_TO_RAD;
    } else {
      //The latitude_longitude projection ignores x_0 and y_0,
      //the following projection is equal to +proj=latlong but does not ignore x_0 and y_0
      ost << "+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=90";
      // Calculate grid resolution
      gridResolutionX = gridspec[2] * DEG_TO_RAD;
      gridResolutionY = gridspec[3] * DEG_TO_RAD;
      // False Easting
      ost << " +x_0=" << (gridspec[0] * DEG_TO_RAD * -1.);
      // False Northing
      ost << " +y_0=" << (gridspec[1] * DEG_TO_RAD * -1.);
    }
    ost << str_earth;
    projDefinition = ost.str();
    break;
  case spherical_rotated:
    /*
     gridspec in mi
     g[0] - western boundary (degrees)
     g[1] - southern boundary (degrees)
     g[2] - longitude increment (degrees)
     g[3] - latitude increment (degrees)
     g[4] - xcen: longitude position of rotated equator (degrees)
     g[5] - ycen: latitude  position of rotated equator (degrees)
     (lamda,theta)=(xcen,ycen) at (lamda',theta')=(0,0),
     where (lamda,theta) are usual spherical coord. and
     (lamda',theta') are rotated spherical coord.
     xcen = ycen = 0 for usual spherical coord.
     */
    // ob_tran: General Oblique Transformation
    ost << "+proj=ob_tran +o_proj=longlat";
    // Longitude Position of Rotated Pole
    ost << " +lon_0=" << gridspec[4];
    // Latitude Position of Rotated Pole
    ost << " +o_lat_p=" << (90.0 - gridspec[5]);
    // Calculate grid resolution
    gridResolutionX = gridspec[2] * DEG_TO_RAD ;
    gridResolutionY = gridspec[3] * DEG_TO_RAD ;
    // False Easting
    ost << " +x_0=" << (gridspec[0] * DEG_TO_RAD  * -1.);
    // False Northing
    ost << " +y_0=" << (gridspec[1] * DEG_TO_RAD  * -1.);
    ost << str_earth;
    projDefinition = ost.str();
    break;
  case mercator:
    /*
     g(0) - western boundary (longitude for x=1) (degrees)
     g(1) - southern boundary (latitude for y=1) (degrees)
     g(2) - x (longitude) increment (km)
     g(3) - y (latitude)  increment (km)
     g(4) - reference (construction) latitude (degrees)
     g(5) - 0.  (not used)
     */
    ost << "+proj=merc";
    // Latitude of True Scale
    ost << " +lat_ts=" << gridspec[4];
    // Calculate grid resolution
    gridResolutionX = 1000.0 * gridspec[2];
    gridResolutionY = 1000.0 * gridspec[3];
    // Spherical Earth
    ost << str_earth;
    tmpProj.set_proj_definition(ost.str(), 1, 1);
    x = gridspec[0];
    y = gridspec[1];
    npos = 1;
    tmpProj.convertFromGeographic(npos, &x, &y);
    // False Easting
    ost << " +x_0=" << -x;
    // False Northing
    ost << " +y_0=" << -y;
    projDefinition = ost.str();
    break;
  case lambert:
    /*
     g(0) - west (longitude for x=1,y=1) (degrees)
     g(1) - south (latitude for x=1,y=1) (degrees)
     g(2) - x (easting) increment (km)
     g(3) - y (northing) increment (km)
     g(4) - reference longitude (degrees)
     g(5) - reference latitude (cone tangent) (degrees)
     */
    ost << "+proj=lcc"; // Lambert Conformal Conic Alternative
    // Latitude of origin
    ost << " +lat_0=" << gridspec[5];
    ost << " +lat_1=" << gridspec[5];
    ost << " +lat_2=" << gridspec[5];
    // Longitude of origin
    ost << " +lon_0=" << gridspec[4];
    // Calculate grid resolution
    gridResolutionX = 1000.0 * gridspec[2];
    gridResolutionY = 1000.0 * gridspec[3];
    // Spherical Earth
    ost << str_earth;
    // trick to reveal the false northern/eastern
    tmpProj.set_proj_definition(ost.str(), 1, 1);
    x = gridspec[0];
    y = gridspec[1];
    npos = 1;
    tmpProj.convertFromGeographic(npos, &x, &y);
    // False Easting
    ost << " +x_0=" << -x;
    // False Northing
    ost << " +y_0=" << -y;
    projDefinition = ost.str();
    break;
  default:
    projDefinition = "";
  }
  if (gridResolutionX == 0.0)
    gridResolutionX = 1.0;
  if (gridResolutionY == 0.0)
    gridResolutionY = 1.0;
  if (gridtype!=undefined_projection) {
    METLIBS_LOG_DEBUG("GridDef: " << gridtype << " - " << gridspec[0] << ","
        << gridspec[1] << "," << gridspec[2] << "," << gridspec[3] << ","
        << gridspec[4] << "," << gridspec[5] << "; ProjDef: " << projDefinition
        << "; GridRes: " << gridResolutionX << "  -  " << gridResolutionY);
  }
  projObject = boost::shared_ptr<PJ>(pj_init_plus(projDefinition.c_str()), pj_free);

}

// set mi grid-parameters
void Projection::set_mi_gridspec(const int gt, const float gs[speclen],
    double& resolutionX, double& resolutionY, bool addDatum)
{

  gridtype = gt;

  /*
   Fix gridspec from fortran indexing (from 1) to C/C++ indexing (from 0).
   To be used in fortran routines xyconvert and uvconvert,
   gridspecstd must be used in fortran routine mapfield
   and when writing a field to a field file.
   */
  for (int i = 0; i < speclen; i++) {
    gridspecstd[i] = gs[i];
    gridspec[i] = gs[i];
  }

  /*
   projection type 1 : polarstereographic at 60 degrees north !!!
   (mapareas defined in setup without setting true latitude to 60.,
   no problem for fields when using gridpar)
   */
  if (gt == 1) {
    gridspecstd[4] = 60.;
    gridspec[4] = 60.;
  }

  createProjDefinition( addDatum );

  if (gridtype != undefined_projection) {
    defined = true;
  }

  resolutionX = gridResolutionX;
  resolutionY = gridResolutionY;

}

bool Projection::set_proj_definition(const std::string& projStr,
    const double projResX, const double projResY)
{
  // initialize LibMI grid definition
  gridtype = generic;
  for (int i = 0; i < speclen; i++) {
    gridspecstd[i] = 0.0;
    gridspec[i] = 0.0;
  }
  // Set PROJ definition
  projDefinition = projStr;
  gridResolutionX = projResX;
  gridResolutionY = projResY;

  miutil::replace(projDefinition, "\"", "");
  defined = true;
  projObject = boost::shared_ptr<PJ>(pj_init_plus(projDefinition.c_str()), pj_free);

  if (projObject == NULL) {
    gridtype = 0;
    defined = false;
  }

  return defined;

}

// Copy constructor
Projection::Projection(const Projection &rhs)
{
  // elementwise copy
  memberCopy(rhs);
  // just duplicate the projObject
  projObject = boost::shared_ptr<PJ>(pj_init_plus(rhs.projDefinition.c_str()), pj_free);
  projDefinition = rhs.projDefinition;
}

// Destructor
Projection::~Projection()
{
}

// Assignment operator
Projection& Projection::operator=(const Projection &rhs)
{
  if (this == &rhs)
    return *this;

  // elementwise copy
  memberCopy(rhs);
  // a projObject may have been created before
  projObject = boost::shared_ptr<PJ>(pj_init_plus(rhs.projDefinition.c_str()), pj_free);
  projDefinition = rhs.projDefinition;
  return *this;
}

// Equality operator
bool Projection::operator==(const Projection &rhs) const
{
  return ((projDefinition     == rhs.projDefinition)
      && (fabs(gridResolutionX - rhs.gridResolutionX) < 0.0001)
      && (fabs(gridResolutionY - rhs.gridResolutionY) < 0.0001)
      && (fabs(earthRadius     - rhs.earthRadius)     < 0.0001));
}

// Inequality operator
bool Projection::operator!=(const Projection &rhs) const
{
  return !(*this == rhs);
}

ostream& operator<<(ostream& output, const Projection& p)
{
  //   output << " gridtype: " << p.gridtype << " gridspec";
  //   for (int i = 0; i < Projection::speclen; i++)
  //     output << " : " << p.gridspec[i];
  output << " proj4string=\"" << p.projDefinition<<"\"";
  //   output << " gridResolutionX: " << p.gridResolutionX;
  //   output << " gridResolutionY: " << p.gridResolutionY;

  return output;
}

void Projection::memberCopy(const Projection& rhs)
{
  METLIBS_LOG_SCOPE(LOGVAL(rhs));

  gridtype = rhs.gridtype;
  for (int i = 0; i < speclen; i++)
    gridspecstd[i] = rhs.gridspecstd[i];
  for (int i = 0; i < speclen; i++)
    gridspec[i] = rhs.gridspec[i];
  gridResolutionX = rhs.gridResolutionX;
  gridResolutionY = rhs.gridResolutionY;
  earthRadius = rhs.earthRadius;
  defined = rhs.defined;
  // projObject
  // This is a pointer !
  // so it must be duplicated.
  // Unfortunately, ew can not do this here,
  // for it depends if memberCopy is called from
  // the copy constructor or the assignment operator

  usingLatLonValues = rhs.usingLatLonValues;
}

int Projection::convertPoints(const Projection& srcProj, int npos, float * x,
    float * y, bool silent) const
{
  METLIBS_LOG_SCOPE();
  if (!srcProj.projObject) {
    METLIBS_LOG_ERROR("projPJ not initialized, definition=" << srcProj);
    return -1;
  }

  if (!projObject) {
    METLIBS_LOG_ERROR("projPJ not initialized, definition=" << *this);
    return -1;
  }

  double * xd = new double[npos];
  double * yd = new double[npos];
  double * zd = 0;

  for (int i = 0; i < npos; i++) {
    xd[i] = x[i];
    yd[i] = y[i];
  }
  // Transformation
  int ret = pj_transform(srcProj.projObject.get(), projObject.get(), npos, 1, xd, yd, zd);
  if (ret != 0 && ret !=-20) {
    //ret=-20 :"tolerance condition error"
    //ret=-14 : "latitude or longitude exceeded limits"
    if (!silent && ret != -14) {
      METLIBS_LOG_ERROR("error in pj_transform = " << pj_strerrno(ret) << "  " << ret);
    }
    delete[] xd;
    delete[] yd;
    return -1;
  }

  for (int i = 0; i < npos; i++) {
    x[i] = static_cast<float> (xd[i]);
    y[i] = static_cast<float> (yd[i] );
  }

  // End convertPoints - clean up
  delete[] xd;
  delete[] yd;

  return 0;
}

// Support Functions for convertVector
namespace {

struct uv {
  float u;
  float v;

  uv(float u_, float v_) :
    u(u_), v(v_)
  {
  }

  explicit uv(float angle) :
    u(std::sin(angle * DEG_TO_RAD)), v(std::cos(angle * DEG_TO_RAD))
  {
  }

  float angle() const
  {
    if (0 == u and 0 == v) {
      return HUGE_VAL;
    }

    if (0 == v) {
      if (0 < u)
        return 90;
      else
        return 270;
    }

    float ret = std::atan(u / v) * RAD_TO_DEG;
    if (v < 0)
      ret += 180;

    if (ret < 0)
      ret += 360;
    if (360 < ret)
      ret -= 360;

    return ret;
  }
};

/// Get the direction to north from point x, y
float * north(Projection p, int nvec, const float * x, const float * y)
{
  float* north_x = new float[nvec];
  float* north_y = new float[nvec];
  std::copy(x, x + nvec, north_x);
  std::copy(y, y + nvec, north_y);

  p.convertToGeographic(nvec, north_x, north_y);

  for (int i = 0; i < nvec; i++){
    north_y[i] = std::min<float>(north_y[i] + 0.1, 90);
  }
  p.convertFromGeographic(nvec, north_x, north_y);
  float * ret = new float[nvec];

  for (int i = 0; i < nvec; i++){
    ret[i] = uv(north_x[i] - x[i], north_y[i] - y[i]).angle();
  }
  delete[] north_x;
  delete[] north_y;
  return ret;
}

float turn(float angle_a, float angle_b)
{
  float angle = angle_a + angle_b;
  if (angle >= 360)
    angle -= 360;
  if (angle < 0)
    angle += 360;
  return angle;
}

}

int Projection::convertVectors(const Projection& srcProj, int nvec,
    const float * to_x,  const float * to_y, float * u, float * v) const
{

  int ierror = 0;
  float udef= +1.e+35;

  float * from_x = new float[nvec];
  float * from_y = new float[nvec];

  std::copy(to_x, to_x + nvec, from_x);
  std::copy(to_y, to_y + nvec, from_y);

  try {
    srcProj.convertPoints(*this, nvec, from_x, from_y); // convert back to old projection
    float * from_north = north(srcProj, nvec, from_x, from_y); // degrees
    float * to_north = north(*this, nvec, to_x, to_y); // degrees

    for (int i = 0; i < nvec; ++i) {
      if(u[i] != udef && v[i] != udef ) {
        const float length = std::sqrt(u[i]*u[i] + v[i]*v[i]);

        // the difference between angles in the two projections:
        float angle_diff = to_north[i] - from_north[i];

        float new_direction = turn(uv(u[i], v[i]).angle(), angle_diff);
        // float new_direction = to_north[ i ]; // This makes all directions be north.
        uv convert(new_direction);
        u[i] = convert.u * length;
        v[i] = convert.v * length;
      }
    }
    delete[] from_north;
    delete[] to_north;
  } catch (std::exception & e) {
    METLIBS_LOG_ERROR("exception in convertVectors:" << e.what());
    ierror = 1;
  }

  delete[] from_x;
  delete[] from_y;

  return ierror;
}

int Projection::calculateVectorRotationElements(const Projection& srcProj,
    int nvec, const float * to_x, const float * to_y, float * cosa,
    float * sina) const
{
  int err = 0;

  float * from_x = new float[nvec];
  float * from_y = new float[nvec];

  std::copy(to_x, to_x + nvec, from_x);
  std::copy(to_y, to_y + nvec, from_y);

  err = srcProj.convertPoints(*this, nvec, from_x, from_y); // convert back to old projection
  if(err!=0){
    return err;
  }
  float * from_north = north(srcProj, nvec, from_x, from_y); // degrees
  float * to_north = north(*this, nvec, to_x, to_y); // degrees

  for (int i = 0; i < nvec; ++i) {
    // the difference between angles in the two projections:
    if (from_north[i] == HUGE_VAL || to_north[i] == HUGE_VAL) {
      cosa[i] = HUGE_VAL;
      sina[i] = HUGE_VAL;
    } else {
      double angle_diff = from_north[i] - to_north[i];
      // return cos() and sin() of this angle
      cosa[i] = std::cos(angle_diff * DEG_TO_RAD);
      sina[i] = std::sin(angle_diff * DEG_TO_RAD);
    }
  }
  delete[] from_north;
  delete[] to_north;
  delete[] from_x;
  delete[] from_y;

  return err;
}

float Projection::getGridResolutionX() const
{
  return gridResolutionX;
}

float Projection::getGridResolutionY() const
{
  return gridResolutionY;
}

namespace /* anonymous */ {
std::string find_in_map(const map<std::string, std::string>& m, const std::string& k)
{
  const std::map<std::string, std::string>::const_iterator it = m.find(k);
  if (it == m.end())
    return "";
  return it->second;
}
double double_in_map(const map<std::string, std::string>& m, const std::string& k, double dflt)
{
  const std::map<std::string, std::string>::const_iterator it = m.find(k);
  if (it == m.end())
    return dflt;
  return miutil::to_double(it->second);
}
} // namespace anonymous

bool Projection::convert_from_grib(const map<std::string, std::string>& definitions,
    double resolutionX, double resolutionY)
{
  METLIBS_LOG_SCOPE();

  int gt = undefined_projection;
  float gs[speclen];
  for (unsigned int i = 0; i < speclen; i++)
    gs[i] = 0.0;

#ifdef DEBUGPRINT
  {
    std::ostringstream o;
    for(std::map<std::string,std::string>::const_iterator it=definitions.begin(); it!=definitions.end();it++)
      o << itr->first << " => " << itr->second << "; ";
    METLIBS_LOG_DEBUG(o.str());
  }
#endif

  // NetCDF CF-standard
  const std::string name = find_in_map(definitions, "grid_mapping_name");
  if (name.empty()) {
    METLIBS_LOG_ERROR("missing grid_mapping_name");
    return false;
  }

  /*
   Supported projections:
   NetCDF                        = libmi
   -----------------------------------------------------
   1. polarstereographic         = polar_stereographic
   2. stereographic              = polar_stereographic ???
   3. geographic                 = geographic
   4. rotated_latitude_longitude = spherical_rotated
   5. mercator                   = mercator
   */

  if (name != "polar_stereographic" && name != "stereographic" &&
      name != "lambert" && name != "mercator" &&
      name != "rotated_latitude_longitude" && name != "geographic")
  {
    METLIBS_LOG_ERROR("grid_mapping:" << name << " not supported");
    return false;
  }

  // Set earth radius if set...
  earthRadius = double_in_map(definitions, "earth_radius", 0.0);

  // fetch various attributes
  float false_easting = double_in_map(definitions, "false_easting", 0.0);
  float false_northing = double_in_map(definitions, "false_northing", 0.0);
  float grid_north_pole_latitude = double_in_map(definitions, "grid_north_pole_latitude", 90.0);
  float grid_north_pole_longitude = double_in_map(definitions, "grid_north_pole_longitude", 0.0);
  float latitude_of_projection_origin = double_in_map(definitions, "latitude_of_projection_origin", 0.0);
  float longitude_of_projection_origin = double_in_map(definitions, "longitude_of_projection_origin", 0.0);
  /*
   float longitude_of_central_meridian =
   ( definitions.count("longitude_of_central_meridian") == 0 ?
   0.0 : definitions["longitude_of_central_meridian"].toFloat() );
   float north_pole_grid_longitude =
   ( definitions.count("north_pole_grid_longitude") == 0 ?
   0.0 : definitions["north_pole_grid_longitude"].toFloat() );
   float scale_factor_at_central_meridian =
   ( definitions.count("scale_factor_at_central_meridian") == 0 ?
   1.0 : definitions["scale_factor_at_central_meridian"].toFloat() );
   */
  float scale_factor_at_projection_origin = double_in_map(definitions, "scale_factor_at_projection_origin", 1.0);
  float standard_parallel = double_in_map(definitions, "standard_parallel", 0.0);
  float straight_vertical_longitude_from_pole = double_in_map(definitions, "straight_vertical_longitude_from_pole", 0.0);

  // the x-spacing of the grid
  float grid_resolution_x = double_in_map(definitions, "grid_resolution_x", 1.0); // in arbitrary unit
  // the y-spacing of the grid
  float grid_resolution_y = double_in_map(definitions, "grid_resolution_y", 1.0); // in arbitrary unit
  // the x_value of the first datapoint
  float start_x = double_in_map(definitions, "start_x", 0);
  // the y_value of the first datapoint
  float start_y = double_in_map(definitions, "start_y", 0);
  // scale from km
  float coordinate_scale_factor = double_in_map(definitions, "coordinate_scale_factor", 1.0);

  const float earth_radius = earthRadius / 1000.0;
  //   const float earth_circumference = earth_radius * 2 * pi;
  //   const float pole_to_equator = earth_circumference / 4.0;
  //   const float pole_to_equator = (150000.0 * 79.0)/1000;

  // treat the different projection types
  if (name == "polar_stereographic") {
    /*
     ===========================================
     polar stereographic
     ===========================================
     */
    /*
     supported attributes in NetCDF:
     - straight_vertical_longitude_from_pole ( -180.0 <= real < 180.0 )
     - latitude_of_projection_origin         ( real = +90.0 | -90.0   )
     either:
     - standard_parallel                   (  -90.0 <= real <  90.0 )
     or:
     - scale_factor_at_projection_origin   ( real >= 0.0            )
     - false_easting                         ( real )
     - false_northing                        ( real )
     */

    /*
     gridspec in mi
     gs[0] - x-position of pole
     gs[1] - y-position of pole
     gs[2] - number of grid distances between pole and equator
     gs[3] - rotation angle of the grid (degrees)
     gs[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     gs[5] - 0. (not used)
     */

    if (latitude_of_projection_origin != 90.0 && latitude_of_projection_origin != -90.0) {
      METLIBS_LOG_ERROR("latitude_of_projection_origin!=[-90|90]");
      return false;
    }
    // determine latitude where scale is true
    float latitude_of_true_scale;
    if (definitions.count("standard_parallel") != 0) {
      latitude_of_true_scale = standard_parallel;
    } else if (definitions.count("scale_factor_at_projection_origin") != 0) {
      if (scale_factor_at_projection_origin > 1.0 || scale_factor_at_projection_origin < -1.0) {
        METLIBS_LOG_ERROR("bad scale_factor_at_projection_origin:"
            << scale_factor_at_projection_origin);
        return false;
      }
      /*
       Calculate standard_parallel from scale factor 1):
       scale_factor_at_origin = (1 + sin(standard_parallel))/2
       ==>
       standard_parallel = asin(2*scale_factor_at_origin - 1)

       */
      latitude_of_true_scale = asin(2 * scale_factor_at_projection_origin - 1.0) * RAD_TO_DEG;

      /*
       Calculate standard_parallel from scale factor 2):
       scale_factor_at_origin = cos2(45-standard_parallel/2)

       R1    = Diameter of earth, R2 = R1 - (height from standard parallel to pole)
       Scale factor equals to R2/R1 = cos2(beta)
       beta  = angle from opposite pole to standard parallel = 45 - standard_parallel/2

       */
      //       float beta = acosf( sqrtf( scale_factor_at_projection_origin ) ) * RAD_TO_DEG;
      //       latitude_of_true_scale = 90 - 2*beta;

      METLIBS_LOG_DEBUG("calculated latitude_of_true_scale=" << latitude_of_true_scale
          << " from scale_factor_at_projection_origin=" << scale_factor_at_projection_origin);
    } else {
      METLIBS_LOG_ERROR("use either standard_parallel or scale_factor_at_projection_origin");
      return false;
    }

    //int ysign = ( grid_resolution_y < 0.0 ? -1 : 1 );

    // x-position of pole in grid-coordinates
    gs[0] = -(false_easting + start_x) / grid_resolution_x;
    // y-position of pole in grid-coordinates
    gs[1] = -(false_northing + start_y) / grid_resolution_y;
    // number of grid distances between pole and equator
    gs[2] = (earth_radius * (1 + sin(latitude_of_true_scale * DEG_TO_RAD))) / ((grid_resolution_y) * coordinate_scale_factor);
    //     gs[2] = (150000.0 * 79.0)/grid_resolution_x;
    // rotation angle of the grid
    gs[3] = straight_vertical_longitude_from_pole;
    // Latitude of true scale
    gs[4] = latitude_of_true_scale;
    // not used
    gs[5] = 0.0;

#ifdef DEBUGPRINT
    METLIBS_LOG_DEBUG("values for polar_stereographic grid: "
        << "false_easting:" << false_easting << "; "
        << "false_northing:" << false_northing << "; "
        << "start_x:" << start_x << "; "
        << "start_y:" << start_y << "; "
        << "grid_resolution_x:" << grid_resolution_x << "; "
        << "grid_resolution_y:" << grid_resolution_y << "; "
        << "coordinate_scale_factor:" << coordinate_scale_factor << "; "
        << "latitude_of_true_scale:" << latitude_of_true_scale << "; "
        << "straight_vertical_longitude_from_pole:" << straight_vertical_longitude_from_pole);
#endif

    // finally, set gridtype
    gt = polarstereographic;

  } else if (name == "stereographic") {
    /*
     ===================================================
     stereographic,
     with the constraint that projection origin = [+90|-90]
     ===================================================
     */

    /*
     supported attributes in NetCDF:
     - longitude_of_projection_origin        ( -180.0 <= real < 180.0  )
     - latitude_of_projection_origin         (  -90.0 <= real <  90.0  )
     - scale_factor_at_projection_origin     ( real >= 0.0             )
     - false_easting                         ( real )
     - false_northing                        ( real )
     */

    /*
     gridspec in mi
     gs[0] - x-position of north pole
     gs[1] - y-position of north pole
     gs[2] - number of grid distances between pole and equator
     gs[3] - rotation angle of the grid (degrees)
     gs[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     gs[5] - 0. (not used)
     */

    if (latitude_of_projection_origin != 90.0 && latitude_of_projection_origin
        != -90.0) {
      METLIBS_LOG_ERROR("latitude_of_projection_origin!=[-90|90]");
      return false;
    }

    float latitude_of_true_scale;
    if (definitions.count("scale_factor_at_projection_origin") != 0) {
      if (scale_factor_at_projection_origin > 1.0
          || scale_factor_at_projection_origin < -1.0) {
        METLIBS_LOG_ERROR("bad scale_factor_at_projection_origin:"
            << scale_factor_at_projection_origin);
        return false;
      }
      /*
       Calculate standard_parallel from scale factor:
       scale_factor_at_origin = (1 + sin(standard_parallel))/2
       ==>
       standard_parallel = asin(2*scale_factor_at_origin - 1)
       */
      latitude_of_true_scale = asin(2 * scale_factor_at_projection_origin - 1.0) * RAD_TO_DEG;
      METLIBS_LOG_DEBUG("calculated latitude_of_true_scale=" << latitude_of_true_scale
          << " from scale_factor_at_projection_origin=" << scale_factor_at_projection_origin);
    } else {
      METLIBS_LOG_ERROR("missing scale_factor_at_projection_origin");
      return false;
    }

    // x-position of pole in grid-coordinates
    gs[0] = -(false_easting + start_x) / grid_resolution_x;
    // y-position of pole in grid-coordinates
    gs[1] = -(false_northing + start_y) / grid_resolution_y;
    // number of grid distances between pole and equator
    gs[2] = (earth_radius * (1 + sin(latitude_of_true_scale * DEG_TO_RAD)))
                                / (grid_resolution_y * coordinate_scale_factor);
    //     gs[2] = pole_to_equator / (grid_resolution_y*coordinate_scale_factor);
    // rotation angle of the grid
    gs[3] = longitude_of_projection_origin;
    // Latitude of true scale
    gs[4] = latitude_of_true_scale;
    // not used
    gs[5] = 0.0;

    // finally, set gridtype
    gt = polarstereographic;

  } else if (name == "geographic") {
    /*
     ===========================================
     geographic - straight lat/lon
     ===========================================
     */
    /*
     gridspec in mi
     gs[0] - west
     gs[1] - south
     gs[2] - lon_inc (deg)
     gs[3] - lat_inc (deg)
     gs[4] - 0
     gs[5] - 0
     */

    // west origin
    gs[0] = (false_easting + start_x);
    // south origin
    gs[1] = (false_northing + start_y);
    // longitude increment (deg)
    gs[2] = grid_resolution_x;
    // latitude increment (deg)
    gs[3] = grid_resolution_y;
    // not in use
    gs[4] = 0.0;
    // not in use
    gs[5] = 0.0;

    // finally, set gridtype
    gt = geographic;

  } else if (name == "rotated_latitude_longitude") {
    /*
     ===========================================
     spherical rotated
     ===========================================
     */

    /*
     supported attributes in NetCDF:
     - grid_north_pole_latitude
     - grid_north_pole_longitude
     - north_pole_grid_longitude (optional, default 0)
     */

    /*
     gridspec in mi
     gs[0] - western boundary (degrees)
     gs[1] - southern boundary (degrees)
     gs[2] - longitude increment (degrees)
     gs[3] - latitude increment (degrees)
     gs[4] - xcen: longitude position of rotated equator (degrees)
     gs[5] - ycen: latitude  position of rotated equator (degrees)
     (lamda,theta)=(xcen,ycen) at (lamda',theta')=(0,0),
     where (lamda,theta) are usual spherical coord. and
     (lamda',theta') are rotated spherical coord.
     xcen = ycen = 0 for usual spherical coord.
     */

    // west origin
    gs[0] = (false_easting + start_x);
    // south origin
    gs[1] = (false_northing + start_y);
    // longitude increment (deg)
    gs[2] = grid_resolution_x;
    // latitude increment (deg)
    gs[3] = grid_resolution_y;
    // longitude position of rotated equator (deg)
    gs[4] = grid_north_pole_longitude;
    // latitude  position of rotated equator (deg)
    gs[5] = 90 - grid_north_pole_latitude;

    // finally, set gridtype
    gt = spherical_rotated;

  } else if (name == "lambert") {
    /*
     * gridspec in mi
     g(0) - west (longitude for x=1,y=1) (degrees)
     g(1) - south (latitude for x=1,y=1) (degrees)
     g(2) - x (easting) increment (km)
     g(3) - y (northing) increment (km)
     g(4) - reference longitude (degrees)
     g(5) - reference latitude , first and second parallel
     */

    // west (longitude for x=1,y=1) (degrees)
    gs[0] = false_easting + start_x;
    // south (latitude for x=1,y=1) (degrees)
    gs[1] = false_northing + start_y;
    //  x (easting) increment (km)
    gs[2] = grid_resolution_x * coordinate_scale_factor; // in km
    //  y (northing) increment (km)
    gs[3] = grid_resolution_y * coordinate_scale_factor; // in km
    // reference longitude (degrees)
    gs[4] = longitude_of_projection_origin;
    // reference latitude , first and second parallel
    gs[5] = standard_parallel;

    // finally, set gridtype
    gt = lambert;

  } else if (name == "mercator") {

    /*
     ===========================================
     mercator
     ===========================================
     */
    /*
     supported attributes in NetCDF:
     - standard_parallel             (  -90.0 <= real <  90.0 )
     - longitude_of_central_meridian ( -180.0 <= real < 180.0 )
     - false_easting                 ( real )
     - false_northing                ( real )
     */

    /*
     gridspec in mi
     gs[0] - western boundary (longitude for x=1) (degrees)
     gs[1] - southern boundary (latitude for y=1) (degrees)
     gs[2] - x (longitude) increment (km)
     gs[3] - y (latitude)  increment (km)
     gs[4] - reference (construction) latitude (degrees)
     gs[5] - 0.  (not used)
     */

    // western boundary (longitude for x=1) (degrees)
    //     gs[0] = -false_easting / grid_resolution_x;
    gs[0] = false_easting + start_x;
    // southern boundary (latitude for y=1) (degrees)
    //     gs[1] = -false_northing / grid_resolution_y;
    gs[1] = false_northing + start_y;
    //  x (latitude)  increment (km)
    gs[2] = grid_resolution_x * coordinate_scale_factor; // in km
    //  y (latitude)  increment (km)
    gs[3] = grid_resolution_y * coordinate_scale_factor; // in km
    // reference (construction) latitude (degrees)
    gs[4] = (standard_parallel);
    // not used
    gs[5] = 0.0;

    // finally, set gridtype
    gt = mercator;

    }

    METLIBS_LOG_DEBUG("FINAL gridspec:" << gs[0] << ", " << gs[1] << ", " << gs[2]
        << ", " << gs[3] << ", " << gs[4] << ", " << gs[5]);

  // set mi grid-specifications
  set_mi_gridspec(gt, gs, resolutionX, resolutionY);

  return true;
}

// set projection from string (gridtype:gridspec:gridspec....)
bool Projection::setProjection(const std::string& projectionString,
    double& resolutionX, double& resolutionY)
{
  // Assuming fortran style grid specification
    std::vector<std::string> tokens = miutil::split(projectionString, ":");

  if (tokens.size() != speclen + 1)
    return false;

  const int gt = miutil::to_int(tokens[0]);

  float gs[speclen];
  for (int i = 0; i < speclen; i++) {
      gs[i] = miutil::to_double(tokens[i + 1]);
  }

  set_mi_gridspec(gt, gs, resolutionX, resolutionY);

  return true;
}

// set projection from string (name=name proj=gt grid=gridspec:gridspec.. )
bool Projection::setProjectionFromLog(const std::string& logtext,
    double& resolutionX, double& resolutionY)

{
  const char key_proj[] = "proj";
  const char key_grid[] = "grid";
  const char key_def[] = "projdefinition";
  const char key_resx[] = "resolutionx";
  const char key_resy[] = "resolutiony";

  std::string projStr, gridStr, projDefStr, resxStr, resyStr;

  // split on blank, preserve ""
  std::vector<std::string> tokens = miutil::split_protected(logtext, '"', '"', " ", true);

  int n = tokens.size();
  for (int i = 0; i < n; i++) {
      std::vector<std::string> stokens = miutil::split(tokens[i], 1, "=");
    if (stokens.size() > 1) {
        const std::string key = miutil::to_lower(stokens[0]);
      if (key == key_proj) {
        projStr = stokens[1];
      } else if (key == key_grid) {
        gridStr = stokens[1];
      } else if (key == key_def) {
        projDefStr = stokens[1];
      } else if (key == key_resx) {
          resolutionX = miutil::to_double(stokens[1]);
      } else if (key == key_resy) {
        resolutionY = miutil::to_double(stokens[1]);
      }
    }
  }

  if (not projDefStr.empty()) {
    set_proj_definition(projDefStr, resolutionX, resolutionY);

  } else {

    // Assuming fortran style grid specification

    const std::string proj_undefined = "undefined";
    const std::string proj_pstereographic = "pstereographic";
    const std::string proj_pstereographic_60 = "pstereo_60";
    const std::string proj_mercator = "mercator";
    const std::string proj_geographic = "geographic";
    const std::string proj_spherical_rot = "spherical_rot";
    const std::string proj_lambert = "lambert";
    const std::string proj_generic = "generic";

    int gt = undefined_projection;
    if (projStr == proj_pstereographic)
      gt = Projection::polarstereographic;
    else if (projStr == proj_pstereographic_60)
      gt = Projection::polarstereographic_60;
    else if (projStr == proj_mercator)
      gt = Projection::mercator;
    else if (projStr == proj_geographic)
      gt = Projection::geographic;
    else if (projStr == proj_spherical_rot)
      gt = Projection::spherical_rotated;
    else if (projStr == proj_undefined)
      gt = Projection::undefined_projection;
    else if (projStr == proj_lambert)
      gt = Projection::lambert;
    else if (projStr == proj_generic)
      gt = Projection::generic;
    else {
      gt = Projection::undefined_projection;
    }

    std::vector<std::string> tokens = miutil::split(gridStr, ":");

    if (tokens.size() > speclen)
      METLIBS_LOG_ERROR("wrong speclen size");
    if (tokens.size() > speclen)
      return false;

    float gs[speclen];
    for (int i = 0; i < speclen; i++) {
      gs[i] = 0.0;
    }
    for (unsigned int i = 0; i < tokens.size(); i++) {
        gs[i] = miutil::to_double(tokens[i]);
    }

    set_mi_gridspec(gt, gs, resolutionX, resolutionY);
  }
  return true;
}

// get string (gridtype:gridspec:gridspec....)
std::string Projection::toString(bool fortranStyle) const
{
  std::string str = miutil::from_number(gridtype);
  if (fortranStyle) {
    for (int i = 0; i < speclen; i++) {
      str += ":" + std::string(gridspecstd[i], 8);
    }
  } else {
    for (int i = 0; i < speclen; i++) {
      str += ":" + std::string(gridspec[i], 8);
    }

  }
  return str;
}

// get string (gridtype:gridspec:gridspec....)
std::string Projection::toLogString() const
{
  ostringstream ost;
  ost << "proj=" << gridtype << " grid=";
  for (int i = 0; i < speclen; i++) {
    if (i > 0) {
      ost << ":";
    }
    ost << std::string(gridspecstd[i], 8);
  }
  ost << " projdefinition=\"" << projDefinition << "\"" << " resolutionx="
      << gridResolutionX << " resolutiony=" << gridResolutionY;

  /*
   std::string str = "proj=" + std::string(gridtype) + " grid=";
   for (int i=0; i<speclen; i++) {
   if(i>0) {
   str += ":";
   }
   str +=  std::string(gridspecstd[i],8);
   }
   */

  return ost.str();
}

float Projection::dx() const
{
  const float earth_radius = earthRadius / 1000.0; // km
  // the x-spacing of the grid
  float grid_resolution_x = 1.0;
  // the y-spacing of the grid
  float grid_resolution_y = 1.0;

  //  int gridtype = area.P().Gridtype();
  //  float *gridspec = new float[6];
  //  area.P().Gridspec(gridspec);

  if (gridtype == Projection::polarstereographic || gridtype
      == Projection::polarstereographic_60) {
    /*
     gridspec in mi
     gs[0] - x-position of pole
     gs[1] - y-position of pole
     gs[2] - number of grid distances between pole and equator
     gs[3] - rotation angle of the grid (degrees)
     gs[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     gs[5] - 0. (not used)
     */
    float standard_parallel =
        (gridtype == Projection::polarstereographic_60 ? 60.0 : gridspec[4]);
    float lengthOfDeg = cos(standard_parallel) * 111.325; // km

    grid_resolution_y = (earth_radius * (1 + sin(standard_parallel * DEG_TO_RAD)) / gridspec[2]) / lengthOfDeg;
    grid_resolution_x = grid_resolution_y;

  } else if (gridtype == Projection::geographic || gridtype
      == Projection::spherical_rotated) {
    /*
     gridspec in mi
     gs[0] - west
     gs[1] - south
     gs[2] - lon_inc (deg)
     gs[3] - lat_inc (deg)
     gs[4] - 0
     gs[5] - 0
     */
    grid_resolution_x = gridspec[2];

  } else if (gridtype == Projection::mercator) {
    /*
     gridspec in mi
     gs[0] - western boundary (longitude for x=1) (degrees)
     gs[1] - southern boundary (latitude for y=1) (degrees)
     gs[2] - x (longitude) increment (km)
     gs[3] - y (latitude)  increment (km)
     gs[4] - reference (construction) latitude (degrees)
     gs[5] - 0.  (not used)
     */
    float lengthOfDeg = cos(gridspec[4]) * 111.325; // km

    grid_resolution_x = gridspec[2] / lengthOfDeg;
  }

  //  delete [] gridspec;

  return grid_resolution_x;
}

float Projection::dy() const
{
  const float earth_radius = earthRadius / 1000.0; // km
  // the y-spacing of the grid
  float grid_resolution_y = 1.0;

  //  int gridtype = area.P().Gridtype();
  //  float *gridspec = new float[6];
  //  area.P().Gridspec(gridspec);

  if (gridtype == Projection::polarstereographic || gridtype
      == Projection::polarstereographic_60) {
    /*
     gridspec in mi
     gs[0] - x-position of pole
     gs[1] - y-position of pole
     gs[2] - number of grid distances between pole and equator
     gs[3] - rotation angle of the grid (degrees)
     gs[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     gs[5] - 0. (not used)
     */
    float standard_parallel =
        (gridtype == Projection::polarstereographic_60 ? 60.0 : gridspec[4]);

    /// max error 1.13 km
    float lengthOfDeg = 111.325; /// km

    grid_resolution_y = (earth_radius * (1 + sin(standard_parallel * DEG_TO_RAD)) / gridspec[2]) / lengthOfDeg;

  } else if (gridtype == Projection::geographic || gridtype
      == Projection::spherical_rotated) {
    /*
     gridspec in mi
     gs[0] - west
     gs[1] - south
     gs[2] - lon_inc (deg)
     gs[3] - lat_inc (deg)
     gs[4] - 0
     gs[5] - 0
     */
    grid_resolution_y = gridspec[3];

  } else if (gridtype == Projection::mercator) {
    /*
     gridspec in mi
     gs[0] - western boundary (longitude for x=1) (degrees)
     gs[1] - southern boundary (latitude for y=1) (degrees)
     gs[2] - x (longitude) increment (km)
     gs[3] - y (latitude)  increment (km)
     gs[4] - reference (construction) latitude (degrees)
     gs[5] - 0.  (not used)
     */
    /// max error 1.13 km
    float lengthOfDeg = 111.325; /// km

    grid_resolution_y = gridspec[3] / lengthOfDeg;
  }

  //  delete [] gridspec;

  return grid_resolution_y;
}

float Projection::boundaryWest() const
{
  //  int gridtype = area.P().Gridtype();
  //  float *gridspec = new float[6];
  //  area.P().Gridspec(gridspec);

  float start_x = 0.;

  if (gridtype == Projection::polarstereographic || gridtype
      == Projection::polarstereographic_60) {
    /*
     gridspec in mi
     gs[0] - x-position of pole
     gs[1] - y-position of pole
     gs[2] - number of grid distances between pole and equator
     gs[3] - rotation angle of the grid (degrees)
     gs[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     gs[5] - 0. (not used)
     */
    start_x = -1.;

  } else if (gridtype == Projection::geographic || gridtype
      == Projection::spherical_rotated || gridtype == Projection::mercator) {
    /*
     gridspec in mi
     gs[0] - west
     gs[1] - south
     gs[2] - lon_inc (deg)
     gs[3] - lat_inc (deg)
     gs[4] - 0
     gs[5] - 0
     */
    start_x = gridspec[0];
  }

  //  delete [] gridspec;

  return start_x;
}

float Projection::boundarySouth() const
{
  //  int gridtype = area.P().Gridtype();
  //  float *gridspec = new float[6];
  //  area.P().Gridspec(gridspec);
  //
  float start_y = 0.;

  if (gridtype == Projection::polarstereographic || gridtype
      == Projection::polarstereographic_60) {
    /*
     gridspec in mi
     gs[0] - x-position of pole
     gs[1] - y-position of pole
     gs[2] - number of grid distances between pole and equator
     gs[3] - rotation angle of the grid (degrees)
     gs[4] - projection latitude (degrees) (60 degrees north for gridtype=1)
     gs[5] - 0. (not used)
     */
    start_y = -1.;

  } else if (gridtype == Projection::geographic || gridtype
      == Projection::spherical_rotated || gridtype == Projection::mercator) {
    /*
     gridspec in mi
     gs[0] - west
     gs[1] - south
     gs[2] - lon_inc (deg)
     gs[3] - lat_inc (deg)
     gs[4] - 0
     gs[5] - 0
     */
    start_y = gridspec[1];
  }

  //  delete [] gridspec;

  return start_y;
}

bool Projection::isLegal(float lon, float lat) const
{
  float px = lon;
  float py = lat;
  int n = 1;
  int ierror = convertToGeographic(n, &px, &py);

  const float maxval = 1000000;
  bool legal = (ierror == 0 && fabsf(px) < maxval && fabsf(py) < maxval);
  return legal;
}

bool Projection::isAlmostEqual(const Projection& p) const
{
  vector<std::string> thisProjectionParts = miutil::split(projDefinition, 0, " ");
  vector<std::string> pProjectionParts = miutil::split(p.getProjDefinition(), 0, " ");

  map<std::string, std::string> partMap;

  //Loop through this proj string, put key and value in map (except x_0 and y_0)
  for( size_t i=0; i<thisProjectionParts.size(); ++i ) {
    vector<std::string> token = miutil::split(thisProjectionParts[i], "=");
    if ( token.size() == 2 && token[0] != "+x_0" && token[0] != "+y_0" ){
      partMap[token[0]] = token[1];
    }
  }

  //Loop through the proj string of p, compare key and value, remove equal parts
  for( size_t i=0; i<pProjectionParts.size(); ++i ) {
    vector<std::string> token = miutil::split(pProjectionParts[i], "=");
    if ( token.size() == 2 && token[0] != "+x_0" && token[0] != "+y_0" ){
      map<std::string,std::string>::iterator it =partMap.find(token[0]);
      if ( it == partMap.end() || partMap[token[0]] != token[1] ) {
        return false;
      }
      partMap.erase(it);
    }
  }

  //if all parts, except x_0 and y_0, are equal, return true
  if ( partMap.size() ==0 ) {
    return true;
  }

  //Not all parts are equal
  return false;

}

void Projection::filledMapCutparameters(bool & cutnorth, bool & cutsouth) const
{
  cutsouth = !isLegal(0.0, -90.0);
  cutnorth = !isLegal(0.0, 90.0);
  if (gridtype == Projection::polarstereographic_60) {
    cutsouth = true;
  } else if (gridtype == Projection::polarstereographic) {
    if (gridspec[4] > 0)
      cutsouth = true;
    else if (gridspec[4] < 0)
      cutnorth = true;
  } else if (gridtype == Projection::spherical_rotated) {
    if (gridspec[5] > 0)
      cutsouth = true;
    else if (gridspec[5] < 0)
      cutnorth = true;
  }
}

float Projection::getMapLinesJumpLimit() const
{
  float jumplimit = 100000000;
  // some projections do not benefit from using jumplimits
  if (miutil::contains(projDefinition, "+proj=stere"))
    return jumplimit;
  // find position of two geographic positions
  float px[2] = { 0.0, 0.0 };
  float py[2] = { 45.0, -45.0 };
  int n = 2;
  int ierror = convertFromGeographic(n, px, py);

  if (ierror) {
    METLIBS_LOG_ERROR("getMapLinesJumpLimit:" << ierror);
  } else {
    float dx = px[1] - px[0];
    float dy = py[1] - py[0];
    jumplimit = sqrtf(dx * dx + dy * dy) / 4;
  }
  return jumplimit;
}

int Projection::convertToGeographic(int n, float * tx, float * ty) const
{
  Projection pa;
  pa.setGeographic();
  int ierror = convertToGeographic(n, tx, ty, pa);

  return ierror;
}


int Projection::convertToGeographic(int n, float * tx, float * ty, const Projection& geoProj) const
{
  int ierror = geoProj.convertPoints(*this, n, tx, ty);
  for (int i = 0; i < n; i++) {
    if (tx[i] == HUGE_VAL) {
      tx[i] = HUGE_VAL;
    } else {
      tx[i] = static_cast<float> (tx[i] / DEG_TO_RAD);
    }
    if (ty[i] == HUGE_VAL) {
      ty[i] = HUGE_VAL;
    } else {
      ty[i] = static_cast<float> (ty[i] / DEG_TO_RAD);
    }
  }

  return ierror;
}

int Projection::convertFromGeographic(int npos, float* x, float* y) const
{
  Projection pa;
  pa.setGeographic();

  int ierror = convertFromGeographic(npos,x,y,pa);

  return ierror;
}

int Projection::convertFromGeographic(int npos, float* x, float* y, const Projection& geoProj) const
{

  for (int i = 0; i < npos; i++) {
    if (x[i] == 0)
      x[i] = 0.01; //todo: Bug:  x[i]=0 converts to x[i]=nx-1 when transforming between geo-projections
    x[i] *=  DEG_TO_RAD;
    y[i] *=  DEG_TO_RAD;
  }
  //return 0;
  int ierror = convertPoints(geoProj, npos, x, y);
  return ierror;
}

void Projection::setDefault()
{

  projDefinition="+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=25 +x_0=0.811578 +y_0=0.637045 +ellps=WGS84 +towgs84=0,0,0 +no_defs";
  projObject = boost::shared_ptr<PJ>(pj_init_plus(projDefinition.c_str()), pj_free);
  gridResolutionX= DEG_TO_RAD;
  gridResolutionY= DEG_TO_RAD;

  defined = true;
}

void Projection::setGeographic()
{

  gridtype = 2; //needed in isGeographic, fix this!!
  projDefinition="+proj=longlat  +ellps=WGS84 +towgs84=0,0,0 +no_defs";
  gridResolutionX=DEG_TO_RAD;
  gridResolutionY=DEG_TO_RAD;
  projObject = boost::shared_ptr<PJ>(pj_init_plus(projDefinition.c_str()), pj_free);
}

bool Projection::calculateLatLonBoundingBox(const Rectangle & maprect,
    float & lonmin, float & lonmax, float & latmin, float & latmax) const
{
  // find (approx.) geographic area on map

  lonmin = FLT_MAX;
  lonmax = -FLT_MAX;
  latmin = FLT_MAX;
  latmax = -FLT_MAX;

  int n, i, j;
  int nt = 9;
  float *tx = new float[nt * nt];
  float *ty = new float[nt * nt];
  float dx = (maprect.x2 - maprect.x1) / float(nt - 1);
  float dy = (maprect.y2 - maprect.y1) / float(nt - 1);

  n = 0;
  for (j = 0; j < nt; j++) {
    for (i = 0; i < nt; i++) {
      tx[n] = maprect.x1 + dx * i;
      ty[n] = maprect.y1 + dy * j;
      n++;
    }
  }

  int ierror = convertToGeographic(n, tx, ty);

  if (ierror) {
    METLIBS_LOG_ERROR("calculateLatLonBoundingBox: " << ierror);
    delete[] tx;
    delete[] ty;
    return false;
  }

  for (i = 0; i < n; i++) {
    if (lonmin > tx[i])
      lonmin = tx[i];
    if (lonmax < tx[i])
      lonmax = tx[i];
    if (latmin > ty[i])
      latmin = ty[i];
    if (latmax < ty[i])
      latmax = ty[i];
  }

  delete[] tx;
  delete[] ty;
  return true;
}

bool Projection::adjustedLatLonBoundingBox(const Rectangle & maprect,
    float & lonmin, float & lonmax, float & latmin, float & latmax) const
{
  // when UTM: keep inside a strict lat/lon-rectangle
  if (miutil::contains(projDefinition, "+proj=utm")) {
    if (!calculateLatLonBoundingBox(maprect, lonmin, lonmax, latmin, latmax)) {
      return false;
    }
    return true;
  }

  lonmin = -180;
  lonmax = 180;
  latmin = -90;
  latmax = 90;

  return true;
}

bool Projection::calculateGeogridParameters(float * xylim, float & lonmin,
    float & lonmax, float & latmin, float & latmax, float & jumplimit) const
{
  jumplimit = getMapLinesJumpLimit();

  // Rectangle usage: x1, x2, y1, y2
  Rectangle maprect(xylim[0], xylim[2], xylim[1], xylim[3]);
  if (!adjustedLatLonBoundingBox(maprect, lonmin, lonmax, latmin, latmax)) {
    return false;
  }

  bool cutsouth = !isLegal(0.0, -90.0);
  bool cutnorth = !isLegal(0.0, 90.0);
  if (cutsouth && latmin < -89.95) {
    latmin = -89.95;
  }
  if (cutnorth && latmax > 89.95) {
    latmax = 89.95;
  }

  return true;
}

int Projection::getMapRatios(int imaprc, int icoric, int nx, int ny,
    float* xmapr, float* ymapr, float* coriolis, float &dxgrid, float &dygrid) const
{

  Projection pr;
  pr.setGeographic();
  int npos = nx * ny;
  float *x = new float[npos];
  float *y = new float[npos];
  int i = 0;
  for (int iy = 0; iy < ny; iy++) {
    // init x,y arrays
    for (int ix = 0; ix < nx; ix++) {
      x[i] = ix*gridResolutionX;
      y[i] = iy*gridResolutionY;
      i++;
    }
  }

  int ierror = convertToGeographic(npos, x, y);
  if (ierror < 0) {
    return ierror;
  }

  //HACK: in geographic projection wrapping the world x[0]=x[nx-1]=180
  //must set x[0]=-180 in order to get map ratios right
  for (int iy = 0; iy < ny; iy++) {
    if(x[nx*iy] > 179 && x[nx*iy] < 181) {
      x[nx*iy] = -180;
    }
  }

  if (icoric > 0) {
    for (int j = 0; j < npos; ++j) {
      coriolis[j] = 2.0 * 0.7292e-4 * sin(y[j] * DEG_TO_RAD);
    }
  }

  if (imaprc < 2) {
    METLIBS_LOG_ERROR("getMapRatios: Don't know how to calculate map ratios when imaprc<2");
    return -1;
  }

  i = 0;
  for (int iy = 0; iy < ny; iy++) { // init x,y arrays
    for (int ix = 0; ix < nx; ix++) {

      if (iy == ny - 1) {
        ymapr[i] = ymapr[i - nx];
      } else {
        float dy = ((y[i + nx] - y[i]) * earthRadius * DEG_TO_RAD);
        float dx = ((x[i + nx] - x[i]) * earthRadius * DEG_TO_RAD) * cos(y[i] * DEG_TO_RAD);
        float dd = (imaprc-1) * sqrt(pow(dy, 2) + pow(dx, 2));
        ymapr[i] = 1 / dd;
      }
      if (ix == nx - 1) {
        xmapr[i] = xmapr[i - 1];
      } else {
        float dy = ((y[i + 1] - y[i]) * earthRadius * DEG_TO_RAD);
        float dx = ((x[i + 1] - x[i]) * earthRadius * DEG_TO_RAD) * cos(y[i] * DEG_TO_RAD);
        float dd = (imaprc-1) * sqrt(pow(dy, 2) + pow(dx, 2));
        xmapr[i] = 1 / dd;
      }
      i++;
    }
  }
  return ierror;
}

bool Projection::getLatLonIncrement(const float lat, const float lon,
    float& dlat, float& dlon) const
{
  dlat = RAD_TO_DEG / earthRadius;
  dlon = RAD_TO_DEG / earthRadius / cos(lat * DEG_TO_RAD);
  return true;
}

bool Projection::isGeographic() const
{
    return (gridtype == geographic
            || miutil::contains(projDefinition, "+proj=eqc")
            || miutil::contains(projDefinition, "+proj=longlat")
            || miutil::contains(projDefinition, "+proj=ob_tran +o_proj=longlat +lon_0=0 +o_lat_p=90"));
}
