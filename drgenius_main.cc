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
#include <glade/glade.h>
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
main_prog (int argc, char *argv[])
{
  int arg;


  bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
  textdomain (GETTEXT_PACKAGE);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  gtk_init (&argc, &argv);

  /* Init Glade stuff */
  glade_init ();


  setlocale (LC_NUMERIC, "C");

  /* Load Scm code */
  drgeoInit ();
     
  /* Load user preferences */
  loadUserPreferences ();

  /* declare drgeo API in guile */
  gh_new_procedure ("move", (SCM (*) ()) drgeo_scm_move, 2, 0, 0);
  gh_new_procedure ("getAbscissa", (SCM (*) ()) drgeo_scm_getAbscissa, 1, 0, 0);
  gh_new_procedure ("setAbscissa", (SCM (*) ()) drgeo_scm_setAbscissa, 2, 0, 0);
  gh_new_procedure ("getCoordinates", (SCM (*) ()) drgeo_scm_getCoordinates, 1, 0, 0);
  gh_new_procedure ("setCoordinates", (SCM (*) ()) drgeo_scm_setCoordinates, 2, 0, 0);
  gh_new_procedure ("getUnit", (SCM (*) ()) drgeo_scm_getUnit, 1, 0, 0);
  gh_new_procedure ("getNormal", (SCM (*) ()) drgeo_scm_getNormal, 1, 0, 0);
  gh_new_procedure ("getSlope", (SCM (*) ()) drgeo_scm_getSlope, 1, 0, 0);
  gh_new_procedure ("getNorm", (SCM (*) ()) drgeo_scm_getNorm, 1, 0, 0);
  gh_new_procedure ("getLength", (SCM (*) ()) drgeo_scm_getLength, 1, 0, 0);
  gh_new_procedure ("getCenter", (SCM (*) ()) drgeo_scm_getCenter, 1, 0, 0);
  gh_new_procedure ("getRadius", (SCM (*) ()) drgeo_scm_getRadius, 1, 0, 0);
  gh_new_procedure ("getValue", (SCM (*) ()) drgeo_scm_getValue, 1, 0, 0);
  gh_new_procedure ("setValue", (SCM (*) ()) drgeo_scm_setValue, 2, 0, 0);
  gh_new_procedure ("getAngle", (SCM (*) ()) drgeo_scm_getAngle, 1, 0, 0);

  /* declare the drgeo interface in Guile */
  gh_new_procedure ("drgeoMake-figure", (SCM (*) ()) drgeo_scm_make_figure, 1, 0, 0);
  gh_new_procedure ("drgeoMake-freePoint", (SCM (*) ()) drgeo_scm_make_freePoint, 4, 0, 0);
  gh_new_procedure ("drgeoMake-pointOnCurve", (SCM (*) ()) drgeo_scm_make_pointOnCurve, 4, 0, 0);
  gh_new_procedure ("drgeoMake-middle2Points", (SCM (*) ()) drgeo_scm_make_middle2Points, 4, 0, 0);
  gh_new_procedure ("drgeoMake-middleSegment", (SCM (*) ()) drgeo_scm_make_middleSegment, 3, 0, 0);
  gh_new_procedure ("drgeoMake-intersection", (SCM (*) ()) drgeo_scm_make_intersection, 5, 0, 0);
  gh_new_procedure ("drgeoMake-segmentExtremity", (SCM (*) ()) drgeo_scm_make_segmentExtremity, 4, 0, 0);
  gh_new_procedure ("drgeoMake-line", (SCM (*) ()) drgeo_scm_make_line, 4, 0, 0);
  gh_new_procedure ("drgeoMake-parallel", (SCM (*) ()) drgeo_scm_make_parallel, 4, 0, 0);
  gh_new_procedure ("drgeoMake-orthogonal", (SCM (*) ()) drgeo_scm_make_orthogonal, 4, 0, 0);
  gh_new_procedure ("drgeoMake-ray", (SCM (*) ()) drgeo_scm_make_ray, 4, 0, 0);
  gh_new_procedure ("drgeoMake-vector", (SCM (*) ()) drgeo_scm_make_vector, 4, 0, 0);
  gh_new_procedure ("drgeoMake-circle2Points", (SCM (*) ()) drgeo_scm_make_circle2Points, 4, 0, 0);
  gh_new_procedure ("drgeoMake-circleRadius", (SCM (*) ()) drgeo_scm_make_circleRadius, 4, 0, 0);
  gh_new_procedure ("drgeoMake-circleSegment", (SCM (*) ()) drgeo_scm_make_circleSegment, 4, 0, 0);
  gh_new_procedure ("drgeoMake-arc", (SCM (*) ()) drgeo_scm_make_arc, 5, 0, 0);
  gh_new_procedure ("drgeoMake-locus", (SCM (*) ()) drgeo_scm_make_locus, 4, 0, 0);
  gh_new_procedure ("drgeoMake-polygon", (SCM (*) ()) drgeo_scm_make_polygon, 3, 0, 0);
  gh_new_procedure ("drgeoMake-freeValue", (SCM (*) ()) drgeo_scm_make_freeValue, 5, 0, 0);
  gh_new_procedure ("drgeoMake-segmentLength", (SCM (*) ()) drgeo_scm_make_segmentLength, 5, 0, 0);
  gh_new_procedure ("drgeoMake-vectorNorm", (SCM (*) ()) drgeo_scm_make_vectorNorm, 5, 0, 0);
  gh_new_procedure ("drgeoMake-distancePointCircle", (SCM (*) ()) drgeo_scm_make_distancePointCircle, 6, 0, 0);
  gh_new_procedure ("drgeoMake-distancePointLine", (SCM (*) ()) drgeo_scm_make_distancePointLine, 6, 0, 0);
  gh_new_procedure ("drgeoMake-distancePointPoint", (SCM (*) ()) drgeo_scm_make_distancePointPoint, 6, 0, 0);
  gh_new_procedure ("drgeoMake-circleLength", (SCM (*) ()) drgeo_scm_make_circleLength, 5, 0, 0);
  gh_new_procedure ("drgeoMake-lineSlope", (SCM (*) ()) drgeo_scm_make_lineSlope, 5, 0, 0);
  gh_new_procedure ("drgeoMake-arcLength", (SCM (*) ()) drgeo_scm_make_arcLength, 5, 0, 0);
  gh_new_procedure ("drgeoMake-geometricAngle", (SCM (*) ()) drgeo_scm_make_geometricAngle, 5, 0, 0);
  gh_new_procedure ("drgeoMake-orientedAngle", (SCM (*) ()) drgeo_scm_make_orientedAngle, 6, 0, 0);

  gh_new_procedure ("drgeoMake-rotation", (SCM (*) ()) drgeo_scm_make_rotation, 5, 0, 0);
  gh_new_procedure ("drgeoMake-scale", (SCM (*) ()) drgeo_scm_make_scale, 5, 0, 0);
  gh_new_procedure ("drgeoMake-symmetry", (SCM (*) ()) drgeo_scm_make_symmetry, 4, 0, 0);
  gh_new_procedure ("drgeoMake-reflexion", (SCM (*) ()) drgeo_scm_make_reflexion, 4, 0, 0);
  gh_new_procedure ("drgeoMake-translation", (SCM (*) ()) drgeo_scm_make_translation, 4, 0, 0);
  gh_new_procedure ("change-color", (SCM (*) ()) drgeo_scm_change_color, 3, 0, 0);
  gh_new_procedure ("change-thickness", (SCM (*) ()) drgeo_scm_change_thickness, 3, 0, 0);
  gh_new_procedure ("change-pointShape", (SCM (*) ()) drgeo_scm_change_pointShape, 3, 0, 0);
  gh_new_procedure ("change-masked", (SCM (*) ()) drgeo_scm_change_masked, 2, 0, 0);

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
}

int
main (int argc, char *argv[])
{
  gh_enter (argc, argv, main_prog);
  return 0;
}

