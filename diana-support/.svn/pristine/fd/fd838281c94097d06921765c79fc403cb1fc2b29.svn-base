/*
 * GridCollection.cc
 *
 *  Created on: Mar 15, 2010
 *      Author: audunc
 */

#include "GridCollection.h"
#include "GridIO.h"
#ifdef FIMEX
#include "FimexIO.h"
#endif
#include "TestGridIO.h"
#include "diFieldFunctions.h"
#include "TimeFilter.h"

#include <puCtools/puCglob.h>
#include <puTools/miTime.h>
#include <puTools/miString.h>
#include "puTools/mi_boost_compatibility.hh"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#define MILOGGER_CATEGORY "diField.GridCollection"
#include "miLogger/miLogging.h"

using namespace std;
using namespace gridinventory;

GridCollection::GridCollection()
: gridsetup(0)
{
}

// initialize collection from a list of sources
bool GridCollection::setContents(const std::string& type,
    const std::string& name, const vector<std::string>& filenames,
    const vector<std::string>& format,
    const vector<std::string>& config,
    const vector<std::string>& option,
    GridIOsetup* setup)
{
  sourcetype = type;
  collectionname = name;
  gridsetup = setup;

  formats = format;
  configs = config;
  options = option;

  rawsources = filenames;

  // clear GridIO instances
  clearGridSources();

  if (gridsetup == 0){
    METLIBS_LOG_ERROR("setContents: gridsetup is empty");
    return false;
  }
  //return makeGridIOinstances();
  return true;
}

GridCollection::~GridCollection()
{
  clearGridSources();
}

// clear the gridsources vector
void GridCollection::clearGridSources()
{
  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io)
    delete *it_io;
  gridsources.clear();
  inventoryOK.clear();
}

void GridCollection::updateGridSources()
{
}

// unpack the raw sources and make one or more GridIO instances
bool GridCollection::makeGridIOinstances()
{
  METLIBS_LOG_TIME();

  clearGridSources();

  // do not allow empty setup
  if (gridsetup == 0) {
    METLIBS_LOG_ERROR("gridsetup is empty");
    return false;
  }

  bool ok = true;

  // unpack each raw source - creating one or more GridIO instances from each
  int index = -1;
  BOOST_FOREACH(std::string sourcestr, rawsources) {
    ++index;
    sources.clear();
    refTimes.clear();
    sources_with_wildcards.clear();

    // init time filter and replace yyyy etc. with ????
    TimeFilter tf;
    std::string part1, part2=sourcestr;
    if(sourcestr.find("/") != sourcestr.npos) {
      part1 = sourcestr.substr(0,sourcestr.find_last_of("/")+1);
      part2 = sourcestr.substr(sourcestr.find_last_of("/")+1,sourcestr.size()-1);
    }
    tf.initFilter(part2,true);
    sourcestr = part1 + part2;
    // check for wild cards - expand filenames if necessary
    if (sourcestr.find_first_of("*?") != sourcestr.npos && sourcestr.find("glob:") == sourcestr.npos) {
      sources_with_wildcards.push_back(sourcestr);
      glob_t globBuf;
      glob(sourcestr.c_str(), GLOB_BRACE, 0, &globBuf);
      for (size_t k = 0; k < globBuf.gl_pathc; k++) {
        const std::string path = globBuf.gl_pathv[k];
        sources.insert(path);
      }
      globfree(&globBuf);
    } else {
      sources.insert(sourcestr);
    }

    //if #formats == #rawsources, use corresponding files. If not use first format
    std::string format;
    if ( rawsources.size() == formats.size() ) {
      format = formats[index];
    } else if ( formats.size() > 0) {
      format = formats[0];
    }

    //if #configs == #rawsources, use corresponding files. If not use first config
    std::string config;
    if ( rawsources.size() == configs.size() ) {
      config = configs[index];
    } else if ( configs.size() > 0) {
      config = configs[0];
    }

    // loop through sources (filenames)
    BOOST_FOREACH(const std::string& sourcename, sources) {
      const std::string reftime_from_filename = tf.getTimeStr(sourcename);
      // new source - add them according to the source type
#ifdef FIMEX
      if (sourcetype == FimexIO::getSourceType()) {
        gridsources.push_back(new FimexIO(collectionname, sourcename, reftime_from_filename, format, config,
            options, static_cast<FimexIOsetup*> (gridsetup)));
        continue;
      }
#endif
      if (sourcetype == TestGridIO::getSourceType()) {
        gridsources.push_back(new TestGridIO(collectionname,
            static_cast<TestGridIOsetup*> (gridsetup)));
        continue;
      }
      METLIBS_LOG_ERROR("unknown type:" << sourcetype << " for source:" << sourcename);
      ok = false;
    }
  }

  return ok;
}

