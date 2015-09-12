/*
 libpets2 - presentation and editing of time series

 Copyright (C) 2013 met.no

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

// ptEditLineElement.cc : Definitions for EditLineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptEditLineElement.h>
#include <puCtools/puMath.h>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace miutil;
using namespace std;

EditLineElement::EditLineElement(yAxisElement* ya, const DataSpec cds,
                                 const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
    : AxisChildElement(ya, cds, field, layout, xtime)
    , editMode(LE_SINGLE)
    , markerType(LE_RECT)
    , markerSize(layout.size)
    , backcolor(layout.color2)
    , histocolor(layout.color3)
    , printValue(false)
    , printValueLock(layout.printValueLock)
    , modified(false)
    , activenodes(true)
    , pformat(layout.pformat)
    , priority(layout.priority)
    , wrapdegrees(layout.wrapdegrees)
    , wraplimit(layout.wraplimit)
    , editstyle(layout.editstyle)
    , fstyle(layout.fillstyle)
    , pointAlign(layout.align)
    , pMarked(false) // FIXME public
{
#ifdef DEBUG
  cout << "Inside EditLineElement's constructor" << endl;
#endif
  type = EDITLINE;

  // Reset marked points array and
  // accumulated values (for quantized lines)
  for (size_t i = 0; i < xtime->xcoord.size(); i++) {
    markedPoints.push_back(false);
    accuvalues.push_back(0.0);
  }
  // Init undo ringbuffer
  undobuffer = new ring<vector<float> > (20);
  // check pformat
  if (!pformat.length())
    pformat = "%1.2f";
  // if using quantized editing: quantize input-date
  if (quantized) {
    int n = datasize();
    for (int j = 0; j < n; j++)
      setdval(j, puQuantify(dval(j), quantum));
    calcDataProperties();
    // clear dirty flag
    fdata->clearTempDirty();
  }

  if (layout.minValue > -50000)
    minLegal = layout.minValue;
  if (layout.maxValue < 50000)
    maxLegal = layout.maxValue;
}

EditLineElement::~EditLineElement()
{
  delete undobuffer;
}

void EditLineElement::plot()
{

  if (enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "EditLineElement::plot()" << endl;
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _prePlot();
    _setColor(color);

    // plot curve
    float mSizeX = markerSize * pixWidth;
    float mSizeY = markerSize * pixHeight;
    vector<float> dataX, dataY;

    bool fakestipple = false;
    if ((!useColour) || pInColour) {
      if (!useFakeStipple) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(LineStyle[style][0], LineStyle[style][1]);
      } else
        fakestipple = true;
    }
    glLineWidth(lineWidth);
    glPointSize(lineWidth);

    if (editstyle == ES_HISTOGRAM && fstyle != SOLID) {
      glEnable(GL_POLYGON_STIPPLE);
      glPolygonStipple(fillPattern(fstyle));
    }

    float prevx = -2000, prevy;
    bool firstsegment = true;

    if (editstyle == ES_LINE) {
      if (fakestipple)
        glBegin(GL_POINTS);
      else
        glBegin(GL_LINE_STRIP);
    }

    int j = 0;
    int k = 0, i;
    j = datastart();
    int prepoints = j;
    float oldx = -1000, oldy, x1, x2;
    bool drawprevhisto = false, firstisstart;

    for (i = startT; i <= stopT; i++) {
      if (valid(i)) {
        dataX.push_back(xval(i));
        dataY.push_back(yval(j));
        if (editstyle == ES_HISTOGRAM) { // HISTOGRAM
          if (pointAlign == LEFT) { // pointAlign == LEFT
            if (oldx > -999)
              x1 = oldx;
            else
              x1 = dataX[k];
            x2 = dataX[k];

            if ((dataY[k] - startY) > 0) {
              _setColor(histocolor);
              glRectf(x1, startY, x2, dataY[k]);
              _setColor(color);
              if (lineWidth > 0) {
                glBegin(GL_LINE_STRIP);
                glVertex2f(x1, startY);
                glVertex2f(x1, dataY[k]);
                glVertex2f(x2, dataY[k]);
                glVertex2f(x2, startY);
                glEnd();
              }
            }

          } else if (pointAlign == CENTER) { // pointAlign == CENTER
            if (oldx > -999) { // oldx > -999
              float d2 = (dataX[k] - oldx) / 2.0;
              if (drawprevhisto) {
                x1 = oldx;
                if (!firstisstart)
                  x1 -= d2;
                x2 = oldx + d2;
                if ((oldy - startY) > 0) {
                  _setColor(histocolor);
                  glRectf(x1, startY, x2, oldy);
                  _setColor(color);
                  if (lineWidth > 0) {
                    glBegin(GL_LINE_STRIP);
                    glVertex2f(x1, startY);
                    glVertex2f(x1, oldy);
                    glVertex2f(x2, oldy);
                    glVertex2f(x2, startY);
                    glEnd();
                  }
                }
                drawprevhisto = false;
              }

              if (i != startT)
                x1 = dataX[k] - d2;
              else
                x1 = dataX[k];
              if (i != stopT)
                x2 = dataX[k] + d2;
              else
                x2 = dataX[k];

              if ((dataY[k] - startY) > 0) {
                _setColor(histocolor);
                glRectf(x1, startY, x2, dataY[k]);
                _setColor(color);
                if (lineWidth > 0) {
                  glBegin(GL_LINE_STRIP);
                  glVertex2f(x1, startY);
                  glVertex2f(x1, dataY[k]);
                  glVertex2f(x2, dataY[k]);
                  glVertex2f(x2, startY);
                  glEnd();
                }
              }
            } else {
              drawprevhisto = true;
              firstisstart = (i == startT);
              oldy = dataY[k];
            }
          }

        } else if (editstyle == ES_SWITCH) {// SWITCH
          if (lineWidth > 0) {
            if (i != startT)
              x1 = dataX[k] - 20;
            else
              x1 = dataX[k];
            if (i != stopT)
              x2 = dataX[k] + 20;
            else
              x2 = dataX[k];
            glBegin(GL_LINES);
            glVertex2f(x1, dataY[k]);
            glVertex2f(x2, dataY[k]);
            glEnd();
          }

        } else if (editstyle == ES_LINE) { // LINE
          if (fakestipple) {
            if (prevx > -1000) {
              lineSegment(prevx, prevy, dataX[k], dataY[k],
                  LineStyle[style][0], LineStyle[style][1], firstsegment);
              firstsegment = false;
            }
            prevx = dataX[k];
            prevy = dataY[k];
          } else {
            if (wrapdegrees && k > 0 && fabsf(dval(k) - dval(k - 1))
                >= wraplimit) {
              glEnd();
              glBegin(GL_LINE_STRIP);
            }
            glVertex2f(dataX[k], dataY[k]);
          }
        } // editstyle if
        j++;
        k++;
        oldx = xval(i);
      }
    }
    if (editstyle == ES_LINE) {
      glEnd();
    }
    _updatePrinting();

    glDisable(GL_POLYGON_STIPPLE);
    glDisable(GL_LINE_STIPPLE);

    if (activenodes) {
      int act = activePoint - prepoints;
      if (act >= 0 && act < dataX.size()) {
        actPx = dataX[act];
        actPy = dataY[act];
      }
      // Mark each point..
      float ox, oy;
      float sw, sh;
      char svalue[80];
      if (printValue) {
        _prepFont();
      }
      for (i = 0; i < dataX.size(); i++) {
        ox = dataX[i];
        oy = dataY[i];
        // editpoint displaced
        if (i > 0 && editstyle == ES_HISTOGRAM && pointAlign == LEFT) {
          ox = (dataX[i - 1] + dataX[i]) / 2.0;
        }
        if (markedPoints[i + prepoints]) {
          glRectf(ox - mSizeX, oy - mSizeY, ox + mSizeX, oy + mSizeY);
          _updatePrinting();
          if (printValue && activePoint == i + prepoints) {
            if (Yaxis->hasUserLabels()) {
              miString label = Yaxis->userValueLabel(dval(prepoints + i));
              //cerr << "Got label:" << label << " from value:" << dval(prepoints+i) << endl;
              snprintf(svalue, sizeof(svalue), "%s", label.c_str());
            } else {
              snprintf(svalue, sizeof(svalue), pformat.c_str(), dval(prepoints + i));
            }
            _getStringSize(svalue, sw, sh);
            _setColor(backcolor);
            glRectf(ox - 3, oy + mSizeY * 3 - 5, ox + sw + 3, oy + mSizeY * 3
                + sh/*mSizeY*5*/);
            _updatePrinting();
            _setColor(color);
            _printString(svalue, ox, oy + mSizeY * 3);
            _updatePrinting();
          }
        } else {
          _glBegin(GL_LINE_LOOP,5);
          glVertex2f(ox - mSizeX, oy - mSizeY);
          glVertex2f(ox + mSizeX, oy - mSizeY);
          glVertex2f(ox + mSizeX, oy + mSizeY);
          glVertex2f(ox - mSizeX, oy + mSizeY);
          _glEnd();
          _updatePrinting();
        }
      }
    }
    glDisable(GL_BLEND);
  }
}

