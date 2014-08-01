## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(pkgdatadir)/gui/ui

AM_CPPFLAGS = \
	-DDRGEO_LOCALE_DIR=\""$(localedir)"\" \
	-DDRGEO_SRC_DIR=\""$(srcdir)"\" \
	-DDRGEO_UI_DIR=\""$(uidir)"\" \
	-DDRGEO_DATA_DIR=\""$(pkgdatadir)"\" \
	-I$(srcdir)/gui/ui \
	$(DRGEO_CFLAGS)

noinst_LIBRARIES = libgeo.a

libgeo_a_SOURCES = \
	drgeoTDI.cpp \
	drgeoTDI.h \
	drgeoDrawingArea.cpp \
	drgeoDrawingArea.h \
	drgeoPainter.cpp \
	drgeoPainter.h

CLEANFILES = $(BUILT_SOURCES) 

SUBDIRS = \
	ui