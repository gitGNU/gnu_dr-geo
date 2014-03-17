/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1998
 * hilaire@ext.cri74.org 
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
#ifndef traite_h
#define traite_h

#include "macro.h"
#include "drgeo_geometricObject.h"

gboolean usedItem (liste_elem & itemList, liste_elem & exclude,
		   geometricObject * item);
char fig_utilisee (liste_elem & liste_figure, geometricObject * fig);
void updateItems (drgeoDrawable & area, liste_elem * liste_figure, int indicei,
		  int indicef, char mise_a_jour_lieu, gboolean update []);
gboolean needUpdate (geometricObject *item, gint freePtIndex, gboolean update [],
		     liste_elem *figures);
#endif