void EditLineElement::setEditMode(ptLineEditMode em)
{
  editMode = em;
  if (editMode == LE_SINGLE)
    releasePoints();
}

// -------------------------------------------------------------------------
// grabPoint used for:
// - checking for existance of datapoint near x,y
// - toggle marking for point (and mark a whole region)
// mark = true:
//   Returns true if x,y near a datapoint.
//   toggle marking.
//   fillInterval = true:
//     mark all points up to nearest marked neighbour (if any)
// mark = false:
//   Returns true if x,y near a datapoint, AND point is already marked.
// -------------------------------------------------------------------------
bool EditLineElement::grabPoint(float x, float y, bool mark, bool fillInterval)
{
  if (!activenodes)
    return false;
  float sensi = 7 * pixWidth; // sensibility in pixels
  float px, py;
  int i, j, k, l = -1;
  bool aHit = false; // return value
  pMarked = false; // reset flag
  if (quantized)
    _resetAccu(); // reset accumulated edit-values
  l = datastart();
  int dstart = l;
  for (i = startT; i <= stopT; i++) {
    if (valid(i)) {
      px = xval(i); // GL coordinates for current datapoint
      py = yval(l);
      // editpoint displaced
      if (l != dstart && editstyle == ES_HISTOGRAM && pointAlign == LEFT) {
        px = (xval(i - 1) + xval(i)) / 2.0;
      }
      if ((fabs(x - px) < sensi) && (fabs(y - py) < sensi)) { // a hit..
        if (mark) { // marking on, toggle flag..
          //markedPoints[l] = !markedPoints[l];
          markedPoints[l] = true;
          if (markedPoints[l] && fillInterval) { // if marking turned on, check neighbours
            // if fillInterval, check if any other points are marked
            if (l > 0) { // check points to the left
              for (j = l - 1; j >= 0; j--)
                if (markedPoints[j]) {
                  for (k = j; k < l; k++)
                    markedPoints[k] = true;
                  break;
                }
            }
            if (l < xtime->xcoord.size() - 1) { // check points to the right
              for (j = l + 1; j < xtime->xcoord.size(); j++)
                if (markedPoints[j]) {
                  for (k = j; k > l; k--)
                    markedPoints[k] = true;
                  break;
                }
            }
          }
          aHit = true;
          activePoint = l; // remember this point for later moving..
        } else {
          aHit = markedPoints[l]; // return true only if point marked.
          activePoint = l; // remember this point for later moving..
        }
        //activePoint = l; // remember this point for later moving..
        //cout << "ACTIVEPOINT = " << activePoint << endl;
        break;
      }
      //pMarked = pMarked || markedPoints[l]; // Update marked points flag
      l++;
    }
  }
  return aHit;
}

