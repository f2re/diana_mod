#!/bin/bash

#
# copyright  almukantarat from http://linuxforum.ru/topic/35326
#

echo " "
echo " This script will install weather software from met.no "
echo " This script whrote for openSUSE 13.1 x86-64 linux based operation system "
echo " "
echo " Now we'll install all nessesary dependences "
echo " ***** "

#sudo zypper ar --check --refresh --name ocefpaf http://download.opensuse.org/repositories/home:/ocefpaf/openSUSE_13.1/ ocefpaf
#sudo zypper ref
#sudo zypper in libproj-devel udunits2-devel udunits-devel libxml2-devel netcdf-devel grib_api-devel libjasper-devel libpng12-devel libjpeg-turbo libpqxx-devel log4cpp-devel geotiff libgeotiff-devel freeglut-devel libshp-devel libqt4-devel ftgl-devel
sudo brew install libproj-devel udunits2-devel udunits-devel libxml2-devel netcdf-devel grib_api-devel libjasper-devel libpng12-devel libjpeg-turbo libpqxx-devel log4cpp-devel geotiff libgeotiff-devel freeglut-devel libshp-devel libqt4-devel ftgl-devel

echo " "
echo " Now we'll downdload sources "
echo " "
read -p "Press [Enter] key to continue..."

svn co https://svn.met.no/fimex/tags/version0_51 fimex
svn co https://svn.met.no/metlibs/common
svn co https://svn.met.no/metlibs/diana-support
svn co https://svn.met.no/metlibs/gl-support
svn co https://svn.met.no/metlibs/tseries-support
svn co https://svn.met.no/metlibs/qt-utilities
svn co https://svn.met.no/diana/trunk diana

echo " "
echo " Before we continue, you must change next file: /diana-support/diField/trunk/configure.ac "
echo " Find next string: METNO_REQUIRE_PKG([UDUNITS], [udunits], [2.1]) "
echo " and replace it with: METNO_REQUIRE_C_LIBRARY([UDUNITS], [udunits2], [udunits2.h], [ut_parse]) "
echo " "
read -p "Press [Enter] key to continue..."

set -e
root_path=pwd

echo " "
echo " Now configure, make & install Fimex software "
echo " "
cd $root_path/fimex
./configure && make
sudo make install

echo " "
echo " Now configure, make & install Metlibs software "
echo " "

cd $root_path/common/miLogger/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/common/puCtools/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/common/puTools/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/tseries-support/puDatatypes/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/diana-support/diField/trunk
./autogen.sh && ./configure --prefix=/usr/local --with-udunits2-includedir=/usr/include/udunits2 && make
sudo make install

cd $root_path/diana-support/miRaster/trunk
./autogen.sh && ./configure --prefix=/usr/local --with-geotiff && make
sudo make install

cd $root_path/gl-support/glp/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/gl-support/miFTGL/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/gl-support/glText/trunk
./autogen.sh && ./configure --prefix=/usr/local --enable-xfonts && make
sudo make install

cd $root_path/qt-utilities/qUtilities/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

cd $root_path/qt-utilities/coserver/trunk
./autogen.sh && ./configure --prefix=/usr/local && make
sudo make install

echo " "
echo " Now configure, make & install Diana software "
echo " "

cd $root_path/diana
./autogen.sh
./configure --enable-xlib --enable-geotiff --prefix=/usr/local --disable-video-export --with-png-libdir=/usr/lib64 --enable-vcross-v2 --with-udunits2-includedir=/usr/include/udunits2 && make
sudo make install

sudo /sbin/ldconfig

echo " "
echo " Now download and install additional files for Diana "
echo " "

cd $root_path
wget -Nc ftp://ftp.met.no/projects/diana/diana_maps.tar.gz
wget -Nc ftp://ftp.met.no/projects/diana/diana_demodata.tar.gz
cd /usr/local/share/diana/ls /usr/local/share/diana/
sudo tar zxf $root_path/diana_maps.tar.gz
sudo tar zxf $root_path/diana_demodata.tar.gz

echo " "
echo " Congratulation! All steps are copmleted. "
echo " "

exit 0 
