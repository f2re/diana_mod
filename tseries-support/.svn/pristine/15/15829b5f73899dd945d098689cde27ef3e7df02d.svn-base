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


// ptLayout.cc

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptLayout.h"
#include <iostream>

static std::string trimmed_upper(const std::string& text)
{
  return miutil::trimmed(miutil::to_upper(text));
}

ptAxis Str2Axis(const std::string& buf){
  const std::string axis = trimmed_upper(buf);
  if (axis=="LEFTLEFT")        return LEFTLEFT;
  else if (axis=="RIGHTRIGHT") return RIGHTRIGHT;
  else if (axis=="LEFTRIGHT")  return LEFTRIGHT;
  else if (axis=="RIGHTLEFT")  return RIGHTLEFT;
  else if (axis=="NO_AXIS")    return NO_AXIS;
  else return NO_AXIS;
}

ptAlign Str2Align(const std::string& buf){
  const std::string align = trimmed_upper(buf);
  if (align=="CENTER")     return CENTER;
  else if (align=="LEFT")  return LEFT;
  else if (align=="RIGHT") return RIGHT;
  else if (align=="OVER")  return OVER;
  else if (align=="BELOW") return BELOW;
  else return CENTER;
}

ptSize Str2Size(const std::string& buf){
  const std::string size = trimmed_upper(buf);
  if (size=="X_SMALL")      return X_SMALL;
  else if (size=="SMALL")   return SMALL;
  else if (size=="MEDIUM")  return MEDIUM;
  else if (size=="LARGE")   return LARGE;
  else if (size=="X_LARGE") return X_LARGE;
  else if (size=="ZERO")    return ZERO;
  else return ZERO;
}

ptHorSides Str2HorSides(const std::string& buf){
  const std::string side = trimmed_upper(buf);
  if (side=="NONE")       return SH_NONE;
  else if (side=="LEFT")  return SH_LEFT;
  else if (side=="RIGHT") return SH_RIGHT;
  else if (side=="BOTH")  return SH_BOTH;
  else return SH_NONE;
}

ptVerSides Str2VerSides(const std::string& buf){
  const std::string side = trimmed_upper(buf);
  if (side=="NONE")        return SV_NONE;
  else if (side=="TOP")    return SV_TOP;
  else if (side=="BOTTOM") return SV_BOTTOM;
  else if (side=="BOTH")   return SV_BOTH;
  else return SV_NONE;
}

ptEditStyle Str2EditStyle(const std::string& buf){
  const std::string style = trimmed_upper(buf);
  if (style=="LINE")           return ES_LINE;
  else if (style=="HISTOGRAM") return ES_HISTOGRAM;
  else if (style=="SWITCH")    return ES_SWITCH;
  else return ES_LINE;
}

std::vector<std::string> Str2TextLabels(const std::string& buf)
{
  return miutil::split(buf, "|", false);
}

std::vector<float> Str2FloatList(const std::string& buf){
  const std::vector<std::string> vs = miutil::split(buf, ",", true);
  std::vector<float> vf;
  for (int i=0; i<vs.size(); i++)
    vf.push_back(miutil::to_double(vs[i], 0.0));
  return vf;
}

std::vector<int> Str2IntList(const std::string& buf){
  const std::vector<std::string> vs = miutil::split(buf, ",", true);
  std::vector<int> vf;
  for (int i=0; i<vs.size(); i++)
    vf.push_back(miutil::to_int(vs[i], 0));
  return vf;
}

ptDateStyle Str2DateStyle(const std::string& buf){
  const std::string style = trimmed_upper(buf);
  if (style=="DATE")        return DS_DATE;
  else if (style=="DAY")    return DS_DAY;
  else if (style=="WEEK")   return DS_WEEK;
  else if (style=="MONTH") return DS_MONTH;
  else if (style=="YEAR")   return DS_YEAR;
  else return DS_DATE;
}

std::string Axis2Str(ptAxis axis){
  if (axis==LEFTLEFT)        return "LEFTLEFT";
  else if (axis==RIGHTRIGHT) return "RIGHTRIGHT";
  else if (axis==LEFTRIGHT)  return "LEFTRIGHT";
  else if (axis==RIGHTLEFT)  return "RIGHTLEFT";
  else if (axis==NO_AXIS)    return "NO_AXIS";
  else return "NO_AXIS";
}

std::string Align2Str(ptAlign align){
  if (align==CENTER)     return "CENTER";
  else if (align==LEFT)  return "LEFT";
  else if (align==RIGHT) return "RIGHT";
  else if (align==OVER)  return "OVER";
  else if (align==BELOW) return "BELOW";
  else return "CENTER";
}

