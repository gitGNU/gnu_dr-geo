/*
 *  Dr Geo 
 * (C) Copyright Hilaire Fernandes  2003
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

#include "drgenius_mdi.h"
#include "drgeo_scm_interface.h"
#include "drgeo_scm_helper.h"
#include "drgeo_point.h"
#include "drgeo_segment.h"
#include "drgeo_line.h"
#include "drgeo_halfLine.h"
#include "drgeo_vector.h"
#include "drgeo_circle.h"
#include "drgeo_arcCircle.h"
#include "drgeo_locus.h"
#include "drgeo_polygon.h"
#include "drgeo_numeric.h"
#include "drgeo_angle.h"
#include "liste_elem.h"
#include "drgenius_mdi.h"
#include "geo_view.h"
#include "drgeo_gtkdrawable.h"

extern drgeniusMDI *mdi;

ScmInterface::ScmInterface (drgeoFigure *figure)
{
  this->figure = figure;
  parents = new liste_elem ();
}


/* 
   INSTANTIATE A FREE POINT 
   Usage:
   (make-freePoint nom x y)
   Returned value: the address object
*/
SCM 
ScmInterface::make_freePoint (SCM name, SCM x, SCM y)
{
  geometricObject *item;

  parents->vide ();

  item = (geometricObject *) new 
    point (scmCoord2drgeoPoint (x, y), *parents, FREE, 
	   FALSE, figure->getFigureList());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}
SCM
ScmInterface::make_pointOnCurve (SCM name, SCM c, SCM v)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (c));

  item = (geometricObject *) 
    new point (gh_scm2double (v), *parents, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}
SCM 
ScmInterface::make_middle2Points (SCM name, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  item = (geometricObject *) new
    point (*parents, MIDDLE_2PTS, FALSE, figure->getFigureList (), 0);
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM 
ScmInterface::make_middleSegment (SCM name, SCM s)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (s));
  item = (geometricObject *) new
    point (*parents, MIDDLE_SEGMENT, FALSE, figure->getFigureList (), 0);
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM
ScmInterface::make_intersection (SCM name, SCM obj1, SCM obj2, SCM k)
{
  geometricObject *item;
  gint myK;

  myK = gh_scm2int (k);

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (obj1));
  parents->ajoute ((void *) gh_scm2ulong (obj2));
  item = (geometricObject *) new
    point (*parents, INTERSECTION, FALSE, figure->getFigureList (), myK);
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);    
}

