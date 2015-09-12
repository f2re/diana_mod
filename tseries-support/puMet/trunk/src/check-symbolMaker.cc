/*
  libpuMet - Meteorological algorithms (Weather symbol generator etc.)

  $Id: addWeather.h 3273 2010-05-18 17:32:21Z dages $

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "symbolMaker.h"

#include <gtest/gtest.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>

using namespace miutil;
using namespace std;

static void readData(const std::string &filename,
                     std::vector<paramet> &modelData, std::vector<miTime> &termins,
                     float latitude=10 /*dummy latitude*/)
{
    std::set<miTime> times;
    std::map< int, std::map<miutil::miTime,float> > allParameters;
    std::string buf;
    std::vector<std::string> vals;
    int par;
    miTime termin;
    float val;

    modelData.clear();
    termins.clear();
    
    std::ifstream fin( filename.c_str() );
    ASSERT_TRUE(fin);
    
    while (std::getline(fin, buf)) {
        boost::split( vals, buf, boost::is_any_of(",") );
        ASSERT_LE(3, vals.size());
        
        par = boost::lexical_cast<int>( vals[0] );
        termin = miTime( vals[1] );
        val = boost::lexical_cast<float>( vals[2] );
        
        times.insert( termin );
        allParameters[par][termin] = val;
    }
    
    for (std::map< int, std::map<miutil::miTime,float> >::const_iterator it = allParameters.begin();
         it != allParameters.end(); ++it )
    {
        if (allParameters.count(it->first)) {
            paramet p;
            p.AddPara( it->first, allParameters[it->first], latitude );
            modelData.push_back( p );
        }
    }
    
    termins.reserve( times.size() );
   
    for (std::set<miTime>::const_iterator it=times.begin(); it != times.end(); ++it)
        termins.push_back( *it );
}

static void testCreateSymbol(symbolMaker::Symboltype type)
{
    miSymbol symbol = symbolMaker::createSymbol(type);
    if (symbol.index() != type or symbolMaker::Symboltype( symbol.index() ) != type)
        FAIL() << "createSymbol: symbolType: " << type;
    
    symbolMaker::Symboltype symbolType = symbolMaker::symbol2type( symbol );
    if (symbolType != type)
        FAIL() << "symbol2type: symbolType: " << type ;
}

TEST(SymbolMakerTest, CreateSymbolAndSymbol2type)
{
    ASSERT_NO_THROW(testCreateSymbol( symbolMaker::Sun ));
    ASSERT_NO_THROW(testCreateSymbol( symbolMaker::PartlyCloud ));
    ASSERT_NO_THROW(testCreateSymbol( symbolMaker::SleetSunThunder ));
    ASSERT_NO_THROW(testCreateSymbol( symbolMaker::SleetThunder  ));
    ASSERT_NO_THROW(testCreateSymbol( symbolMaker::ErrorSymbol ));
}

TEST(SymbolMakerTest, CustomNumber)
{
    symbolMaker::readSymbols(std::string(PUMETDIR) + "/check-qbSymbols.def");
    ASSERT_NO_THROW(testCreateSymbol(symbolMaker::PartlyCloud));

    miSymbol symbol = symbolMaker::createSymbol( symbolMaker::PartlyCloud );

    EXPECT_EQ(3, symbol.customNumber(true));
    EXPECT_EQ(17, symbol.customNumber(false));
    EXPECT_EQ("delvis skyet", symbol.customName(true));
};

TEST(SymbolMakerTest, ComputeWithoutStateOfAggregate)
{
    std::vector<paramet> modelData;
    std::vector<miTime> termins;
    std::string datafile(std::string(PUMETDIR)+ "/check-data1.dat");
    readData(datafile, modelData, termins);

    symbolMaker sm;
    std::vector<miSymbol> symbols;
    symbols = sm.computeWithoutStateOfAggregate(modelData, termins, 1, 0, true);

    ASSERT_FALSE(symbols.empty());
}
