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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "symbolMaker.h"

#include <puTools/puMathAlgo.h>

#include <cfloat>
#include <iostream>
#include <fstream>

using namespace std;
using namespace miutil;
using namespace puMathAlgo;

#define SLOG( logmsg ) { \
      if( slog ) { \
         *slog << logmsg; \
      } \
}

namespace {
// initializes the symbols with an
// internal number and the WMO-code
const float DUMMY = -999;
bool fogstatus = true;
bool thunderstatus = false;
bool visiblestatus = false;
bool picturestatus = false;
bool longterm = false;
bool useDynamicRainLimits=false;

vector<miSymbol> symbolbuffer;

// initialise symbol generation at runtime to avoid linker trouble!
void initializeSymbols()
{
   vector<miSymbol> sbuffer(20);

   sbuffer[symbolMaker::Sun]          = miSymbol( symbolMaker::Sun, false );
   sbuffer[symbolMaker::LightCloud]   = miSymbol( symbolMaker::LightCloud, false );
   sbuffer[symbolMaker::PartlyCloud]  = miSymbol( symbolMaker::PartlyCloud, false );
   sbuffer[symbolMaker::Cloud]        = miSymbol( symbolMaker::Cloud, false );
   sbuffer[symbolMaker::Fog]          = miSymbol( symbolMaker::Fog, true );
   sbuffer[symbolMaker::LightRainSun] = miSymbol( symbolMaker::LightRainSun, true );
   sbuffer[symbolMaker::LightRain]    = miSymbol( symbolMaker::LightRain, true );
   sbuffer[symbolMaker::Rain]         = miSymbol( symbolMaker::Rain, true );
   sbuffer[symbolMaker::SleetSun]     = miSymbol( symbolMaker::SleetSun, true );
   sbuffer[symbolMaker::SnowSun]      = miSymbol( symbolMaker::SnowSun, true );
   sbuffer[symbolMaker::Sleet]        = miSymbol( symbolMaker::Sleet, true );
   sbuffer[symbolMaker::Snow]         = miSymbol( symbolMaker::Snow, true );
   sbuffer[symbolMaker::LightRainThunderSun] = miSymbol( symbolMaker::LightRainThunderSun, true );
   sbuffer[symbolMaker::RainThunder]      = miSymbol( symbolMaker::RainThunder, true );
   sbuffer[symbolMaker::SnowThunder]      = miSymbol( symbolMaker::SnowThunder, true );
   sbuffer[symbolMaker::SleetSunThunder]  = miSymbol( symbolMaker::SleetSunThunder, true );
   sbuffer[symbolMaker::SnowSunThunder]   = miSymbol( symbolMaker::SnowSunThunder, true );
   sbuffer[symbolMaker::LightRainThunder] = miSymbol( symbolMaker::LightRainThunder, true );
   sbuffer[symbolMaker::SleetThunder]     = miSymbol( symbolMaker::SleetThunder, true );
   sbuffer[symbolMaker::ErrorSymbol]      = miSymbol( symbolMaker::ErrorSymbol, true );

   symbolbuffer=sbuffer;
}
}

int symbolMaker::MAXcustom = -1000;
int symbolMaker::MINcustom = 1000;

miSymbol
symbolMaker::
createSymbol(enum symbolMaker::Symboltype type)
{
   if( symbolbuffer.size() == 0 )
      initializeSymbols();

   //This should probably never happend. If it does,
   //check that the enum Symboltype is in sync with
   //the size of symbolbuffer.
   if ( static_cast<size_t>(type) > symbolbuffer.size() )
      return symbolbuffer[ ErrorSymbol ];

   return symbolbuffer[type];
}

symbolMaker::Symboltype
symbolMaker::
symbol2type( const miSymbol &symbol )
{
   int n=symbol.index();
   if(  n < 0 || n >= ErrorSymbol )
      return ErrorSymbol;

   return Symboltype( n );
}

int
symbolMaker::
visibility(enum symbolMaker::Symboltype type)
{
   if ( static_cast<size_t>(type) > symbolbuffer.size() || type < 0)
      return 0;
   return symbolbuffer[type].vis();
}


int
symbolMaker::
customNumber(Symboltype type, bool lights)
{
   return createSymbol(type).customNumber(lights);
}

miutil::miString
symbolMaker::
picture(Symboltype type, bool lights)
{
   return createSymbol(type).picture(lights);
}



// reads symbol data from file, and builds the classes for symbols out of it