bool GridCollection::makeInventory(const std::string& refTime,
    bool updateSourceList)
{
  METLIBS_LOG_TIME();

  bool ok = true;
  // decide if we should make new GridIO instances
  if (gridsources.empty() || updateSourceList) {
    ok = makeGridIOinstances();
  }
  // make inventory on all sources and make a combined inventory for the whole collection
  inventoryOK.clear();
  inventory.clear();

  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io) {
    // enforce the reference time limits
    //    (*itr)->setReferencetimeLimits(limit_min, limit_max); // not used yet

    // make inventory on each GridIO
    bool result = false;

    // make referencetime inventory from referencetime given in filename, if possible
    if (refTime.empty()) {
      const std::string reftime_from_filename = (*it_io)->getReferenceTime();
      if (!reftime_from_filename.empty()) {
        result = true;
        refTimes.insert(reftime_from_filename);
      }
    }

    if (!result && (*it_io)->referenceTimeOK(refTime)) {
      result = (*it_io)->makeInventory(refTime);
      inventoryOK[refTime] = result;
      if (result)
        inventory = inventory.merge((*it_io)->getInventory());
    }
    ok |= result;
  }

  if (not ok)
    METLIBS_LOG_WARN("makeInventory failed for GridIO with source:" << refTime);
  return ok;
}

/**
 * Add derived zaxes defined in setup
 */
void GridCollection::addDerivedZaxes()
{
  using namespace gridinventory;
    Inventory::reftimes_t& reftimes = expanded_inventory.reftimes;
    for(Inventory::reftimes_t::iterator it_r = reftimes.begin(); it_r != reftimes.end(); ++it_r) {
      ReftimeInventory& rti = it_r->second;
      BOOST_FOREACH(const Zaxis& zaxis, rti.zaxes) {
        // does this zaxis have a derived axis?
        if (FieldFunctions::Zaxis_info_map.count(zaxis.verticalType)
            and not FieldFunctions::Zaxis_info_map[zaxis.verticalType].derivedZaxis.empty())
        {
          // make new axis
          // calc. new levels, at present only pLevel2flightLevel available
          std::vector<std::string> flightlevels;
          BOOST_FOREACH(const std::string& level, zaxis.values) {
            if (FieldFunctions::pLevel2flightLevel.find(level) != FieldFunctions::pLevel2flightLevel.end())
              flightlevels.push_back(FieldFunctions::pLevel2flightLevel[level]);
          }

          std::string name = FieldFunctions::Zaxis_info_map[zaxis.name].derivedZaxis;
          std::string id = zaxis.id;
          boost::replace_first(id, zaxis.name, name);
          int vc_type = zaxis.vc_type;
          bool positive = false;
          std::string nativeName = zaxis.verticalType;
          std::string nativeZ = zaxis.name;
          Zaxis FLaxis = Zaxis(id, name, vc_type, positive,
              flightlevels, nativeName,"flightlevel");
          rti.zaxes.insert(FLaxis); // does not invalidate iterators, see doc for std::set

          //add parameters
          BOOST_FOREACH(const GridParameter& par, rti.parameters) {
            if (par.key.zaxis == nativeZ) {
              GridParameter newparameter = par;
              newparameter.key.zaxis = name;
              newparameter.zaxis_id = id;
              rti.parameters.insert(newparameter); // does not invalidate iterators, see doc for std::set
            }
          }
        }
      }
    }

}

bool GridCollection::sourcesChanged()
{
  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io)
    if ((*it_io)->sourceChanged(false))
      return true;
  return false;
}

std::vector<gridinventory::Inventory> GridCollection::getInventories() const
{
  std::vector<gridinventory::Inventory> invs;
  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io)
    invs.push_back((*it_io)->getInventory());
  return invs;
}

/**
 * Get the grid
 */
gridinventory::Grid GridCollection::getGrids() const
{
  METLIBS_LOG_SCOPE();

  gridinventory::Grid grid;
  Inventory::reftimes_t::const_iterator it_r = inventory.reftimes.begin();
  if ( it_r != inventory.reftimes.end() ) {
    if (not it_r->second.grids.empty()) {
      grid = *it_r->second.grids.begin();
    }
  }
  return grid;
}

/**
 * Get the reference times
 */
std::set<std::string> GridCollection::getReferenceTimes() const
{
  if (not refTimes.empty())
    return refTimes;
  std::set<std::string> reftimes;
  const Inventory::reftimes_t& m_reftimes = inventory.reftimes;
  for(Inventory::reftimes_t::const_iterator it_r = m_reftimes.begin(); it_r != m_reftimes.end(); ++it_r) {
    reftimes.insert(it_r->first);
  }
  return reftimes;
}

/**
 * Check if data exists
 */
