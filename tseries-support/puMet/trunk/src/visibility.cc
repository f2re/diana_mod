/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  $Id: visibility.cc 4344 2014-01-02 09:24:27Z juergens $

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

#include <math.h>
#include <iostream>


// REMARKS:
//   If crain and cfrzr = 0 and cicep and csnow = 0;
//   then qc determines vis alone.
//   if crain or cfrzr =1 and cicep and csnow = 0;
//   then qc and qr determine   vis;
//   if crain or cfrzr =1 and cicep or csnow =1;
//   then qc and a combo of 50 percent rain and snow is used to determine vis;
//   if crain or cfrzr =0 and cicep or csnow =1;
//   then qc and qs deternine   vis;
//
//   The routine uses the following
//   expressions for extinction coefficient, beta (in km**-1),
//   with C being the mass concentration (in g/m**3):
//
//      cloud water:  beta = 144.7 * C ** (0.8800)
//      rain water:   beta =  2.24 * C ** (0.7500)
//      cloud ice:    beta = 327.8 * C ** (1.0000)
//      snow:         beta = 10.36 * C ** (0.7776)
//
//   These expressions were obtained from the following sources:
//
//   for cloud water: from Kunkel (1984)
//   for rainwater: from M-P dist'n, with No=8e6 m**-4 and rho_w=1000 kg/m**3
//   for cloud ice: assume randomly oriented plates which follow
//         mass-diameter relationship from Rutledge and Hobbs (1983)
//   for snow: from Stallabrass (1985), assuming beta = -ln(.02)/vis
//
//   The extinction coefficient for each water species present is
//   calculated, and then all applicable betas are summed to yield
//   a single beta. Then the following relationship is used to
//   determine visibility (in km), where epsilon is the threshhold
//   of contrast, usually taken to be .02:
//
//      vis = -ln(epsilon)/beta      [found in Kunkel (1984)]
//
//   This procedure was first developed by Stoelinga and Warner, adapted to the
//   eta model postprocessor by Geoff Manakin, and adapted by Larry Burroughs for
//   use with the GFS to replace the statistical open ocean fog and visibility
//   system.
//
#include <puTools/miString.h>

double visibility(const double&  qv,     // water vapor mixing ratio (kg/kg)
		  const double&  qc,     // cloud water mixing ratio (kg/kg)
		  const double&  qr,     // rain water mixing ratio  (kg/kg)
		  const double&  ta,     // air temperature          (k)
		  const double&  pp,     // pressure                 (hPa)
		  const int&     crain,  // categorical rain         (y=1,n=0)
		  const int&     cfrzr,  // categorical freezing rain(y=1,n=0)
		  const int&     cicep,  // categorical ice pellets  (y=1.n=0)
		  const int&     csnow)  // categorical snow         (y=1,n=0)
{


  const double rdc    = 0.0034838; //virker bedre enn 0.34838, alt. pp i Pa ikke hPa
  const double coeflc = 144.7;
  const double coeflp = 2.24;
  const double coeffc = 327.8;
  const double coeffp = 10.36;
  const double exponlc= 0.8800;
  const double exponlp= 0.75;
  const double exponfc= 1.0000;
  const double exponfp= 0.7776;
  const double rhoice = 916.8;
  const double rhowat = 997.37;

  const double const1 = -log(0.02);
  double sh     = qv/(1+qv);
  double tv     = ta*(1.0+0.608*sh);
  double rhoair = rdc*pp/tv;

//   Determine precipitation type and mixing ratio values

  double conclc = 0.;
  double conclp = 0.;
  double concfc = 0.;
  double concfp = 0.;

  if(crain==0 && cfrzr==0 && cicep==0 && csnow==0){
    double qcld=qc;
    double vovermd=(1.+qv)/rhoair+qcld/rhowat;
    conclc = qcld/vovermd*1000.;
  } else if((crain==1 || cfrzr==1) && cicep==0 && csnow==0){
    double qprr=qr;
    double qcld=qc;
    double vovermd=(1.+qv)/rhoair+(qprr+qcld)/rhowat;
    conclc = qcld/vovermd*1000.;
    conclp = qprr/vovermd*1000.;
  } else if((crain==1 || cfrzr==1) && (cicep==1 ||  csnow==1)){
    double qprr=0.5*qr;
    double qprs=0.5*qr;
    double qcld=0.5*qc;
    double qcldi=0.5*qc;
    double vovermd=(1.+qv)/rhoair+(qprr+qcld)/rhowat+(qprs+qcldi)/rhoice;
    conclc = qcld/vovermd*1000.;
    conclp = qprr/vovermd*1000.;
    concfc = qcldi/vovermd*1000.;
    concfp = qprs/vovermd*1000.;
  } else if(crain==0 && cfrzr==0 && (cicep==1 || csnow==1)) {
    double qprs=qr;
    double qcldi=qc;
    if(ta <= 263.){
      double vovermd=(1.+qv)/rhoair+(qprs+qcldi)/rhoice;
      concfc = qcldi/vovermd*1000.;
      concfp = qprs/vovermd*1000.;
    } else {
      double vovermd=(1.+qv)/rhoair+qcldi/rhowat+qprs/rhoice;
      conclc = qcldi/vovermd*1000.;
      concfp = qprs/vovermd*1000.;
    }
  }

  double betav=coeffc*pow(concfc,exponfc)+coeffp*pow(concfp,exponfp)
    +coeflc*pow(conclc,exponlc)+coeflp*pow(conclp,exponlp)+1.e-10;

  std::cerr << "betav:" << betav << std::endl;
  std::cerr << "const1:" << const1 << std::endl;

//  vis      - visual range in m (not km)

// changed gsm 3-10-00 -->  no point in distinguishing values
//       above 20 km, so make that value the max (prev max was 80)

  double vis=1.e3*(20.<const1/betav ? 20 : const1/betav);

  return vis;

}