void
symbolMaker::
readSymbols(const miString &SymbolFname)
{
   initializeSymbols();
   MAXcustom = -1000;
   MINcustom = 1000;

   code normal;
   code darkness;

   ifstream codeFile;
   miString readMi;
   miString darkName, normalName, className;
   miString darkPic, normalPic;
   int darkIndex, normalIndex;
   int Vision;
   vector<miString> words;

   // reading begins here

   codeFile.open(SymbolFname.c_str(), ios::in);

   if (!codeFile)
      cerr << "SymbolMaker error: File " << SymbolFname << " not found" << endl;

   while (codeFile.good()) {

      getline(codeFile, readMi);
      readMi.trim();
      if (readMi.contains("#"))
         continue;

      // flags

      if (readMi.contains("FOGSTATUS"))
         if (readMi.contains("false"))
            fogstatus = false;

      if (readMi.contains("THUNDERSTATUS"))
         if (readMi.contains("true"))
            thunderstatus = true;

      if (readMi.contains("VISIBILITY"))
         if (readMi.contains("true"))
            visiblestatus = true;

      if (readMi.contains("PICTURES"))
         if (readMi.contains("true"))
            picturestatus = true;

      if (readMi.contains("LONGTERM"))
         if (readMi.contains("true"))
            longterm = true;

      if (readMi.contains("DYNAMIC_RAINLIMITS"))
         if (readMi.contains("true"))
            useDynamicRainLimits = true;


      if (readMi == "{") {
         getline(codeFile, className);
         className.trim();
         getline(codeFile, normalName);
         words = normalName.split();
         normalName.replace(words[0], "");
         normalIndex = atoi(words[0].c_str());
         normalName.trim();
         if (picturestatus) {
            getline(codeFile, normalPic);
            normalPic.trim();
         }
         getline(codeFile, darkName);
         darkName.trim();
         words = darkName.split();
         darkName.replace(words[0], "");
         darkIndex = atoi(words[0].c_str());
         darkName.trim();
         if (picturestatus) {
            getline(codeFile, darkPic);
            darkPic.trim();
         }

         // here starts the input of the symboldependet
         // first guess for the vision
         if (visiblestatus)
            codeFile >> Vision;
         else
            Vision = 9;

         if (darkIndex > MAXcustom)
            MAXcustom = darkIndex; // 2000-11-17: normalIndex here!!

         if (normalIndex < MINcustom)
            MINcustom = normalIndex;

         normal.AddValues(normalIndex, normalName, Vision, normalPic);
         darkness.AddValues(darkIndex, darkName, Vision, darkPic);

         // and here it ends
         // compares and sorts the readed data into  the symbols

         if (className == "SUN")
            symbolbuffer[Sun].AddCodes(normal, darkness);

         if (className == "LIGHTCLOUD")
            symbolbuffer[LightCloud].AddCodes(normal, darkness);

         if (className == "PARTLYCLOUD")
            symbolbuffer[PartlyCloud].AddCodes(normal, darkness);

         if (className == "CLOUD")
            symbolbuffer[Cloud].AddCodes(normal, darkness);

         if (className == "FOG")
            symbolbuffer[Fog].AddCodes(normal, darkness);

         if (className == "LIGHTRAINSUN")
            symbolbuffer[LightRainSun].AddCodes(normal, darkness);

         if (className == "LIGHTRAIN")
            symbolbuffer[LightRain].AddCodes(normal, darkness);

         if (className == "RAIN")
            symbolbuffer[Rain].AddCodes(normal, darkness);

         if (className == "SLEETSUN")
            symbolbuffer[SleetSun].AddCodes(normal, darkness);

         if (className == "SNOWSUN")
            symbolbuffer[SnowSun].AddCodes(normal, darkness);

         if (className == "SLEET")
            symbolbuffer[Sleet].AddCodes(normal, darkness);

         if (className == "SNOW")
            symbolbuffer[Snow].AddCodes(normal, darkness);

         if (className == "LIGHTRAINTHUNDERSUN")
            symbolbuffer[LightRainThunderSun].AddCodes(normal, darkness);

         if (className == "RAINTHUNDER")
            symbolbuffer[RainThunder].AddCodes(normal, darkness);

         if (className == "SNOWTHUNDER")
            symbolbuffer[SnowThunder].AddCodes(normal, darkness);

         if (className == "SLEETSUNTHUNDER")
            symbolbuffer[SleetSunThunder].AddCodes(normal, darkness);

         if (className == "SNOWSUNTHUNDER")
            symbolbuffer[SnowSunThunder].AddCodes(normal, darkness);

         if (className == "LIGHTRAINTHUNDER")
            symbolbuffer[LightRainThunder].AddCodes(normal, darkness);

         if (className == "SLEETTHUNDER")
            symbolbuffer[SleetThunder].AddCodes(normal, darkness);

         if (className == "ERROR")
            symbolbuffer[ErrorSymbol].AddCodes(normal, darkness);

      }
   }
   codeFile.close();
}

// sorts the input parameters into the given classes by ident number

