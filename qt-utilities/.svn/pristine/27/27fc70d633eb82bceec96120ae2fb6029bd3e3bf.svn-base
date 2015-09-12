/*
  libqUtilities - Diverse Qt-classes and coserver base

  Copyright (C) 2006-2013 met.no

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


/*!
    ------------------------------------
    Created by:
    met.no/FoU/pu/
    at Mon Jun 13 12:26:44 2005
    $Revision$
    at $Date$
    ------------------------------------
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <miLogFile.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;


std::map<std::string,miLogFile::xy >  miLogFile::pos;
std::map<std::string,miLogFile::xy >  miLogFile::size;
std::map<std::string,std::string >       miLogFile::tokens;
std::map<std::string,std::string>        miLogFile::sections;
std::string                      miLogFile::filename="unnamed.log";
int                           miLogFile::xmax    = 1280;
int                           miLogFile::ymax    = 1024;

void miLogFile::setMaxXY(int xm, int ym)
{
  xmax = xm;
  ymax = ym;
}

bool miLogFile::hasSize(std::string key) const
{
  if(!size.count(key))
    return false;

  xy s = size[key];
  return (s.x<=xmax && s.y<=ymax);
}

bool miLogFile::hasPos(std::string key) const
{
  if(!pos.count(key))
    return false;

  xy p = pos[key];
  return (p.x<xmax-20 && p.y<ymax-20);
}


int miLogFile::posx(std::string key)
{
  return ( pos.count(key) ? pos[key].x : 0 );
}

int miLogFile::posy(std::string key)
{
  return ( pos.count(key) ? pos[key].y : 0 );
}

int miLogFile::sizex(std::string key)
{
  return ( size.count(key) ? size[key].x : 0 );
}

int miLogFile::sizey(std::string key)
{
  return ( size.count(key) ? size[key].y : 0 );
}

void miLogFile::addPos( std::string key,int x,int y)
{
  if(x > 0 && y > 0 )
    pos[key] = xy(x,y);
  if((not section.empty()))
    sections[key+".pos"] = section;
}

void miLogFile::addSize(std::string key,int x,int y)
{
  if (x>20 && y>20 )
    size[key] = xy(x,y);
  if((not section.empty()))
    sections[key+".size"] = section;
}

void miLogFile::addToken(std::string key,std::string tok )
{
  tokens[key] = tok;
  if((not section.empty()))
      sections[key] = section;
}

void miLogFile::addToken(std::string key,bool tok )
{
  tokens[key] = (tok ? "TRUE" : "FALSE");
  if((not section.empty()))
      sections[key] = section;
}

void miLogFile::addToken(std::string key, int token)
{
  ostringstream ost;
  ost << token;
  tokens[key] = ost.str();
  if((not section.empty()))
       sections[key] = section;
}



std::string miLogFile::token(std::string key) const
{
  return ( tokens.count(key) ? tokens[key] : "" );
}

bool miLogFile::booleanToken(std::string key) const
{
  if(tokens.count(key))
    if(tokens[key] == "TRUE") return true;
  return false;
}

bool miLogFile::hasBooleanToken(std::string key) const
{
  if(tokens.count(key))
    if(tokens[key] == "TRUE" || tokens[key] == "FALSE" )
      return true;

  return false;
}

bool miLogFile::hasIntToken(std::string key) const
{
  tokens_t::const_iterator it = tokens.find(key);
  if (it == tokens.end())
    return false;

  try  {
    int  number = boost::lexical_cast<int>(it->second);
  } catch(boost::bad_lexical_cast& e) {
    return false;
  }
  return true;
}

int miLogFile::intToken(std::string key) const
{
  tokens_t::const_iterator it = tokens.find(key);
  int number = 0;
  if (it != tokens.end()) {
    try  {
      int number = boost::lexical_cast<int>(it->second);
      return number;
    } catch(boost::bad_lexical_cast& e) {
      number = 0;
    }
  }
  return number;
}




bool miLogFile::read( std::string f )
{
  if((not f.empty()))
    filename=f;

  ifstream log(filename.c_str());

  if(!log) {
    cerr << "could not open log file: " << filename << endl;
    return false;
  }

  string line;
  std::vector<string> lines;

  while(log) {
    getline(log,line);
    lines.push_back(line);
  }
  return readStrings(lines);
}

bool miLogFile::readStrings(const std::vector<string>& lines, const string& section)
{
  std::string         line;
  std::string         key,tok;
  std::vector<std::string> words;

  for (size_t i = 0; i < lines.size(); ++i) {
    line=lines[i];
    if(boost::find_first(line,"#"))
      continue;
    if(boost::find_first(line,"==="))
      continue;

    boost::trim(line);
    if((not !line.empty()))
      continue;

    size_t c = line.find_first_of(" ", 0);
    if (c == string::npos)
      continue;
    int k = line.length() -  c;

    key =  line.substr(0,c);
    tok =  line.substr(c,k);
    boost::trim(tok);

    if((not !tok.empty()))
      continue;

    if (not section.empty())
      sections[key]=section;

   
    if(boost::find_first(key, ".size")) {
     
      boost::erase_all(key,".size");
      
      boost::split(words, tok, boost::algorithm::is_any_of(" "));

      if(words.size() < 2)
        continue;

      try  {
	int sx = boost::lexical_cast<int>(words[0]);
	int sy = boost::lexical_cast<int>(words[1]);

	if( sx > 20 && sy > 20 )
	  size[key] = xy(sx,sy);

      }  catch(boost::bad_lexical_cast& e) {
	// this one went bad - we just ignore it - using default values
      }
      continue;
    }


    if(boost::find_first(key, ".pos")) {
        boost::erase_all(key,".pos");
      boost::split(words, tok, boost::algorithm::is_any_of(" "));

      if(words.size() < 2)
        continue;

      
      
      try  {
	int px = boost::lexical_cast<int>(words[0]);
	int py = boost::lexical_cast<int>(words[1]);

	if(px > 0 && py > 0 )
	  pos[key] = xy(px,py);

      }  catch(boost::bad_lexical_cast& e) {
	// this one went bad - we just ignore it - using default values
      }
      continue;
    }

    tokens[key] = tok;
  }

  return true;
}




bool miLogFile::write( std::string f )
{
  if((not f.empty()))
    filename=f;

  ofstream log(filename.c_str());

  if(!log) {
    cerr << "could not open log file: " << filename << endl;
    return false;
  }
  log << "# automatically generated file. DO NOT EDIT!\n\n";
  log << writeString();
  return true;
}


std::string  miLogFile::writeString(std::string section)
{
  ostringstream log;
  std::string key;
  std::map<std::string,xy>::iterator       ipos  = pos.begin();
  std::map<std::string,xy>::iterator       isize = size.begin();
  std::map<std::string,std::string>::iterator itok  = tokens.begin();


  for (;isize!=size.end();isize++) {
    key=isize->first + ".size";
    if((not section.empty()) && sections[key] != section)
        continue;
    log <<  key << " " << isize->second.x << " " << isize->second.y << endl;
  }

  log << "===========================" << endl;

  for (;ipos!=pos.end();ipos++) {
    key=ipos->first + ".pos";
    if((not section.empty()) && sections[key] != section)
      continue;
    log << key << " "  << ipos->second.x << " " << ipos->second.y << endl;
  }

  log << "===========================" << endl;

  for (;itok!=tokens.end();itok++) {
    key=itok->first;
    if((not section.empty()) && sections[key] != section)
      continue;
    log << key  << " " << itok->second << endl;
  }
  return log.str();
}

void miLogFile::logSizeAndPos(QWidget* w,std::string name)
{
  if(!w)             return;
  if((not !name.empty())) name=w->objectName().toStdString();
  if((not !name.empty())) return;


  addSize(name, w->width(), w->height());
  addPos( name, w->x(),     w->y()     );

}

void miLogFile::restoreSizeAndPos(QWidget* w,std::string name)
{
  if(!w)             return;
  if((not !name.empty())) name=w->objectName().toStdString();
  if((not !name.empty())) return;

  if(hasSize(name))
    w->resize(sizex(name),sizey(name));
  if(hasPos(name))
    w->move(posx(name),posy(name));
}

void miLogFile::logVisibility(QWidget* w,std::string name)
{
  if(!w)             return;
  if((not !name.empty())) name=w->objectName().toStdString();
  if((not !name.empty())) return;

  name+="IsVisible";
  addToken(name,w->isVisible());

}

void miLogFile::restoreVisibility(QWidget* w,std::string name)
{
  if(!w)             return;
  if((not !name.empty())) name=w->objectName().toStdString();
  if((not !name.empty())) return;
  name+="IsVisible";

  if(hasBooleanToken(name)) {
    if(booleanToken(name))
      w->show();
    else
      w->hide();
  }
}
