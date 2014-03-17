AC_INIT(drgeo,1.1.0,http://bugzilla.gnome.org/enter_bug.cgi?product=drgeo)

AC_CONFIG_SRCDIR([geo/drgeo_point.cc])

AC_PROG_INTLTOOL

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

AC_REVISION($Revision: 1.37 $)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl AM_PROG_LIBTOOL
dnl AM_PATH_GLIB(1.2.0,,,gmodule)


dnl **********************************************************************
dnl * CHECK FOR GTK/GUILE STUFF						 *
dnl **********************************************************************

PKG_CHECK_MODULES(DRGEO,gtk+-2.0 libglade-2.0 libxml-2.0)
AC_SUBST(DRGEO_CFLAGS)
AC_SUBST(DRGEO_LIBS)
GUILE_PROGS

dnl **********************************************************************
dnl * 	DEFINE EXTRA 
dnl **********************************************************************

GUILE_FLAGS


dnl **********************************************************************
dnl *	CREATE OUTPUT VARIABLE 						 *
dnl **********************************************************************

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(GUILE_CFLAGS)
AC_SUBST(GUILE_LDFLAGS)

dnl **********************************************************************
dnl * 	INTERNATIONALIZATION SUPPORT					 *
dnl **********************************************************************
ALL_LINGUAS="az ca cs da de el en_CA en_GB es eu fr ga gl gu hi hr hu it ja ml mr nb ne nl no pl pt pt_BR ru rw sl sq sr sr@Latn sv uk zh_CN zh_TW"

GETTEXT_PACKAGE=drgeo
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", "definition of GETTEXT_PACKAGE")


AC_CONFIG_FILES([
drgeo.spec
Makefile
geo/Makefile
icons/Makefile
glade/Makefile
po/Makefile.in
examples/Makefile
examples/figures/Makefile
examples/macro/Makefile
examples/scheme/Makefile
scm/Makefile
encode/Makefile
plugin/Makefile
plugin/drgeo/Makefile
plugin/drgeo/progs/Makefile
])
AC_OUTPUT