bool
symbolMaker::
initializeModel( const vector<paramet> &AllData)
{
   t2m.erase();
   rrf.erase();
   rrk.erase();
   rrt.erase();
   cs.erase();
   cl.erase();
   cm.erase();
   ch.erase();
   ct.erase();
   agr.erase();
   lli.erase();
   fogi.erase();

   for ( vector<paramet>::size_type i = 0; i < AllData.size(); i++) {

      if (AllData[i].paraNO == t2m.paraNO)
         t2m = AllData[i];

      if (AllData[i].paraNO == rrf.paraNO)
         rrf = AllData[i];

      if (AllData[i].paraNO == rrk.paraNO)
         rrk = AllData[i];

      if (AllData[i].paraNO == rrt.paraNO)
         rrt = AllData[i];

      if (AllData[i].paraNO == cs.paraNO)
         cs = AllData[i];

      if (AllData[i].paraNO == cl.paraNO)
         cl = AllData[i];

      if (AllData[i].paraNO == cm.paraNO)
         cm = AllData[i];

      if (AllData[i].paraNO == ch.paraNO)
         ch = AllData[i];

      if (AllData[i].paraNO == ct.paraNO)
         ct = AllData[i];

      if (AllData[i].paraNO == agr.paraNO)
         agr = AllData[i];

      if (AllData[i].paraNO == lli.paraNO)
         lli = AllData[i];

      if (AllData[i].paraNO == fogi.paraNO)
         fogi = AllData[i];

   }

   return true;
}
;

// checks out for periods of rain and computes
// averaged values of the prec. and clouds of the 6 or 12 hours
// round the forecast termin. (dependet on the input)

void
symbolMaker::
periods( const vector<miTime> &termin,
         int min, int max,
         bool compute_minmax)
{

   average<float> avRR(DUMMY);
   average<float> avCL(DUMMY);
   average<float> avCS(DUMMY);
   average<float> avCM(DUMMY);
   average<float> avCH(DUMMY);
   average<float> avCT(DUMMY);

   miTime runTerm;
   miTime stopTerm;
   float rrto, rrtot, rrconv, rrfront;
   float nrr1, nrr0;

   periodOfRain.erase();
   periodOfNoRain.erase();
   rrtotal.erase();
   csurf.erase();
   clow.erase();
   cmiddle.erase();
   cheight.erase();
   ctotal.erase();

   for (vector<miTime>::size_type i = 0; i < termin.size(); i++) {
      nrr0 = 0;
      nrr1 = 0;
      rrto = DUMMY;
      rrtot = DUMMY;

      // compute new min/max
      if (compute_minmax) {

         if (i == 0) {
            runTerm = termin[i];
            runTerm.addHour(-min + 1);
            stopTerm = termin[i];
            stopTerm.addHour(max);
         } else {
            runTerm = stopTerm;
            int diff;
            if (i < termin.size() - 1) { // not the last timepoint
               // rhs cover half the interval to next timepoint
               diff = miTime::hourDiff(termin[i + 1], termin[i]) / 2;
            } else {
               // last timepoint; rhs equal to lhs
               diff = miTime::hourDiff(termin[i], runTerm);
            }
            stopTerm = termin[i];
            stopTerm.addHour(diff);
            runTerm.addHour(1);
         }
         //       cerr << "--runTerm:" << runTerm
         //       << " termin:" << termin[i]
         //       << " stopTerm:" << stopTerm << endl;

      } else {

         runTerm = termin[i];
         stopTerm = termin[i];

         runTerm.addHour(-min);
         stopTerm.addHour(max);
      }

      while (runTerm <= stopTerm) {
         rrtot = rrt.value(runTerm);
         latitude = rrt.lat();

         if (rrtot == DUMMY) {
            rrconv = rrk.value(runTerm);
            rrfront = rrf.value(runTerm);

            if (rrconv != DUMMY && rrfront == DUMMY) {
               rrtot = rrconv;
               latitude = rrk.lat();
            } else if (rrconv == DUMMY && rrfront != DUMMY) {
               rrtot = rrfront;
               latitude = rrf.lat();
            } else if (rrconv != DUMMY && rrfront != DUMMY)
               rrtot = rrfront + rrconv;
         }

         if (rrtot != DUMMY) {
            if (rrto == DUMMY)
               rrto = 0;
            rrto += rrtot;
         }

         if (runTerm == termin[i] || runTerm == stopTerm) {
            if (rrto > rainLimit /*0.6*/)
               nrr1++;
            else if (rrto < noRainLimit /*0.2*/ && rrto > DUMMY)
               nrr0++;
            avRR.add(rrto);
            rrto = DUMMY;
         }

         avCL.add(cl.value(runTerm));
         avCS.add(cs.value(runTerm));
         avCM.add(cm.value(runTerm));
         avCH.add(ch.value(runTerm));
         avCT.add(ct.value(runTerm));

         runTerm.addHour(1);
      }

      periodOfRain.push(termin[i], nrr1);
      periodOfNoRain.push(termin[i], nrr0);
      rrtotal.push(termin[i], avRR.middle());
      csurf.push(termin[i], avCS.middle());
      clow.push(termin[i], avCL.middle());
      cmiddle.push(termin[i], avCM.middle());
      cheight.push(termin[i], avCH.middle());
      ctotal.push(termin[i], avCT.middle());
   }
}
;

// checks out for clouds > 1st step

