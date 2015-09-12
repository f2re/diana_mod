// -*- c++ -*-
#ifndef _miSliderWidget_h
#define _miSliderWidget_h


/*!
    ------------------------------------
    Created by:
    met.no/FoU/pu/
    at Tue Apr  3 09:42:51 2007
    $Revision$
    at $Date$
    ------------------------------------
*/

#include <QWidget>
#include <string>

QT_BEGIN_NAMESPACE
class QSlider;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class miSliderWidget : public QWidget {
  Q_OBJECT
private:
  QLabel  * desclabel;
  QSlider * slider;
  QLabel  * vallabel;
  QLineEdit * valedit;

  int nsteps;
  float minValue;
  float maxValue;
  float stepValue;
  float Value;
  bool tracking;
  std::string parname;
  bool editfield;
  bool buttons;

  float fValue(int v);
  void writeValue(float V);

public:
  miSliderWidget(float minV,  float maxV, float stepV, float Val,
		 Qt::Orientation orientation, const std::string& descript,
		 const std::string& unit, bool usetracking, QWidget* p, const char * name = 0,
		 bool editf=false, bool btns=false);

  float value();
  void  setValue(float v);

public Q_SLOTS:
  void valueChanged(int v);
  void sliderMoved(int v);
  void editingFinished();

Q_SIGNALS:
  void valueChanged(float);
  void valueChangedForPar(float, const std::string&);
  void sliderReleased();
};

#endif
