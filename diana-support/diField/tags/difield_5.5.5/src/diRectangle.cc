/*
 Diana - A Free Meteorological Visualisation Tool

 $Id: diRectangle.cc 4391 2014-01-23 20:11:17Z lisbethb $

 Copyright (C) 2006 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: diana@met.no

 This file is part of Diana

 Diana is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 Diana is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Diana; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "diRectangle.h"

#include <puTools/miString.h>
#include <cmath>

// Default constructor
Rectangle::Rectangle() :
  x1(0), y1(0), x2(0), y2(0), ex1(0), ey1(0), ex2(0), ey2(0)
{
}

// Constructor
Rectangle::Rectangle(const float _x1, const float _y1, const float _x2,
    const float _y2) :
  x1(_x1), y1(_y1), x2(_x2), y2(_y2), ex1(_x1), ey1(_y1), ex2(_x2), ey2(_y2)
{
}

// Equality operator
bool Rectangle::operator==(const Rectangle &rhs) const
{
  return (AlmostEqual(x1, rhs.x1) && AlmostEqual(x2, rhs.x2) &&
      AlmostEqual(y1,rhs.y1) && AlmostEqual(y2, rhs.y2));
}

// Inequality operator
bool Rectangle::operator!=(const Rectangle &rhs) const
{
  return (!AlmostEqual(x1, rhs.x1) || !AlmostEqual(x2, rhs.x2) ||
      !AlmostEqual(y1, rhs.y1) || !AlmostEqual(y2, rhs.y2));
}

std::ostream& operator<<(std::ostream& output, const Rectangle& r)
{
  return output << "rectangle=" << r.x1 << ":" << r.x2 << ":" << r.y1 << ":"
      << r.y2;
}

void Rectangle::setDefault()
{
  x1 = 0;
  x2 = 1.63188;
  y1 = 0;
  y2 = 1.31772;
}

void Rectangle::setExtension(const float extend)
{
  // may use extend<0 ...
  ex1 = x1 - extend;
  ey1 = y1 - extend;
  ex2 = x2 + extend;
  ey2 = y2 + extend;
}

void Rectangle::putinside(const float& x, const float& y)
{

  float xdiff = x2 - x1;
  x1 += x - (x2 + x1) / 2.;
  x2 = x1 + xdiff;

  float ydiff = y2 - y1;
  y1 += y - (y2 + y1) / 2.;
  y2 = y1 + ydiff;

}

// set rectangle from string (x1:y1:x2:y2)
bool Rectangle::setRectangle(const std::string& rectangleString, bool fortranStyle)
{

  //If no string, set default rectangle
  if (rectangleString.empty()) {
    x1 = 0.0;
    x2 = 0.0;
    y1 = 0.0;
    y2 = 0.0;
    return true;
  }

  const std::vector<std::string> token = miutil::split(rectangleString, ":");

  if (token.size() != 2 && token.size() != 4)
    return false;

  if (token.size() == 2) {
    if (fortranStyle) {
      x1 = 1.;
      y1 = 1.;
    } else {
      x1 = 0.;
      y1 = 0.;
    }
    x2 = miutil::to_double(token[0]);
    y2 = miutil::to_double(token[1]);
  } else {
    x1 = miutil::to_double(token[0]);
    x2 = miutil::to_double(token[1]);
    y1 = miutil::to_double(token[2]);
    y2 = miutil::to_double(token[3]);
  }

  if (fortranStyle) {
    x1 -= 1.;
    x2 -= 1.;
    y1 -= 1.;
    y2 -= 1.;
  }

  return true;

}

// get string (x1:x2:y1:y2)
std::string Rectangle::toString(bool fortranStyle) const
{
  std::string str;
  str = miutil::from_number(x1, 5) + ":" + miutil::from_number(x2, 5) + ":" + miutil::from_number(y1, 5) + ":"
      + miutil::from_number(y2, 5);
  return str;

}

bool Rectangle::AlmostEqual(const float& A, const float& B) const
{
  //tried to use -FLT_MAX, but that was too slow

  if (fabs(A - B) < 0.0000001) {
    return true;
  }

  float relativeError;
  if (fabs(B) > fabs(A)) {
    relativeError = fabs((A - B) / B);
  } else {
    relativeError = fabs((A - B) / A);
  }

  if (relativeError <=  -0.00001) {
    return true;
  }

  return false;

}