bool
symbolMaker::
cloudMaker( const miTime &now)
{
   myerror.AddErr("DATA ERROR > NO CLOUD DATA");
   symbol = ErrorSymbol;

   if( ctotal.value( now ) != DUMMY)
      symbol = Sun;

   if( clow.value( now ) != DUMMY && cmiddle.value( now ) != DUMMY &&
        cheight.value( now ) != DUMMY && csurf.value( now ) != DUMMY )
      symbol = Sun;

   if( symbol == ErrorSymbol )
      return false;

   if( ctotal.value( now ) >= 13 )
      symbol = LightCloud;

   if( ctotal.value( now ) > 38 )
      symbol = PartlyCloud;

   if( ctotal.value( now ) > 86 )
      symbol = Cloud;

   if( csurf.value( now ) > 15 || clow.value( now ) > 15 ||
        cmiddle.value( now ) > 15 || cheight.value(now) > 25 )
      symbol = LightCloud;

   if( csurf.value( now ) > 45 || clow.value( now ) > 45 ||
        cmiddle.value( now ) > 45 || cheight.value( now ) > 55)
      symbol = PartlyCloud;

   if( csurf.value( now ) > 86 || clow.value( now ) > 86 ||
        cmiddle.value( now ) > 86 || cheight.value( now ) > 91)
      symbol = Cloud;

   //  if(fogstatus && csurf.value(now) > 75 )
   //     symbol = fog;

   return true;
}


// checks out for precipitation >  2nd step

bool
symbolMaker::
rainMaker( const miTime &now)
{
   myerror.AddErr("DATA ERROR > NO PRECIPITATION DATA");

   float rrNow = rrtotal.value( now );
   float pRain = periodOfRain.value( now );
   float noRain = periodOfNoRain.value( now );

   if (rrtotal.value(now) == DUMMY) {
      symbol = ErrorSymbol;
      return false;
   }

   if (longterm)
      rrNow /= 2;

   // changes: 2002-08-21
   // range 0.1 -> 0.2
   // range 0.3 -> 0.6


   if (rrNow > noRainLimit /*0.2*/ || pRain > 0) {

      if (symbol == Sun || symbol == LightCloud)
         symbol = LightRainSun;

      if (symbol == PartlyCloud) {
         if (rrNow <= rainLimit /*0.6*/ || noRain >= pRain)
            symbol = LightRainSun;
         else
            symbol = LightRain;
      }

      if (symbol == Cloud && rrNow > noRainLimit /*0.2*/)
         symbol = LightRain;

      if (symbol == LightRain && rrNow > 5)
         symbol = Rain;
   }

   return true;
}


// checks out the weather relevant temperatures (checks for snow etc.)
// only relevant if precipitation occurs > 3rd step

bool
symbolMaker::
tempMaker( const miTime &now)
{

   float temperature;

   myerror.AddErr("DATA ERROR > NO TEMPERATURE DATA");

   temperature = t2m.value(now);

   if (temperature == DUMMY) {
      if (symbol == LightRainSun || symbol == LightRain || symbol == Rain) {
         symbol = ErrorSymbol;
         return false;
      }
   }

   if (symbol == LightRainSun) {
      if (temperature <= 0.5)
         symbol = SnowSun;
      if (temperature > 0.5 && temperature < 1.5)
         symbol = SleetSun;
   }

   if (symbol == LightRain || symbol == Rain) {
      if (temperature <= 0.5)
         symbol = Snow;
      if (temperature > 0.5 && temperature < 1.5)
         symbol = Sleet;
   }
   return true;
}


// main program
void
symbolMaker::
rainLimits( int hours, float &noRain, float &rain )
{
   if( ! useDynamicRainLimits ) {
      noRain = 0.2;
      rain = 0.6;
   } else {
      --hours;
      noRain = 0.2 + 0.05*(hours<0?0:hours);
      rain = noRain + 0.4;
   }
}

vector<miSymbol>
symbolMaker::
computeWithoutStateOfAggregate( const vector<paramet> &AllModelData,
                                const vector<miTime> &termin,
                                int min, int max,
                                bool useNewLightningMaker )
{
   vector<miSymbol> resultSymbols;
   miSymbol result;

   if( ! useDynamicRainLimits ) {
      noRainLimit = 0.2;
      rainLimit = 0.6;
   } else {
      int times=min+max-1;
      noRainLimit = 0.2 + 0.05*(times<0?0:times);
      rainLimit = noRainLimit + 0.4;
      SLOG( "symbolMaker::compute: min: " << min << " max: " << max << " periods: " << (min+max-1)
            << " noRainLimit: " << noRainLimit << " rainLimit: " << rainLimit );
   }

   initializeModel( AllModelData );
   periods( termin, min, max );

   for (vector<miTime>::size_type yet = 0; yet < termin.size(); yet++) {
      cloudMaker( termin[yet] );

      if (symbol != ErrorSymbol )
         rainMaker( termin[yet] );

      if( thunderstatus && symbol != ErrorSymbol ) {
         if( useNewLightningMaker )
            lightningMakerNew( symbol, lli.value( termin[yet] ) );
         else
            lightningMaker( symbol, lli.value( termin[yet] ) );
      }

      if( fogstatus && symbol != ErrorSymbol )
         fogMaker( symbol, fogi.value( termin[yet] ) );

      result = createSymbol( symbol );
      result.setTime( termin[yet] );
      result.setLightStat( termin[yet], latitude );
      resultSymbols.push_back( result );
   }

   return resultSymbols;
}


