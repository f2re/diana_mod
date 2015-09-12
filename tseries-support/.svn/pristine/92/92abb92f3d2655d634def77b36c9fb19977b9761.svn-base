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

#ifndef _miSymbol_h
#define _miSymbol_h

#include <puTools/miString.h>
#include <puTools/miTime.h>

#include <vector>

// class to hold a numeric weather identifier, and a string
// with the description of the weather. This part of the symbol
// is customer specific and should be read in  from file
// PU-JS 6/98
// last update 9/98

class code {
  friend class miSymbol;
private:
  int extNum;          // the customers identification Number
  miutil::miString name;       // the name 1of the weather situation
  int vision;          // an identidier for the visibillity in km
  miutil::miString pic;        // an identifier for a picture (file Name or something )
public:
  code(){}
  void AddValues(int ex, miutil::miString nam, int vis, miutil::miString p){
    extNum =ex;
    name = nam;
    vision = vis;     // here is one of these famous vis addings
    pic = p;
  }
  void AddValues(int ex, miutil::miString nam, int vis){
    extNum =ex;
    name = nam;
    vision = vis;     // here is one of these famous vis addings
  }




};

// class to hold a symbol
// which uses  a code for normal daytime and one for darktime
// additional an internal index and the wmo number for verification


class miSymbol {
private:
  int intNum;                 // Internal Number
  bool changeSig;             // Significance check by changing from one sit. to another
  code light;                 // code for normal time
  code dark;                  // code for  darkness
  miutil::miTime tim;                 // time where the symbol is valid
  bool lightstat;             // switch from light to dark
public:
  miSymbol(){}
  miSymbol(const int i , const bool ch)
    : intNum(i),
      changeSig(ch) {}
  void AddCodes(code l, code d){
    light = l;
    dark = d;
  }

  void setBase(const int i , const bool ch) { 
    intNum=i;
    changeSig=ch; 
  }
  void AddErr(miutil::miString errMessage );
  void setLightStat(miutil::miTime,float);
  void setLightStat(bool);
  bool getLightStat()const;
  void setTime(miutil::miTime t){tim = t;};

  int vis();
  int index()const {return intNum;}


  int customNumber()const;
  int customNumber(bool lightState)const;
  bool change() const {return changeSig;};
  miutil::miString customName()const;
  miutil::miString customName( bool lightState )const;
  miutil::miTime getTime() const {return tim;};
  miutil::miString picture();
  miutil::miString picture(bool);
  friend bool operator==( const miSymbol& lhs, const miSymbol& rhs){
    return(lhs.intNum == rhs.intNum);}

  friend bool operator!=( const miSymbol& lhs, const miSymbol& rhs){
    return(lhs.intNum != rhs.intNum);}

};


#endif









