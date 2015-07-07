/*
 * GridInventoryTypes.cc
 *
 *  Created on: Mar 11, 2010
 *      Author: audunc
 */

#include "GridInventoryTypes.h"

#include "puTools/mi_boost_compatibility.hh"

#include <boost/foreach.hpp>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <climits>

#define MILOGGER_CATEGORY "diField.GridInventoryTypes"
#include "miLogger/miLogging.h"

using namespace gridinventory;
using namespace std;

//#define DEBUGPRINT

std::vector<std::string> InventoryBase::getNumericalSortedValues() const
{
  std::vector<float> fvalues;

  std::vector<std::string>::const_iterator itr = values.begin();
  for (; itr != values.end(); ++itr) {
    if (itr->length() > 0) {
      fvalues.push_back(atof(itr->c_str()));
    }
  }

  std::sort(fvalues.begin(), fvalues.end());

  size_t n = fvalues.size();

  std::vector<std::string> sortedvalues;
  for (size_t i = 0; i < n; ++i) {
    ostringstream ost;
    ost <<setprecision(15)<< fvalues[i];
    sortedvalues.push_back(ost.str());
  }

  return sortedvalues;
}

std::vector<std::string> InventoryBase::getIntSortedValues() const
{
  std::vector<int> ivalues;

  std::vector<std::string>::const_iterator itr = values.begin();
  for (; itr != values.end(); ++itr) {
    if (itr->length() > 0) {
      ivalues.push_back(atoi(itr->c_str()));
    }
  }

  std::sort(ivalues.begin(), ivalues.end());

  size_t n = values.size();

  std::vector<std::string> sortedvalues;
  for (size_t i = 0; i < n; ++i) {
    ostringstream ost;
    ost <<setprecision(15)<< ivalues[i];
    sortedvalues.push_back(ost.str());
  }

  return sortedvalues;
}


std::vector<std::string> InventoryBase::getLexicalSortedValues() const
{

  std::vector<std::string> sortedvalues = values;

  std::sort(sortedvalues.begin(), sortedvalues.end());

  return sortedvalues;
}

std::set<long> InventoryBase::getSortedNumbers() const
{
  std::set<long> lvalues;

  std::vector<std::string>::const_iterator itr = values.begin();
  for (; itr != values.end(); ++itr) {
    if (itr->length() > 0) {
      lvalues.insert(atol(itr->c_str()));
    }
  }

  return lvalues;
}

bool InventoryBase::valueExists(std::string value) const
{

  std::vector<std::string>::const_iterator itr = values.begin();
  for (; itr != values.end(); ++itr) {
    if (*itr == value) {
      return true;
    }
  }

  return false;
}

long InventoryBase::getMinStep() const
{

  std::set<long> sorted = getSortedNumbers();

  long minValue = LONG_MAX;

  std::set<long>::const_iterator itr1 =sorted.begin();
  std::set<long>::const_iterator itr2 = sorted.begin();
  if(itr2 != sorted.end())   ++itr2;
  for (; itr2 != sorted.end(); ++itr1, ++itr2) {
    if (minValue > (*itr2 - *itr1) ) {
      minValue = int(*itr2 - *itr1) ;
    }
  }

  return minValue;
}

long Taxis::getForecastLength() const
{

  std::set<long> sorted = getSortedNumbers();
  size_t n = sorted.size();

  if (n<2) {
    return -1;
  }

  std::set<long>::const_iterator itr1 =sorted.begin();
  std::set<long>::const_reverse_iterator itr2 =sorted.rbegin();

  return (*itr2 - *itr1);

}

std::string VCrossSection::sortkey() const
{
  std::ostringstream sk;
  sk << id << '_' << xaxis_name << '_' << yaxis_name << '_';
  if (lons.empty() or lats.empty()) {
    sk << "empty";
  } else {
    const float scale = 1e6;
    sk << int(lons.front()*scale) << '_' << int(lats.front()*scale)
       << '_' << int(lons.back()*scale) << '_' << int(lats.back()*scale);
  }
  return sk.str();
}


static Taxis emptytaxis;

const Taxis& ReftimeInventory::getTaxis(const std::string& name) const
{
  std::set<Taxis>::const_iterator itr = taxes.begin();
  for (; itr != taxes.end(); ++itr) {
    if (itr->name == name ) {
      return *itr;
    }
  }

  // TODO: do we need to return reference to time axis?
  return emptytaxis;

}

static Zaxis emptyzaxis;

const Zaxis& ReftimeInventory::getZaxis(const std::string& name) const
{
  std::set<Zaxis>::const_iterator itr = zaxes.begin();
  for (; itr != zaxes.end(); ++itr) {
    if (itr->name == name ) {
      return *itr;
    }
  }

  return emptyzaxis;

}

static ExtraAxis emptyextraaxis;

const ExtraAxis& ReftimeInventory::getExtraAxis(const std::string& name) const
{
  std::set<ExtraAxis>::const_iterator itr = extraaxes.begin();
  for (; itr != extraaxes.end(); ++itr) {
    if (itr->name == name ) {
      return *itr;
    }
  }

  return emptyextraaxis;

}