vector<miSymbol>
symbolMaker::
compute( const vector<paramet> &AllModelData,
         const vector<miTime> &termin, int min, int max)
{
   vector<miSymbol> resultSymbols;
   miSymbol result;

   if( ! useDynamicRainLimits ) {
      noRainLimit = 0.2;
      rainLimit = 0.6;
   } else {
      int times=min+max-1;
      noRainLimit = 0.2 + 0.05*(times<0?0:times);
      rainLimit = noRainLimit + 0.4; //diff: 0.6 - 0.2
      SLOG( "symbolMaker::compute: min: " << min << " max: " << max << " periods: " << (min+max-1)
            << " noRainLimit: " << noRainLimit << " rainLimit: " << rainLimit );
   }

   initializeModel( AllModelData );
   periods( termin, min, max );

   for (vector<miTime>::size_type yet = 0; yet < termin.size(); yet++) {
      cloudMaker( termin[yet] );
      if (symbol != ErrorSymbol )
         rainMaker( termin[yet] );
      if ( symbol != ErrorSymbol )
         tempMaker( termin[yet] );

      result = createSymbol( symbol );
      result.setTime( termin[yet] );
      result.setLightStat( termin[yet], latitude );
      resultSymbols.push_back( result );
   }

   return resultSymbols;
}


miSymbol
symbolMaker::
getSymbol(int Cnumber)
{

   miSymbol result;

   for (int testlight = 1; testlight >= 0; testlight--) {
      Symboltype symbolt = getSymboltype(Cnumber, testlight);
      if(symbolt != ErrorSymbol ) {
         result =  createSymbol(symbolt);
         result.setLightStat(testlight);
         return result;
      }
   }

   myerror.AddErr("symbol doesn't exist");
   return myerror;

}


symbolMaker::Symboltype
symbolMaker::
getSymboltype(int Cnumber)
{
   Symboltype symbolt;
   for (int testlight = 1; testlight >= 0; testlight--) {
      symbolt = getSymboltype(Cnumber, testlight);

      if(symbolt != ErrorSymbol )
         return symbolt;
   }
   return ErrorSymbol;
}



symbolMaker::Symboltype
symbolMaker::
getSymboltype(int Cnumber, int testlight)
{
   for( int i=Sun; i < ErrorSymbol; ++i ) {
      if( symbolbuffer[i].customNumber( testlight ) == Cnumber )
         return Symboltype( i );
   }

   return ErrorSymbol;
}




bool
symbolMaker::
signChange(int custom1, int custom2)
{

   miSymbol symbol1, symbol2;

   symbol1 = getSymbol(custom1);
   symbol2 = getSymbol(custom2);

   if (symbol1 == symbol2)
      return false;

   if (!symbol1.change() && !symbol2.change())
      return false;

   return true;
}


bool
symbolMaker::
isDry(int custom)
{
   int weatherType = getSymboltype(custom);

   if (weatherType == Sun || weatherType == LightCloud || weatherType == PartlyCloud ||
         weatherType == Cloud)
      return true;

   return false;
}


bool
symbolMaker::
isPrecip(int custom)
{
   int weatherType = getSymboltype(custom);


   if (weatherType == Rain || weatherType == Sleet || weatherType == LightRain ||
         weatherType == Snow || weatherType == RainThunder || weatherType == SnowThunder ||
         weatherType == SleetSunThunder || weatherType == SnowSunThunder ||
         weatherType == LightRainThunder || weatherType == SleetThunder )
      return true;

   return false;
}

bool
symbolMaker::
isShower(int custom)
{

   int weatherType = getSymboltype(custom);

   if (weatherType == LightRainSun || weatherType == SnowSun || weatherType == SleetSun
         || weatherType == LightRainThunderSun)
      return true;

   return false;
}

