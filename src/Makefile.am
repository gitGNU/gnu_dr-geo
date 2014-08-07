## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(pkgdatadir)/gui/ui

AM_CPPFLAGS = \
	-DDRGEO_LOCALE_DIR=\""$(localedir)"\" \
	-DDRGEO_SRC_DIR=\""$(srcdir)"\" \
	-DDRGEO_UI_DIR=\""$(uidir)"\" \
	-DDRGEO_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(srcdir)/gui \
	$(DRGEO_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = drgeo

drgeo_SOURCES = \
	main.cpp 

drgeo_LDFLAGS = -export-dynamic

drgeo_LDADD = gui/libgeo.a\
	$(DRGEO_LIBS)
	

EXTRA_DIST = $(ui_DATA)

CLEANFILES = *~ core

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)


SUBDIRS = \
	gui \
	kernel