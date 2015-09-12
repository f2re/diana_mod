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


#ifndef _ptColor_h
#define _ptColor_h

#include <string>
#include <vector>

class ptColor {
public:
  std::string name;
  float colTable[4];

  ptColor();
  ptColor(const std::string& n);
  ptColor(const std::string& n, float r, float g, float b, float a=1);

  void fromStr(const std::string&);

  std::string     Color2Str() const;

  static std::vector<ptColor> Str2Colorlist(const std::string&);
  static std::string Colorlist2Str(const std::vector<ptColor>&);


  bool operator==(const ptColor &rhs) const
  { return (name == rhs.name); }
  bool operator!=(const ptColor &rhs) const
  { return (name != rhs.name); }
  friend std::ostream& operator<<(std::ostream& out, const ptColor& rhs)
  { return out << rhs.name ;   }

};


#endif
