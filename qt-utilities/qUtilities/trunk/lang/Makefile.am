
languagedir = $(pkgdatadir)/translations
language_sources = qUtilities_de.ts qUtilities_nb.ts qUtilities_sv.ts
language_DATA = $(language_sources:%.ts=%.qm)

%.qm: %.ts
	$(LRELEASE4) $<

CLEANFILES = $(language_DATA)
