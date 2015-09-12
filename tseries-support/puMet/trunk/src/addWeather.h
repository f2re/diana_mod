/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
  $Id: addWeather.h 4344 2014-01-02 09:24:27Z juergens $

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


#ifndef _addWeather_h
#define _addWeather_h

#include "weatherSpec.h"

class addWeather : public weatherSpec {
private:
  int MAXcustom;
  int MINcustom;
public:
  addWeather();
  int maxCustom() const {return MAXcustom;}
  int minCustom() const {return MINcustom;}


};

#endif
