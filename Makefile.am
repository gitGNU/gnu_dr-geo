helpdir = $(datadir)/drgeo/help
gladedir = $(datadir)/drgeo/glade
scmdir = $(datadir)/drgeo/scm
appicondir = $(datadir)/pixmaps
SUBDIRS = po geo examples icons glade scm plugin encode

INCLUDES = \
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" 	\
	-DDRGEO_GLADEDIR=\""$(gladedir)"\"		\
	-DDRGEO_SCMDIR=\""$(scmdir)"\"			\
	-DDRGEO_HELPDIR=\""$(helpdir)"\"		\
	-DDRGEO_ICONDIR=\""$(appicondir)"\"		\
	-I$(includedir) 				\
	-I$(srcdir)/geo 				\
	$(DRGEO_CFLAGS) 				\
	$(GUILE_CFLAGS)

bin_PROGRAMS = drgeo

drgeo_SOURCES = \
	drgenius_main.cc	\
	drgenius_mdi.cc		\
	drgenius_mdi.h		\
	drgenius_config.cc	\
	drgenius_config.h	\
	xmlinclude.h		\
	drgenius_help.h		\
	drgeo_adaptDialog.h	\
	drgeo_adaptDialog.cc	\
	drgenius_view.cc	\
	drgenius_view.h		\
	geo_view.cc		\
	geo_view.h		\
	editor_view.h		\
	editor_view.cc		\
	drgeo_init.h		\
	drgeo_init.cc		\
	drgeo_printer.h		\
	drgeo_printer.cc

drgeo_LDFLAGS = -export-dynamic

drgeo_LDADD = \
   $(top_builddir)/geo/libgeo.a \
	$(INTLLIBS) \
	$(DRGEO_LIBS) \
	$(GUILE_LDFLAGS) \
	-lcrypt 		
#	-lefence

EXTRA_DIST = \
	drgeo.spec.in $(desktop_in_files) TODO README NEWS AUTHORS COPYING \
	intltool-extract.in intltool-merge.in intltool-update.in

CLEANFILES = *~ core

desktopdir = $(datadir)/applications
desktop_in_files = drgeo.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

dist-hook:
	cp drgeo.spec $(distdir)









