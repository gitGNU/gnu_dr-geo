/*
 *  Dr Geo an interactive geometry software
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

#ifndef DRGEO_DIRECTION_H
#define DRGEO_DIRECTION_H

#include "drgeo_geometricObject.h"

class direction:public geometricObject
{
public:
  /* we use this constructor to build its descendand 
     based on a transformation */
  direction (liste_elem & parents,
	     transformationType type,
	     gboolean createdFromMacro, liste_elem * figureList);
  direction (xmlNodePtr tree, liste_elem * figureList):geometricObject (tree,
									figureList)
  {
  }
   ~direction ()
  {
  };
  /* we use this method to update its descendand 
     based on a transformation */
  void update (drgeoDrawable & area);
  drgeoVector & getDirection ();
  drgeoVector & getNormal ();
  gboolean isCollinear (drgeoVector & v);
  drgeoPoint & getOrigin ();
  gboolean directionIntersection (direction & d, drgeoPoint & p);
protected:
  drgeoVector u;
  drgeoPoint origin;
};

#endif /* DRGEO_DIRECTION_H */
