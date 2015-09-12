
pkgname = puDatatypes

lib_LTLIBRARIES = libpuDatatypes.la

libpuDatatypes_la_SOURCES= \
	miCoordinates.cc \
	miLine.cc  \
	miPosition.cc \
	miRegions.cc

pkg_includedir = ${includedir}/puDatatypes

pkg_include_HEADERS = $(libpuDatatypes_la_SOURCES:.cc=.h)

libpuDatatypes_la_CPPFLAGS = -fPIC ${BOOST_CPPFLAGS}

libpuDatatypes_la_LDFLAGS = -fPIC ${VERSION_INFO}

INSTALL=install -p # tells install to preserve timestamp