/*
 GrabPoint for keyboard use
 */
bool EditLineElement::grabPoint(int step, bool mark, bool fillInterval)
{
  if (!activenodes)
    return false;
  int i, j, k, l;
  bool aHit = false; // return value
  pMarked = false; // reset flag
  if (quantized)
    _resetAccu(); // reset accumulated edit-values

  int first_point = datastart();
  int last_point = dataend();

  if (activePoint < 0 || activePoint >= markedPoints.size()) {
    activePoint = first_point;
  }

  if (step == 0) {
    if (mark)
      markedPoints[activePoint] = true;
    return true;
  }

  activePoint += step;
  if (activePoint < first_point)
    activePoint = last_point;
  else if (activePoint > last_point)
    activePoint = first_point;

  l = first_point;

  for (i = startT; i <= stopT; i++) {
    if (valid(i)) {
      if (l == activePoint) {
        if (mark) { // marking on, toggle flag..
          markedPoints[l] = true;
          if (markedPoints[l] && fillInterval) { // if marking turned on, check neighbours
            // if fillInterval, check if any other points are marked
            if (l > 0) { // check points to the left
              for (j = l - 1; j >= 0; j--)
                if (markedPoints[j]) {
                  for (k = j; k < l; k++)
                    markedPoints[k] = true;
                  break;
                }
            }
            if (l < xtime->xcoord.size() - 1) { // check points to the right
              for (j = l + 1; j < xtime->xcoord.size(); j++)
                if (markedPoints[j]) {
                  for (k = j; k > l; k--)
                    markedPoints[k] = true;
                  break;
                }
            }
          }
          aHit = true;
        } else {
          aHit = markedPoints[l]; // return true only if point marked.
        }
        // 	cerr << "ACTIVEPOINT = " << activePoint << endl;
        break;
      }
      l++;
    }
  }
  return aHit;
}

