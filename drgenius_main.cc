/*
 *  Dr Geo
 * (C) Copyright OFSET 2001
 * Author: hilaire@ofset.org 
 * 
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public Licences as by published
 * by the Free Software Foundation; either version 2; or (at your option)
 * any later version
 *
 * This program is distributed in the hope that it will entertaining,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILTY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Publis License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <locale.h>
#include <config.h>
#include <gtk/gtk.h>

#include <libguile.h>
#include <libintl.h>
#include <string.h>

#include "drgeo_init.h"
#include "drgenius_mdi.h"
#include "drgenius_config.h"
#include "drgeo_scm_api.h"
#include "drgeo_scm_interface.h"
#include "drgenius_help.h"


#define  _(x)  gettext (x)
#define N_(x)  x


void main_prog (int argc, char *argv[]);

static char *file_name = NULL;


drgeniusMDI *mdi;
gboolean texmacsMode;

void
endBadParameter (void)
{
  printf ("GNU Dr Geo %s - Copyright 1997-2003 Hilaire Fernandes, OFSET\n\n", VERSION);
  printf (_ ("Command line options:\nTerm under [] are options, || means OR:\n   drgeo [--file geo_file_name || macro_file_name] [geo_file_name.fgeo] [macro_file_name.mgeo] [--evaluate dsf_file_name] [--texmacs] [--help]\n\n--file       : to load a Dr. Geo file\n--evaluate   : to load and evaluate a Dr. Geo Scheme Figure (DSF)\n--texmacs    : start Dr. Geo as a TeXmacs plug-in\n--help       : this help summary\n\nSeveral files can be loaded using a --file command for each\n\n"));

  exit (0);
}


void
main_prog (void *closure, int argc, char *argv[])
{
  int arg;


  bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
  textdomain (GETTEXT_PACKAGE);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  gtk_init (&argc, &argv);

  setlocale (LC_NUMERIC, "C");

  /* Load Scm code */
  drgeoInit ();
     
  /* Load user preferences */
  loadUserPreferences ();

  /* declare drgeo API in guile */
   scm_c_define_gsubr ("move", 2, 0, 0, (void *) drgeo_scm_move);
  scm_c_define_gsubr ("getAbscissa", 1, 0, 0,  (void *) drgeo_scm_getAbscissa);
  scm_c_define_gsubr ("setAbscissa", 2, 0, 0, (void *) drgeo_scm_setAbscissa);
  scm_c_define_gsubr ("getCoordinates", 1, 0, 0, (void *) drgeo_scm_getCoordinates);
  scm_c_define_gsubr ("setCoordinates", 2, 0, 0, (void *) drgeo_scm_setCoordinates);
  scm_c_define_gsubr ("getUnit", 1, 0, 0, (void *) drgeo_scm_getUnit);
  scm_c_define_gsubr ("getNormal", 1, 0, 0, (void *) drgeo_scm_getNormal);
  scm_c_define_gsubr ("getSlope", 1, 0, 0, (void *) drgeo_scm_getSlope);
  scm_c_define_gsubr ("getNorm", 1, 0, 0, (void *) drgeo_scm_getNorm);
  scm_c_define_gsubr ("getLength", 1, 0, 0, (void *) drgeo_scm_getLength);
  scm_c_define_gsubr ("getCenter", 1, 0, 0, (void *) drgeo_scm_getCenter);
  scm_c_define_gsubr ("getRadius", 1, 0, 0, (void *) drgeo_scm_getRadius);
  scm_c_define_gsubr ("getValue", 1, 0, 0, (void *) drgeo_scm_getValue);
  scm_c_define_gsubr ("setValue", 2, 0, 0, (void *) drgeo_scm_setValue);
  scm_c_define_gsubr ("getAngle", 1, 0, 0, (void *) drgeo_scm_getAngle);

  /* declare the drgeo interface in Guile */
  scm_c_define_gsubr ("drgeoMake-figure", 1, 0, 0, (void *) drgeo_scm_make_figure);
  scm_c_define_gsubr ("drgeoMake-freePoint", 4, 0, 0, (void *) drgeo_scm_make_freePoint);
  scm_c_define_gsubr ("drgeoMake-pointOnCurve", 4, 0, 0, (void *) drgeo_scm_make_pointOnCurve);
  scm_c_define_gsubr ("drgeoMake-middle2Points", 4, 0, 0, (void *) drgeo_scm_make_middle2Points);
  scm_c_define_gsubr ("drgeoMake-middleSegment", 3, 0, 0, (void *) drgeo_scm_make_middleSegment);
  scm_c_define_gsubr ("drgeoMake-intersection", 5, 0, 0, (void *) drgeo_scm_make_intersection);
  scm_c_define_gsubr ("drgeoMake-segmentExtremity", 4, 0, 0, (void *) drgeo_scm_make_segmentExtremity);
  scm_c_define_gsubr ("drgeoMake-line", 4, 0, 0, (void *) drgeo_scm_make_line);
  scm_c_define_gsubr ("drgeoMake-parallel", 4, 0, 0, (void *) drgeo_scm_make_parallel);
  scm_c_define_gsubr ("drgeoMake-orthogonal", 4, 0, 0, (void *) drgeo_scm_make_orthogonal);
  scm_c_define_gsubr ("drgeoMake-ray", 4, 0, 0, (void *) drgeo_scm_make_ray);
  scm_c_define_gsubr ("drgeoMake-vector", 4, 0, 0, (void *) drgeo_scm_make_vector);
  scm_c_define_gsubr ("drgeoMake-circle2Points", 4, 0, 0, (void *) drgeo_scm_make_circle2Points);
  scm_c_define_gsubr ("drgeoMake-circleRadius", 4, 0, 0, (void *) drgeo_scm_make_circleRadius);
  scm_c_define_gsubr ("drgeoMake-circleSegment", 4, 0, 0, (void *) drgeo_scm_make_circleSegment);
  scm_c_define_gsubr ("drgeoMake-arc", 5, 0, 0, (void *) drgeo_scm_make_arc);
  scm_c_define_gsubr ("drgeoMake-locus", 4, 0, 0, (void *) drgeo_scm_make_locus);
  scm_c_define_gsubr ("drgeoMake-polygon", 3, 0, 0, (void *) drgeo_scm_make_polygon);
  scm_c_define_gsubr ("drgeoMake-freeValue", 5, 0, 0, (void *) drgeo_scm_make_freeValue);
  scm_c_define_gsubr ("drgeoMake-segmentLength", 5, 0, 0, (void *) drgeo_scm_make_segmentLength);
  scm_c_define_gsubr ("drgeoMake-vectorNorm", 5, 0, 0, (void *) drgeo_scm_make_vectorNorm);
  scm_c_define_gsubr ("drgeoMake-distancePointCircle", 6, 0, 0, (void *) drgeo_scm_make_distancePointCircle);
  scm_c_define_gsubr ("drgeoMake-distancePointPoint", 6, 0, 0, (void *) drgeo_scm_make_distancePointPoint);
  scm_c_define_gsubr ("drgeoMake-circleLength", 5, 0, 0, (void *) drgeo_scm_make_circleLength);
  scm_c_define_gsubr ("drgeoMake-lineSlope", 5, 0, 0, (void *) drgeo_scm_make_lineSlope);
  scm_c_define_gsubr ("drgeoMake-arcLength", 5, 0, 0, (void *) drgeo_scm_make_arcLength);
  scm_c_define_gsubr ("drgeoMake-geometricAngle", 5, 0, 0, (void *) drgeo_scm_make_geometricAngle);
  scm_c_define_gsubr ("drgeoMake-orientedAngle", 6, 0, 0, (void *) drgeo_scm_make_orientedAngle);
  scm_c_define_gsubr ("drgeoMake-rotation", 5, 0, 0, (void *) drgeo_scm_make_rotation);
  scm_c_define_gsubr ("drgeoMake-scale", 5, 0, 0, (void *) drgeo_scm_make_scale);
  scm_c_define_gsubr ("drgeoMake-symmetry", 4, 0, 0, (void *) drgeo_scm_make_symmetry);
  scm_c_define_gsubr ("drgeoMake-reflexion", 4, 0, 0, (void *) drgeo_scm_make_reflexion);
  scm_c_define_gsubr ("drgeoMake-translation", 4, 0, 0, (void *) drgeo_scm_make_translation);
  scm_c_define_gsubr ("change-color", 3, 0, 0, (void *) drgeo_scm_change_color);
  scm_c_define_gsubr ("change-thickness", 3, 0, 0, (void *) drgeo_scm_change_thickness);
  scm_c_define_gsubr ("change-pointShape", 3, 0, 0, (void *) drgeo_scm_change_pointShape);
  scm_c_define_gsubr ("change-masked", 2, 0, 0, (void *) drgeo_scm_change_masked);

  /* not in texmacs mode by default */
  texmacsMode = false;

  /* Main app */
  mdi = new drgeniusMDI ();


  arg = 1;
  while (arg != argc)
    {
      if ((strcasecmp (argv[arg], "--file") == 0) || 
	  (strcasecmp (argv[arg], "-f") == 0))
	{
	  arg++;
	  if (arg == argc)
	    {
	      printf (_ ("\nMiss a filename in --file\n\n"));
	      endBadParameter ();
	    }
	  else
	    mdi->openSession (argv[arg++]);
	}
      else if ((strcasecmp (argv[arg], "--evaluate") == 0) || 
	  (strcasecmp (argv[arg], "-e") == 0))
	{
	  arg++;
	  if (arg == argc)
	    {
	      printf (_ ("\nMiss a filename to evaluate in --evaluate\n\n"));
	      endBadParameter ();
	    }
	  else
	    mdi->evaluateScmFigure (argv[arg++]);
	}
      else if (strcasecmp (argv[arg], "--texmacs") == 0)
	{
	  texmacsMode = true;
	  arg++;
	}
      else if (strcasecmp (argv[arg], "--help") == 0)
	endBadParameter ();
      else if (strcasecmp (argv[arg], "--version") == 0)
	{
	  printf ("Dr Geo %s - Copyright 1997-2003 Hilaire Fernandes, OFSET\n\n", VERSION);
	  exit (0);
	}
      else if (strrchr (argv[arg], '.'))
	{
	if ((strcasecmp (strrchr (argv[arg], '.') + 1, "fgeo") == 0) ||
	    ((strcasecmp (strrchr (argv[arg], '.') + 1, "mgeo") == 0)))
	  mdi->openSession (argv[arg++]);
	else
	    printf (_ ("Unknown file type: %s (try .fgeo, .mgeo)\n"), strrchr (argv[arg++], '.'));
	}
      else
	{
	  printf (_ ("\nUnknows command: %s\n"), argv[arg++]);
	  endBadParameter ();
	}
    }


  gtk_main ();
  scm_shell (argc, argv);
}

int
main (int argc, char *argv[])
{
  scm_boot_guile (argc, argv, main_prog, 0);
  return 0;
}
