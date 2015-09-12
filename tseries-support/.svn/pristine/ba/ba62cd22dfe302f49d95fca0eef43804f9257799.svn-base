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


// windProfile.cc

// This functions computes all kind of
// wind profiles over water surface.
// in dependence of stability (Ri bulk) and a first guess wind.
// More information is written in windProfile.h .
// developed by j.schulze in may 98.
// last modified june 98

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "windProfile.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

// general constants and functions

const float  K   = 0.4            ; // von Karman const.
const float  KSQ = 0.16           ; // square of von karman
const float  CH  = 0.0012538226   ; // charnocks coeff. 0.0123/g
const float  B   = 5              ; // emp. coeff. (see louis)
const float  C   = 7.5            ; // emp. coeff. (see louis)
const float  D   = 5              ; // emp. coeff. (see louis)
const float  RiN = 0.00001        ; // Richardson Bulk border to a neutral condition



// computation of zZero out of Ustar with Charnocks parametrisation

double charnock(double Ustar){
  return(CH*pow(Ustar,2));
}


// ln(z/z0) after charnock parametrization

double lnZbyZo(double Ustar , double z){
  return(log(z)-log(CH)-2*log(Ustar));
}


// NEUTRAL CASE

// neutral wind profil
// called by  NewtonLouis and profile

double NeutralLouis(double Ustar ,double z){
  return (Ustar*lnZbyZo(Ustar,z)/K );
}


// differential of the neutral wind profile
// called by  NewtonLouis

double DiffNeutralLouis(double Ustar,  double z){
  return (lnZbyZo(Ustar, z)/K + 2/K );
}


// UNSTABLE CASE

//Louis computation of the unstable Drag coefficient (intern)
// called by bisecLouis and profile

double UnstableLouis(double Ustar , double z, double Ri  ){
  return (pow(Ustar,2)/((KSQ/pow(lnZbyZo(Ustar,z),2))*
			(1- (2*B*Ri)/(1+(B*C*2*KSQ*sqrt(z*fabs(Ri)/CH))/
				      (Ustar*pow(lnZbyZo(Ustar,z),2))))));
}


// STABLE CASE

// Louis computation of the stable Drag coefficient
// called by bisecLouis and profile

double StableLouis(double Ustar , double z, double Ri  ){
  return (pow(Ustar,2)/(KSQ/(pow(lnZbyZo(Ustar,z),2))*
			(1/(1+2*B*Ri/(sqrt(1+D*Ri))))));
}


// ITERATIVE COMPUTING OF USTAR

// computing of Ustar for stable/unstable cases with bisection method
// called by stability

bool bisecLouis(double z, double Ri, double speedZ ,double& Ustar){

  double fxdx  = 1        ;    //  output f(x)  at xmin+dx
  double fxmin            ;    //  output f(x)  at xmin
  double xmin  = 0.000001 ;    //  xmin
  double dx    = 10       ;    //  dx
  int iter = 0;

  while(fabs(fxdx)>1.0e-10 && iter<20000){

    iter++;

    if(Ri<0){
      fxdx  = pow(speedZ,2) - UnstableLouis(xmin+dx, z, Ri);
      fxmin = pow(speedZ,2) - UnstableLouis(xmin   , z, Ri);
    }
    if(Ri>0){
      fxdx  = pow(speedZ,2) - StableLouis(xmin+dx, z, Ri);
      fxmin = pow(speedZ,2) - StableLouis(xmin   , z, Ri);
    }


    if(fxmin>0 && fxdx<0)
      dx *= .5;

    if(fxmin>0 && fxdx>0)
      xmin = xmin + dx ;

  }

  Ustar= xmin;

  if(iter>=20000)
    return (false);

  return (true);
}


// computing Ustar for neutral cases with newtons method
// called by stability

bool NewtonLouis(double z, double speedZ,double& Ustar){

  double start  = 0.0001 ; // first guess
  double out    = 1      ; // solving of function (goes to Zero)
  double outOld = 10     ; // last computation of Ustar
  double iter = 0;


  while(fabs(out)>1.0e-10 && iter<20000){
    iter++;

    Ustar = Ustar -(NeutralLouis(Ustar, z) - speedZ)/
      DiffNeutralLouis(Ustar, z);

    // test for non-konvergent behavior

    if((out+outOld)== 0 || Ustar<0){
      start *=10;
      Ustar = start;
    }

    outOld = out;

    if(Ustar>100000)
      return false;

    // Computing solution
    out = NeutralLouis(Ustar, z) - speedZ;
  }


  if(iter >= 20000)
    return (false);

  return (true);
}


// CONTROL AND COMPUTING

// Checking stability
// called by uprofile::compute
// checks out the richardson number and decides the
// method to compute Ustar calls functions to compute Ustar
// needs    z      : height over ground
//          Ri     : Richardson Bulk number
//          speedZ : speed at height z
//          Ustar  : something (maybe zero .. whatever)
// returns  0      : but computes Ustar


