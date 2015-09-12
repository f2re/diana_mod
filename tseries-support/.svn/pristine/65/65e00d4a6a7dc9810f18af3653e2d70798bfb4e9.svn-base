/*
  libpuDatatypes - Diverse datatypes: Regions, coordinates and alike

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


#ifndef puDatatypes_miCoordinates_h
#define puDatatypes_miCoordinates_h

#include <string>
#include <vector>

/*! mean earth radius in m */
extern const double EARTH_RADIUS_M;

class LonLat {
public:
  LonLat()
    : mLon(1000), mLat(1000) { }
  LonLat(double lon, double lat)
    : mLon(lon), mLat(lat) { }

  static LonLat fromDegrees(double lonDeg, double latDeg);

  //! longitude in radians
  double lon() const
    { return mLon; }

  //! latitude in radians
  double lat() const
    { return mLat; }

  //! longitude in degrees
  double lonDeg() const;

  //! latitude in degrees
  double latDeg() const;

  //! distance along great circle in m
  double distanceTo(const LonLat& to) const;

  //! initial bearing along great circle in radians
  double bearingTo(const LonLat& to) const;

  //! calculate final point going 'distance' meters from here in direction 'bearing' (in radians)
  LonLat stepDirection(double distance, double bearing) const;

  //! calculate final point going 'distance' meters from here towards in direction 'bearing' (in radians)
  LonLat stepTo(double distance, const LonLat& there) const
    { return stepDirection(distance, bearingTo(there)); }

private:
  double mLon;
  double mLat;
};

// class to hold station coordinates
// the usual format (taken from the klima database is
// degrees, minutes and centiminutes ( if wanted ).
// JS-/PU 5/99

struct coor {
  int deg;
  int cmin;

  coor() : deg(0), cmin(0) {}
  coor(const coor& c) : deg(c.deg), cmin(c.cmin) {}
  coor(const int&);
  coor(const float&);

  bool round(int,bool best=true);
  void addCmin(const int& );
  float fCoor() const
  { return ( (float(cmin)/6000.0) + float(deg) ); }

  friend bool operator==(const coor& lhs, const coor& rhs);
  friend bool operator!=(const coor& lhs, const coor& rhs);
  friend bool operator<(const coor& lhs, const coor& rhs);
  friend bool operator>(const coor& lhs, const coor& rhs);
  friend bool operator<=(const coor& lhs, const coor& rhs);
  friend bool operator>=(const coor& lhs, const coor& rhs);
};


class miCoordinates {
private:
  coor lon_;
  coor lat_;

  void   addLon( coor );
  void   addLat( coor );
  int    iCoor(const coor&) const;
  float  fCoor(const coor&) const;
  double sekant(double) const;

public:
  miCoordinates() {}

  // rnd=round valid = 1-4 ....

  miCoordinates( const int&   lon, const int&   lat,int rnd=0);
  miCoordinates( const float& lon, const float& lat,int rnd=0);
  miCoordinates( const coor&  lon, const coor&  lat,int rnd=0);

  void setLat(const float l) {lat_ = coor(l);}
  void setLon(const float l) {lon_ = coor(l);}

  // round - round off cmin
  void round(int);

  std::vector<miCoordinates> roundedGrid(int r); // the four possible gridpoints
                                            // in a rounded grid
                                            // around this coordinate...
  int km2deg( int, bool ) const ;
  double toRad( const coor& ) const ;

  coor invert(const coor&) const ;

  float dLon() const  { return fCoor(lon_); }
  float dLat() const  { return fCoor(lat_); }
  int   iLon() const  { return iCoor(lon_); }
  int   iLat() const  { return iCoor(lat_); }
  coor  Lat()  const  { return lat_;        }
  coor  Lon()  const  { return lon_;        }
  double rLon() const { return toRad(lon_); }
  double rLat() const { return toRad(lat_); }

  std::string sLon();  // example: "11° 05' W"
  std::string sLat();  // example: "59° 19' N"
  std::string str();   // sLon()" : "sLat()
  std::string encode();// "lon_deg:lon_min:lat_deg:lat_min"
  bool decode(const std::string&);// decodes encode-string

  friend miCoordinates operator+(const miCoordinates&, const miCoordinates&);
  friend miCoordinates operator-(const miCoordinates&, const miCoordinates&);
  friend miCoordinates operator/(miCoordinates,float);

  friend bool operator>(const miCoordinates&,const miCoordinates&);
  friend bool operator<(const miCoordinates&,const miCoordinates&);

  friend bool operator==(const miCoordinates&, const miCoordinates& );
  friend bool operator!=(const miCoordinates&, const miCoordinates& );

  friend std::ostream& operator<<(std::ostream&, const miCoordinates&);

  bool isInRect(const miCoordinates&, const miCoordinates&) const ;

  int distance(const miCoordinates& c) const // distance in km's
    { return static_cast<int>(distanceTo(c)/1000); }
  double distanceTo(const miCoordinates&) const; // distance in m's

  double bearingToR(const miCoordinates& to) const; // initial bearing in radians
  double bearingToD(const miCoordinates& to) const; // initial bearing in degrees

  double cross(const miCoordinates&) const; // cross product

  bool isCloserThan(const miCoordinates&, int tolerance);// distance in km's
};

#endif // puDatatypes_miCoordinates_h
