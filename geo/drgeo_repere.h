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

#ifndef DRGEO_REPERE_H
#define DRGEO_REPERE_H

#include "drgeo_geometricObject.h"

class repere:public geometricObject
{
/* type can be 
   THREE_PTS
   1PT_2VECTORS
*/
public:
  repere (liste_elem & parents, repereType type,
	  gboolean createdFromMacro, liste_elem * figureList);
  repere (xmlNodePtr tree, GHashTable * itemIdToAdress,
	  liste_elem * figureList);
  ~repere ()
  {
  };
  void draw (drgeoDrawable & area, char force);
  void update (drgeoDrawable & area);
  gboolean overObject (drgeoPoint & mouse, drgeoDrawable & area);
  void move (drgeoVector & t);
  void initName ();
  void save (xmlNodePtr tree, liste_elem & figureList);
  drgeoPoint & repereToWorld (drgeoPoint & p);
  drgeoPoint & worldToRepere (drgeoPoint & p);
  drgeoVector & vectorWorldToRepere (drgeoVector & v);
  drgeoVector & vectorRepereToWorld (drgeoVector & v);
private:
  repereType type;
  drgeoPoint O;
  drgeoVector ir, jr;
  /* Word base exprimed with the (ir, jr) base */
  drgeoVector i, j;
};


#endif /* DRGEO_REPERE_H */
