/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2000
 * hilaire@seul.org 
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

#include "drgeo_direction.h"
#include "drgeo_point.h"
#include "drgeo_value.h"
#include "liste_elem.h"

direction::direction (liste_elem & parents, transformationType type, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  switch (type)
    {
    case ROTATION:
    case SCALE:
      /* the object parentList will contain in this order
         DIRECTION (object transformed) - POINT - VALUE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								DIRECTION));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VALUE));
      break;
    case SYMMETRY:
      /* DIRECTION - POINT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								DIRECTION));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      break;
    case REFLEXION:
      /* DIRECTION - LINE */
      geometricObject * i1, *i2;
      objectCategory c1, c2;
      parents.init_lire ();
      i1 = (geometricObject *) parents.lire (0);
      i2 = (geometricObject *) parents.lire (0);
      c1 = i1->getCategory ();
      c2 = i2->getCategory ();
      if (c1 == LINE && c2 != LINE)
	{
	  parentList = g_list_append (parentList, (gpointer) i2);
	  parentList = g_list_append (parentList, (gpointer) i1);
	}
      else
	{
	  parentList = g_list_append (parentList, (gpointer) i1);
	  parentList = g_list_append (parentList, (gpointer) i2);
	}
      break;
    case TRANSLATION:
      /* DIRECTION - VECTOR 
         We don't translate vector 
         so we don't handle VECTOR - VECTOR situation */
      geometricObject * i;
      parents.init_lire ();
      i = searchForCategory (parents, DIRECTION);
      if (i->getCategory () == VECTOR)
	{
	  /* search next */
	  parentList = g_list_append (parentList,
				      (gpointer) searchForCategory (parents,
								    DIRECTION));
	  parents.init_lire ();
	}
      else
	parentList = g_list_append (parentList, (gpointer) i);
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VECTOR));
      break;
    }
}

void
direction::update (drgeoDrawable & area)
{
  /* adjust the direction according to the transformation.
     We are there only if got a transformed direction
     We also compute a transformed  orgin (non sense for vector
   */

  u = ((direction *) getFirst)->getDirection ();
  if (category != VECTOR)
    origin = ((direction *) getFirst)->getOrigin ();
  switch (type)
    {
    case ROTATION:
      u = u.rotate (((value *) getLast)->getValue ());
      if (category != VECTOR)
	origin = origin.rotatePoint (((point *) getN (1))->getCoordinate (),
				     ((value *) getN (2))->getValue ());
      break;
    case SYMMETRY:
      u = (-1) * u;
      if (category != VECTOR)
	origin = 2 * ((point *) getN (1))->getCoordinate () - origin;
      break;
    case REFLEXION:
      u = u.reflexion (((direction *) getN (1))->getDirection ());
      if (category != VECTOR)
	origin = origin.reflexionPoint (((direction *) getN (1))->
					getOrigin (),
					((direction *) getN (1))->
					getDirection ());
      break;
    case TRANSLATION:
      origin += ((direction *) getN (1))->getDirection ();
      break;
    case SCALE:
      u *= ((value *) getLast)->getValue ();
      if (category != VECTOR)
	origin = (origin - ((point *) getN (1))->getCoordinate ())
	  * ((value *) getLast)->getValue ()
	  + ((point *) getN (1))->getCoordinate ();
      break;
    }
}

drgeoVector & direction::getDirection ()
{
  return u;
}

drgeoVector & direction::getNormal ()
{
  return u.normal ();
}

gboolean
direction::isCollinear (drgeoVector & v)
{
  return ABS (v ^ u) < EPSILON;
}

drgeoPoint & direction::getOrigin ()
{
  return origin;
}

gboolean
direction::directionIntersection (direction & d, drgeoPoint & p)
{
  /* FIXME: once sectionDirectionDirection() is known working
     replace this code by:
     return sectionDirectionDirection (origin, u, 
     d.getOrigin (), d.getDirection (),
     p);
   */
  gdouble det, c1, c2;

  det = u ^ d.getDirection ();
  if (det == 0)
    return FALSE;
  c1 = origin ^ u;
  c2 = d.getOrigin () ^ d.getDirection ();

  p.set ((-c1 * d.getDirection ().getX () + c2 * u.getX ()) / det,
	 (u.getY () * c2 - d.getDirection ().getY () * c1) / det);
  return TRUE;
}
