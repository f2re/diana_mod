/*
  Diana - A Free Meteorological Visualisation Tool

  $Id: diArea.h 3309 2010-06-29 11:37:34Z lisbethb $

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

#ifndef FieldSpecTranslation_h
#define FieldSpecTranslation_h

#include <string>

/**
  \brief Translates between new and old field syntax
*/

class FieldSpecTranslation {
public:
  FieldSpecTranslation();

  static std::string getNewFieldString(std::string pin, bool decodeModel=false);
  static  std::string getVlevelFromLevel(std::string level);
  static  std::string getVcoorFromLevel(std::string level);
  static std::string getModelString(std::string model);

  static  std::string getOldModel(const std::string& model, int refHour, int refOffset, const std::string& grid );
  static  std::string getOldLevel(const std::string& vcoor, const std::string& vlevel);
};

#endif