std::string Size2Str(ptSize size){
  if (size==X_SMALL)      return "X_SMALL";
  else if (size==SMALL)   return "SMALL";
  else if (size==MEDIUM)  return "MEDIUM";
  else if (size==LARGE)   return "LARGE";
  else if (size==X_LARGE) return "X_LARGE";
  else if (size==ZERO)    return "ZERO";
  else return "ZERO";
}

std::string HorSides2Str(ptHorSides side){
  if (side==SH_NONE)       return "NONE";
  else if (side==SH_LEFT)  return "LEFT";
  else if (side==SH_RIGHT) return "RIGHT";
  else if (side==SH_BOTH)  return "BOTH";
  else return "NONE";
}

std::string VerSides2Str(ptVerSides side){
  if (side==SV_NONE)        return "NONE";
  else if (side==SV_TOP)    return "TOP";
  else if (side==SV_BOTTOM) return "BOTTOM";
  else if (side==SV_BOTH)   return "BOTH";
  else return "NONE";
}

std::string EditStyle2Str(ptEditStyle style){
  if (style==ES_LINE)           return "LINE";
  else if (style==ES_HISTOGRAM) return "HISTOGRAM";
  else if (style==ES_SWITCH)    return "SWITCH";
  else return "LINE";
}

std::string TextLabels2Str(const std::vector<std::string>& vs)
{
  std::string s;
  for (int i=0; i<vs.size(); i++)
    s+= vs[i] + "|";
  return s;
}

std::string FloatList2Str(const std::vector<float>& vf)
{
  std::string s;
  for (int i=0; i<vf.size(); i++)
    s+= miutil::from_number(vf[i]) + ",";
  return s;
}

std::string IntList2Str(const std::vector<int>& vf)
{
  std::string s;
  for (int i=0; i<vf.size(); i++)
    s+= miutil::from_number(vf[i]) + ",";
  return s;
}

std::string DateStyle2Str(ptDateStyle style){
  if (style==DS_DATE)        return "DATE";
  else if (style==DS_DAY)    return "DAY";
  else if (style==DS_WEEK)   return "WEEK";
  else if (style==DS_MONTH)  return "MONTH";
  else if (style==DS_YEAR)   return "YEAR";
  else return "DATE";
}

Layout::Layout() :
    visible(true), height(-1), trueheight(0), position(0), spacing(0),
        intSpacing(0), font(NORMAL), lineWidth(1.0), label(true), unit(false),
        patternInColour(false), linePattern(FULL), fillstyle(SOLID),
        align(CENTER), axis(LEFTLEFT), horsides(SH_NONE), versides(SV_NONE),
        yaid(0), axisgrid(false), gridwidth(1), gridcolor("GREY95"),
        gridstyle(DOTTED), axisRectangle(false), gridxonly(true),
        plotDayPattern(false), axisWidth(2.0), minSkipX(0), useTimes(1),
        tickWidth(1.0), tickLen(10), minRange(0), interval(1), delta(1), numTickMajor(5),
        numTickMinor(2), labelSpace(100), horLabels(false), horLabelOffset(0),
        minMargin(0), minValue(-1000000), maxValue(1000000), maxIsSet(false),
        minIsSet(false), useMinMax(false), quantized(false), quantum(1.0),
        reverse(false), centerVector(true), vectorArrow(false),
        lineArrows(false), arrowLength(40), arrowSize(0.25), fromFile(true),
        pformat("%2.1f"), vectorF(false), keepinaxis(false), labelOnLine(false),
        lineLabelPos(50), doubleLine(false), marker(NO_MARKER),
        markerFill(SOLID), size(1), shadow(false), histStart(0), histStop(10),
        fittopage(false), cutoff(-FLT_MAX), priority(0), language("EN"),
        drawbackground(false), editstyle(ES_LINE), printValueLock(false),
        lineBar(false), lineStep(false), asNumber(false), datestyle(DS_DATE),
        legendlineinside(false), colorbyvalue(false), linestylebyvalue(false),
        fillbyvalue(false), smoothing(false), smoothdiv(10), datainknots(true),
        scalewidth(true), wrapdegrees(false), wraplimit(330)
{
}

void Layout::print() const
{
    std::cout << "Printing of layout:\n" << "\theight: " << height << "\tspacing: "
        << spacing << "\tintSpacing: " << intSpacing << "\tcolor: " << color
        << "\tfont: " << font << "\tlineWidth: " << lineWidth << "\tlabel: "
        << label << "\tunit: " << unit << "\tlinePattern: " << linePattern
        << "\tfillstyle: " << fillstyle << "\talign: " << align << "\ttext: "
        << text << /*"\taxis: " << axis <<*/"\taxisWidth: " << axisWidth
        << "\tcolor2: " << color2 << "\ttickWidth: " << tickWidth
        << "\tminRange: " << minRange << "\tinterval: " << interval
        << "\tdelta: " << delta << "\tminMargin: " << minMargin
        << "\tfromFile: " << fromFile << std::endl;
}