// lock/unlock printValue
void EditLineElement::lockValuePrinting(const bool flag)
{
  printValueLock = flag;
}

// set valuePrinting
void EditLineElement::setValuePrinting(const bool flag)
{
  if (!printValueLock)
    printValue = flag;
}

// saveundo saves datavector for later undo
//
void EditLineElement::saveundo()
{
  vector<float> v;
  if (undobuffer) {
    //v = data->data;
    for (int i = 0; i < datasize(); i++)
      v.push_back(dval(i));
    undobuffer->push(v);
  }
}

// undo retrieves latest saved datavector and copies it to data
void EditLineElement::undo()
{
  if (undobuffer->size()) {
    //cout << "Undobuffer.size:" << undobuffer->size() << endl;
    vector<float> v = undobuffer->pop();
    for (int i = 0; i < v.size(); i++)
      setdval(i, v[i]);
    calcDataProperties();
  }
}

// replace data for marked points with data from source
void EditLineElement::replaceDataValues(const WeatherParameter & source){
  vector<float> sd = source.copyDataVector();
  if (sd.size() != datasize())
    return;
  //saveundo();
  for (int i = 0; i < datasize(); i++)
    if (markedPoints[i]) {
      if (_legalValue(sd[i])) {
        setdval(i, sd[i]);
        modified = true;
      }
    }
  calcDataProperties();
}


// try to set marked points to zero
void EditLineElement::zero()
{
  //saveundo();
  for (int i = 0; i < datasize(); i++)
    if (markedPoints[i]) {
      if (_legalValue(0.0)) {
        setdval(i, 0.0);
        modified = true;
      }
    }
  calcDataProperties();
}

// reset accumulated delta-y's
void EditLineElement::_resetAccu()
{
  for (size_t i = 0; i < xtime->xcoord.size(); i++)
    accuvalues[i] = 0.0;
}

// // accumulate delta-y's, and return quantized diff
// void EditLineElement::_accumulate(const int idx, float& diff)
// {
//   if (quantum<0.00001) return;
//   float nv;
//   accuvalues[idx] += diff;
//   diff = quantum*rint(double(accuvalues[idx]/quantum));
//   // stop at modulo quantum
//   if (fabs(diff)>0) {
//     nv= dval(idx)+diff;
//     diff -= fmodf(nv,quantum);
//     accuvalues[idx]=0.0;
//   }
// }