SCM
ScmInterface::make_rotation (SCM name, SCM object, SCM c, SCM a)
{
  geometricObject *item, *myObject;
  objectCategory category;
  
  myObject = (geometricObject *)  gh_scm2ulong (object);
  category = myObject->getCategory ();

  parents->vide ();
  parents->ajoute ((void *) myObject);
  parents->ajoute ((void *) gh_scm2ulong (c));
  parents->ajoute ((void *) gh_scm2ulong (a));


  if (category & POINT)
    item = (geometricObject *) new
      point (*parents, (pointType) ROTATION, FALSE, 
	     figure->getFigureList (), 0);
  else if (category & SEGMENT)
    item = (geometricObject *) new
      segment (*parents, (segmentType) ROTATION, FALSE,
	       figure->getFigureList ());
  else if (category & LINE)
    item = (geometricObject *) new
      line (*parents, (lineType) ROTATION, FALSE,
	    figure->getFigureList ());
  else if (category & HALF_LINE)
    item = (geometricObject *) new
      halfLine (*parents, (halfLineType) ROTATION, FALSE,
		figure->getFigureList ());
  else if (category & VECTOR)
    item = (geometricObject *) new
      vector (*parents, (vectorType) ROTATION, FALSE,
		figure->getFigureList ());
  else if (category & CIRCLE)
    item = (geometricObject *) new
      circle (*parents, (circleType) ROTATION, FALSE,
		figure->getFigureList ());
  else if (category & ARC_CIRCLE)
    item = (geometricObject *) new
      arcCircle (*parents, (arcCircleType) ROTATION, FALSE,
		figure->getFigureList ());
  else if (category & POLYGON)
    item = (geometricObject *) new
      polygon (*parents, (polygonType) ROTATION, FALSE,
		figure->getFigureList ());
  else
      g_print ("Cannot rotate the object\n");

  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item); 
}
SCM
ScmInterface::make_scale (SCM name, SCM object, SCM c, SCM f)
{
  geometricObject *item, *myObject;
  objectCategory category;
  
  myObject = (geometricObject *)  gh_scm2ulong (object);
  category = myObject->getCategory ();

  parents->vide ();
  parents->ajoute ((void *) myObject);
  parents->ajoute ((void *) gh_scm2ulong (c));
  parents->ajoute ((void *) gh_scm2ulong (f));


  if (category & POINT)
    item = (geometricObject *) new
      point (*parents, (pointType) SCALE, FALSE, 
	     figure->getFigureList (), 0);
  else if (category & SEGMENT)
    item = (geometricObject *) new
      segment (*parents, (segmentType) SCALE, FALSE,
	       figure->getFigureList ());
  else if (category & LINE)
    item = (geometricObject *) new
      line (*parents, (lineType) SCALE, FALSE,
	    figure->getFigureList ());
  else if (category & HALF_LINE)
    item = (geometricObject *) new
      halfLine (*parents, (halfLineType) SCALE, FALSE,
		figure->getFigureList ());
  else if (category & VECTOR)
    item = (geometricObject *) new
      vector (*parents, (vectorType) SCALE, FALSE,
		figure->getFigureList ());
  else if (category & CIRCLE)
    item = (geometricObject *) new
      circle (*parents, (circleType) SCALE, FALSE,
		figure->getFigureList ());
  else if (category & ARC_CIRCLE)
    item = (geometricObject *) new
      arcCircle (*parents, (arcCircleType) SCALE, FALSE,
		figure->getFigureList ());
  else if (category & POLYGON)
    item = (geometricObject *) new
      polygon (*parents, (polygonType) SCALE, FALSE,
		figure->getFigureList ());
  else
      g_print ("Cannot scale the object\n");

  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item); 
}
SCM
ScmInterface::make_symmetry (SCM name, SCM object, SCM c)
{
  geometricObject *item, *myObject;
  objectCategory category;
  
  myObject = (geometricObject *)  gh_scm2ulong (object);
  category = myObject->getCategory ();

  parents->vide ();
  parents->ajoute ((void *) myObject);
  parents->ajoute ((void *) gh_scm2ulong (c));

  if (category & POINT)
    item = (geometricObject *) new
      point (*parents, (pointType) SYMMETRY, FALSE, 
	     figure->getFigureList (), 0);
  else if (category & SEGMENT)
    item = (geometricObject *) new
      segment (*parents, (segmentType) SYMMETRY, FALSE,
	       figure->getFigureList ());
  else if (category & LINE)
    item = (geometricObject *) new
      line (*parents, (lineType) SYMMETRY, FALSE,
	    figure->getFigureList ());
  else if (category & HALF_LINE)
    item = (geometricObject *) new
      halfLine (*parents, (halfLineType) SYMMETRY, FALSE,
		figure->getFigureList ());
  else if (category & VECTOR)
    item = (geometricObject *) new
      vector (*parents, (vectorType) SYMMETRY, FALSE,
		figure->getFigureList ());
  else if (category & CIRCLE)
    item = (geometricObject *) new
      circle (*parents, (circleType) SYMMETRY, FALSE,
		figure->getFigureList ());
  else if (category & ARC_CIRCLE)
    item = (geometricObject *) new
      arcCircle (*parents, (arcCircleType) SYMMETRY, FALSE,
		figure->getFigureList ());
  else if (category & POLYGON)
    item = (geometricObject *) new
      polygon (*parents, (polygonType) SYMMETRY, FALSE,
		figure->getFigureList ());
  else
      g_print ("Cannot compute the symmetric the object\n");

  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item); 
}
SCM
ScmInterface::make_reflexion (SCM name, SCM object, SCM d)
{
  geometricObject *item, *myObject;
  objectCategory category;
  
  myObject = (geometricObject *)  gh_scm2ulong (object);
  category = myObject->getCategory ();

  parents->vide ();
  parents->ajoute ((void *) myObject);
  parents->ajoute ((void *) gh_scm2ulong (d));

  if (category & POINT)
    item = (geometricObject *) new
      point (*parents, (pointType) REFLEXION, FALSE, 
	     figure->getFigureList (), 0);
  else if (category & SEGMENT)
    item = (geometricObject *) new
      segment (*parents, (segmentType) REFLEXION, FALSE,
	       figure->getFigureList ());
  else if (category & LINE)
    item = (geometricObject *) new
      line (*parents, (lineType) REFLEXION, FALSE,
	    figure->getFigureList ());
  else if (category & HALF_LINE)
    item = (geometricObject *) new
      halfLine (*parents, (halfLineType) REFLEXION, FALSE,
		figure->getFigureList ());
  else if (category & VECTOR)
    item = (geometricObject *) new
      vector (*parents, (vectorType) REFLEXION, FALSE,
		figure->getFigureList ());
  else if (category & CIRCLE)
    item = (geometricObject *) new
      circle (*parents, (circleType) REFLEXION, FALSE,
		figure->getFigureList ());
  else if (category & ARC_CIRCLE)
    item = (geometricObject *) new
      arcCircle (*parents, (arcCircleType) REFLEXION, FALSE,
		figure->getFigureList ());
  else if (category & POLYGON)
    item = (geometricObject *) new
      polygon (*parents, (polygonType) REFLEXION, FALSE,
		figure->getFigureList ());
  else
      g_print ("Cannot compute the symmetric the object\n");

  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item); 
}
SCM
ScmInterface::make_translation (SCM name, SCM object, SCM v)
{
  geometricObject *item, *myObject;
  objectCategory category;
  
  myObject = (geometricObject *)  gh_scm2ulong (object);
  category = myObject->getCategory ();

  parents->vide ();
  parents->ajoute ((void *) myObject);
  parents->ajoute ((void *) gh_scm2ulong (v));

  if (category & POINT)
    item = (geometricObject *) new
      point (*parents, (pointType) TRANSLATION, FALSE, 
	     figure->getFigureList (), 0);
  else if (category & SEGMENT)
    item = (geometricObject *) new
      segment (*parents, (segmentType) TRANSLATION, FALSE,
	       figure->getFigureList ());
  else if (category & LINE)
    item = (geometricObject *) new
      line (*parents, (lineType) TRANSLATION, FALSE,
	    figure->getFigureList ());
  else if (category & HALF_LINE)
    item = (geometricObject *) new
      halfLine (*parents, (halfLineType) REFLEXION, FALSE,
		figure->getFigureList ());
  else if (category & CIRCLE)
    item = (geometricObject *) new
      circle (*parents, (circleType) TRANSLATION, FALSE,
		figure->getFigureList ());
  else if (category & ARC_CIRCLE)
    item = (geometricObject *) new
      arcCircle (*parents, (arcCircleType) TRANSLATION, FALSE,
		figure->getFigureList ());
  else if (category & POLYGON)
    item = (geometricObject *) new
      polygon (*parents, (polygonType) TRANSLATION, FALSE,
		figure->getFigureList ());
  else
      g_print ("Cannot compute the symmetric the object\n");

  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item); 
}
SCM
ScmInterface::make_segmentExtremity (SCM name, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  item = (geometricObject *) new
    segment (*parents, SEGMENT_2PTS, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}

SCM
ScmInterface::make_line (SCM name, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  item = (geometricObject *) new
    line (*parents, LINE_2PTS, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM
ScmInterface::make_parallel (SCM name, SCM p, SCM d)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (d));
  item = (geometricObject *) new
    line (*parents, LINE_PAR_DIRECTION, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM
ScmInterface::make_orthogonal (SCM name, SCM p, SCM d)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (d));
  item = (geometricObject *) new
    line (*parents, LINE_PER_DIRECTION, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}

SCM
ScmInterface::make_ray (SCM name, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  item = (geometricObject *) new
    halfLine (*parents, HALFLINE_2PTS, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}

SCM
ScmInterface::make_vector (SCM name, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  item = (geometricObject *) new
    vector (*parents, VECTOR_2PTS, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}

SCM
ScmInterface::make_circle2Points (SCM name, SCM p, SCM c)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (c));
  item = (geometricObject *) new
    circle (*parents, CIRCLE_2PTS, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM
ScmInterface::make_circleRadius (SCM name, SCM p, SCM r)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (r));
  item = (geometricObject *) new
    circle (*parents, CIRCLE_VALUE, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM
ScmInterface::make_circleSegment (SCM name, SCM p, SCM s)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (s));
  item = (geometricObject *) new
    circle (*parents, CIRCLE_SEGMENT, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}

SCM 
ScmInterface::make_arc (SCM name, SCM p1, SCM p2, SCM p3)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  parents->ajoute ((void *) gh_scm2ulong (p3));
  item = (geometricObject *) new
    arcCircle (*parents, ARCCIRCLE_3PTS, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}
SCM
ScmInterface::make_locus (SCM name, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  item = (geometricObject *) new
    locus (*parents, FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}

SCM
ScmInterface::make_polygon (SCM name, SCM list)
{
  geometricObject *item;

  parents->vide ();
  while (!gh_null_p (list))
    {
      parents->ajoute ((void *) gh_scm2ulong (gh_car(list)));
      list = gh_cdr (list);
    }
  item = (geometricObject *) new polygon (*parents, POLYGON_NPTS, 
					  FALSE, figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);  
}


SCM
ScmInterface::make_freeValue (SCM name, SCM x, SCM y, SCM v)
{
  geometricObject *item;

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), 
					  gh_scm2double (v), 
					  (numericType) FREE_VALUE, false);
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_segmentLength (SCM name, SCM x, SCM y, SCM s)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (s));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) SEGMENT_LENGTH, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}
SCM
ScmInterface::make_vectorNorm (SCM name, SCM x, SCM y, SCM s)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (s));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) VECTOR_NORM, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}
SCM
ScmInterface::make_distancePointCircle (SCM name, SCM x, SCM y, 
					SCM p, SCM c)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (c));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) DISTANCE_PT_CIRCLE, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_distancePointLine (SCM name, SCM x, SCM y, SCM p, SCM l)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p));
  parents->ajoute ((void *) gh_scm2ulong (l));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) DISTANCE_PT_LINE, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_distancePointPoint (SCM name, SCM x, SCM y, SCM p1, SCM p2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) DISTANCE_2PTS, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_circleLength (SCM name, SCM x, SCM y, SCM c)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (c));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) CIRCLE_PERIMETER, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_lineSlope (SCM name, SCM x, SCM y, SCM l)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (l));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) LINE_SLOPE, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_arcLength (SCM name, SCM x, SCM y, SCM arc)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (arc));

  item = (geometricObject *) new numeric (scmCoord2drgeoPoint (x, y), *parents,
					  (numericType) ARC_CIRCLE_LENGTH, false,
					  figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_geometricAngle (SCM name, SCM p1, SCM p2, SCM p3)
{
  geometricObject *item;
  drgeoPoint p;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (p1));
  parents->ajoute ((void *) gh_scm2ulong (p2));
  parents->ajoute ((void *) gh_scm2ulong (p3));

  item = (geometricObject *) new angle (p, *parents,
					(angleType) ANGLE_3PTS, false,
					figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

SCM
ScmInterface::make_orientedAngle (SCM name, SCM x, SCM y, SCM v1, SCM v2)
{
  geometricObject *item;

  parents->vide ();
  parents->ajoute ((void *) gh_scm2ulong (v1));
  parents->ajoute ((void *) gh_scm2ulong (v2));

  item = (geometricObject *) new angle (scmCoord2drgeoPoint (x, y), *parents,
					(angleType) ANGLE_2VEC, false,
					figure->getFigureList ());
  setObjectName (name, item);
  figure->addToFigureListNoRedraw (item, TRUE);
  return gh_ulong2scm ((unsigned long) item);
}

void 
ScmInterface::change_color (SCM item, SCM color)
{
  ((geometricObject *)gh_scm2ulong(item))->getStyle ().color = 
    ((drgeoColorType) gh_scm2int(color));
  figure->getDrawable ()->refresh ();
}

void 
ScmInterface::change_thickness (SCM item, SCM thickness)
{
  ((geometricObject *)gh_scm2ulong(item))->getStyle ().thick = 
    ((drgeoThicknessType) gh_scm2int(thickness));
  figure->getDrawable ()->refresh ();
}

void 
ScmInterface::change_pointShape (SCM item, SCM shape)
{
  ((geometricObject *)gh_scm2ulong(item))->getStyle ().pointShape = 
    ((drgeoPointType) gh_scm2int(shape));
  figure->getDrawable ()->refresh ();
}

void 
ScmInterface::change_masked (SCM item)
{
  masked myMask;

  myMask = ((geometricObject *)gh_scm2ulong(item))->getStyle ().mask;
  if (myMask == yes)
    ((geometricObject *)gh_scm2ulong(item))->getStyle ().mask = no;
  else
    ((geometricObject *)gh_scm2ulong(item))->getStyle ().mask = yes;    
}


void
ScmInterface::setObjectName (SCM name, geometricObject *item)
{
  gchar *str;

  str = gh_scm2newstr (name, NULL);
  item->setName (str); 
  g_free (str);
  item->initName ();
}


/* Figure builder */

SCM
drgeo_scm_make_figure (SCM name)
{
  drgeoFigure *figure;
  gchar *str;

  mdi->newGeometricDocument (NULL);
  figure = ((geoView *) mdi->activeView ())->figure ();

  str = gh_scm2newstr (name, NULL);
  mdi->renameView (((drgeoGtkDrawable *) figure->getDrawable())->getGeoView (), str);
  g_free (str);

  return gh_ulong2scm ((unsigned long) figure);

}

/************************************************************ 
   Wrapper to the Scheme method from the scmInterface class 
*************************************************************/

/* Define some macro to make our life easier */

#define SCM_INTERFACE(fig) (((drgeoFigure *)gh_scm2ulong(fig))->getScmInterface ())

SCM
drgeo_scm_make_freePoint (SCM figure, SCM name, SCM x, SCM y)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_freePoint (name, x, y);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_pointOnCurve (SCM figure, SCM name, SCM c, SCM a)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_pointOnCurve (name, c, a);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_middle2Points (SCM figure, SCM name, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_middle2Points (name, p1, p2);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_middleSegment (SCM figure, SCM name, SCM s)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_middleSegment (name, s);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_intersection (SCM figure, SCM name, SCM obj1, SCM obj2, SCM k)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_intersection (name, obj1, obj2, k);
  else
    return gh_ulong2scm(0);
}
/***********************/
/* THE TRANSFORMATIONS */
/***********************/
SCM 
drgeo_scm_make_rotation (SCM figure, SCM name, SCM object, SCM c, SCM a)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_rotation (name, object, c, a);
  else
    return gh_ulong2scm(0);
}
SCM 
drgeo_scm_make_scale (SCM figure, SCM name, SCM object, SCM c, SCM f)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_scale (name, object, c, f);
  else
    return gh_ulong2scm(0);
}
SCM 
drgeo_scm_make_symmetry (SCM figure, SCM name, SCM object, SCM c)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_symmetry (name, object, c);
  else
    return gh_ulong2scm(0);
}
SCM 
drgeo_scm_make_reflexion (SCM figure, SCM name, SCM object, SCM d)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_reflexion (name, object, d);
  else
    return gh_ulong2scm(0);
}
SCM 
drgeo_scm_make_translation (SCM figure, SCM name, SCM object, SCM v)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_translation (name, object, v);
  else
    return gh_ulong2scm(0);
}


