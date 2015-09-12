# $Id: Makefile.am 4361 2014-01-03 11:17:42Z juergens $

pkgname = pets2

lib_LTLIBRARIES = libpets2.la

cc_sources= \
	ptAxesElement.cc \
	ptAxisHistogramElement.cc \
	ptBoxElement.cc \
	ptCloudElement.cc \
	ptColor.cc \
	ptDateElement.cc \
	ptDayElement.cc \
	ptDiagram.cc \
	ptDoubleLineElement.cc \
	ptEditLineElement.cc \
	ptFontDefs.cc \
	ptFontManager.cc \
	ptGridlineElement.cc \
	ptHistogramElement.cc \
	ptImage.cc \
	ptIntervalElement.cc \
	ptLayout.cc \
	ptLineElement.cc \
	ptLogoElement.cc \
	ptMarker.cc \
	ptMultiLineElement.cc \
	ptPatterns.cc \
	ptPlotElement.cc \
	ptProgElement.cc \
	ptQBoxElement.cc \
	ptStyle.cc \
	ptSymbolElement.cc \
	ptTableElement.cc \
	ptTextElement.cc \
	ptTimemarkerElement.cc \
	ptUTCElement.cc \
	ptVectorElement.cc \
	ptWindVectorElement.cc \
	ptYaxisElement.cc


libpets2_la_SOURCES = \
	$(cc_sources) \
	ptAxisChildElement.cc \
	ptStaticYaxisElement.cc


pkg_includedir = ${includedir}/pets2

pkg_include_HEADERS =        \
	$(cc_sources:.cc=.h) \
	polyStipMasks.h      \
	ptEditModes.h        \
	ptGlobals.h          \
	ptImagePlot.h        \
	ptParameterRelation.h\
	ptStyleOrder.h       \
	ptXAxisInfo.h



libpets2_la_CPPFLAGS = \
	${GLTEXT_CPPFLAGS} \
	${OPENGL_CPPFLAGS} \
	${PUTOOLS_CPPFLAGS} \
	${TSDATA_CPPFLAGS} \
	${PNG_CPPFLAGS}

libpets2_la_LDFLAGS = ${VERSION_INFO}
libpets2_la_LIBADD =

if ENABLE_SHARED_LIBRARY
libpets2_la_LDFLAGS += \
	${GLTEXT_LDFLAGS} \
	${OPENGL_LDFLAGS} \
	${PUTOOLS_LDFLAGS} \
	${TSDATA_LDFLAGS} \
	${PNG_LDFLAGS}

libpets2_la_LIBADD += \
	${GLTEXT_LIBS} \
	${OPENGL_LIBS} \
	${PUTOOLS_LIBS} \
	${TSDATA_LIBS} \
	${PNG_LIBS}
endif


INSTALL=install -p 