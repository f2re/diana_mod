
pkgname = puMet

lib_LTLIBRARIES=libpuMet.la

libpuMet_la_SOURCES = \
	gust.cc \
	windProfile.cc \
	vision.cc \
	symbolMaker.cc \
	miSymbol.cc \
	addWeather.cc \
	paramet.cc \
	cloudGrp.cc \
	weatherSpec.cc \
	ptStatistics.cc \
	usrTab.cc \
	humidity.cc \
	ff2beaufort.cc \
	iceAccretion.cc \
	visibility.cc \
	WindCalc.cc


pkg_includedir= ${includedir}/puMet

pkg_include_HEADERS = $(libpuMet_la_SOURCES:.cc=.h)



libpuMet_la_CPPFLAGS = ${PUTOOLS_CPPFLAGS}

libpuMet_la_LDFLAGS = ${VERSION_INFO}

if ENABLE_SHARED_LIBRARY
libpuMet_la_LDFLAGS += ${PUTOOLS_LDFLAGS}
libpuMet_la_LIBADD   = ${PUTOOLS_LIBS}
endif

if HAVE_GTEST
check_PROGRAMS = check-symbolMaker
TESTS = $(check_PROGRAMS)

check_symbolMaker_SOURCES  =  check-symbolMaker.cc
check_symbolMaker_CPPFLAGS = -DPUMETDIR=\"$(srcdir)\" ${PUTOOLS_CPPFLAGS} $(gtest_CFLAGS)
check_symbolMaker_LDFLAGS  =  $(PUTOOLS_LDFLAGS) $(gtest_LDFLAGS)
check_symbolMaker_LDADD    = -lgtest_main libpuMet.la \
	$(PUTOOLS_LIBS) -lpthread $(gtest_LIBS)
endif

EXTRA_DIST = check-qbSymbols.def

INSTALL = install -p