SCM
drgeo_scm_make_segmentExtremity (SCM figure, SCM name, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_segmentExtremity (name, p1, p2);
  else
    return gh_ulong2scm (0);
}

SCM
drgeo_scm_make_line (SCM figure, SCM name, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_line (name, p1, p2);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_parallel (SCM figure, SCM name, SCM p, SCM d)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_parallel (name, p, d);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_orthogonal (SCM figure, SCM name, SCM p, SCM d)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_orthogonal (name, p, d);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_ray (SCM figure, SCM name, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_ray (name, p1, p2);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_vector (SCM figure, SCM name, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_vector (name, p1, p2);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_circle2Points (SCM figure, SCM name, SCM c, SCM p)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_circle2Points (name, c, p);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_circleRadius (SCM figure, SCM name, SCM c, SCM r)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_circleRadius (name, c, r);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_circleSegment (SCM figure, SCM name, SCM c, SCM s)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_circleSegment (name, c, s);
  else
    return gh_ulong2scm(0);
}

SCM 
drgeo_scm_make_arc (SCM figure, SCM name, SCM p1, SCM p2, SCM p3)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_arc (name, p1, p2, p3);
  else
    return gh_ulong2scm(0);
}


SCM
drgeo_scm_make_locus (SCM figure, SCM name, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_locus (name, p1, p2);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_polygon (SCM figure, SCM name, SCM list)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_polygon (name, list);
  else
    return gh_ulong2scm(0);
}


SCM
drgeo_scm_make_freeValue (SCM figure, SCM name, SCM x, SCM y, SCM v)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_freeValue (name, x, y, v);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_segmentLength (SCM figure, SCM name, SCM x, SCM y, SCM s)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_segmentLength (name, x, y, s);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_vectorNorm (SCM figure, SCM name, SCM x, SCM y, SCM v)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_vectorNorm (name, x, y, v);
  else
    return gh_ulong2scm(0);
}
SCM
drgeo_scm_make_distancePointCircle (SCM figure, SCM name, SCM x, SCM y, SCM p, SCM c)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_distancePointCircle (name, x, y, p, c);
  else
    return gh_ulong2scm(0);
}

