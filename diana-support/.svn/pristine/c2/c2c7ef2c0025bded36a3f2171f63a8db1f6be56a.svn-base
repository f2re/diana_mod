
SUBDIRS = gtest .

if HAVE_GTEST

check_PROGRAMS = diFieldTest
TESTS = $(check_PROGRAMS)

diFieldTest_SOURCES =
diFieldTest_CPPFLAGS = -DTEST_SRCDIR=\"$(abs_srcdir)\" -DTEST_BUILDDIR=\"$(abs_builddir)\" \
    $(AM_CPPFLAGS) -I$(top_srcdir)/src
diFieldTest_LDFLAGS =
diFieldTest_LDADD =

if WITH_FIMEX
diFieldTest_SOURCES += \
	DataReshapeTest.cc \
	FieldManagerTest.cc \
	FimexSourceTest.cc \
	MetConstantsTest.cc \
	TimeFilterTest.cc \
	ShapeTest.cc \
	VcrossDataTest.cc
	VcrossUtilTest.cc
diFieldTest_CPPFLAGS += \
	-DFIMEX ${FIMEX_CPPFLAGS}
diFieldTest_LDFLAGS += \
	${FIMEX_LDFLAGS}
diFieldTest_LDADD += \
	${FIMEX_LIBS}
endif # WITH_FIMEX

diFieldTest_SOURCES += \
    FieldFunctionsTest.cc

diFieldTest_LDFLAGS += \
    $(PUDATATYPES_LDFLAGS) $(PUTOOLS_LDFLAGS) $(MILOGGER_LDFLAGS) $(PUCTOOLS_LDFLAGS) \
    ${UDUNITS_LDFLAGS} \
    $(PROJ_LDFLAGS) $(BOOST_LDFLAGS) \
    $(gtest_LDFLAGS)

diFieldTest_CPPFLAGS += \
    $(PUDATATYPES_CPPFLAGS) $(PUTOOLS_CPPFLAGS) $(PUCTOOLS_CPPFLAGS) $(MILOGGER_CPPFLAGS) \
    ${UDUNITS_CPPFLAGS} \
    $(PROJ_CPPFLAGS) $(BOOST_CPPFLAGS) \
    $(gtest_CPPFLAGS)

diFieldTest_req_libs = $(top_builddir)/src/libdiField.la

diFieldTest_LDADD += \
    -lgtest_main \
    $(diFieldTest_req_libs) \
    $(PUDATATYPES_LIBS) $(PUTOOLS_LIBS) $(MILOGGER_LIBS) $(PUCTOOLS_LIBS) \
    ${UDUNITS_LIBS} \
    $(PROJ_LIBS) $(BOOST_DATE_TIME_LIBS)  \
    $(gtest_LIBS)

diFieldTest_DEPENDENCIES = $(diFieldTest_req_libs)

if ENABLE_GRIBFILE
diFieldTest_LDADD += -lgrib_api
endif

endif # HAVE_GTEST

EXTRA_DIST = test_fimexio_rw.nc hirlam_vct1.nc hirlam_vct2.nc arome.nc \
    arome_vc.nc hirlam_vc.nc simra_vc.nc simra_vc_dyn.nc emep_etna_dyn.nc \
    wavespec.nc
