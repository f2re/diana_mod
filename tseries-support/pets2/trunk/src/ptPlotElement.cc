/*
  libpets2 - presentation and editing of time series

  $Id: ptPlotElement.cc 4354 2014-01-02 13:07:57Z juergens $

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


// ptPlotElement.cc : Class definition for generic plotelement

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptPlotElement.h"
#include <glp/svgdocument.h>
#include <iostream>
#include <math.h>

using namespace miutil;

GLPcontext* PlotElement::psoutput=0;    // document factory module

bool PlotElement::printing= false;      // postscript plotting
bool PlotElement::useFakeStipple=false; // imitate linestippling
bool PlotElement::useColour=true;       // colour or black/white

PlotElement::PlotElement(const Layout& layout,
    const ptVertFieldf& field,
    XAxisInfo *pXtime,
    const bool& en,
    PlotElement* pNext)
: type(DUM_PRIMITIVE)
, name(layout.name)
, enabled(en)
, visible(layout.visible)
, startY(field.y1)
, stopY(field.y2)
, color(layout.color)
, xtime(pXtime)
, fontSize(layout.font)
, align(layout.align)
, glwidth(1.0), glheight(1.0)
, scwidth(1), scheight(1)
, pixWidth(1.0), pixHeight(1.0)
, startT(0), stopT(pXtime->xcoord.size()-1)
, AllTimesAxisScale(true)
, pInColour(layout.patternInColour)
, circle_list(0)
, datalimits(layout.datalimits)
, colorlist(layout.colorlist)
, next(pNext) // FIXME public
{
#ifdef DEBUG
  cout << "Inside PlotElement's constructor" << endl;
#endif
}

PlotElement::~PlotElement()
{
  if (circle_list) glDeleteLists(circle_list,1);
}

void PlotElement::setViewport(int sw, int sh, float gw, float gh){
  scwidth = sw;
  scheight = sh;
  glwidth = gw;
  glheight = gh;
  pixWidth = glwidth / scwidth;
  pixHeight = glheight / scheight;
}


void PlotElement::setTimeInterval(const int start, const int stop)
{
  if (start >= 0 && start < xtime->xcoord.size())
    startT = start;
  if (stop >= 0 && stop < xtime->xcoord.size())
    stopT = stop;
}

void PlotElement::lineSegment(const float &x1, const float &y1,
    const float &x2, const float &y2,
    const unsigned int &repeat,
    const unsigned int &bitmask,
    const bool &reset)
{
  const int stip = 16;
  const unsigned int mask[16] = {   1,   2,    4,    8,
      16,  32,   64,  128,
      256, 512, 1024, 2048,
      4096,8192,16384,32768};
  static int bi=0; // static bit-index
  static int ri=0; // static repeat-index
  if (reset) {
    bi = 0;
    ri = 0;
  }

  float dx = x2-x1;
  float dy = y2-y1;
  double L = sqrt(float(dx*dx + dy*dy));
  if (L<0.001) return;

  double xl = dx*1.0/L;
  double yl = dy*1.0/L;

  int l = 0;

  while (l<L) {
    if (mask[bi] & bitmask) // bitmask decides..
      glVertex2f(l*xl+x1, l*yl+y1);
    l++;  // increment line length
    ri++; // increment patternrepeat
    if (ri>=repeat){
      ri=0; // reset repeat
      bi++; // increment bitmask pointer
      if (bi==stip) bi=0; // reset bitmask pointer
    }
  }
}


// plot an ellipse centered in x and y with major axes: rx and ry
void PlotElement::ellipse(const float& x, const float& y,
    const float& rx, const float& ry)
{
  // NOTE: maybe this works ?!
  if (circle_list==0){
    circle_list= glGenLists(1);
    if (circle_list != 0)
      glNewList(circle_list,GL_COMPILE);

    _glBegin(GL_POLYGON, 100);
    for(int i=0;i<100;i++){
      float a  = i*2*M_PI/100.0;
      glVertex2f(cos(a),sin(a));
    }
    _glEnd();
    if (circle_list != 0)
      glEndList();
  }
  glPushMatrix();
  glTranslatef(x,y,0.0);
  glScalef(rx,ry,0.0);
  if (circle_list != 0)
    glCallList(circle_list);
  glPopMatrix();
}


// for postscript output
bool PlotElement::startPSoutput(const miString& fname,
    const bool incolour,
    const bool inlandscape,
    const bool doEPS){
  if (printing) return false;

  int print_options = GLP_FIT_TO_PAGE | GLP_DRAW_BACKGROUND | GLP_AUTO_ORIENT;
  int feedsize= 3000000;
  if (!incolour)
    print_options = print_options | GLP_GREYSCALE;
  bool makeeps= doEPS;

  if (fname.contains(".svg")){
    psoutput = new SvgDocument(const_cast<char*>(fname.c_str()),
        print_options, feedsize,0);
  } else {
    psoutput = new GLPfile(const_cast<char*>(fname.c_str()),
        print_options, feedsize,0,makeeps);
  }

  // set line and point scale
  psoutput->setScales(0.5, 0.5);
  psoutput->StartPage();
  printing= true;

  // inform fontpacks
  FM.startHardcopy(psoutput);
  return true;
}

bool PlotElement::startPSnewpage()
{
  if (!printing || !psoutput) return false;
  glFlush();
  if (psoutput->EndPage() != 0) {
    std::cerr << "startPSnewpage: EndPage BAD!!!" << std::endl;
    return false;
  }
  psoutput->StartPage();
  return true;
}


bool PlotElement::endPSoutput(){
  if (!printing || !psoutput) return false;
  glFlush();
  if (psoutput->EndPage() == 0) {
    delete psoutput;
    psoutput = 0;
  }
  printing= false;

  // inform fontpacks
  FM.endHardcopy();
  return true;
}

void PlotElement::psAddImage(const GLvoid* data,GLint size,GLint nx,GLint ny,
    GLfloat x,GLfloat y,GLfloat sx,GLfloat sy,
    GLint x1,GLint y1,GLint x2,GLint y2,
    GLenum format,GLenum type){
  if (!psoutput || !printing) return;
  psoutput->AddImage(data,size,nx,ny,x,y,sx,sy,x1,y1,x2,y2,format,type);
}

void PlotElement::getRectangle(float& x1,float& y1,
    float& x2,float& y2)
{
  x1= globalWindow.x1;
  x2= globalWindow.x2;
  y1= startY;
  y2= stopY;
}

void PlotElement::getTimeRectangle(float& x1,float& y1,
    float& x2,float& y2)
{
  x1= xtime->xcoord[startT];
  x2= xtime->xcoord[stopT];
  y1= startY;
  y2= stopY;
}

int PlotElement::dataLimitIndex(const float v)
{
  int nd= datalimits.size();
  int i;
  for (i=0; i<nd; i++){
    if ( v < datalimits[i] ){
      break;
    }
  }
  return ( (i==0 ? 0 : i - 1 ) );
}

ptColor PlotElement::colorfromvalue(const float v)
{
  ptColor c;
  int nd= datalimits.size();
  int nc= colorlist.size();
  if ( nc > 0 ) c = colorlist[0];
  int i;
  for (i=0; i<nd; i++){
    if ( v < datalimits[i] ){
      break;
    } else {
      c = ( (i < nc) ? colorlist[i] : colorlist[nc-1] );
    }
  }
  return c;
}

ptColor PlotElement::colorfromindex(const int i)
{
  ptColor c;
  int nc= colorlist.size();
  if ( nc > 0 ) c = colorlist[0];
  if ( i >= 0 ) c = ( (i < nc) ? colorlist[i] : colorlist[nc-1] );
  return c;
}

/*
  B-spline smooth
 */
