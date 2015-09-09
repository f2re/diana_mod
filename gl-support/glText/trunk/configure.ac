#
# configure.ac for glText
#

AC_PREREQ(2.59)
AC_INIT([glText], [5.0.1], [diana@met.no])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([src/glText.h])
AC_CONFIG_MACRO_DIR([m4])

METNO_SET_VERSION_INFO(6.0.0)
METNO_PROG_PKG_CONFIG

AC_SUBST([LIBTOOL_DEPS]) 

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL


# Language stack
AC_LANG_PUSH(C++)

# Checks for libraries.
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_LANG_POP([C++])


METNO_ENABLE_CONDITIONAL([SHARED_LIBRARY], [shared])

METNO_ENABLE([XFONTS], [xfonts], [support for X bitmap fonts], [no])
METNO_ENABLE_CONDITIONAL([XFONTS], [xfonts])


METNO_REQUIRE_QT4([Core Gui OpenGL])

METNO_REQUIRE_PKG([MIFTGL], [miFTGL])
METNO_REQUIRE_PKG([GLP], [glp])



AC_CONFIG_FILES([
	glText.pc
	Makefile
	src/Makefile
])

AC_OUTPUT
