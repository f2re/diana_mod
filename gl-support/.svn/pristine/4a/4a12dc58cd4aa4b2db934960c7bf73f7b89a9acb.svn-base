# $Id: Makefile.am 4314 2013-12-18 08:25:30Z juergens $

pkgname = glText

lib_LTLIBRARIES = libglText.la

libglText_la_SOURCES = \
	glText.cc \
	glTextTT.cc \
	glTextQtTexture.cc \
	glTextTTBitmap.cc \
	glTextTTPixmap.cc \
	glTextTTTexture.cc \
	textrenderer.cc

if ENABLE_XFONTS
libglText_la_SOURCES += \
	glTextX.cc
endif


pkg_includedir = ${includedir}/glText

pkg_include_HEADERS = \
	glText.h \
	glTextTT.h \
	glTextQtTexture.h \
	glTextTTBitmap.h \
	glTextTTPixmap.h \
	glTextTTTexture.h \
	textrenderer.h

if ENABLE_XFONTS
pkg_include_HEADERS += glTextX.h
endif


libglText_la_CPPFLAGS = \
	${MIFTGL_CPPFLAGS} \
	${GLP_CPPFLAGS} \
	${OPENGL_CPPFLAGS} \
	${QT4_CPPFLAGS}

if ENABLE_XFONTS
libglText_la_CPPFLAGS += -DXFONTS
endif



libglText_la_LDFLAGS = \
	${VERSION_INFO} \
	${MIFTGL_LDFLAGS} \
	${GLP_LDFLAGS} \
	${OPENGL_LDFLAGS} \
	${QT4_LDFLAGS}

if ENABLE_SHARED_LIBRARY
libglText_la_LIBADD = \
	${MIFTGL_LIBS} \
	${GLP_LIBS} \
	${OPENGL_LIBS} -lX11\
	${QT4_LIBS}
endif





INSTALL=install -p # tells install to preserve timestamp
