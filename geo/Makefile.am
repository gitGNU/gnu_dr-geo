gladedir = $(datadir)/drgeo/glade
helpdir = $(datadir)/drgeo/help
encodedir = $(datadir)/drgeo/encode

SUBDIRS = 

AM_CPPFLAGS = \
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" 	\
	-DLOCALEDIR=\"$(datadir)/locale\" 		\
	-DLIBRARY_DIR=\""$(datadir)/geo/"\"		\
	-DDRGEO_HELPDIR=\""$(helpdir)"\"		\
	-DDRGEO_GLADEDIR=\""$(gladedir)"\"		\
	-DDRGEO_ENCODEDIR=\""$(encodedir)"\"		\
	-I$(includedir) 				\
	$(DRGEO_CFLAGS)				

noinst_LIBRARIES = libgeo.a

libgeo_a_SOURCES = \
	drgeo_drawable.cc drgeo_drawable.h 			\
	drgeo_gtkdrawable.cc drgeo_gtkdrawable.h 		\
	drgeo_latexdrawable.cc drgeo_latexdrawable.h 		\
	drgeo_postscriptdrawable.cc drgeo_postscriptdrawable.h 	\
	drgeo_flydrawable.cc drgeo_flydrawable.h	 	\
	drgeo_figure.cc drgeo_figure.h 				\
	macro.cc macro.h 					\
	drgeo_macro.cc drgeo_macro.h 				\
	drgeo_gtkmacro.cc drgeo_gtkmacro.h 			\
	drgeo_tool.cc drgeo_tool.h 				\
	liste_elem.cc liste_elem.h 				\
	mode_obj.cc mode_obj.h 					\
	traite.cc traite.h 					\
	drgeo_control.cc drgeo_control.h 			\
	drgeo_menu.cc drgeo_menu.h 				\
	drgeo_toolbar.cc drgeo_toolbar.h			\
	drgeo_dialog.cc drgeo_dialog.h				\
	drgeo_type.cc drgeo_type.h 				\
	couleur.h 						\
	define.h 						\
	var_decl.h 						\
	xml.cc xml.h						\
	drgeo_drgeoStyle.h					\
	drgeo_drgeoVector.h drgeo_drgeoVector.cc		\
	drgeo_geometricObject.h drgeo_geometricObject.cc	\
	drgeo_point.h drgeo_point.cc				\
	drgeo_repere.h drgeo_repere.cc				\
	drgeo_direction.h drgeo_direction.cc			\
	drgeo_vector.h	drgeo_vector.cc				\
	drgeo_buildObject.h drgeo_buildObject.cc		\
	drgeo_line.h drgeo_line.cc				\
	drgeo_halfLine.h drgeo_halfLine.cc			\
	drgeo_segment.h drgeo_segment.cc			\
	drgeo_circle.h drgeo_circle.cc				\
	drgeo_arcCircle.h drgeo_arcCircle.cc			\
	drgeo_value.h drgeo_value.cc				\
	drgeo_angle.h drgeo_angle.cc				\
	drgeo_numeric.h drgeo_numeric.cc			\
	drgeo_string.h drgeo_string.cc				\
	drgeo_equation.h drgeo_equation.cc			\
	drgeo_locus.h drgeo_locus.cc				\
	drgeo_command.h drgeo_command.cc			\
	drgeo_gtkstyle.h drgeo_gtkstyle.cc			\
	drgeo_gtkproperty.h drgeo_gtkproperty.cc		\
	drgeo_polygon.h drgeo_polygon.cc			\
	drgeo_script.h drgeo_script.cc				\
	drgeo_scm_interface.h drgeo_scm_interface.cc		\
	drgeo_scm_api.h drgeo_scm_api.cc			\
	drgeo_scm_helper.h drgeo_scm_helper.cc			\
	drgeo_gtkhelpers.h drgeo_gtkhelpers.cc

CLEANFILES = $(BUILT_SOURCES)

EXTRA_DIST = DevelopersFAQ

