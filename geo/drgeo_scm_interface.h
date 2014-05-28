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

#ifndef DRGEO_SCM_INTERFACE_H
#define DRGEO_SCM_INTERFACE_H

#include <guile/gh.h>
#include "drgeo_figure.h"

/* define scm_interface into a class */

class ScmInterface
{
 public:
  ScmInterface (drgeoFigure *figure);

  SCM make_freePoint (SCM name, SCM x, SCM y);
  SCM make_pointOnCurve (SCM name, SCM c, SCM a);
  SCM make_middle2Points (SCM name, SCM p1, SCM p2);
  SCM make_middleSegment (SCM name, SCM s);
  SCM make_intersection (SCM name, SCM obj1, SCM obj2, SCM k);
  SCM make_segmentExtremity (SCM name, SCM p1, SCM p2);

  SCM make_line (SCM name, SCM p1, SCM p2);
  SCM make_parallel (SCM name, SCM p, SCM d);
  SCM make_orthogonal (SCM name, SCM p, SCM d);

  SCM make_ray (SCM name, SCM p1, SCM p2);

  SCM make_vector (SCM name, SCM p1, SCM p2);

  SCM make_circle2Points (SCM name, SCM c, SCM p);
  SCM make_circleRadius (SCM name, SCM c, SCM r);
  SCM make_circleSegment (SCM name, SCM c, SCM s);

  SCM make_arc (SCM name, SCM p1, SCM p2, SCM p3);

  SCM make_locus (SCM name, SCM p1, SCM p2);

  SCM make_polygon (SCM name, SCM list);

  SCM make_freeValue (SCM name, SCM x, SCM y, SCM v); 
  SCM make_segmentLength (SCM name, SCM x, SCM y, SCM s); 
  SCM make_vectorNorm (SCM name, SCM x, SCM y, SCM v); 
  SCM make_distancePointCircle (SCM name, SCM x, SCM y, SCM p, SCM c); 
  SCM make_distancePointLine (SCM name, SCM x, SCM y, SCM p, SCM l); 
  SCM make_distancePointPoint (SCM name, SCM x, SCM y, SCM p1, SCM p2); 
  SCM make_circleLength (SCM name, SCM x, SCM y, SCM c); 
  SCM make_lineSlope (SCM name, SCM x, SCM y, SCM l); 
  SCM make_arcLength (SCM name, SCM x, SCM y, SCM arc); 

  SCM make_geometricAngle (SCM name, SCM p1, SCM p2, SCM p3);
  SCM make_orientedAngle (SCM name, SCM x, SCM y, SCM v1, SCM v2);
  
  SCM make_rotation (SCM name, SCM object, SCM c, SCM a);
  SCM make_scale (SCM name, SCM object, SCM c, SCM f);
  SCM make_symmetry (SCM name, SCM object, SCM c);
  SCM make_reflexion (SCM name, SCM object, SCM d);
  SCM make_translation (SCM name, SCM object, SCM v);

  void change_color (SCM item, SCM color);
  void change_thickness (SCM item, SCM thickness);
  void change_pointShape (SCM item, SCM shape);
  void change_masked (SCM item);

  void setObjectName (SCM name, geometricObject *item);
 protected:
  drgeoFigure *figure;
  liste_elem *parents;
};


SCM
drgeo_scm_make_figure (SCM name);

/* Wrapper to the Scheme method from the scmInterface class */

/*********/
/* POINT */
/*********/
SCM
drgeo_scm_make_freePoint (SCM figure, SCM name, SCM x, SCM y);
SCM
drgeo_scm_make_pointOnCurve (SCM figure, SCM name, SCM c, SCM a);
SCM
drgeo_scm_make_middle2Points (SCM figure, SCM name, SCM p1, SCM p2);
SCM
drgeo_scm_make_middleSegment (SCM figure, SCM name, SCM s);
SCM
drgeo_scm_make_intersection (SCM figure, SCM name, SCM obj1, SCM obj2, SCM k);
SCM
drgeo_scm_make_rotation (SCM figure, SCM name, SCM object, SCM c, SCM a);
SCM 
drgeo_scm_make_scale (SCM figure, SCM name, SCM object, SCM c, SCM f);
SCM 
drgeo_scm_make_symmetry (SCM figure, SCM name, SCM object, SCM c);
SCM 
drgeo_scm_make_reflexion (SCM figure, SCM name, SCM object, SCM d);
SCM 
drgeo_scm_make_translation (SCM figure, SCM name, SCM object, SCM v);