// accumulate delta-y's, and return quantized value
bool EditLineElement::_accumulate(const int idx, float& diff)
{
  if (quantum < 0.00001)
    return false;

  accuvalues[idx] += diff;

  bool res = false;
  float val = dval(idx);
  float retdiff = 0.0;
  int sign = ((accuvalues[idx] < 0.0) ? -1 : 1);
  int vsign = ((val < 0.0) ? -1 : 1);

  float tresh;
  int cstep;

  if (sign == 1) { // increasing accuvalue
    if (vsign == 1) { // current value > 0.0
      cstep = int(val / quantum) + 1; // next step
      tresh = cstep * quantum - val; // diff to next step
    } else { // current value < 0.0
      cstep = int(val / quantum);
      tresh = cstep * quantum - val;
      if (fabs(tresh) < 0.00001) { // if exact value
        cstep += 1;
        tresh = quantum;
      }
    }

    if (accuvalues[idx] >= tresh) {
      res = true;
      //       retdiff = tresh;
      retdiff = cstep * quantum;
      accuvalues[idx] -= tresh;

      int rmod = int(accuvalues[idx] / quantum);
      if (rmod > 0) {
        retdiff += rmod * quantum;
        accuvalues[idx] -= rmod * quantum;
      }
    }
  } else { // decreasing accuvalue
    if (vsign == 1) { // current value > 0.0
      cstep = int(val / quantum); // previous step
      tresh = cstep * quantum - val; // diff to previous step
      if (fabs(tresh) < 0.00001) { // if exact value
        cstep -= 1;
        tresh = -quantum;
      }
    } else { // current value < 0.0
      cstep = int(val / quantum) - 1;
      tresh = cstep * quantum - val;
    }

    if (accuvalues[idx] <= tresh) {
      res = true;
      //       retdiff = tresh;
      retdiff = cstep * quantum;
      accuvalues[idx] -= tresh;

      int rmod = int(accuvalues[idx] / quantum);
      if (rmod < 0) {
        retdiff += rmod * quantum;
        accuvalues[idx] -= rmod * quantum;
      }
    }
  }
  if (res) {
    //     cerr.precision(12);
    //     cerr << "retdiff:" << retdiff << endl;
    diff = puQuantify(retdiff, quantum);
    //     cerr.precision(12);
    //     cerr << "after puQuantify:" << diff << endl;
  }
  return res;
}