void
symbolMaker::
make_pos_symbols( map<int, int>& index,
                  map<int, miString>& image )
{

   index.clear();
   image.clear();

   index[customNumber(ErrorSymbol,true)] = 0;
   index[customNumber(Sun,true)] = 1;
   index[customNumber(LightCloud,true)] = 2;
   index[customNumber(PartlyCloud,true)] = 3;
   index[customNumber(Cloud,true)] = 4;
   index[customNumber(LightRainSun,true)] = 5;
   index[customNumber(LightRainThunderSun,true)] = 6;
   index[customNumber(SleetSun,true)] = 7;
   index[customNumber(SnowSun,true)] = 8;
   index[customNumber(LightRain,true)] = 9;
   index[customNumber(Rain,true)] = 10;
   index[customNumber(RainThunder,true)] = 11;
   index[customNumber(Sleet,true)] = 12;
   index[customNumber(Snow,true)] = 13;
   index[customNumber(SnowThunder,true)] = 14;
   index[customNumber(Fog,true)] = 15;
   index[customNumber(SleetSunThunder,true)] = 20;
   index[customNumber(SnowSunThunder,true)] = 21;
   index[customNumber(LightRainThunder,true)] = 22;
   index[customNumber(SleetThunder,true)] = 23;

   index[customNumber(Sun,false)] = 16;
   index[customNumber(PartlyCloud,false)] = 17;
   index[customNumber(LightRainSun,false)] = 18;
   index[customNumber(SnowSun,false)] = 19;

   image[0] = picture(ErrorSymbol,true);
   image[1] = picture(Sun,true);
   image[2] = picture(LightCloud,true);
   image[3] = picture(PartlyCloud,true);
   image[4] = picture(Cloud,true);
   image[5] = picture(LightRainSun,true);
   image[6] = picture(LightRainThunderSun,true);
   image[7] = picture(SleetSun,true);
   image[8] = picture(SnowSun,true);
   image[9] = picture(LightRain,true);
   image[10] = picture(Rain,true);
   image[11] = picture(RainThunder,true);
   image[12] = picture(Sleet,true);
   image[13] = picture(Snow,true);
   image[14] = picture(SnowThunder,true);
   image[15] = picture(Fog,true);

   image[16] = picture(Sun,false);
   image[17] = picture(PartlyCloud,false);
   image[18] = picture(LightRainSun,false);
   image[19] = picture(SnowSun,false);
   image[20] = picture(SleetSunThunder,true);
   image[21] = picture(SnowSunThunder,true);
   image[22] = picture(LightRainThunder,true);
   image[23] = picture(SleetThunder,true);
}


vector<miSymbol>
symbolMaker::
compute_new(const vector<paramet> &AllModelData,
            const vector<miTime> &termin,
            int min = 3, int max = 6,
            bool compute_minmax)
{
   vector<miSymbol> resultSymbols;
   miSymbol result;

   initializeModel(AllModelData);
   periods(termin, min, max, compute_minmax);

   for (vector<miTime>::size_type yet = 0; yet < termin.size(); yet++) {
      cloudMaker(termin[yet]);
      if (symbol != ErrorSymbol)
         rainMaker(termin[yet]);
      if (symbol != ErrorSymbol)
         stateMaker(termin[yet]);
      if (symbol != ErrorSymbol)
         lightningMaker(termin[yet]);
      if (symbol != ErrorSymbol)
         fogMaker(termin[yet]);

      result = createSymbol(symbol);
      result.setTime(termin[yet]);
      result.setLightStat(termin[yet], latitude);
      resultSymbols.push_back(result);
   }
   return (resultSymbols);
}



int
symbolMaker::
stateOfAggregateFromTemperature(float temperature)
{
   return stateOfAggregateFromTemperature( temperature, AggregateLimits() );
}

int
symbolMaker::
stateOfAggregateFromTemperature( float temperature, const AggregateLimits &limits )
{
   if (temperature == FLT_MAX || temperature <= DUMMY)
         return INT_MAX;

   if (temperature <= limits.snowLimit)
      return 0; //Snow

   if (temperature > limits.snowLimit && temperature < limits.sleetLimit )
      return 1; //Sleet

   return 2; //rain
}

bool
symbolMaker::
stateMaker( const miutil::miTime &termin )
{
   float stateOfAggregate =  agr.value( termin );
   float temperature      = t2m.value( termin );

   if( ! stateMaker( symbol, temperature, stateOfAggregate ) ) {
      myerror.AddErr("DATA ERROR > NO STATE OR TEMPERATURE DATA");
      return false;
   }

   return true;

}

bool
symbolMaker::
stateMaker( symbolMaker::Symboltype &symbol_,
            int state )
{

   // state: 0=snow, 1=sleet, 2=rain

   if (state < 0 || state > 2) {
      symbol_ = ErrorSymbol;
      return false;
   }

   if( symbol_ == LightRainSun ) {
      if (state == 0)
         symbol_ = SnowSun;
      if (state == 1)
         symbol_ = SleetSun;
   } else if( symbol_ == LightRain || symbol_ == Rain ) {
      if (state == 0)
         symbol_  = Snow;
      if (state == 1)
         symbol_ = Sleet;
   } else if( symbol_ == Snow ) {
      if (state == 1)
         symbol_  = Sleet;
      if (state == 2)
         symbol_  = Rain;
   } else if( symbol_ == SnowSun ) {
      if (state == 1)
         symbol_  = SleetSun;
      if (state == 2)
         symbol_  = LightRainSun;
   } else if( symbol_ == Sleet ) {
      if (state == 0)
         symbol_  = Snow;
      if (state == 2)
         symbol_  = Rain;
   } else if( symbol_ == SleetSun ) {
      if (state == 0)
         symbol_  = SnowSun;
      if (state == 2)
         symbol_  = LightRainSun;
   } else if( symbol_ == LightRainThunderSun) {
      if( state == 0 )
         symbol_ = SnowSunThunder;
      else if( state == 1 )
         symbol_ = SleetSunThunder;
   } else if( symbol_  == RainThunder ) {
      if( state == 0 )
         symbol_  = SnowThunder;
      else if( state == 1 )
         symbol_ = SleetThunder;
   } else if( symbol_ == LightRainThunder ) {
      if( state == 0 )
         symbol_  = SnowThunder;
      else if( state == 1 )
         symbol_  = SleetThunder;
   } else if( symbol_ == SleetThunder ) {
      if( state == 0 )
         symbol_  = SnowThunder;
      else if( state == 2 )
         symbol_  = RainThunder;
   } else if( symbol_ == SleetSunThunder ) {
      if( state == 0 )
         symbol_  = SnowSunThunder;
      else if( state == 2 )
         symbol_  = LightRainThunderSun;
   } else if( symbol_ == SnowThunder ) {
      if( state == 1 )
         symbol_  = SleetThunder;
      else if( state == 2 )
         symbol_  = RainThunder;
   } else if( symbol_ == SnowSunThunder ) {
      if( state == 1 )
         symbol_ = SleetSunThunder;
      else if( state == 2 )
         symbol_ = LightRainThunderSun;
   }

   return true;
}