/***********/
/* SEGMENT */
/***********/
SCM
drgeo_scm_make_segmentExtremity (SCM figure, SCM name, SCM p1, SCM p2);

/********/
/* LINE */
/********/
SCM
drgeo_scm_make_line (SCM figure, SCM name, SCM p1, SCM p2);
SCM
drgeo_scm_make_parallel (SCM figure, SCM name, SCM p, SCM d);
SCM
drgeo_scm_make_orthogonal (SCM figure, SCM name, SCM p, SCM d);

/*******/
/* RAY */
/*******/
SCM
drgeo_scm_make_ray (SCM figure, SCM name, SCM p1, SCM p2);

/**********/
/* VECTOR */
/**********/
SCM
drgeo_scm_make_vector (SCM figure, SCM name, SCM p1, SCM p2);

/**********/
/* CIRCLE */
/**********/
SCM
drgeo_scm_make_circle2Points (SCM figure, SCM name, SCM c, SCM p);
SCM
drgeo_scm_make_circleRadius (SCM figure, SCM name, SCM c, SCM r);
SCM
drgeo_scm_make_circleSegment (SCM figure, SCM name, SCM c, SCM s);

/*******/
/* ARC */
/*******/
SCM 
drgeo_scm_make_arc (SCM figure, SCM name, SCM p1, SCM p2, SCM p3);

/*********/
/* LOCUS */
/*********/
SCM
drgeo_scm_make_locus (SCM figure, SCM name, SCM p1, SCM p2);

/***********/
/* POLYGON */
/***********/
SCM
drgeo_scm_make_polygon (SCM figure, SCM name, SCM list);

/**********/
/* NUMBER */
/**********/
SCM
drgeo_scm_make_freeValue (SCM figure, SCM name, SCM x, SCM y, SCM v); 
SCM 
drgeo_scm_make_segmentLength (SCM figure, SCM name, SCM x, SCM y, SCM s); 
SCM 
drgeo_scm_make_vectorNorm (SCM figure, SCM name, SCM x, SCM y, SCM v); 
SCM 
drgeo_scm_make_distancePointCircle (SCM figure, SCM name, SCM x, SCM y, SCM p, SCM c); 
SCM 
drgeo_scm_make_distancePointLine (SCM figure, SCM name, SCM x, SCM y, SCM p, SCM l); 
SCM
drgeo_scm_make_distancePointPoint (SCM figure, SCM name, SCM x, SCM y, SCM p1, SCM p2); 
SCM 
drgeo_scm_make_circleLength (SCM figure, SCM name, SCM x, SCM y, SCM c); 
SCM 
drgeo_scm_make_lineSlope (SCM figure, SCM name, SCM x, SCM y, SCM l); 
SCM 
drgeo_scm_make_arcLength (SCM figure, SCM name, SCM x, SCM y, SCM arc); 


/*********/
/* ANGLE */
/*********/
SCM 
drgeo_scm_make_geometricAngle (SCM figure, SCM name, SCM p1, SCM p2, SCM p3);
SCM 
drgeo_scm_make_orientedAngle (SCM figure, SCM name, SCM x, SCM y, SCM v1, SCM v2);

/*******************/
/* EDIT ATTRIBUTES */
/*******************/
SCM
drgeo_scm_change_color (SCM figure, SCM item, SCM color);
SCM
drgeo_scm_change_thickness (SCM figure, SCM item, SCM thickness);
SCM
drgeo_scm_change_pointShape (SCM figure, SCM item, SCM shape);
SCM
drgeo_scm_change_masked (SCM figure, SCM item);


/* static helper */

#endif				/* DRGEO_SCM_INTERFACE_H */