Inventory Inventory::merge(const Inventory& i) const
{
  METLIBS_LOG_SCOPE(LOGVAL(reftimes.size()) << LOGVAL(i.reftimes.size()));
  Inventory result;

    // grid and axes: if grid or axis with same id but different values exist -> skip all parameters from this refTimeInventory

    // Exception
    // zaxis, taxis, extraaxis: if axis with same id but different values exist AND the new axis has just one value -> merge new value into the existing axis


  for(Inventory::reftimes_t::const_iterator it_r1 = reftimes.begin(); it_r1 != reftimes.end(); ++it_r1) {
    const ReftimeInventory& r1 = it_r1->second;
    METLIBS_LOG_DEBUG(" checking reftime:" << r1.referencetime);

    ReftimeInventory reftime = r1; // first make a copy of this reference time
    std::map<std::string, ReftimeInventory>::const_iterator r2itr = i.reftimes.find(r1.referencetime);
    if (r2itr != i.reftimes.end()) { // this reference time also exist in i for this model
      METLIBS_LOG_DEBUG("    this reftime is also in 2");
      const ReftimeInventory& r2 = r2itr->second;

      // Check attributes
      std::set<GridParameter> parameters = r2.parameters, newparameters;

      bool axisOk = true; //false if grid or axes not ok

      // Do not merge grids, and do not use grid as part of the parameter id
      // Currently the changes are only commented out.  Corresponding changes have been made in FieldManager


      // - zaxis:
      BOOST_FOREACH(const Zaxis& zax2, r2.zaxes) {
        std::set<Zaxis>::const_iterator zaxis1itr = reftime.zaxes.find(zax2);
        if (zaxis1itr == reftime.zaxes.end()) {
          // add new zaxis
          reftime.zaxes.insert(zax2);
        } else {
          // identical zaxis name, check the other attributes
          if (zaxis1itr->values != zax2.values) {
            // same id, different values
            if(zax2.values.size() == 1){
              //one value, assume merge values
              vector<std::string> newvalues = zaxis1itr->values;
              newvalues.insert(newvalues.begin(), zax2.values.begin(), zax2.values.end());
              Zaxis newzaxis(zaxis1itr->id, zaxis1itr->name, zaxis1itr->vc_type, zaxis1itr->positive, newvalues);
              METLIBS_LOG_DEBUG("     must change zaxis:");
              reftime.zaxes.erase(zaxis1itr); // remove zaxis
              reftime.zaxes.insert(newzaxis); // add zaxis
            } else {
              METLIBS_LOG_WARN("same zaxis:"<<zaxis1itr->id<<" already exist with different values: old size :"
                  <<zaxis1itr->values.size()<<" new size:"<<zax2.values.size());
              axisOk = false;
            }
          }
        }
      }

      // - Taxis
      BOOST_FOREACH(const Taxis& tax2, r2.taxes) {
        std::set<Taxis>::iterator taxis1itr = reftime.taxes.find(tax2);
        if (taxis1itr == reftime.taxes.end()) {
          // add new taxis
          reftime.taxes.insert(tax2);
        } else {
          // identical taxis name, check the other attributes
          if (taxis1itr->values != tax2.values) {
            // same id, different values
            if (tax2.values.size() == 1) {
              //one value, assume merge values
              vector<std::string> newvalues = taxis1itr->values;
              newvalues.insert(newvalues.begin(), tax2.values.begin(), tax2.values.end());
              Taxis newtaxis(taxis1itr->id, taxis1itr->name, newvalues);
              METLIBS_LOG_DEBUG("     must change taxis: old size:" <<taxis1itr->values.size()
                  << " new size: " << newvalues.size());
              reftime.taxes.erase(*taxis1itr); // remove taxis
              reftime.taxes.insert(newtaxis); // add taxis
            } else {
              METLIBS_LOG_WARN("same taxis:"<<taxis1itr->id<<" already exist with different values: old size :"
                  <<taxis1itr->values.size()<<" new size:"<<tax2.values.size());
              axisOk = false;
            }
          }
        }
      } // Taxis end


      // - ExtraAxis
      BOOST_FOREACH(const ExtraAxis& eax2, r2.extraaxes) {
        std::set<ExtraAxis>::iterator eax1it = reftime.extraaxes.find(eax2);
        if (eax1it == reftime.extraaxes.end()) {
          // add new extraaxis
          reftime.extraaxes.insert(eax2);
        } else {
          // identical extraaxis name, check the other attributes
          if (eax1it->values != eax2.values) {
            // same id, different values
            if ( eax2.values.size() == 1 ) {
              //one value, assume merge values
              vector<std::string> newvalues = eax1it->values;
              newvalues.insert(newvalues.begin(),eax2.values.begin(),eax2.values.end());
              ExtraAxis newextraaxis(eax1it->id, eax1it->name, newvalues);
              METLIBS_LOG_DEBUG("     must change extraaxis:");
              reftime.extraaxes.erase(*eax1it); // remove old extraaxis
              reftime.extraaxes.insert(newextraaxis); // add extraaxis
            } else {
              METLIBS_LOG_ERROR("same extraxis:" << eax1it->id <<" already exist with different values: old size :"
                  << eax1it->values.size() << " new size:" << eax2.values.size());
              axisOk = false;
            }
          }
        }
      }

      // add new parameters
      if ( axisOk ) {
        std::set<VCrossSection>& rvc = reftime.verticalCrossSections;
        rvc = r1.verticalCrossSections;
        rvc.insert(r2.verticalCrossSections.begin(), r2.verticalCrossSections.end());

        BOOST_FOREACH(const GridParameter& p, parameters) {
          if (reftime.parameters.find(p) == reftime.parameters.end()) {
            reftime.parameters.insert(p);
          }
        }
      }
    }

    result.reftimes[reftime.referencetime] = reftime; // add
  } // reference time loop

  // add new models from i
  for(Inventory::reftimes_t::const_iterator it_rf = i.reftimes.begin(); it_rf != i.reftimes.end(); ++it_rf) {
    const ReftimeInventory& rf = it_rf->second;
    if (result.reftimes.find(rf.referencetime) == result.reftimes.end()) {
      METLIBS_LOG_DEBUG(" Adding new reftime:" << rf.referencetime);
      result.reftimes[rf.referencetime] = rf;
    }
  }

  return result;
}

