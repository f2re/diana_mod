/*
  libpets2 - presentation and editing of time series

  $Id: ptFontDefs.cc 4354 2014-01-02 13:07:57Z juergens $

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

#include "ptFontDefs.h"

using namespace std;
using namespace miutil;


ptFontSize Str2FontSize(miString buf){
  miString siz= buf.upcase();
  siz.trim(true,true);
  if (siz=="HEADER1")        return HEADER1;
  else if (siz=="HEADER2")   return HEADER2;
  else if (siz=="HEADER3")   return HEADER3;
  else if (siz=="NORMAL")    return NORMAL;
  else if (siz=="SUBSCRIPT") return SUBSCRIPT;
  else return NORMAL;
}

ptFontFace Str2FontFace(miString buf){
  miString fac= buf.upcase();
  fac.trim(true,true);
  if (fac=="F_NORMAL")      return F_NORMAL;
  else if (fac=="F_BOLD")   return F_BOLD;
  else if (fac=="F_ITALIC") return F_ITALIC;
  else return F_NORMAL;
}

miString FontSize2Str(ptFontSize siz){
  if (siz==HEADER1)        return "HEADER1";
  else if (siz==HEADER2)   return "HEADER2";
  else if (siz==HEADER3)   return "HEADER3";
  else if (siz==NORMAL)    return "NORMAL";
  else if (siz==SUBSCRIPT) return "SUBSCRIPT";
  else return "NORMAL";
}


miString FontFace2Str(ptFontFace fac){
  if (fac==F_NORMAL)      return "F_NORMAL";
  else if (fac==F_BOLD)   return "F_BOLD";
  else if (fac==F_ITALIC) return "F_ITALIC";
  else return "F_NORMAL";
}
