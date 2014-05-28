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

#ifndef DRGEO_POINT_H
#define DRGEO_POINT_H

#include "drgeo_geometricObject.h"

class point:public geometricObject
{
public:
  point (drgeoPoint & p, liste_elem & parents, pointType type, 
	 gboolean createdFromMacro, liste_elem * figureList);
  point (gdouble a, liste_elem & parents, gboolean createdFromMacro, 
	 liste_elem * figureList);
  point (liste_elem & parents, pointType type,
	 gboolean createdFromMacro, liste_elem * figureList, gint k);
  point (xmlNodePtr tree, GHashTable * itemIdToAdress,
	 liste_elem * figureList);
  ~point ()
  {
  };
  void update (drgeoDrawable & area);
  void move (drgeoVector & t);
  void save (xmlNodePtr tree, liste_elem & figureList);
  void updateDescription ();
  void draw (drgeoDrawable & area, char force);
  gboolean overObject (drgeoPoint & mouse, gdouble range);
  void initName ();
  drgeoPoint & getCoordinate ();
  void setCoordinate (drgeoPoint & p);
  void setAbscissa (gdouble abscissa);
  gdouble getAbscissa ();
  void * getExtra ();
  drgeoPointType preferedShape (gchar *attr);
protected:
  drgeoPoint p;
  gdouble abscissa;
  int extra;
};


#endif /* DRGEO_POINT_H */
