/*
 *  Dr Genius 
 * (C) Copyright Hilaire Fernandes  2000
 * hilaire@ofset.org 
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

#include "drgeo_figure.h"
#include "drgeo_point.h"
#include "drgeo_vector.h"
#include "drgeo_line.h"
#include "drgeo_halfLine.h"
#include "drgeo_segment.h"
#include "drgeo_circle.h"
#include "drgeo_arcCircle.h"
#include "drgeo_angle.h"
#include "drgeo_numeric.h"
#include "drgeo_equation.h"
#include "drgeo_locus.h"
#include "drgeo_polygon.h"
#include "drgeo_script.h"
#include "var_decl.h"

GList *
buildFreePoint (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  if (cas == 0)
    {
      item = new point (at, *selection, ON_CURVE, FALSE,
			figure->getFigureList ());
      build = g_list_append (build, (gpointer) item);
    }
  return build;
}

GList *
buildMiddlePoint (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* middle of a segment */
      item = new point (*selection, MIDDLE_SEGMENT, FALSE,
			figure->getFigureList (), 0);
      break;
    case 1:
      /* middle of two points */
      item = new point (*selection, MIDDLE_2PTS, FALSE,
			figure->getFigureList (), 0);
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildIntersectionPoint (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  if (cas == 0)
    {
      item = new point (*selection, INTERSECTION,
			FALSE, figure->getFigureList (), 0);
      if (figure->doesItemExist (item))
	delete (item);
      else
	build = g_list_append (build, (gpointer) item);
      selection->init_lire ();
      if (searchForCategory (*selection,
			     (objectCategory) (CIRCLE | ARC_CIRCLE)) != NULL)
	{
	  /* one of the object is a circle, so we build the  
	     second intersection point */

	  item = new point (*selection, INTERSECTION,
			    FALSE, figure->getFigureList (), 1);
	  build = g_list_append (build, (gpointer) item);
	}
    }
  return build;
}

