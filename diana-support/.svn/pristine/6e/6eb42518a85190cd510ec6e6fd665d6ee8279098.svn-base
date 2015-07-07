/* -*- c++ -*-
 * TestGridIO.h
 *
 *  Created on: Mar 11, 2010
 *      Author: audunc
 */
/*
 $Id:$

 Copyright (C) 2006 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 email: diana@met.no

 This is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TESTGRIDIO_H_
#define TESTGRIDIO_H_

#include "GridIO.h"

class TestGridIOsetup: public GridIOsetup {
public:
  TestGridIOsetup()
  {
  }
  virtual ~TestGridIOsetup()
  {
  }
  /**
   * Return name of section in setup file
   * @return name
   */
  static std::string getSectionName()
  {
    return "TESTGRID_PARAMETERS";
  }

  /**
   * Parse setup information
   * @param lines
   * @param errors
   * @return status
   */
  bool parseSetup(std::vector<std::string> lines, std::vector<
      std::string>& errors);
};

class TestGridIO: public GridIO {
private:
  std::string model_name;
  TestGridIOsetup * setup;
  std::string reftime_from_filename;

public:
  TestGridIO();
  TestGridIO(const std::string & modelname, TestGridIOsetup * s);
  ~TestGridIO();

  /**
   * What kind of source is this.
   * @return Sourcetype
   */
  static std::string getSourceType()
  {
    return "testgrid";
  }

  // ===================== IMPLEMENTATIONS OF VIRTUAL FUNCTIONS BELOW THIS LINE ============================

  /**
   * Returns whether the source has changed since the last makeInventory
   * @param modelname, model to check for. Use empty string for all models
   * @param reftime, reference time to check for. Use miTime::undef for all
   * @return bool
   */
  virtual bool sourceChanged(bool update);

  /**
    * Return referencetime from filename given in constructor
    * @return reftime_from_filename
    */
   virtual std::string getReferenceTime() const
   {return reftime_from_filename;}

   /**
    * Returns true if referencetime matches
    * @return bool
    */
  virtual bool referenceTimeOK(const std::string & refTime)
  {return (reftime_from_filename.empty() || reftime_from_filename == refTime);}

  /**
   * Build the inventory from source
   * @param modelname, model to make inventory for. Use empty string for all models
   * @param reftime, reference time to make inventory for. Use miTime::undef for none
   * @return status
   */
  bool makeInventory(const std::string& reftime);

  /**
   * Get data slice as Field
   */
  virtual Field * getData(const std::string& reftime, const gridinventory::GridParameter& param,
      const std::string& level, const miutil::miTime& time,
      const std::string& run, const std::string& unit);

  virtual vcross::Values_p getVariable(const std::string& /*varName*/)
    { return vcross::Values_p(); }
};

#endif /* TESTGRIDIO_H_ */
