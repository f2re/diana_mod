/*
 * TestGridIO.cc
 *
 *  Created on: Mar 11, 2010
 *      Author: audunc
 */

#include "TestGridIO.h"

using namespace std;
using namespace miutil;
using namespace gridinventory;

bool TestGridIOsetup::parseSetup(std::vector<std::string>,
    std::vector<std::string>&)
{
  return true;
}

TestGridIO::TestGridIO() :
  GridIO("test-source"), model_name("test-model")
{
}

TestGridIO::TestGridIO(const std::string & modelname, TestGridIOsetup * s) :
  GridIO(), model_name(modelname), setup(s)
{
}

TestGridIO::~TestGridIO()
{
}

vector<std::string> timeseries(std::string reftime)
{
  miTime t = miTime(reftime);
  vector<std::string> st;
  for (int i = 0; i < 36; i += 6) {
    t.addHour(6);
    st.push_back(t.isoTime());
  }
  return st;
}

/**
 * Returns whether the source has changed since the last makeInventory
 */
bool TestGridIO::sourceChanged(bool)
{
  return false;
}


/**
 * Build the inventory from source
 */
bool TestGridIO::makeInventory(const std::string&)
{
  // Initialize inventory
  miutil::miTime tmpTime = miTime::nowTime();
  std::string reftime1 = tmpTime.isoTime();
  tmpTime.addHour(12);
  std::string reftime2 = tmpTime.isoTime();

  if (inventory.reftimes.size() == 0 ) {

    // make two reference times: reftime1 and 2
    ReftimeInventory rinv1(reftime1);
    ReftimeInventory rinv2(reftime2);

    // insert the model
    inventory.clear();
    inventory.reftimes[rinv1.referencetime] = rinv1;
    inventory.reftimes[rinv2.referencetime] = rinv2;
  }


  // make two Taxis objects
  Taxis taxis1("timeaxis1","timeaxis1", timeseries(reftime1));
  Taxis taxis2("timeaxis2","timeaxis2", timeseries(reftime2));

  // make one grid
  Grid grid("1");
  grid.nx = 80;
  grid.ny = 80;
  grid.x_0 = 0.0;
  grid.y_0 = 0.0;
  grid.x_resolution = DEG_TO_RAD;
  grid.y_resolution = DEG_TO_RAD;
  grid.projection = "+proj=latlong +R=6371000.0";

  // make two Zaxis objects,
  // - one "surface" (empty)
  // - one "pressure" with levels: 100, 500 and 850
  Zaxis pressureaxis("pressure");
  pressureaxis.values.push_back("100");
  pressureaxis.values.push_back("500");
  pressureaxis.values.push_back("850");

  Zaxis surfaceaxis("surface");

  // make two ExtraAxis objects,
  // - one empty
  // - one "eps"
  ExtraAxis emptyextraaxis("");

  ExtraAxis epsextraaxis("eps");
  epsextraaxis.values.push_back("1");
  epsextraaxis.values.push_back("2");

  // make three parameters: "mslp", "p" and "mslp(eps)"
  GridParameterKey gridp1key;
  gridp1key.name = "mslp";
  gridp1key.zaxis = surfaceaxis.name;
  gridp1key.extraaxis = emptyextraaxis.name;
  gridp1key.version = "1";
  GridParameter gridp1(gridp1key);
  gridp1.grid = grid.name;
  gridp1.unit = "hpa";
  gridp1.calibration = "a + b.t";
  gridp1.nativename = "nativename";
  gridp1.nativekey = "nativekey";

  GridParameterKey gridp2key;
  gridp2key.name = "p";
  gridp2key.zaxis = pressureaxis.name;
  gridp2key.extraaxis = emptyextraaxis.name;
  gridp2key.version = "1";
  GridParameter gridp2(gridp2key);
  gridp2.grid = grid.name;
  gridp2.unit = "hpa";
  gridp2.calibration = "a + b.t";
  gridp2.nativename = "nativename";
  gridp2.nativekey = "nativekey";

  GridParameterKey gridp3key;
  gridp3key.name = "mslp";
  gridp3key.zaxis = surfaceaxis.name;
  gridp3key.extraaxis = epsextraaxis.name;
  gridp3key.version = "1";
  GridParameter gridp3(gridp3key);
  gridp3.grid = grid.name;
  gridp3.unit = "hpa";
  gridp3.calibration = "a + b.t";
  gridp3.nativename = "nativename";
  gridp3.nativekey = "nativekey";

  // reftime 1 : have all three parameters
  gridp1.key.taxis = taxis1.name;
  gridp2.key.taxis = taxis1.name;
  gridp3.key.taxis = taxis1.name;
  inventory.reftimes[reftime1].grids.insert(grid);
  inventory.reftimes[reftime1].taxes.insert(taxis1);
  inventory.reftimes[reftime1].zaxes.insert(pressureaxis);
  inventory.reftimes[reftime1].zaxes.insert(surfaceaxis);
  inventory.reftimes[reftime1].extraaxes.insert(emptyextraaxis);
  inventory.reftimes[reftime1].extraaxes.insert(epsextraaxis);
  inventory.reftimes[reftime1].parameters.insert(gridp1);
  inventory.reftimes[reftime1].parameters.insert(gridp2);
  inventory.reftimes[reftime1].parameters.insert(gridp3);
  // reftime 2 : only have the first two parameters
  gridp1.key.taxis = taxis2.name;
  gridp2.key.taxis = taxis2.name;
  inventory.reftimes[reftime2].grids.insert(grid);
  inventory.reftimes[reftime2].taxes.insert(taxis2);
  inventory.reftimes[reftime2].zaxes.insert(pressureaxis);
  inventory.reftimes[reftime2].zaxes.insert(surfaceaxis);
  inventory.reftimes[reftime2].extraaxes.insert(emptyextraaxis);
  inventory.reftimes[reftime2].parameters.insert(gridp1);
  inventory.reftimes[reftime2].parameters.insert(gridp2);

  return true;
}

/**
 * Get data slice
 */
Field * TestGridIO::getData(const std::string& reftime, const gridinventory::GridParameter& param,
    const std::string& level, const miutil::miTime& time,
    const std::string& elevel, const std::string& unit)
{
  Field * field = initializeField(model_name, reftime, param, level, time, elevel,unit);

  if (field == 0) {
    return field;
  }

  // fill data etc.

  float dmin = 900, dmax = 1050;
  // modify by time
  dmin += ((dmax - dmin) / 100) * field->forecastHour;
  // modify by level
  int ilevel = 1000;
  if (level != "")
    ilevel = atoi(level.c_str());
  dmin -= (1000 - ilevel);
  dmax -= (1000 - ilevel) / 2;
  // modify by elevel
  int irun = 0;
  if (elevel != "")
    irun = atoi(elevel.c_str());
  dmin += 6 * irun;
  dmax -= 6 * irun;

  const float maxr2 = field->nx * field->nx + field->ny * field->ny;
  for (int y = 0; y < field->ny; ++y) {
    for (int x = 0; x < field->nx; ++x) {
      unsigned int i = y * field->nx + x;
      float r2 = x * x + y * y;
      field->data[i] = dmin + ((dmax - dmin) / maxr2) * r2;
    }
  }

  return field;

}
