/*
 *  Dr Genius 
 * (C) Copyright Hilaire Fernandes  2001-2003
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

/*
  HERE, WE DEFINE THE API OF THE DrGeoScript
  IT IS USED BY SCRIPT PLUGGUED IN A FIGURE
*/
  


#include "drgeo_scm_api.h"
#include "drgeo_scm_helper.h"
#include "drgeo_geometricObject.h"
#include "drgeo_point.h"
#include "drgeo_vector.h"
#include "drgeo_segment.h"
#include "drgeo_circle.h"
#include "drgeo_arcCircle.h"
#include "drgeo_value.h"
#include <math.h>


/* each API function use it */
static geometricObject *item;



/*******************/
/* geometricObject */
/*******************/


/*
   GET THE 'CURVILIGNE' ABSCISSA OF A POINT (A POINT ON A LINE) on [0 ; 1]
   Usage:
   (getAbscissa a1)
   a1 : a point object on a curve
*/
SCM
drgeo_scm_getAbscissa (SCM object)
{
  static gdouble d;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & FREE_PT_ON_CURVE)
    {
      d = ((point *) item)->getAbscissa ();
      return scm_from_double (d);
    }
  /* This is not a point object */
  return SCM_UNSPECIFIED;
}

/*
   SET THE 'CURVILIGNE' ABSCISSA OF A POINT (A POINT ON A LINE) on [0 ; 1]
   Usage:
   (setAbscissa a1 x)
   a1 : a point object on a curve
   x : a value in the range [0 ; 1]
*/

SCM
drgeo_scm_setAbscissa (SCM object, SCM x)
{
  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & FREE_PT_ON_CURVE)
    ((point *) item)->setAbscissa (scm_to_double (x));
  return SCM_UNSPECIFIED;
}


/*
   GET THE COORDINATES OF A POINT OR VECTOR
   Usage:
   (getCoordinates a1)
   a1 : a point or vector object
   Returned value: A 2-upplet list containing the coordinates
*/
SCM
drgeo_scm_getCoordinates (SCM object)
{
  static drgeoPoint p;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & POINT)
    {
      p = ((point *) item)->getCoordinate ();
      return drgeoPoint2scmList (p);
    }
  else if (item->getCategory () & VECTOR)
    {
      p = ((vector *) item)->getDirection ();
      return drgeoPoint2scmList (p);
    }
  /* This is not a point object */
  return SCM_UNSPECIFIED;
}

/*
   GET THE COORDINATES OF THE UNIT VECTOR OF A DIRECTION
   Usage:
   (getUnit a1)
   a1 : a direction object (line, half-line, segment, vector)
   Returned value: A 2-upplet list containing the coordinates
*/
SCM
drgeo_scm_getUnit (SCM object)
{
  static drgeoVector v;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & DIRECTION)
    {
      v = ((direction *) item)->getDirection ();
      v /= v.norm ();
      return drgeoPoint2scmList (v);
    }
  /* This is not a direction object */
  return SCM_UNSPECIFIED;
}

/*
   GET THE COORDINATES OF THE NORMAL DIRECTION (A VECTOR)
   Usage:
   (getNormal a1)
   a1 : a direction object (line, half-line, segment, vector)
   Returned value: A 2-upplet list containing the coordinates
*/
SCM
drgeo_scm_getNormal (SCM object)
{
  static drgeoVector v;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & DIRECTION)
    {
      v = ((direction *) item)->getNormal ();
      v /= v.norm ();
      return drgeoPoint2scmList (v);
    }
  /* This is not a direction object */
  return SCM_UNSPECIFIED;
}

/*
   SET THE COORDINATES OF A POINT
   Usage:
   (setCoordinates a1 coord)
   a1 : a point or vector object
   coord : A 2-upplet list containing the coordinates
*/
SCM
drgeo_scm_setCoordinates (SCM object, SCM coord)
{
  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & FREE_PT)
    ((point *) item)->setCoordinate (scmList2drgeoVector (coord));
  return SCM_UNSPECIFIED;
}

/*
   GET THE SLOPE OF A DIRECTION
   Usage:
   (getSlope a1)
   a1 : a direction object
   Returns : the slope, a value
*/
SCM
drgeo_scm_getSlope (SCM object)
{
  static drgeoVector v;
  static gdouble p;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & DIRECTION)
    {
      v = ((direction *) item)->getDirection ();
      if (v.getX () != 0)
	{
	  p = v.getY () / v.getX ();
	  return scm_from_double (p);
	}
    }
  return SCM_UNSPECIFIED;
}


