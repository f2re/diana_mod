/*
  libpets2 - presentation and editing of time series

  $Id: ptAxisChildElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptAxisChildElement.cc : Definitions for AxisChildElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptYaxisElement.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>

using namespace miutil;

AxisChildElement::AxisChildElement(yAxisElement* ya,
				   const DataSpec cds,
				   const ptVertFieldf& field,
				   const Layout& layout, XAxisInfo* xtime)
  : dataPlotElement(cds, layout, field, xtime)
  , Yaxis(ya)
  , lineWidth(layout.lineWidth)
  , style(layout.linePattern)
  , quantized(layout.quantized)
  , quantum(layout.quantum)
  , labelOnLine(layout.labelOnLine)
  , lineLabelPos(layout.lineLabelPos)
  , labelside(layout.versides)
  , keepinaxis(layout.keepinaxis)
{
#ifdef DEBUG
  cout << "Inside AxisChildElement's constructor" << endl;
#endif

  //axisText = (layout.text.length()) ? layout.text : data->axisText;
  axisText  = layout.text;
  labelText = layout.text2;

  _informYaxis();
  calcAllDataProperties();
}



// find intersection point (Px,Py) between line-segments AB and CD
// returns true if such exists
// Algorithm from comp.graphics.algorithms FAQ subject 1.03:
//-------------------------------------------------------------------
//     Let A,B,C,D be 2-space position vectors.  Then the directed line
//     segments AB & CD are given by:

//         AB=A+r(B-A), r in [0,1]
//         CD=C+s(D-C), s in [0,1]

//     If AB & CD intersect, then

//         A+r(B-A)=C+s(D-C), or

//         Ax+r(Bx-Ax)=Cx+s(Dx-Cx)
//         Ay+r(By-Ay)=Cy+s(Dy-Cy)  for some r,s in [0,1]

//     Solving the above for r and s yields

//             (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
//         r = -----------------------------  (eqn 1)
//             (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

//             (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
//         s = -----------------------------  (eqn 2)
//             (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

//     Let P be the position vector of the intersection point, then

//         P=A+r(B-A) or

//         Px=Ax+r(Bx-Ax)
//         Py=Ay+r(By-Ay)

//     By examining the values of r & s, you can also determine some
//     other limiting conditions:

//         If 0<=r<=1 & 0<=s<=1, intersection exists
//             r<0 or r>1 or s<0 or s>1 line segments do not intersect

//         If the denominator in eqn 1 is zero, AB & CD are parallel
//         If the numerator in eqn 1 is also zero, AB & CD are collinear.

//     If they are collinear, then the segments may be projected to the x-
//     or y-axis, and overlap of the projected intervals checked.

//     If the intersection point of the 2 lines are needed (lines in this
//     context mean infinite lines) regardless whether the two line
//     segments intersect, then

//         If r>1, P is located on extension of AB
//         If r<0, P is located on extension of BA
//         If s>1, P is located on extension of CD
//         If s<0, P is located on extension of DC

//     Also note that the denominators of eqn 1 & 2 are identical.

//     References:

//     [O'Rourke (C)] pp. 249-51
//     [Gems III] pp. 199-202 "Faster Line Segment Intersection,"
//------------------------------------------------------------------
static bool intersection(float Ax, float Ay, float Bx, float By,
			 float Cx, float Cy, float Dx, float Dy,
			 float& r, float& s, float &Px, float& Py){

  const float slimit= 0.000001;
  float r1= (Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy);
  float s1= (Ay-Cy)*(Bx-Ax)-(Ax-Cx)*(By-Ay);

  float den= (Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx);


  if (fabsf(den)<slimit) {
    return false;
  }

  r= r1/den;
  s= s1/den;

  Px= Ax + r*(Bx-Ax);
  Py= Ay + r*(By-Ay);
//   cerr << "intersecting at Px:" << Px << " Py:" << Py << endl;

  return (r>=0.0 && r<=1.0 && s>=0.0 && s<=1.0);
}




// find crossing between a linesegment P1-P2 and Axis-box
bool AxisChildElement::crossPoint(const float x1, const float y1,
				  const float x2, const float y2,
				  float& cx, float& cy)
{
  float Ax,Ay,Bx,By,Cx,Cy,Dx,Dy,r,s,Px,Py;
  bool res= false;
  Ax= x1; Ay= y1;
  Bx= x2; By= y2;
  Cx= -10000; Dx= 10000;

  if (y1 > stopY || y2 > stopY){
    Cy=Dy= stopY;
  } else {
    Cy=Dy= startY;
  }

  res= intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy,r,s,Px,Py);
  cx= static_cast<int>(Px);
  cy= static_cast<int>(Py);
//   if (!res){
//     cerr << "Found no intersection:" << endl;
//     cerr << "x1:" << x1 << " y1:" << y1 << endl;
//     cerr << "x2:" << x2 << " y2:" << y2 << endl;
//     cerr << "Cy=Dy:" << Cy << endl;
//   }
  return res;
}

// find crossing between a linesegment P1-P2 and horisontal line defined by datavalue d
bool AxisChildElement::dataCrossPoint(const float x1, const float y1,
				      const float x2, const float y2,
				      const float d,
				      float& cx, float& cy)
{
  float Ax,Ay,Bx,By,Cx,Cy,Dx,Dy,r,s,Px,Py;
  bool res= false;
  Ax= x1; Ay= y1;
  Bx= x2; By= y2;
  Cx= -10000; Dx= 10000;

  float Y = startY + deltaY*((d - minPlotY)/plotRange);

  Cy=Dy= Y;

  res= intersection(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy,r,s,Px,Py);
  cx= Px;
  cy= Py;

//   if (!res){
//     cerr << "Found no intersection:" << endl;
//     cerr << "x1:" << x1 << " y1:" << y1 << endl;
//     cerr << "x2:" << x2 << " y2:" << y2 << endl;
//     cerr << "Cy=Dy:" << Cy << endl;
//   }
  return res;
}



AxisChildElement::~AxisChildElement()
{
}

