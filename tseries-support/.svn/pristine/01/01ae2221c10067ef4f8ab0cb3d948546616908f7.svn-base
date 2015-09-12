/*
  libpuDatatypes - Diverse datatypes: Regions, coordinates and alike

  Copyright (C) 2013 met.no

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


#ifndef _miPosition_h
#define _miPosition_h

// general class to hold a position (or station)
// first used and developed for QUBA
// JS/PU 8/99

#include "miCoordinates.h"

class miPosition {
public:
  enum sort_mode {sort_name, sort_synop,
		  sort_lat, sort_lon,
		  sort_dbkey, sort_distance };
private:
  miCoordinates pos_;
  int synop_;
  int dbKey_;
  std::string name_;
  int hoh_;
  int priority_;
  std::string group_;
  std::string icaoid_;

  static sort_mode smode_;
  static miCoordinates origo_;
  static int sort_key(const miPosition& p1,const miPosition& p2);

public:
  miPosition():synop_(0),dbKey_(0),hoh_(0),priority_(0){}
  miPosition(const miCoordinates& pos, const int synop,
	     const int dbkey, const std::string& name,
	     const int hoh = 0, const int priority = 0,
	     const std::string group = "")
  {setPos(pos,synop,dbkey,name,hoh,priority,group);}

  void setPos(const miCoordinates& pos, const int synop, const int dbkey,
	      const std::string& name, const int hoh= 0,
	      const int priority= 0, const std::string group= "");

  void setDbKey(const int i)           { dbKey_  = i;     }
  void setCoor(const miCoordinates& c) { pos_    = c;     }
  void setLat(const float& l)          { pos_.setLat(l);  }
  void setLon(const float& l)          { pos_.setLon(l);  }
  void setHoH(const int h)             { hoh_    = h;     }
  void setName(const  std::string& name)  { name_   = name;  }
  void setIcao(const  std::string& icaoid){ icaoid_ = icaoid;}
  void setGroup(const std::string& group) { group_  = group; }

  float lon() const {return pos_.dLon();}
  float lat() const {return pos_.dLat();}

  const miCoordinates& Coordinates() const {return pos_;}

  const std::string& Name() const {return name_;}
  int Synop()     const {return synop_;}
  int DbKey()     const {return dbKey_;}
  int height()    const {return hoh_;}
  int Priority()  const {return priority_;}
  const std::string& Group()  const {return group_;}
  const std::string& icaoID() const {return icaoid_;}

  int distance(const miPosition& pos) const
  { return pos_.distance(pos.Coordinates()); }
  int distance(const miCoordinates& c) const
  { return pos_.distance(c);}


  bool isInRect(const miCoordinates& nw,const miCoordinates& se ) const
  { return pos_.isInRect(nw,se); }

  bool isGrp(const std::string & group ) const { return group == group_ ;}

  static void setSortMode(sort_mode smode){smode_= smode;}
  static void setSortOrigo(const miCoordinates& c){origo_=c;}

  friend bool operator==(const miPosition& lhs, const miPosition& rhs);
  friend bool operator==(const miPosition& lhs, const std::string& rhs);
  friend bool operator==(const miPosition& lhs, const int& rhs);
  friend bool operator!=(const miPosition& lhs, const miPosition& rhs);
  friend bool operator<(const miPosition& lhs, const miPosition& rhs);
  friend bool operator>(const miPosition& lhs, const miPosition& rhs);
  friend bool operator<=(const miPosition& lhs, const miPosition& rhs);
  friend bool operator>=(const miPosition& lhs, const miPosition& rhs);

  friend std::ostream& operator<<(std::ostream&, const miPosition&);

};

#endif