bool GridCollection::dataExists(const std::string& reftime, const std::string& paramname,
    const std::string& zaxis,
    const std::string& taxis, const std::string& extraaxis,
    const std::string& version, const std::string& level,
    const miutil::miTime& time, const std::string& elevel,
    const int & time_tolerance, gridinventory::GridParameter & param,
    miutil::miTime & actualtime)
{
  METLIBS_LOG_DEBUG("searching for: " <<paramname<<" : "<< zaxis<<" : "<< taxis<<" : "<< extraaxis<< " : "<< version);
  gridinventory::GridParameterKey gp(paramname, zaxis, taxis, extraaxis, version);
  return dataExists_gridParameter(expanded_inventory, reftime, gp, level, time, elevel, time_tolerance, param, actualtime);
}

bool GridCollection::dataExists_gridParameter(const gridinventory::Inventory& inv,
    const std::string& reftime, const gridinventory::GridParameterKey& parkey,
    const std::string& level, const miutil::miTime& time,
    const std::string& elevel, const int & time_tolerance,
    gridinventory::GridParameter & param, miutil::miTime & actualtime)
{
  METLIBS_LOG_SCOPE();

  using namespace gridinventory;

  METLIBS_LOG_DEBUG("searching for" << LOGVAL(reftime));
  const map<std::string, ReftimeInventory>::const_iterator ritr = inv.reftimes.find(reftime);
  if (ritr == inv.reftimes.end())
    return false;
  const ReftimeInventory& reftimeInv = ritr->second;

  METLIBS_LOG_DEBUG("searching for parameter:" << parkey.name << "|" << parkey.extraaxis
      << "|" <<parkey.taxis<< "|" <<parkey.zaxis);
  BOOST_FOREACH(const gridinventory::GridParameter& p, reftimeInv.parameters) {
  //  METLIBS_LOG_DEBUG("checking against:" << p.key.name << "|" << p.key.extraaxis
   //     << "|" <<p.taxis_id<< "|" <<p.key.zaxis);
    if ((p.key == parkey) or
        (p.key.name == parkey.name))
    {
      // found parameter - check specifics

      // Zaxis
      //if zaxis_native_name (name of zaxis with one level) is given, then search for this name
      std::string zaxis = p.key.zaxis;
      if ( p.zaxis_native_name == parkey.zaxis )
        zaxis = p.zaxis_native_name;
      METLIBS_LOG_DEBUG("searching for z axis '" << zaxis << "'");
      const std::set<Zaxis>::const_iterator zaitr = miutil::find_if(reftimeInv.zaxes, boost::bind(&Zaxis::getName, _1) == zaxis);
      if (zaitr == reftimeInv.zaxes.end()) {
          continue;
      }
      METLIBS_LOG_DEBUG(" => found '" << zaitr->name << "'");

      if ((not zaitr->values.empty()) and /*FIXME*/ (not level.empty())) { // check for correct level if appropriate
        if (std::find(zaitr->values.begin(), zaitr->values.end(), level) == zaitr->values.end())
          continue;
      }

      if(!time.undef()) {
        // Taxis
        METLIBS_LOG_DEBUG("searching for" << LOGVAL(p.key.taxis));

        const std::set<Taxis>::const_iterator taitr = miutil::find_if(reftimeInv.taxes, boost::bind(&Taxis::getId, _1) == p.taxis_id);

        METLIBS_LOG_DEBUG("testing timeaxis:" << taitr->name << " with " << taitr->values.size() << " times");

        if (taitr->values.size() > 0) { // check for correct time if appropriate
          vector<std::string>::const_iterator titr = taitr->values.begin();
          // find nearest time within time tolerance
          long int mdiff = 100000000;
          titr = taitr->values.begin();
          bool found = false;
          for (; titr != taitr->values.end(); ++titr) {
            time_t t = atoi((*titr).c_str());
            miutil::miTime mit( t );
            long int d = labs(miutil::miTime::minDiff(time, mit));
            if (d < mdiff && d <= time_tolerance) {
              mdiff = d;
              actualtime = mit;
              found = true;
              break;
            }
          }
          if (!found) {
            continue;
          }
        }
      }

      // Extraaxis
      const std::set<ExtraAxis>::const_iterator xaitr = miutil::find_if(reftimeInv.extraaxes, boost::bind(&ExtraAxis::getName, _1) == p.key.extraaxis);
      if (xaitr == reftimeInv.extraaxes.end()) {
        METLIBS_LOG_DEBUG("extra axis '" << p.key.extraaxis << "' not found");
        continue;
      }
      if (not xaitr->values.empty() and elevel != "") { // check for correct elevel if appropriate
        const std::vector<std::string>::const_iterator eitr = std::find(xaitr->values.begin(), xaitr->values.end(), elevel);
        if (eitr == xaitr->values.end()) {
          METLIBS_LOG_DEBUG("level '" << elevel << "' on extra axis '" << p.key.extraaxis << "' not found");
          continue;
        }
      }

      METLIBS_LOG_DEBUG("found param :-)  "<<p.key.name<<" :: "<<p.key.zaxis );
      param = p; // make copy
      return true;
    }
  }

  return false;
}

