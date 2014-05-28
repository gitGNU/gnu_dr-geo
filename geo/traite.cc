/* Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
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

#include <gtk/gtk.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#include "define.h"

// Fonctions de traitements
#include "mode_obj.h"

#include "traite.h"
#include <string.h>
//#include "tree.h"


gboolean
usedItem (liste_elem & itemList, liste_elem & exclude, geometricObject * item)
{
  /* Check if item is a parent (relatif or direct) of one of the
     items in itemList but the items in the exclude list 
     Typically itemList is the whole list */
  int n, nb;
  gboolean dependence = FALSE;
  geometricObject *item1;

  n = itemList.position ((gpointer) item) + 1;
  nb = itemList.nb_elem;

  while (n++ <= nb && dependence == FALSE)
    {
      item1 = (geometricObject *) itemList.lire (n);
      if (!exclude.position (item1))
	dependence = ((geometricObject *) item1)->dependOn (item);
    }
  return dependence;
}

// Verfifie s'il exsite des dependances relatives a cette objet (utile apres avoir efface un objet)
char
fig_utilisee (liste_elem & liste_figure, geometricObject * fig)
  //used when deleting one figure item
{
  int n;
  char dependance = FALSE;
  n = liste_figure.nb_elem;
  liste_figure.init_lire ();
  while (n-- != 0 && dependance == FALSE)
    dependance = ((geometricObject *) liste_figure.lire (0))->dependOn (fig);
  return dependance;
}

// mise a jour des figures a partir de la figure numero indicei jusqu'a indicef
// used in the lieu::actualise()
void
updateItems (drgeoDrawable & area, liste_elem * liste_figure, int indicei,
	     int indicef, char mise_a_jour_lieu, gboolean update [])
{
  int a;
  geometricObject *fig;

  if (indicef < indicei)
    indicef = liste_figure->nb_elem;

  for (a = indicei; a <= indicef; a++)
    {
      if ((update[a - indicei]))
	{
	  fig = (geometricObject *) liste_figure->lire (a);
	  if (fig->getCategory () != LOCUS || mise_a_jour_lieu == TRUE)
	    fig->update (area);
	}
    }
}


gboolean needUpdate (geometricObject *item, gint freePtIndex, gboolean update [],
		     liste_elem *figures)
{
  gint index;
  liste_elem parents;
  geometricObject * parent;

  index = figures->position ((void *) item);

  if (index == freePtIndex)
    {
      update[0] = true;
      return true;
    }
  else if (index > freePtIndex)
    {
      if (item->parents (parents) == 0)
	return false;
      parents.init_lire ();
      while ( (parent = (geometricObject *) parents.lire (0)) != NULL)
	{
	  if (needUpdate (parent, freePtIndex, update, figures))
	      update[index - freePtIndex] = true;
	}
      return update[index - freePtIndex];
    }
  else
    return false;
}
