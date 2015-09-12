/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  Copyright (C) 2006-2013 met.no

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


#ifndef _usrTab_h
#define _usrTab_h

#include <puTools/miString.h>
#include <puTools/miTime.h>

#include <map>

class usrTab {
private:
  std::map<miutil::miString, miutil::miString> entity;
  miutil::miString mNull;
public:
  usrTab(): mNull("") {}

  bool create( miutil::miString );

  bool exists(miutil::miString m) const
  { return ( bool(entity.count(m)) ? true: false);}

  miutil::miTime dates( miutil::miString );

  miutil::miString content( miutil::miString m)
  { return ( bool(entity.count(m)) ? entity[m] : mNull); }

  miutil::miString replace( miutil::miString m)
  { return ( bool(entity.count(m)) ? entity[m] : m);}

  miutil::miString file( miutil::miString m)
  { return entity["WORKDIR"]+ content(m); }

  void makeByHand(miutil::miString m, miutil::miString requ)
  { entity[m]= requ;}

  int numInt(miutil::miString m)
    { return atoi(entity[m].c_str());}

  float numFloat( miutil::miString m)
    { return atof(entity[m].c_str());}

  miutil::miString operator[] ( const miutil::miString &m )
  { return ( bool(entity.count(m)) ? entity[m] : mNull); }

  std::map<miutil::miString,miutil::miString> selected(const std::vector<miutil::miString> & );
};

#endif
