/*
 *  Dr Geo an interactive geometry software
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

#ifndef DRGEO_HALFLINE_H
#define DRGEO_HALFLINE_H

#include "drgeo_direction.h"

class halfLine:public direction
{
public:
  halfLine (liste_elem & parents, halfLineType type,
	    gboolean createdFromMacro, liste_elem * figureList);
  halfLine (xmlNodePtr tree, GHashTable * itemIdToAdress,
	    liste_elem * figureList);
  ~halfLine ()
  {
  };
  void draw (drgeoDrawable & area, char force);
  void update (drgeoDrawable & area);
  gboolean overObject (drgeoPoint & mouse, gdouble range);
  void move (drgeoVector & t);
  void initName ();
  void save (xmlNodePtr tree, liste_elem & figureList);
  void updateDescription ();
  drgeoPoint & getPointAt (gdouble abscissa);
  drgeoPoint & getClosestPoint (drgeoPoint & p);
  gdouble getAbscissa (drgeoPoint & p);
  gboolean getIntersection (geometricObject & c, drgeoPoint & p, gint k);
protected:
};



#endif /* DRGEO_HALFLINE_H */
