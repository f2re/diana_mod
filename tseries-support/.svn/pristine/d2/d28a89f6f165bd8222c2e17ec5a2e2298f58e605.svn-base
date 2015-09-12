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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miPosition.h"
#include <sstream>

using namespace std;

miCoordinates miPosition::origo_=miCoordinates(float(0.0),float(89.9));
miPosition::sort_mode miPosition::smode_= miPosition::sort_name;

void miPosition::setPos(const miCoordinates& pos,
			const int synop,
			const int dbkey,
			const std::string& name,
			const int hoh,
			const int priority,
			const std::string group ){
  pos_      = pos;
  synop_    = synop;
  dbKey_    = dbkey;
  name_     = name;
  hoh_      = hoh;
  priority_ = priority;
  group_    = group;
};


ostream& operator<<(ostream& out, const miPosition& rhs){
  out << "miPosition -- Name: " << rhs.name_ <<
    " synop: " <<  rhs.synop_ << " dbKey: " << rhs.dbKey_ <<
    " hoh: " << rhs.hoh_ << " priority: " << rhs.priority_ <<
    " group: " << rhs.group_ << " Coordinates: " << rhs.pos_;
 return out;
};


int miPosition::sort_key(const miPosition& p1,const miPosition& p2)
{
  float d1,d2;
  switch (smode_) {
  case sort_name:
    if (p1.Name() == p2.Name())
      return 0;
    else
      return (p1.Name() > p2.Name() ? 1 : -1);
  case sort_synop:
    if (p1.Synop() == p2.Synop())
      return 0;
    else
      return (p1.Synop() > p2.Synop() ? 1 : -1);
  case sort_lat:
    if (p1.lat() == p2.lat())
      return 0;
    else
      return (p1.lat() > p2.lat() ? 1 : -1);
  case sort_lon:
    if (p1.lon() == p2.lon())
      return 0;
    else
      return (p1.lon() > p2.lon() ? 1 : -1);
  case sort_dbkey:
    if (p1.DbKey() == p2.DbKey())
      return 0;
    else
      return (p1.DbKey() > p2.DbKey() ? 1 : -1);
  case sort_distance:
    d1= p1.distance(origo_);
    d2= p2.distance(origo_);
    if (d1 == d2)
      return 0;
    else
      return (d1 > d2 ? 1 : -1);
  default:
    if (p1.Name() == p2.Name())
      return 0;
    else
      return (p1.Name() > p2.Name() ? 1 : -1);
  }
}


bool operator==(const miPosition& lhs, const miPosition& rhs)
{
  return (miPosition::sort_key(lhs,rhs)==0);
}

bool operator==(const miPosition& lhs, const std::string& rhs)
{
  return lhs.name_ == rhs ;
}

bool operator==(const miPosition& lhs, const int& rhs)
{
  return lhs.dbKey_ == rhs ;
}

bool operator!=(const miPosition& lhs, const miPosition& rhs)
{
  return (miPosition::sort_key(lhs,rhs)!=0);
}

bool operator<(const miPosition& lhs, const miPosition& rhs)
{
  return (miPosition::sort_key(lhs,rhs)<0);
}

bool operator>(const miPosition& lhs, const miPosition& rhs)
{
  return (miPosition::sort_key(lhs,rhs)>0);
}

bool operator<=(const miPosition& lhs, const miPosition& rhs)
{
  return (miPosition::sort_key(lhs,rhs)<=0);
}

bool operator>=(const miPosition& lhs, const miPosition& rhs)
{
  return (miPosition::sort_key(lhs,rhs)>=0);
}





