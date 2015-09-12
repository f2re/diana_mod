/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)
  
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



/* Created by met.no/PU 
   at Fri Apr  6 12:22:41 2001 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int ff2beaufort(int ff)
{
  if(ff < 1)
    return 0;
  if(ff == 1)
    return 1;
  if (ff == 2 || ff == 3)
    return 2;
  if (ff == 4 || ff == 5)
    return 5;
  if (ff == 6 || ff == 7)
    return 7;
  if (ff >= 8 && ff <= 10)
    return 10;
  if (ff >= 11 && ff <= 13)
    return 12;
  if (ff >= 14 && ff <= 17)
    return 15;
  if (ff >= 18 && ff <= 20)
    return 20;
  if (ff >= 21 && ff <= 24)
    return 22;
  if (ff >= 25 && ff <= 28)
    return 25;
  if (ff >= 29 && ff <= 32)
    return 30;
  return 33;
}    
