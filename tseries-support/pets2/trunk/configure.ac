#
# configure.ac for pets2
#

AC_PREREQ(2.59)
AC_INIT([pets2], [5.0.0], [diana@met.no])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_SRCDIR([src/ptStyle.h])
AC_CONFIG_MACRO_DIR([m4])

METNO_SET_VERSION_INFO(6.0.0)
METNO_PROG_PKG_CONFIG

AC_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS]) 

AC_PROG_CXX
AC_LANG_PUSH(C++)
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_LANG_POP([C++])

METNO_ENABLE_CONDITIONAL([SHARED_LIBRARY], [shared])

METNO_REQUIRE_PKG([PUTOOLS],[puTools])
METNO_REQUIRE_PKG([GLP],[glp])
METNO_REQUIRE_PKG([GLTEXT],[glText])
METNO_REQUIRE_PKG([TSDATA],[tsData])
METNO_REQUIRE_PKG([PNG], [libpng])


AC_CONFIG_FILES([
	pets2.pc
	Makefile
	src/Makefile
])

AC_OUTPUT
