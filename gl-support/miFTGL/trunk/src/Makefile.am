# $Id: Makefile.am 4312 2013-12-18 07:51:40Z juergens $

pkgname = miFTGL

lib_LTLIBRARIES = libmiFTGL.la

libmiFTGL_la_SOURCES = \
	FTBitmapGlyph.cpp \
	FTCharmap.cpp \
	FTContour.cpp \
	FTExtrdGlyph.cpp \
	FTFace.cpp \
	FTFont.cpp \
	FTGLBitmapFont.cpp \
	FTGLExtrdFont.cpp \
	FTGLOutlineFont.cpp \
	FTGLPixmapFont.cpp \
	FTGLPolygonFont.cpp \
	FTGLTextureFont.cpp \
	FTGlyph.cpp \
	FTGlyphContainer.cpp \
	FTLibrary.cpp \
	FTOutlineGlyph.cpp \
	FTPixmapGlyph.cpp \
	FTPoint.cpp \
	FTPolyGlyph.cpp \
	FTSize.cpp \
	FTTextureGlyph.cpp \
	FTVectoriser.cpp


pkg_includedir = ${includedir}/miFTGL

pkg_include_HEADERS = \
	FTBBox.h \
	FTBitmapGlyph.h \
	FTCharToGlyphIndexMap.h \
	FTCharmap.h \
	FTContour.h \
	FTExtrdGlyph.h \
	FTFace.h \
	FTFont.h \
	FTGL.h \
	FTGLBitmapFont.h \
	FTGLExtrdFont.h \
	FTGLOutlineFont.h \
	FTGLPixmapFont.h \
	FTGLPolygonFont.h \
	FTGLTextureFont.h \
	FTGlyph.h \
	FTGlyphContainer.h \
	FTLibrary.h \
	FTList.h \
	FTOutlineGlyph.h \
	FTPixmapGlyph.h \
	FTPoint.h \
	FTPolyGlyph.h \
	FTSize.h \
	FTTextureGlyph.h \
	FTVector.h \
	FTVectoriser.h


libmiFTGL_la_CPPFLAGS = \
	${FT2_CPPFLAGS}

libmiFTGL_la_LDFLAGS = ${VERSION_INFO}

if ENABLE_SHARED_LIBRARY
libmiFTGL_la_LDFLAGS += \
	${FT2_LDFLAGS} \
	${OPENGL_LDFLAGS} \
	${FTGL_LDFLAGS}

libmiFTGL_la_LIBADD = \
	${FT2_LIBS} \
	${OPENGL_LIBS} \
	${FTGL_LIBS} -lGL -lGLU
endif



INSTALL=install -p # tells install to preserve timestamp