std::string Inventory::toString() const
{
  ostringstream out;
  out << "=========================================" << endl;
  out << "Inventory dump" << endl;
  out << "=========================================" << endl;
  std::map<std::string, ReftimeInventory>::const_iterator ritr = reftimes.begin();
  for (; ritr != reftimes.end(); ritr++) {
    out << "     Referencetime:" << ritr->second.referencetime << endl;
      out << "     ====================================" << endl;
      std::set<GridParameter>::const_iterator pitr = ritr->second.parameters.begin();
      for (; pitr != ritr->second.parameters.end(); pitr++) {
        out << "        Parameter:" << pitr->key.name << endl;
        out << "        Grid:" << pitr->grid << ":";
        std::set<Grid>::const_iterator ga = ritr->second.grids.find(pitr->grid);
        if (ga != ritr->second.grids.end()) {
          out << "nx=" << ga->nx << " ny=" << ga->ny << " x_0=" << ga->x_0
              << " y_0=" << ga->y_0 << " x_resolution=" << ga->x_resolution
              << " y_resolution=" << ga->y_resolution << " projection=["
              << ga->projection << "]";
        }
        out << endl;

        out << "        Zaxis:" << pitr->key.zaxis << ":";
        std::set<Zaxis>::const_iterator za = ritr->second.zaxes.find(pitr->key.zaxis);
        if (za != ritr->second.zaxes.end()) {
          out << za->vc_type << ":";
          std::vector<string>::const_iterator litr = za->values.begin();
          for (; litr != za->values.end(); litr++) {
            out << *litr << ", ";
          }
        }
        out << endl;

        out << "        Taxis:" << pitr->key.taxis << ":";
        std::set<Taxis>::const_iterator ta = ritr->second.taxes.find(pitr->key.taxis);
        if (ta != ritr->second.taxes.end()) {
          std::vector<std::string>::const_iterator titr = ta->values.begin();
          for (; titr != ta->values.end(); titr++) {
            out << *titr << ", ";
          }
        }
        out << endl;

        out << "        ExtraAxis:" << pitr->key.extraaxis << ":";
        std::set<ExtraAxis>::const_iterator ra = ritr->second.extraaxes.find(pitr->key.extraaxis);
        if (ra != ritr->second.extraaxes.end()) {
          std::vector<std::string>::const_iterator eitr = ra->values.begin();
          for (; eitr != ra->values.end(); eitr++) {
            out << *eitr << ", ";
          }
        }
        out << endl;

        out << "        Version:" << pitr->key.version << endl;
        out << "        Unit:" << pitr->unit << endl;
        out << "        Standard name:" << pitr->standard_name << endl;
        out << "        Native name:" << pitr->nativename << endl;
        out << "        Native key:" << pitr->nativekey << endl;
        out << "        Calibration:" << pitr->calibration << endl;
        out << "        =================================" << endl;

      }
      out << "     ====================================" << endl;
    }

  return out.str();
}

/**
 * Get timestamp of this inventory
 * returns the latest timestamp in all models and reference times
 * optionally return timestamp for a specific model/reftime
 */
miutil::miTime Inventory::getTimeStamp(const std::string modelname,
    const std::string & reftime) const
{
  miutil::miTime time;//(1000,1,1,0,0,0);
  std::map<std::string, ReftimeInventory>::const_iterator ritr = reftimes.begin();
  for (; ritr != reftimes.end(); ritr++) {
    if (!reftime.empty() && ritr->second.referencetime != reftime){
      continue;
    }
    if (!ritr->second.timestamp.undef() && (time.undef() || ritr->second.timestamp > time)){
      time = ritr->second.timestamp;
    }
  }
  return time;
}