bool Uprofile::stability(double z, double speedZ){

  Ustar =0.001;

  if (fabs(Ri)>=RiN){

    if(!bisecLouis(z, Ri, speedZ, Ustar)){
      errorOut="too many iterations";
      return(false);
    }
  }
  if (fabs(Ri)<RiN){
    if(!NewtonLouis(z, speedZ, Ustar)){
      errorOut="too many iterations";
      return(false);
    }
  }

  return(true);
}


// computes speed(z) under knowledge of Ustar
// used by  Uprofile::compute

double profile(double Ustar, double z, double Ri){

  if (fabs(Ri)<RiN)
    return NeutralLouis(Ustar,z);

  if (Ri> 0)
    return sqrt(StableLouis(Ustar, z, Ri));

  return sqrt(UnstableLouis(Ustar, z, Ri));
}


// Computing profiles
// for this function, the knowledge of Ustar is nessesary.
// with that you can compute a wind at any height or a profile,
// the function uses allways the right profile (stable, unstable or neutral)
// needs    z        : height over ground
//          Ri       : Richardson Bulk number
//          speedOut : speed at height z
//          Ustar    : the real Ustar computed by stability(...)
// returns  0 , but computes vektor wind with height og speed

// 1st case single point

float Uprofile::compute( double zref, double speedZref, double z){

  values *speedtmp;
  double spdtmp;



  wind.erase(wind.begin(),wind.end());
  if(stability(zref, speedZref)){

    spdtmp = profile(Ustar, z, Ri);
    speedtmp = new values(z,spdtmp);
    wind.push_back(*speedtmp);
    return(spdtmp);
  }
  return(speedZref);
}


// 2nd case profile from 0 to z

bool Uprofile::compute(double zref,double speedZref, double z, double increm){

  values *speedtmp;
  double spdtmp;
  double zZero;

  double zNew;


  wind.erase(wind.begin(),wind.end());
  if(stability(zref, speedZref)){

    zZero =charnock(Ustar);

    for(double zcount=zZero; zcount<=z+1 ; zcount+= increm){

      // adc: replacing trunc with double(long(X))
      //	zNew =trunc(zcount);
      if(zcount>zZero)
	zNew =double(long(zcount));
      else
	zNew =zZero;

      spdtmp=profile(Ustar, zNew, Ri);
      speedtmp = new values(zNew,spdtmp);
      wind.push_back(*speedtmp);
    }
    return(true);
  }
  return(false);
}


// 3rd case computes a vector with a free number of z

bool Uprofile::compute( double zref,double speedZref, std::vector<double> z)
{
  values *speedtmp;
  double spdtmp;
  double zZero;


  wind.erase(wind.begin(),wind.end());
  if(stability(zref, speedZref)){
    zZero =charnock(Ustar);

    sort(z.begin(),z.end());
    // adc: changed double zcount to int - 26.07.2000
    for(int zcount= 0; zcount<z.size() ; zcount++){

      if(z[zcount]<zZero)
	z[zcount]=zZero;

      spdtmp=profile(Ustar, z[zcount], Ri);
      speedtmp = new values(z[zcount],spdtmp);
      wind.push_back(*speedtmp);
    }
    return(true);
  }
  return(false);
}

double Uprofile::gust(double zgust, double spd){

  if(zgust > 50)
    return(1.3 * spd);

  return((( 10 - zgust)*2.5/1000+1.4)*spd);
}

double Uprofile::gust(int i){
  if(wind[i].z > 50)
    return( 1.3 * wind[i].speedOut);

  return((( 10 - wind[i].z)*2.5/1000+1.4)*wind[i].speedOut);
}


void Uprofile::editRi(double rich){
  Ri = rich;
};


void Uprofile::setRi(double mslp, double sst,
		     double thetaN, double zref, double speedZref){

  const double G      = 9.80665;   // grav. acc.
  const double RL     = 287.04;    // Gasconst. ideal gas (air)
  const double CAPPA  = 0.28573;   // RL/cp
  const double P0     = 100000;    // reference pressure (1000hPa)
  const double KELVIN = 273.15;    // 0deg Celsius in Kelvin
  const double AN     = 0.0;       // def of the low. model level
  const double BN     = 0.9961;    // def of the low. model level
  double theta;                    // Pot.Temp average
  double thetaS;                   // Pot.Temp. at surface
  double tN;                       // Temp. at lvl. N
  double pN;                       // pressure at lvl. N
  double dz;                       // z difference (sea lvl and model lvl 31)
  double ddN2;                     // square of speedZref


  if(mslp < 10000)
    mslp *= 100;                   // if Pascal
  if(sst < 150)
    sst += KELVIN;                 // if Celsius
  if(thetaN  < 150)
    thetaN += KELVIN;              // if Celsius


  thetaS = sst*pow(( P0/mslp ), CAPPA );

  pN = AN + BN * mslp;

  tN = thetaN*pow(( pN/P0 ), CAPPA );

  theta = (thetaS + thetaN )/2;

  dz = RL/G * 0.5 * ( tN + sst ) * ( log(mslp) - log(pN) );

  ddN2 = pow(speedZref,2);

  if(ddN2 < 0.01)
    ddN2 = 0.01;

  Ri = G/theta *( (thetaN - thetaS)/dz ) /( ddN2 / pow(zref,2) );
}

void Uprofile::error()
{
  std::cout << errorOut << std::endl;
}
