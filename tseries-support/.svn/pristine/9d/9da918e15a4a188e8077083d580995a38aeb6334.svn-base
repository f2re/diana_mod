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


#ifndef _visibility_h
#define _visibility_h

//
//   Prgmmr: L. D. Burroughs  org: W/NMC21    Date: 03-03-09             
//   Program history log:                                                  
//   03-02-03  L. D. Burroughs 
// 2006-07-13 L.Bergholt - C++                                          

/**

  \brief  Horizontal visibility (m)

  Computes horizontal visibility at the 1000 hPa layer from
  qv, qc, qr, crain, cfrzr, cicep,csnow, air temp and pressure.

*/
           
double visibility(const double& qv,      // water vapor mixing ratio (kg/kg)
		  const double&  qc,     // cloud water mixing ratio (kg/kg)
		  const double&  qr,     // rain water mixing ratio  (kg/kg)
		  const double&  ta,     // air temperature          (k)
		  const double&  pp,     // pressure                 (hPa)
		  const int&     crain,  // categorical rain         (y=1,n=0)
		  const int&     cfrzr,   // categorical freezing rain(y=1,n=0)
		  const int&     cicep,   // categorical ice pellets  (y=1.n=0)
		  const int&     csnow);  // categorical snow         (y=1,n=0) 



#endif