/*
   GET THE NORM OF A VECTOR
   Usage:
   (getNorm a1)
   a1 : a vector object
   Returns : a value, the norm of the vector
*/
SCM
drgeo_scm_getNorm (SCM object)
{
  static gdouble p;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & VECTOR)
    {
      p = ((vector *) item)->getDirection ().norm ();
      return scm_from_double (p);
    }
  return SCM_UNSPECIFIED;
}


/*
   GET THE LENGTH OF A SEGMENT, CIRCLE, ARC-CIRCLE
   Usage:
   (getLength a1)
   a1 : a segment, circle or arc-circle object
   Returns : a value, the length of the object
*/
SCM
drgeo_scm_getLength (SCM object)
{
  static gdouble l;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & SEGMENT)
    {
      l = ((segment *) item)->getDirection ().norm ();
      return scm_from_double (l);
    }
  else if (item->getCategory () & CIRCLE)
    {
      l = 2 * M_PI * ((circle *) item)->getRadius ();
      return scm_from_double (l);
    }
  else if (item->getCategory () & ARC_CIRCLE)
    {
      l = ABS (((arcCircle *) item)->getLength ()) *
	((arcCircle *) item)->getRadius ();
      return scm_from_double (l);
    }
  return SCM_UNSPECIFIED;
}

/*
   GET THE CENTER OF A CIRCLE ARC-CIRCLE
   Usage:
   (getCenter a1)
   a1 : a circle or arc-circle object
   returns: a 2D list containing the center coordinates
*/
SCM
drgeo_scm_getCenter (SCM object)
{
  static drgeoPoint p;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & CIRCLE)
    {
      p = ((circle *) item)->getCenter ();
      return drgeoPoint2scmList (p);
    }
  else if (item->getCategory () & ARC_CIRCLE)
    {
      p = ((arcCircle *) item)->getCenter ();
      return drgeoPoint2scmList (p);
    }
  return SCM_UNSPECIFIED;
}

/*
   GET THE RADIUS OF A CIRCLE ARC-CIRCLE
   Usage:
   (getRadius a1)
   a1 : a circle or arc-circle object
   returns: a value containing the radius
*/
SCM
drgeo_scm_getRadius (SCM object)
{
  static gdouble r;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & CIRCLE)
    {
      r = ((circle *) item)->getRadius ();
      return scm_from_double (r);
    }
  else if (item->getCategory () & ARC_CIRCLE)
    {
      r = ((arcCircle *) item)->getRadius ();
      return scm_from_double (r);
    }
  /* This is not a value object */
  return SCM_UNSPECIFIED;
}


/*
   GET THE VALUE OF A VALUE OBJECT
   Usage:
   (getValue a1)
   a1 : a value object
*/
SCM
drgeo_scm_getValue (SCM object)
{
  static gdouble d;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & VALUE)
    {
      d = ((value *) item)->getValue ();
      return scm_from_double (d);
    }
  /* This is not a value object */
  return SCM_UNSPECIFIED;
}

/*
   SET THE VALUE OF A VALUE OBJECT
   Usage:
   (setValue a1 v)
   a1 : a value object
   v: a value
*/
SCM
drgeo_scm_setValue (SCM object, SCM v)
{
  item = (geometricObject *) scm_to_ulong (object);
  if (item->getType () & FREE_VALUE)
    ((value *) item)->setValue (scm_to_double (v));
  return SCM_UNSPECIFIED;
}


/*
  GET THE ANGLE IN RADIAN OF A ANGLE OBJECT 
  Usage:
  (getAngle a)
  a1 : the angle 
*/
SCM 
drgeo_scm_getAngle (SCM object)
{
  static gdouble d;

  item = (geometricObject *) scm_to_ulong (object);
  if (item->getCategory () & ANGLE)
    {
      d = 180 * ((value *) item)->getValue () / M_PI;
      return (scm_from_double (d));
    }
  /* This is not an angle object */
  return SCM_UNSPECIFIED;
}


/*
   TRANSLATE A GEOMETRIC OBJECT
   Usage:
   (move a1 t)
   a1 : the geometric object reference to translate
   t : a guile vector 
*/
SCM
drgeo_scm_move (SCM object, SCM t)
{
  item = (geometricObject *) scm_to_ulong (object);
  item->move (scmVector2drgeoVector (t));
  return SCM_UNSPECIFIED;
}
