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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptPatterns.h"
#include <polyStipMasks.h>

using namespace std;
using namespace miutil;


const GLubyte* fillPattern(ptFillStyle fill)
{
  if (fill==NONE)             return empty;
  else if (fill==DIAGRIGHT)   return diagright;
  else if (fill==DIAGLEFT)    return diagleft;
  else if (fill==DIAGCROSS)   return diagcross;
  else if (fill==HORIZONTAL)  return horisontal;
  else if (fill==VERTICAL)    return vertical;
  else if (fill==SHORIZONTAL) return shorisontal;
  else if (fill==SVERTICAL)   return svertical;
  else if (fill==SQUARE)      return square;
  else if (fill==SOLID)       return solid;
  else return square;
}


ptFillStyle Str2FillStyle(miString& buf){
  miString fill = buf.upcase();
  fill.trim(true,true);
  if (fill=="NONE")             return NONE;
  else if (fill=="DIAGRIGHT")   return DIAGRIGHT;
  else if (fill=="DIAGLEFT")    return DIAGLEFT;
  else if (fill=="DIAGCROSS")   return DIAGCROSS;
  else if (fill=="HORIZONTAL")  return HORIZONTAL;
  else if (fill=="VERTICAL")    return VERTICAL;
  else if (fill=="SHORIZONTAL") return SHORIZONTAL;
  else if (fill=="SVERTICAL")   return SVERTICAL;
  else if (fill=="SQUARE")      return SQUARE;
  else if (fill=="SOLID")       return SOLID;
  else return SOLID;
}

vector<ptFillStyle> Str2FillStyleList(miString& fl)
{
  vector<miString> l= fl.split(",");
  int n= l.size();
  vector<ptFillStyle> vf;

  for (int i=0; i<n; i++)
    vf.push_back(Str2FillStyle(l[i]));

  return vf;
}

ptLineStyle Str2LineStyle(miString& buf){
  miString line = buf.upcase();
  line.trim(true,true);
  if (line=="FULL")                return FULL;
  else if (line=="DASHED")         return DASHED;
  else if (line=="DASHDOTTED")     return DASHDOTTED;
  else if (line=="DASHDASHDOTTED") return DASHDASHDOTTED;
  else if (line=="DOTTED")         return DOTTED;
  else return FULL;
}

vector<ptLineStyle> Str2LineStyleList(miString& fl)
{
  vector<miString> l= fl.split(",");
  int n= l.size();
  vector<ptLineStyle> vf;

  for (int i=0; i<n; i++)
    vf.push_back(Str2LineStyle(l[i]));

  return vf;
}

miString FillStyle2Str(ptFillStyle fill){
  if (fill==NONE)             return "NONE";
  else if (fill==DIAGRIGHT)   return "DIAGRIGHT";
  else if (fill==DIAGLEFT)    return "DIAGLEFT";
  else if (fill==DIAGCROSS)   return "DIAGCROSS";
  else if (fill==HORIZONTAL)  return "HORIZONTAL";
  else if (fill==VERTICAL)    return "VERTICAL";
  else if (fill==SHORIZONTAL) return "SHORIZONTAL";
  else if (fill==SVERTICAL)   return "SVERTICAL";
  else if (fill==SQUARE)      return "SQUARE";
  else if (fill==SOLID)       return "SOLID";
  else return "SOLID";
}

miString FillStyleList2Str(vector<ptFillStyle>& vf)
{
  miString s;

  int n= vf.size();
  if (n>0){
    for (int i=0; i<n-1; i++)
      s+= FillStyle2Str(vf[i]) + ",";
    s+= FillStyle2Str(vf[n-1]);
  }
  return s;
}

miString LineStyle2Str(ptLineStyle line){
  if (line==FULL)                return "FULL";
  else if (line==DASHED)         return "DASHED";
  else if (line==DASHDOTTED)     return "DASHDOTTED";
  else if (line==DASHDASHDOTTED) return "DASHDASHDOTTED";
  else if (line==DOTTED)         return "DOTTED";
  else return "FULL";
}

miString LineStyleList2Str(vector<ptLineStyle>& vf)
{
  miString s;

  int n= vf.size();
  if (n>0){
    for (int i=0; i<n-1; i++)
      s+= LineStyle2Str(vf[i]) + ",";
    s+= LineStyle2Str(vf[n-1]);
  }
  return s;
}