int PlotElement::smoothline(int npos, float x[], float y[], int nfirst, int nlast,
    int ismooth, float xsmooth[], float ysmooth[])
{
  // Smooth line, make and return spline through points.
  //
  //  input:
  //     x(n),y(n), n=1,npos:   x and y in "window" coordinates
  //     x(nfrst),y(nfsrt):     first point
  //     x(nlast),y(nlast):     last  point
  //     ismooth:               number of points spline-interpolated
  //                            between each pair of input points
  //
  //  method: 'hermit interpolation'
  //     nfirst=0:      starting condition for spline = relaxed
  //     nfirst>0:      starting condition for spline = clamped
  //     nlast<npos-1:  ending   condition for spline = clamped
  //     nlast=npos-1:  ending   condition for spline = relaxed
  //        relaxed  -  second derivative is zero
  //        clamped  -  derivatives computed from nearest points

  int   ndivs, n, ns, i;
  float rdivs, xl1, yl1, s1, xl2, yl2, s2, dx1, dy1, dx2, dy2;
  float c32, c42, c31, c41, fx1, fx2, fx3, fx4, fy1, fy2, fy3, fy4;
  float tstep, t, t2, t3;

  if (npos<3 || nfirst<0 || nfirst>=nlast
      || nlast>npos-1 || ismooth<1) {
    nfirst = (nfirst>0)     ? nfirst : 0;
    nlast  = (nlast<npos-1) ? nlast  : npos-1;
    ns = 0;
    for (n=nfirst; n<=nlast; ++n) {
      xsmooth[ns] = x[n];
      ysmooth[ns] = y[n];
      ++ns;
    }
    return ns;
  }

  ndivs = ismooth;
  rdivs = 1./float(ismooth+1);

  n = nfirst;
  if (n > 0)
  {
    xl1 = x[n]-x[n-1];
    yl1 = y[n]-y[n-1];
    s1  = sqrtf(xl1*xl1+yl1*yl1);
    xl2 = x[n+1]-x[n];
    yl2 = y[n+1]-y[n];
    s2  = sqrtf(xl2*xl2+yl2*yl2);
    dx2 = (xl1*(s2/s1)+xl2*(s1/s2))/(s1+s2);
    dy2 = (yl1*(s2/s1)+yl2*(s1/s2))/(s1+s2);
  }
  else
  {
    xl2 = x[n+1]-x[n];
    yl2 = y[n+1]-y[n];
    s2  = sqrtf(xl2*xl2+yl2*yl2);
    dx2 = xl2/s2;
    dy2 = yl2/s2;
  }

  xsmooth[0] = x[nfirst];
  ysmooth[0] = y[nfirst];
  ns = 0;

  for (n=nfirst+1; n<=nlast; ++n)
  {
    xl1 = xl2;
    yl1 = yl2;
    s1  = s2;
    dx1 = dx2;
    dy1 = dy2;

    if (n < npos-1) {
      xl2 = x[n+1]-x[n];
      yl2 = y[n+1]-y[n];
      s2  = sqrtf(xl2*xl2+yl2*yl2);
      dx2 = (xl1*(s2/s1)+xl2*(s1/s2))/(s1+s2);
      dy2 = (yl1*(s2/s1)+yl2*(s1/s2))/(s1+s2);
    }
    else {
      dx2 = xl1/s1;
      dy2 = yl1/s1;
    }

    // four spline coefficients for x and y
    c32 =  1./s1;
    c42 =  c32*c32;
    c31 =  c42*3.;
    c41 =  c42*c32*2.;
    fx1 =  x[n-1];
    fx2 =  dx1;
    fx3 =  c31*xl1-c32*(2.*dx1+dx2);
    fx4 = -c41*xl1+c42*(dx1+dx2);
    fy1 =  y[n-1];
    fy2 =  dy1;
    fy3 =  c31*yl1-c32*(2.*dy1+dy2);
    fy4 = -c41*yl1+c42*(dy1+dy2);

    // make 'ismooth' straight lines, from point 'n-1' to point 'n'

    tstep = s1*rdivs;
    t = 0.;

    for (i=0; i<ndivs; ++i) {
      t += tstep;
      t2 = t*t;
      t3 = t2*t;
      ns++;
      xsmooth[ns] = fx1 + fx2*t + fx3*t2 + fx4*t3;
      ysmooth[ns] = fy1 + fy2*t + fy3*t2 + fy4*t3;
    }

    ns++;
    xsmooth[ns] = x[n];
    ysmooth[ns] = y[n];
  }

  ns++;

  return ns;
}

// ================ dataPlotElement

void dataPlotElement::setTimeInterval(const int start, const int stop)
{
  PlotElement::setTimeInterval(start,stop);
  if (AllTimesAxisScale) return;
  // scale Y-axis to the data inside this time-interval
  int i1=0;
  for (int i=0; i<startT; i++) if (valid(i)) i1++;
  int i2=i1;
  for (int i=i2; i<stopT; i++) if (valid(i)) i2++;
  i2--;
  if (fdata){
    fdata->setTimeInterval(i1,i2);
  }
}

int dataPlotElement::datastart()// find number of points before startT
{
  int j = 0;
  for (int i = 0; i < startT; i++)
    if (valid(i))
      j++;
  return j;
}

int dataPlotElement::dataend()// find number of points before stopT
{
  int j = 0;
  for (int i = 0; i < stopT; i++)
    if (valid(i))
      j++;
  return j;
}

