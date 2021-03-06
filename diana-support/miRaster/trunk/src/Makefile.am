# $Id: Makefile.am 4041 2013-02-05 11:18:08Z alexanderb $

pkgname = miRaster

lib_LTLIBRARIES = libmiRaster.la

libmiRaster_la_SOURCES = satimg.cc

libmiRaster_la_CPPFLAGS = -fPIC \
	${LOG4CPP_CPPFLAGS} \
	${PUCTOOLS_CPPFLAGS} \
	${PUTOOLS_CPPFLAGS} \
	${PROJ_CPPFLAGS} \
	${TIFF_CPPFLAGS}

libmiRaster_la_LDFLAGS = -fPIC ${miRaster_VERSION_INFO}
libmiRaster_la_LIBADD =
if ENABLE_SHARED_LIBRARY
libmiRaster_la_LDFLAGS += \
	${LOG4CPP_LDFLAGS} \
	${PUCTOOLS_LDFLAGS} \
	${PUTOOLS_LDFLAGS} \
	${PROJ_LDFLAGS} \
	${TIFF_LDFLAGS}

libmiRaster_la_LIBADD += \
	${LOG4CPP_LIBS} \
	${PUCTOOLS_LIBS} \
	${PUTOOLS_LIBS} \
	${PROJ_LIBS} \
	${TIFF_LIBS}
endif

# XXX needs revisiting - HDF5 is currently unconditional
if WITH_HDF5
libmiRaster_la_SOURCES += satimgh5.cc
libmiRaster_la_CPPFLAGS += -DHDF5 ${HDF5_CPPFLAGS} -D_REENTRANT
libmiRaster_la_LDFLAGS += ${HDF5_LDFLAGS}
libmiRaster_la_LIBADD += ${HDF5_LIBS}
endif

if WITH_GEOTIFF
libmiRaster_la_SOURCES += satgeotiff.cc ImageCache.cpp
libmiRaster_la_CPPFLAGS += -DGEOTIFF ${GEOTIFF_CPPFLAGS}
libmiRaster_la_LDFLAGS += ${GEOTIFF_LDFLAGS} ${BOOST_THREAD_LDFLAGS}
libmiRaster_la_LIBADD += ${GEOTIFF_LIBS} ${BOOST_THREAD_LIBS}
endif

pkg_includedir = ${includedir}/miRaster

pkg_include_HEADERS = satimg.h
pkg_include_HEADERS += satgeotiff.h ImageCache.h

# XXX needs revisiting - HDF5 is currently unconditional
#if HAVE_HDF5
pkg_include_HEADERS += satimgh5.h
#endif

INSTALL=install -p # tells install to preserve timestamp