/**
 * Get data slice
 */
Field * GridCollection::getData(const std::string& reftime, const std::string& paramname,
    const std::string& zaxis,
    const std::string& taxis, const std::string& extraaxis,
    const std::string& version, const std::string& level,
    const miutil::miTime& time, const std::string& elevel,
    const std::string& unit,
    const int & time_tolerance, miutil::miTime & actualtime)
{
  METLIBS_LOG_SCOPE(reftime << " | " << paramname << " | " << zaxis
      << " | " << taxis << " | " << extraaxis << " | " << version << " | "
      << level << " | " << time << " | " << elevel << "|" << time_tolerance);
  gridinventory::GridParameterKey gp(paramname, zaxis, taxis, extraaxis, version);
  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io) {
    gridinventory::GridParameter param;
    if (dataExists_gridParameter((*it_io)->getInventory(), reftime, gp, level, time, elevel, time_tolerance, param, actualtime) )
    {
      // data exists ... calling getData
      return (*it_io)->getData(reftime, param, level, actualtime, elevel, unit);
    }

  }
  METLIBS_LOG_WARN("giving up .. returning 0");
  return 0;
}

/**
 * Get data slice
 */
vcross::Values_p GridCollection::getVariable(const std::string& reftime, const std::string& paramname)
{
  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io) {
    return (*it_io)->getVariable(paramname);
  }
  METLIBS_LOG_WARN("giving up .. returning 0");
  return vcross::Values_p();
}

/**
 * Put data slice
 */
bool GridCollection::putData(const std::string& reftime, const std::string& paramname,
    const std::string& zaxis,
    const std::string& taxis, const std::string& extraaxis,
    const std::string& version, const std::string& level,
    const miutil::miTime& time, const std::string& elevel,
    const std::string& unit, const std::string& output_time,
    const int & time_tolerance, miutil::miTime & actualtime, const Field* field)
{
  METLIBS_LOG_SCOPE(reftime << " | " << paramname << " | " << zaxis
      << " | " << taxis << " | " << extraaxis << " | " << version << " | "
      << level << " | " << time << " | " << elevel << "|" << time_tolerance);

#ifdef FIMEX
  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io) {
    gridinventory::GridParameter param;
    FimexIO* fio = dynamic_cast<FimexIO*>(*it_io);
    if (not fio)
      continue;
    gridinventory::GridParameterKey gp(paramname, zaxis, taxis, extraaxis, version);
    if (dataExists_gridParameter(fio->getInventory(), reftime, gp, level, time, elevel, time_tolerance, param, actualtime) )
    {
      // data exists ... calling getData
      return fio->putData(reftime, param, level, actualtime, elevel, unit, field, output_time);
    }

  }
#endif
  METLIBS_LOG_WARN("giving up .. returning 0");
  return 0;
}

/**
 * Get data slice
 */
Field * GridCollection::getData(const std::string& reftime, const gridinventory::GridParameter& param,
    const std::string& level, const miutil::miTime& time,
    const std::string& elevel, const std::string& unit)
{
  METLIBS_LOG_SCOPE(reftime
      << " | " << param.key.name << " | " << param.key.zaxis << " | " << param.key.taxis << " | "
      << param.key.extraaxis << " | " << param.key.version << " | " << level << " | " << time << " | " << elevel << "|");

  for(gridsources_t::const_iterator it_io=gridsources.begin(); it_io!=gridsources.end(); ++it_io) {
    Field* d = (*it_io)->getData(reftime, param, level, time, elevel, unit);
    if (d)
      return d;
  }
  METLIBS_LOG_WARN("giving up .. returning 0");
  return 0;
}

void GridCollection::updateSources()
{
  METLIBS_LOG_SCOPE();
  //Filenames without wildcards do not change
  if (sources_with_wildcards.empty()) {
    makeGridIOinstances();
    return;
  }

  std::set<std::string> newSources;
  BOOST_FOREACH(const std::string& sourcestr, sources_with_wildcards) {
    // check for wild cards - expand filenames if necessary
    if (sourcestr.find_first_of("*?") != sourcestr.npos) {
      glob_t globBuf;
      glob(sourcestr.c_str(), 0, 0, &globBuf);
      for (size_t k = 0; k < globBuf.gl_pathc; k++) {
        const std::string path = globBuf.gl_pathv[k];
        newSources.insert(path);
      }
      globfree(&globBuf);
    } else {
      newSources.insert(sourcestr);
    }
  }

  if (sources != newSources || sourcesChanged() ) {
    makeGridIOinstances();
  }
}



