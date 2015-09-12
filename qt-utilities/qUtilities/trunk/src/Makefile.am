# NB! coserver stuff has been moved into the coserver library

pkgname = qUtilities

lib_LTLIBRARIES = libqUtilities.la

cc_sources= \
	miLogFile.cc 

qt_sources = \
	miSliderWidget.cc \
	qtHelpDialog.cc 

img_sources=\
	circle_green.xpm \
	circle_red.xpm \
	circle_yellow.xpm \
	tb_close.xpm \
	tb_left_arrow.xpm \
	tb_print.xpm \
	tb_right_arrow.xpm \
	tb_search.xpm 



# Generated Qt source code 
moc_sources = $(qt_sources:.cc=.moc.cc) 
%.moc.cc: %.h
	$(MOC4) -o $@ $<

nodist_libqUtilities_la_SOURCES = $(moc_sources)

libqUtilities_la_SOURCES = $(cc_sources) $(qt_sources)

pkg_includedir = ${includedir}/qUtilities

pkg_include_HEADERS = \
	$(cc_sources:.cc=.h)\
	$(qt_sources:.cc=.h)\
	$(img_sources)


libqUtilities_la_CPPFLAGS = ${QT4_CPPFLAGS} ${BOOST_CPPFLAGS}
libqUtilities_la_LDFLAGS  = ${VERSION_INFO}


if ENABLE_SHARED_LIBRARY

libqUtilities_la_LDFLAGS +=  ${QT4_LDFLAGS}
libqUtilities_la_LIBADD   =  ${QT4_LIBS}

endif


# Languages -------------------------

qutilities_languages = nb sv de
language_sources= \
	$(qutilities_languages:%=$(top_srcdir)/lang/qUtilities_%.ts)

languagedir= $(datarootdir)/metlibs/translations
nodist_language_DATA=$(language_sources:.ts=.qm)


%.qm: %.ts  $(qt_sources)
	$(LUPDATE4)  $(qt_sources) -ts $< ; \
	$(LRELEASE4)  $<

lupdate: $(language_sources) $(qt_sources)
	lupdate $(qt_sources) -ts $(language_sources)

CLEANFILES = $(moc_sources)

INSTALL = install -p
