
SUBDIRS = gtest .

if HAVE_GTEST
TESTS = puDatatypesTest

check_PROGRAMS = puDatatypesTest

puDatatypesTest_SOURCES = \
    MiCoordinatesTest.cc
puDatatypesTest_LDFLAGS = \
    $(AM_LDFLAGS) $(gtest_LDFLAGS)
puDatatypesTest_CPPFLAGS = \
    $(AM_CPPFLAGS) $(gtest_CPPFLAGS) -I$(top_srcdir)/src

puDatatypesTest_req_libs = $(top_builddir)/src/libpuDatatypes.la

puDatatypesTest_LDADD = \
    -lgtest_main \
    $(puDatatypesTest_req_libs) \
    $(gtest_LIBS)

puDatatypesTest_DEPENDENCIES = $(puDatatypesTest_req_libs)
if MUST_COMPILE_GTEST
puDatatypesTest_DEPENDENCIES += \
    $(builddir)/gtest/libgtest.a \
    $(builddir)/gtest/libgtest_main.a 
endif # MUST_COMPILE_GTEST

endif # HAVE_GTEST
