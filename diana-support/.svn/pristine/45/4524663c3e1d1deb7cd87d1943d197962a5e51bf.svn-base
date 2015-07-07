#
# configure.ac for diField
#

AC_PREREQ(2.59)
AC_INIT([diField], [5.8.0], [diana@met.no])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_SRCDIR([src/diProjection.h])
AC_CONFIG_MACRO_DIR([m4])

METNO_SET_VERSION_INFO(6.0.0)
METNO_PROG_PKG_CONFIG

AC_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS]) 

AC_PROG_CXX

AC_LANG_PUSH(C++)
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL

CXXFLAGS="$CXXFLAGS -Wall -Wextra"

# Check compiler supporting openmp in C++ and C
AC_ARG_ENABLE(openmp, [  --enable-openmp         enable use of openmp [[default=no]]],[
  if test "$enableval" = yes; then
AX_OPENMP([AC_DEFINE(HAVE_OPENMP,1,[Define if OpenMP is enabled])
           CXXFLAGS="$CXXFLAGS $OPENMP_CXXFLAGS";CFLAGS="$CFLAGS $OPENMP_CXXFLAGS"],[])
  fi
], [])
AC_LANG_POP(C++)
AC_SUBST(OPENMP_CXXFLAGS)
 
METNO_ENABLE_CONDITIONAL([SHARED_LIBRARY], [shared])

METNO_REQUIRE_PKG([MILOGGER],    [miLogger],    [5.0.1])
METNO_REQUIRE_PKG([PUCTOOLS],    [puCtools],    [5.0.1])
METNO_REQUIRE_PKG([PUTOOLS],     [puTools],     [5.0.1])
METNO_REQUIRE_PKG([PUDATATYPES], [puDatatypes], [5.0.1])

METNO_WITH_LIBRARY([FIMEX], [fimex], [fimex library], [yes])
  METNO_IF_WITH([fimex], [
    METNO_REQUIRE_PKG([FIMEX], [fimex], [0.50])
    METNO_REQUIRE_BOOST
    METNO_REQUIRE_BOOST_FEATURE([date_time])
 ])
METNO_WITH_CONDITIONAL([FIMEX], [fimex])

METNO_REQUIRE_C_LIBRARY([PROJ], [proj], [proj_api.h], [pj_init_plus])

METNO_REQUIRE_PKG([UDUNITS], [udunits], [2.1])

METNO_ENABLE([GRIBFILE], [gribfile], [support for fields in Grib file], [no])
METNO_ENABLE_CONDITIONAL([GRIBFILE], [gribfile])

METNO_IF_ENABLED([gribfile], [
    METNO_REQUIRE_C_LIBRARY([GRIB_API], [grib_api], [grib_api.h], [grib_index_new], [-lm -lpng])
    # For now, smhi uses GribAPI...
    METNO_REQUIRE_C_LIBRARY([GRIBAPI], [GribAPI])
])

AC_SUBST(METNO_PKG_REQUIRED)

GTEST_CHECK([test/gtest])

AC_CONFIG_FILES([
	diField.pc
	Makefile
	src/Makefile
	test/Makefile
	test/gtest/Makefile
])

AC_OUTPUT
