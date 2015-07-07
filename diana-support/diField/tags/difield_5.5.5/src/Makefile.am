# $Id: Makefile.am 4376 2014-01-10 12:30:58Z lisbethb $

pkgname = diField

lib_LTLIBRARIES = libdiField.la

libdiField_la_SOURCES = \
	DataReshape.cc \
	diArea.cc \
	diField.cc \
	diFieldFunctions.cc \
	diFieldManager.cc \
	diGridConverter.cc \
	diMetConstants.cc \
	diProjection.cc \
	diRectangle.cc\
	diFieldCacheKeyset.cc \
	diFieldCacheEntity.cc \
	diFieldCache.cc \
	FieldSpecTranslation.cc \
	GridInventoryTypes.cc \
	GridIO.cc \
	GridCollection.cc \
	TestGridIO.cc \
	TimeFilter.cc \
	FimexSource.cc \
	VcrossSource.cc \
	VcrossData.cc \
	VcrossUtil.cc

libdiField_la_CPPFLAGS = -fPIC \
	${BOOST_CPPFLAGS} \
	${PROJ_CPPFLAGS} \
	${UDUNITS_CPPFLAGS} \
	${LOG4CPP_CPPFLAGS} \
	${PUCTOOLS_CPPFLAGS} \
	${PUTOOLS_CPPFLAGS}

libdiField_la_LDFLAGS = -fPIC ${diField_VERSION_INFO}
libdiField_la_LIBADD =
if ENABLE_SHARED_LIBRARY
libdiField_la_LDFLAGS += \
	${BOOST_LDFLAGS} \
	${PROJ_LDFLAGS} \
	${UDUNITS_LDFLAGS} \
	${PUCTOOLS_LDFLAGS} \
	${PUTOOLS_LDFLAGS}

libdiField_la_LIBADD += \
	${BOOST_DATE_TIME_LIBS} \
	${PROJ_LIBS} \
	${UDUNITS_LIBS} \
	${PUCTOOLS_LIBS} \
	${PUTOOLS_LIBS}
endif # ENABLE_SHARED_LIBRARY


if WITH_FIMEX
libdiField_la_SOURCES += \
	FimexIO.cc
libdiField_la_CPPFLAGS += \
	-DFIMEX \
	${FIMEX_CPPFLAGS}
libdiField_la_LDFLAGS += \
	${FIMEX_LDFLAGS}
libdiField_la_LIBADD += \
	${FIMEX_LIBS}
endif # WITH_FIMEX

pkg_includedir = ${includedir}/diField

pkg_include_HEADERS = \
	diArea.h \
	diCommonFieldTypes.h \
	diField.h \
	diFieldExceptions.h \
	diFieldFunctions.h \
	diFieldManager.h \
	diGridConverter.h \
	diMetConstants.h \
	MetnoFieldFileParamMap.hpp \
	diProjection.h \
	diRectangle.h \
	diFieldCache.h \
	diFieldCacheKeyset.h \
	diFieldCacheEntity.h \
	FieldSpecTranslation.h \
	GridInventoryTypes.h \
	GridDataKey.h \
	GridIO.h \
	GridCollection.h \
	TestGridIO.h \
	TimeFilter.h \
	FimexSource.h \
	VcrossSource.h \
	VcrossData.h \
	VcrossUtil.h \
	DataReshape.h

if WITH_FIMEX
pkg_include_HEADERS += FimexIO.h
endif

INSTALL=install -p # tells install to preserve timestamp
