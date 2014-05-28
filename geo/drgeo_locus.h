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

#ifndef DRGEO_LOCUS_H
#define DRGEO_LOCUS_H

#include "drgeo_geometricObject.h"

class locus:public geometricObject
{
public:
  locus (liste_elem & parents, gboolean createdFromMacro,
	 liste_elem * figureList);
  locus (xmlNodePtr tree, GHashTable * itemIdToAdress,
	 liste_elem * figureList);
  ~locus ()
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
  drgeoPoint p[LOCUS_SAMPLE];
  gint sampleNumber;
};

#endif /* DRGEO_LOCUS_H */
