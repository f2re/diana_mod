#
# configure.ac for puDatatypes
#

AC_PREREQ(2.59)
AC_INIT([puDatatypes], [5.0.2], [diana@met.no])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_SRCDIR([src/miCoordinates.h])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

METNO_SET_VERSION_INFO(6.0.0)
METNO_PROG_PKG_CONFIG
METNO_REQUIRE_BOOST

AC_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS]) 

AC_PROG_CXX

AC_LANG_PUSH(C++)
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_LANG_POP([C++])


METNO_ENABLE_CONDITIONAL([SHARED_LIBRARY], [shared])

GTEST_CHECK([test/gtest])

AC_CONFIG_FILES([
	puDatatypes.pc
	Makefile
	src/Makefile
	test/Makefile
	test/gtest/Makefile
])

AC_OUTPUT