// -------------------------------------------------------------------------
// movePoints moves all marked points dy
// should include updating of undo buffer..
// should be able to move points in various "modes" (linear, log...)
// remember: activePoint points to the last grabbed point..
// -------------------------------------------------------------------------
void EditLineElement::movePoints(float dy)
{
  int i, j;
  float diff;
  float grad = plotRange / deltaY;
  float lingrad, value;

  if (!activenodes)
    return;

  switch (editMode) {
  case LE_SINGLE:
  case LE_MULTI:
    for (i = 0; i < datasize(); i++)
      if (markedPoints[i]) {
        diff = dy * grad;
        if (quantized) {
          if (_accumulate(i, diff)) {
            if (_legalValue(diff)) {
              setdval(i, diff);
              modified = true;
            }
          }
        } else {
          if (diff != 0.0 && _legalValue(dval(i) + diff)) {
            setdval(i, dval(i) + diff);
            modified = true;
          }
        }
      }
    break;
  case LE_ROTATE:
    if ((activePoint == xtime->xcoord.size() - 1 || !markedPoints[activePoint
        + 1]) && activePoint > 0) {
      for (i = 0; i < activePoint && !markedPoints[i]; i++)
        ;
      if (markedPoints[i] && i != activePoint)
        for (j = i; j <= activePoint; j++)
          if (markedPoints[j]) {
            diff = float(j - i) / float(activePoint - i) * grad * dy;
            if (quantized) {
              if (_accumulate(j, diff)) {
                if (_legalValue(diff)) {
                  setdval(j, diff);
                  modified = true;
                }
              }
            } else {
              if (diff != 0.0 && _legalValue(dval(j) + diff)) {
                setdval(j, dval(j) + diff);
                modified = true;
              }
            }
          }
    } else if (activePoint == 0 || !markedPoints[activePoint - 1]) {
      for (i = datasize() - 1; i > activePoint && !markedPoints[i]; i--)
        ;
      if (markedPoints[i] && i != activePoint)
        for (j = i; j >= activePoint; j--)
          if (markedPoints[j]) {
            diff = float(i - j) / float(i - activePoint) * grad * dy;
            if (quantized) {
              if (_accumulate(j, diff)) {
                if (_legalValue(diff)) {
                  setdval(j, diff);
                  modified = true;
                }
              }
            } else {
              if (diff != 0.0 && _legalValue(dval(j) + diff)) {
                setdval(j, dval(j) + diff);
                modified = true;
              }
            }
          }
    }
    break;
  case LE_LINEAR:
    if ((activePoint == xtime->xcoord.size() - 1 || !markedPoints[activePoint
        + 1]) && activePoint > 0) {
      for (i = 0; i < activePoint && !markedPoints[i]; i++)
        ;
      if (markedPoints[i] && i != activePoint) {
        diff = grad * dy;
        if (quantized) { // quantized values
          if (_accumulate(activePoint, diff)) {
            _resetAccu();
            if (_legalValue(diff)) {
              setdval(activePoint, diff);
              modified = true;
            }
            lingrad = (dval(activePoint) - dval(i)) / float(activePoint - i);
            for (j = i; j < activePoint; j++) {
              if (markedPoints[j]) {
                value = dval(i) + lingrad * (j - i);
                diff = value - dval(j);
                if (_accumulate(j, diff)) {
                  if (_legalValue(diff)) {
                    setdval(j, diff);
                    modified = true;
                  }
                }
              }
            }
          }
        } else { // not quantized
          if (diff != 0.0) {
            if (_legalValue(dval(activePoint) + diff)) {
              setdval(activePoint, dval(activePoint) + diff);
              modified = true;
            }
            lingrad = (dval(activePoint) - dval(i)) / float(activePoint - i);
            for (j = i; j < activePoint; j++) {
              if (markedPoints[j]) {
                value = dval(i) + lingrad * (j - i);
                diff = value - dval(j);
                if (diff != 0.0 && _legalValue(dval(j) + diff)) {
                  setdval(j, dval(j) + diff);
                  modified = true;
                }
              }
            }
          }
        }
      }
    } else if (activePoint == 0 || !markedPoints[activePoint - 1]) {
      for (i = datasize() - 1; i > activePoint && !markedPoints[i]; i--)
        ;
      if (markedPoints[i] && i != activePoint) {
        diff = grad * dy;
        if (quantized) { // quantized values
          if (_accumulate(activePoint, diff)) {
            _resetAccu();
            if (_legalValue(diff)) {
              setdval(activePoint, diff);
              modified = true;
            }
            lingrad = (dval(activePoint) - dval(i)) / float(i - activePoint);
            for (j = i; j > activePoint; j--) {
              if (markedPoints[j]) {
                value = dval(i) + lingrad * (i - j);
                diff = value - dval(j);
                if (_accumulate(j, diff)) {
                  if (_legalValue(diff)) {
                    setdval(j, diff);
                    modified = true;
                  }
                }
              }
            }
          }
        } else { //
          if (diff != 0.0) {
            if (_legalValue(dval(activePoint) + diff)) {
              setdval(activePoint, dval(activePoint) + diff);
              modified = true;
            }
            lingrad = (dval(activePoint) - dval(i)) / float(i - activePoint);
            for (j = i; j > activePoint; j--) {
              if (markedPoints[j]) {
                value = dval(i) + lingrad * (i - j);
                diff = value - dval(j);
                if (diff != 0.0 && _legalValue(dval(j) + diff)) {
                  setdval(j, dval(j) + diff);
                  modified = true;
                }
              }
            }
          }
        }
      }
    }
    break;
  default:
    break;
  }
  calcDataProperties();
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
void EditLineElement::movePointsQuant(int dy)
{
  if (!activenodes)
    return;
  
  const float diff = quantum * dy;
  for (int i = 0; i < datasize(); i++)
    if (markedPoints[i]) {
      if (diff != 0.0 && _legalValue(dval(i) + diff)) {
        setdval(i, dval(i) + diff);
        modified = true;
      }
    }

  calcDataProperties();
}

void EditLineElement::releasePoints()
{
  size_t j = 0;
  for (size_t i = 0; i < xtime->xcoord.size(); i++) {
    if (valid(i)) {
      markedPoints[j++] = false;
    }
  }
  pMarked = false;
}

void EditLineElement::releaseButton()
{
  if (editMode == LE_SINGLE)
    releasePoints();
}