SCM 
drgeo_scm_make_distancePointLine (SCM figure, SCM name, SCM x, SCM y, SCM p, SCM l)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_distancePointLine (name, x, y, p, l);
  else
    return gh_ulong2scm(0);
}

SCM 
drgeo_scm_make_distancePointPoint (SCM figure, SCM name, SCM x, SCM y, SCM p1, SCM p2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_distancePointPoint (name, x, y, p1, p2);
  else
    return gh_ulong2scm(0);
}

SCM 
drgeo_scm_make_circleLength (SCM figure, SCM name, SCM x, SCM y, SCM c)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_circleLength (name, x, y, c);
  else
    return gh_ulong2scm(0);
}

SCM 
drgeo_scm_make_lineSlope (SCM figure, SCM name, SCM x, SCM y, SCM l)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_lineSlope (name, x, y, l);
  else
    return gh_ulong2scm(0);
}

SCM 
drgeo_scm_make_arcLength (SCM figure, SCM name, SCM x, SCM y, SCM arc)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_arcLength (name, x, y, arc);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_geometricAngle (SCM figure, SCM name, SCM p1, SCM p2, SCM p3)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_geometricAngle (name, p1, p2, p3);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_make_orientedAngle (SCM figure, SCM name, SCM x, SCM y, SCM v1, SCM v2)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);

  if (interface)
    return interface->make_orientedAngle (name, x, y, v1, v2);
  else
    return gh_ulong2scm(0);
}

SCM
drgeo_scm_change_color (SCM figure, SCM item, SCM color)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);
  if (interface)
    interface->change_color (item, color);
  return SCM_UNSPECIFIED;
}

SCM
drgeo_scm_change_thickness (SCM figure, SCM item, SCM thickness)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);
  if (interface)
    interface->change_thickness (item, thickness);
  return SCM_UNSPECIFIED;
}

SCM
drgeo_scm_change_pointShape (SCM figure, SCM item, SCM shape)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);
  if (interface)
    interface->change_pointShape (item, shape);
  return SCM_UNSPECIFIED;
}

SCM
drgeo_scm_change_masked (SCM figure, SCM item)
{
  ScmInterface *interface;

  interface = SCM_INTERFACE (figure);
  if (interface)
    interface->change_masked (item);
  return SCM_UNSPECIFIED;
}
