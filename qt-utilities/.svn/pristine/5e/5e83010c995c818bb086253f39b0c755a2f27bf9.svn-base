// -*- c++ -*-
/*
 libqUtilities - Diverse Qt-classes and coserver base

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

#ifndef _miLogFile_h
#define _miLogFile_h

/*!
 ------------------------------------
 Created by:
 met.no/FoU/pu/
 at Mon Jun 13 12:26:44 2005
 $Revision$
 at $Date$
 ------------------------------------
 */

#include <QWidget>
#include <map>

class miLogFile {
private:

  typedef struct xy {
    int x;
    int y;
    xy() :
      x(0), y(0)
    {
    }
    xy(int x_, int y_) :
      x(x_), y(y_)
    {
    }
  } xy;

  static std::map<std::string, xy> pos;
  static std::map<std::string, xy> size;
  typedef std::map<std::string, std::string> tokens_t;
  static tokens_t tokens;
  static std::map<std::string, std::string> sections; // which section for each key
  static std::string filename;
  static int xmax;
  static int ymax;

  std::string section; // current section of a single instance
public:
  miLogFile()
  {
  }
  miLogFile(std::string f)
  {
    read(f);
  }

  void setSection(std::string s)
  {
    section = s;
  }

  int posx(std::string key);
  int posy(std::string key);

  int sizex(std::string key);
  int sizey(std::string key);

  std::string token(std::string key) const;
  bool booleanToken(std::string key) const;
  int intToken(std::string key) const;

  bool hasSize(std::string key) const;
  bool hasPos(std::string key) const;
  bool hasToken(std::string key) const
  {
    return bool(tokens.count(key));
  }
  bool hasBooleanToken(std::string key) const;
  bool hasIntToken(std::string key) const;

  void addPos(std::string key, int x, int y);
  void addSize(std::string key, int x, int y);
  void addToken(std::string key, std::string token);
  void addToken(std::string key, bool token);
  void addToken(std::string key, int token);

  bool readStrings(const std::vector<std::string>& lines, const std::string& section = "");
  std::string writeString(std::string section = "");

  bool read(std::string fname = ""); /// none == filenname
  bool write(std::string fname = ""); /// none == filenname

  void setMaxXY(int xm, int ym);

  void logSizeAndPos(QWidget* w, std::string name = ""); // if name ="" w->name() is used
  void restoreSizeAndPos(QWidget* w, std::string name = "");
  void logVisibility(QWidget* w, std::string name = "");
  void restoreVisibility(QWidget* w, std::string name = "");

};

#endif
