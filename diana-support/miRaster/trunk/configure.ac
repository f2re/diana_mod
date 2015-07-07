#
# configure.ac for miRaster
#

AC_PREREQ(2.59)
AC_INIT([miRaster], [5.1.3], [diana@met.no])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([src/satimg.h])
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

METNO_REQUIRE_PKG([MILOGGER],[miLogger])
METNO_REQUIRE_PKG([PUCTOOLS],[puCtools])
METNO_REQUIRE_PKG([PUCTOOLS],[puTools])

METNO_WITH_LIBRARY([GEOTIFF], [geotiff], [geotiff library], [no])
METNO_IF_WITH([geotiff],  
[
       # Check geotiff distribution
AC_CHECK_HEADER([libgeotiff/geotiff.h], [break]) 

if test x"$ac_cv_header_libgeotiff_geotiff_h" == xyes
then
    METNO_REQUIRE_C_LIBRARY([GEOTIFF], [geotiff], [libgeotiff/geotiff.h], [main], [${ZLIB_LIBS} ${TIFF_LIBS}])
	GEOTIFF_CPPFLAGS="${GEOTIFF_CPPFLAGS} -DLIBGEOTIFF"
else
	METNO_REQUIRE_C_LIBRARY([GEOTIFF], [geotiff], [geotiff/geotiff.h], [main], [${ZLIB_LIBS} ${TIFF_LIBS}])
fi  
    ])
METNO_WITH_CONDITIONAL([GEOTIFF], [geotiff])

METNO_WITH_HDF5([no])
METNO_WITH_CONDITIONAL([HDF5], [hdf5])

METNO_REQUIRE_C_LIBRARY([PROJ], [proj], [proj_api.h], [pj_init])

METNO_REQUIRE_C_LIBRARY([JPEG], [jpeg], [jpeglib.h], [jpeg_read_header])
METNO_REQUIRE_C_LIBRARY([TIFF], [tiff], [tiff.h], [TIFFOpen], [${ZLIB_LIBS} ${JPEG_LIBS}])
METNO_REQUIRE_BOOST
METNO_REQUIRE_BOOST_FEATURE([thread])



AC_CONFIG_FILES([
	miRaster.pc
	Makefile
	src/Makefile
])

AC_OUTPUT