bool
symbolMaker::
stateMaker( symbolMaker::Symboltype &symbol_,
            float temperature,
            const AggregateLimits &limits )
{
   return stateMaker( symbol_, stateOfAggregateFromTemperature(temperature, limits ) );
}

bool
symbolMaker::
stateMaker( symbolMaker::Symboltype &symbol_,
            float temperature,
            float stateOfAggregate )
{
   int state;

   if (stateOfAggregate == FLT_MAX || stateOfAggregate == FLT_MIN
         || stateOfAggregate <= DUMMY)
      state = stateOfAggregateFromTemperature(temperature, AggregateLimits() );
   else
      state = int(stateOfAggregate);

   return stateMaker( symbol_, state );
}

bool
symbolMaker::
stateMaker( miSymbol &symbol__,
            float temperature,
            const AggregateLimits &limits )
{
   miSymbol oldSymbol = symbol__; //Save time and lightstate
   Symboltype symbol_ = symbol2type( oldSymbol );

   bool ret = stateMaker( symbol_, temperature, limits );

   //Restore time and lightstate
   symbol__ = createSymbol( symbol_ );
   symbol__.setTime( oldSymbol.getTime() );
   symbol__.setLightStat( oldSymbol.getLightStat() );

   if( !ret ) {
      symbol__.AddErr("DATA ERROR > NO STATE OR TEMPERATURE DATA");
   }

   return ret;

}



bool
symbolMaker::
stateMaker( miSymbol &symbol__, float temperature,
            float stateOfAggregate )
{
   miSymbol oldSymbol = symbol__; //Save time and lightstate
   Symboltype symbol_ = symbol2type( oldSymbol );

   bool ret = stateMaker( symbol_, temperature, stateOfAggregate );

   //Restore time and lightstate
   symbol__ = createSymbol( symbol_ );
   symbol__.setTime( oldSymbol.getTime() );
   symbol__.setLightStat( oldSymbol.getLightStat() );

   if( !ret ) {
      symbol__.AddErr("DATA ERROR > NO STATE OR TEMPERATURE DATA");
   }

   return ret;
}

bool
symbolMaker::
stateMaker(miSymbol &symbol_, float temperature)
{
  return stateMaker(symbol_, temperature, FLT_MAX);
}


void
symbolMaker::
lightningMaker(const miTime &now)
{
   lightningMaker(symbol, lli.value(now));
}

void
symbolMaker::
lightningMaker(symbolMaker::Symboltype &symbol_, float lightningIndex)
{
   if( symbol_ == ErrorSymbol )
      return;

   if (lightningIndex == FLT_MAX || lightningIndex <= DUMMY)
      return;

   int lightning = int(lightningIndex);

   if (lightning != 1)
      return;

   if( symbol_ == LightRainSun || symbol_ == LightRain || symbol_ == SleetSun )
      symbol_ = LightRainThunderSun;
   else if( symbol_ == Rain )
      symbol_ = RainThunder;
   else if( symbol_ == Snow || symbol_ == SnowSun || symbol_ == Sleet )
      symbol_ = SnowThunder;
   else
      return;
}

void
symbolMaker::
lightningMaker( miSymbol &symbol_,
                float lightningIndex )
{
   Symboltype mySymbol=symbol2type( symbol_ );
   lightningMaker( mySymbol, lightningIndex );
   symbol_ = createSymbol( mySymbol );
}

void
symbolMaker::
lightningMakerNew( symbolMaker::Symboltype &symbol_,
                   float lightningIndex )
{
   if( symbol_ == ErrorSymbol )
      return;

   if (lightningIndex == FLT_MAX || lightningIndex <= DUMMY)
      return;

   int lightning = int(lightningIndex);

   if (lightning != 1)
      return;

   if ( symbol_ == LightRainSun )
      symbol_ = LightRainThunderSun;
   else if( symbol_ == LightRain )
      symbol_ = LightRainThunder;
   else if( symbol_ == SleetSun )
      symbol_ = SleetSunThunder;
   else if(symbol_ == Rain )
      symbol_ = RainThunder;
   else if( symbol_ == Snow )
      symbol_ = SnowThunder;
   else if( symbol_ == SnowSun )
      symbol_ = SnowSunThunder;
   else if( symbol_ == Sleet )
      symbol_ = SleetThunder;
   else
      return;
}