GList *
buildCoordinatePoint (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* point defined by its coordinate */
      item = new point (*selection, COORDINATE, FALSE,
			figure->getFigureList (), 0);
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildLine (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* line going through 2 points */
      item = new line (*selection, LINE_2PTS, FALSE,
		       figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildHalfLine (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* half line going through 2 points */
      item = new halfLine (*selection, HALFLINE_2PTS, FALSE,
			   figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildSegment (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* segment going through 2 points */
      item = new segment (*selection, SEGMENT_2PTS, FALSE,
			  figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildVector (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  if (cas == 0)
    {
      item = new vector (*selection, VECTOR_2PTS,
			 FALSE, figure->getFigureList ());
      if (figure->doesItemExist (item))
	delete (item);
      else
	build = g_list_append (build, (gpointer) item);
    }

  return build;
}

GList *
buildCircle (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* circle define by center and point */
      item = new circle (*selection, CIRCLE_2PTS, FALSE,
			 figure->getFigureList ());
      break;
    case 1:
      /* circle define by center and segment */
      item = new circle (*selection, CIRCLE_SEGMENT, FALSE,
			 figure->getFigureList ());
      break;
    case 2:
      /* circle define by center and value */
      item = new circle (*selection, CIRCLE_VALUE, FALSE,
			 figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildArcCircle (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* arc circle define by 3 points */
      item = new arcCircle (*selection, ARCCIRCLE_3PTS, FALSE,
			    figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildLocus (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* locus defined by two points */
      item = new locus (*selection, FALSE, figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildPolygon (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* polygon defined by n points */
      item = new polygon (*selection, POLYGON_NPTS, FALSE,
			  figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildParallelLine (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* line going through 2 points */
      item = new line (*selection, LINE_PAR_DIRECTION,
		       FALSE, figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildOrthogonalLine (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      /* line Perp to one direction */
      item = new line (*selection, LINE_PER_DIRECTION,
		       FALSE, figure->getFigureList ());
      break;
    }
  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildReflexion (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // un point
      item = new point (*selection, (pointType) REFLEXION, FALSE,
			figure->getFigureList (), 0);
      break;
    case 1:
      // droite
      item = new line (*selection, (lineType) REFLEXION,
		       FALSE, figure->getFigureList ());
      break;
    case 2:
      // demi-droite
      item = new halfLine (*selection, (halfLineType) REFLEXION,
			   FALSE, figure->getFigureList ());
      break;
    case 3:
      // segment
      item = new segment (*selection, (segmentType) REFLEXION,
			  FALSE, figure->getFigureList ());
      break;
    case 4:
      // vector
      item = new vector (*selection, (vectorType) REFLEXION,
			 FALSE, figure->getFigureList ());
      break;
    case 5:
      // cercle
      item = new circle (*selection, (circleType) REFLEXION,
			 FALSE, figure->getFigureList ());
      break;
    case 6:
      // arc cercle
      item = new arcCircle (*selection, (arcCircleType) REFLEXION,
			    FALSE, figure->getFigureList ());
      break;
    case 7:
      // polygon
      item = new polygon (*selection, (polygonType) REFLEXION,
			  FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildSymmetry (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // un point
      item = new point (*selection, (pointType) SYMMETRY,
			FALSE, figure->getFigureList (), 0);
      break;
    case 1:
      // droite
      item = new line (*selection, (lineType) SYMMETRY,
		       FALSE, figure->getFigureList ());
      break;
    case 2:
      // demi-droite
      item = new halfLine (*selection, (halfLineType) SYMMETRY,
			   FALSE, figure->getFigureList ());
      break;
    case 3:
      // segment
      item = new segment (*selection, (segmentType) SYMMETRY,
			  FALSE, figure->getFigureList ());
      break;
    case 4:
      // cercle
      item = new circle (*selection, (circleType) SYMMETRY,
			 FALSE, figure->getFigureList ());
      break;
    case 5:
      // arc cercle
      item = new arcCircle (*selection, (arcCircleType) SYMMETRY,
			    FALSE, figure->getFigureList ());
      break;
    case 6:
      // vector
      item = new vector (*selection, (vectorType) SYMMETRY,
			 FALSE, figure->getFigureList ());
      break;
    case 7:
      // polygon
      item = new polygon (*selection, (polygonType) SYMMETRY,
			  FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildTranslation (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // un point
      item = new point (*selection, (pointType) TRANSLATION,
			FALSE, figure->getFigureList (), 0);
      break;
    case 1:
      // droite
      item = new line (*selection, (lineType) TRANSLATION,
		       FALSE, figure->getFigureList ());
      break;
    case 2:
      // demi-droite
      item = new halfLine (*selection, (halfLineType) TRANSLATION,
			   FALSE, figure->getFigureList ());
      break;
    case 3:
      item = new segment (*selection, (segmentType) TRANSLATION,
			  FALSE, figure->getFigureList ());
      // segment
      break;
    case 4:
      // cercle
      item = new circle (*selection, (circleType) TRANSLATION,
			 FALSE, figure->getFigureList ());
      break;
    case 5:
      // arc cercle
      item = new arcCircle (*selection, (arcCircleType) TRANSLATION,
			    FALSE, figure->getFigureList ());
      break;
    case 6:
      // polygon
      item = new polygon (*selection, (polygonType) TRANSLATION,
			  FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildRotation (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // un point
      item = new point (*selection, (pointType) ROTATION,
			FALSE, figure->getFigureList (), 0);
      break;
    case 1:
      // droite
      item = new line (*selection, (lineType) ROTATION,
		       FALSE, figure->getFigureList ());
      break;
    case 2:
      // demi-droite
      item = new halfLine (*selection, (halfLineType) ROTATION,
			   FALSE, figure->getFigureList ());
      break;
    case 3:
      item = new segment (*selection, (segmentType) ROTATION,
			  FALSE, figure->getFigureList ());
      // segment
      break;
    case 4:
      // vector
      item = new vector (*selection, (vectorType) ROTATION,
			 FALSE, figure->getFigureList ());
      break;
    case 5:
      // cercle
      item = new circle (*selection, (circleType) ROTATION,
			 FALSE, figure->getFigureList ());
      break;
    case 6:
      // arc cercle
      item = new arcCircle (*selection, (arcCircleType) ROTATION,
			    FALSE, figure->getFigureList ());
      break;
    case 7:
      // polygon
      item = new polygon (*selection, (polygonType) ROTATION,
			  FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildScale (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // un point
      item = new point (*selection, (pointType) SCALE, FALSE,
			figure->getFigureList (), 0);
      break;
    case 1:
      // droite
      item = new line (*selection, (lineType) SCALE,
		       FALSE, figure->getFigureList ());
      break;
    case 2:
      item = new halfLine (*selection, (halfLineType) SCALE,
			   FALSE, figure->getFigureList ());
      // demi-droite
      break;
    case 3:
      item = new segment (*selection, (segmentType) SCALE,
			  FALSE, figure->getFigureList ());
      // segment
      break;
    case 4:
      // vector
      item = new vector (*selection, (vectorType) SCALE,
			 FALSE, figure->getFigureList ());
      break;
    case 5:
      // cercle
      item = new circle (*selection, (circleType) SCALE,
			 FALSE, figure->getFigureList ());
      break;
    case 6:
      // arc cercle
      item = new arcCircle (*selection, (arcCircleType) SCALE,
			    FALSE, figure->getFigureList ());
      break;
    case 7:
      // polygon
      item = new polygon (*selection, (polygonType) SCALE,
			  FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildNumeric (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // segment's length
      item = new numeric (at, *selection, (numericType) SEGMENT_LENGTH,
			  FALSE, figure->getFigureList ());
      break;
    case 1:
      // vector's norm
      item = new numeric (at, *selection, (numericType) VECTOR_NORM,
			  FALSE, figure->getFigureList ());
      break;
    case 2:
      // distance between 2 points
      item = new numeric (at, *selection, (numericType) DISTANCE_2PTS,
			  FALSE, figure->getFigureList ());
      break;
    case 3:
      // distance point to circle
      item = new numeric (at, *selection, (numericType) DISTANCE_PT_CIRCLE,
			  FALSE, figure->getFigureList ());
      break;
    case 4:
      // distance point to line
      item = new numeric (at, *selection, (numericType) DISTANCE_PT_LINE,
			  FALSE, figure->getFigureList ());
      break;
    case 5:
      // circle perimeter
      item = new numeric (at, *selection, (numericType) CIRCLE_PERIMETER,
			  FALSE, figure->getFigureList ());
      break;
    case 6:
      // line slope
      item = new numeric (at, *selection, (numericType) LINE_SLOPE,
			  FALSE, figure->getFigureList ());
      break;
    case 7:
      // arc circle length
      item = new numeric (at, *selection, (numericType) ARC_CIRCLE_LENGTH,
			  FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildAngle (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:
      // angle 3 points
      item = new angle (at, *selection, (angleType) ANGLE_3PTS,
			FALSE, figure->getFigureList ());
      break;
    case 1:
      // angle 2 vectors
      item = new angle (at, *selection, (angleType) ANGLE_2VEC,
			FALSE, figure->getFigureList ());
      break;
    }

  if (figure->doesItemExist (item))
    delete (item);
  else
    build = g_list_append (build, (gpointer) item);
  return build;
}

GList *
buildCoordinateAndEquation (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  drgeoVector offset;
  GList *build = NULL;

  selection = figure->getSelection ();
  /* a bit ugly, but there is all the condition are not
     symmetric */
  switch (cas)
    {
    case 0:
      item = new numeric (at, *selection, POINT_ABSCISSA,
			  FALSE, figure->getFigureList ());
      if (figure->doesItemExist (item))
	delete (item);
      else
	{
	  offset.set (0, -figure->getDrawable ()->pixelToWorld (20));
	  offset += at;
	  build = g_list_append (build, (gpointer) item);
	  item = new numeric (offset, *selection, POINT_ORDINATE,
			      FALSE, figure->getFigureList ());
	  build = g_list_append (build, (gpointer) item);
	}
      break;
    case 1:
      item = new numeric (at, *selection, VECTOR_ABSCISSA,
			  FALSE, figure->getFigureList ());
      if (figure->doesItemExist (item))
	delete (item);
      else
	{
	  build = g_list_append (build, (gpointer) item);
	  offset.set (0, -figure->getDrawable ()->pixelToWorld (20));
	  offset += at;
	  item = new numeric (offset, *selection, VECTOR_ORDINATE,
			      FALSE, figure->getFigureList ());
	  build = g_list_append (build, (gpointer) item);
	}
      break;
    case 2:
      /* LINE EQUATION */
      item = new equation (at, *selection, LINE_EQUATION,
			   FALSE, figure->getFigureList ());

      if (figure->doesItemExist (item))
	delete (item);
      else
	build = g_list_append (build, (gpointer) item);
      break;
    case 3:
      /* CIRCLE EQUATION */
      item = new equation (at, *selection, CIRCLE_EQUATION,
			   FALSE, figure->getFigureList ());
      if (figure->doesItemExist (item))
	delete (item);
      else
	build = g_list_append (build, (gpointer) item);
      break;
    }
  return build;
}

GList *
buildScript (int cas, drgeoPoint & at, drgeoFigure * figure)
{
  geometricObject *item;
  liste_elem *selection;
  GList *build = NULL;
  gchar *scriptCode;

  selection = figure->getSelection ();
  switch (cas)
    {
    case 0:

      /* FIXME: write code there GtkText... */
      scriptCode = g_strdup ("'\"Dr. Genius\"");

      /* script defined by n objects */
      item = new script (at, *selection, SCRIPT_NITEMS, scriptCode,
			 FALSE, figure->getFigureList ());
      g_free (scriptCode);
      break;
    }

  build = g_list_append (build, (gpointer) item);
  return build;
}