void
symbolMaker::
lightningMakerNew( miSymbol &symbol__,
                   float lightningIndex )
{
   miSymbol oldSymbol = symbol__; //Save time and lightstate
   Symboltype symbol_ = symbol2type( oldSymbol );

   lightningMakerNew( symbol_, lightningIndex );

   //Restore time and lightstate
   symbol__ = createSymbol( symbol_ );
   symbol__.setTime( oldSymbol.getTime() );
   symbol__.setLightStat( oldSymbol.getLightStat() );
}

bool
symbolMaker::
hasThunder( const miSymbol &symbol_ )
{

   if ( symbol_ == LightRainThunderSun ||
         symbol_ == LightRainThunder ||
         symbol_ == SleetSunThunder ||
         symbol_ == RainThunder ||
         symbol_ == SnowThunder ||
         symbol_ == SnowSunThunder ||
         symbol_ == SleetThunder )
      return true;
   else
      return false;
}

void
symbolMaker::
turnOffThunder( miSymbol &symbol__ )
{
   miSymbol oldSymbol = symbol__; //Save time and lightstate
   Symboltype symbol_ = symbol2type( oldSymbol );

   if ( symbol_ == LightRainThunderSun )
      symbol_ = LightRainSun;
   else if( symbol_ == LightRainThunder )
      symbol_ = LightRain;
   else if( symbol_ ==  SleetSunThunder)
      symbol_ = SleetSun;
   else if(symbol_ == RainThunder )
      symbol_ = Rain;
   else if( symbol_ == SnowThunder )
      symbol_ = Snow;
   else if( symbol_ == SnowSunThunder )
      symbol_ = SnowSun;
   else if( symbol_ == SleetThunder )
      symbol_ = Sleet;

   //Restore time and lightstate
   symbol__ = createSymbol( symbol_ );
   symbol__.setTime( oldSymbol.getTime() );
   symbol__.setLightStat( oldSymbol.getLightStat() );
}

void
symbolMaker::
turnOnThunder( miSymbol &symbol__ )
{
   miSymbol oldSymbol = symbol__; //Save time and lightstate
   Symboltype symbol_ = symbol2type( oldSymbol );

   if( symbol_ == Sun ||
         symbol_ == LightRainSun ||
         symbol_ == PartlyCloud )
      symbol_ = LightRainThunderSun;
   else if( symbol_ == Fog ||
         symbol_ == LightCloud ||
         symbol_ == Cloud ||
         symbol_ == LightRain  )
      symbol_ = LightRainThunder;
   else if( symbol_ == Rain)
      symbol_ = RainThunder;
   else if( symbol_ == SleetSun )
      symbol_ = SleetSunThunder;
   else if( symbol_ == SnowSun )
      symbol_ = SnowSunThunder;
   else if( symbol_ == Sleet )
      symbol_ = SleetThunder;
   else if( symbol_ == Snow )
      symbol_ = SnowThunder;
   else
      return;

   //Restore time and lightstate
   symbol__ = createSymbol( symbol_ );
   symbol__.setTime( oldSymbol.getTime() );
   symbol__.setLightStat( oldSymbol.getLightStat() );
}


void
symbolMaker::
fogMaker(miSymbol &symbol_, float fogIndex)
{
   Symboltype mySymbol = symbol2type( symbol_ );
   fogMaker( mySymbol, fogIndex );
   symbol_ = createSymbol( mySymbol );
}


void
symbolMaker::
fogMaker( symbolMaker::Symboltype &symbol_, float fogIndex)
{
   if( symbol_ == ErrorSymbol )
      return;

   if (fogIndex == FLT_MAX || fogIndex <= DUMMY)
      return;

   int foi = int(fogIndex);

   if (foi == 0)
      return;

   symbol_ = Fog;
}



void
symbolMaker::
fogMaker( const miTime &now)
{
   fogMaker( symbol, fogi.value(now) );
}


vector<float>
symbolMaker::
water_state(const vector<float> &temp)
{
   int sz = temp.size();
   vector<float> state;
   float t, s;

   for (int i = 0; i < sz; i++) {
      t = temp[i];

      if (t <= 0.5) {
         s = 0;
      } else if (t >= 1.5) {
         s = 2;
      } else {
         s = 1;
      }
      state.push_back(s);

   }
   return state;

}

bool
operator==( const symbolMaker::Symboltype &lhs, const miSymbol &rhs   )
{
   return lhs == rhs.index();
}


bool
operator==( const miSymbol &lhs, const symbolMaker::Symboltype &rhs )
{
   return rhs==lhs;
}


bool
operator!=( const miSymbol &lhs, const symbolMaker::Symboltype &rhs )
{
   return !(rhs==lhs);
}

bool
operator!=( const symbolMaker::Symboltype &lhs, const miSymbol &rhs )
{
   return !(lhs==rhs);
}
