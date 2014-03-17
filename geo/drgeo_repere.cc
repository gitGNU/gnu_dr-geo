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

#include "config.h"
#include "drgeo_repere.h"
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "liste_elem.h"
#include "drgeo_drawable.h"

repere::repere (liste_elem & parents, repereType type, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  this->type = type;
  category = REPERE;
  if (type == THREE_PTS)
    {
      /* POINT - POINT - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (1));
      parentList = g_list_append (parentList, (gpointer) parents.lire (2));
      parentList = g_list_append (parentList, (gpointer) parents.lire (3));
    }
  else
    {
      /* POINT - VECTOR - VECTOR */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VECTOR));
      parentList =
	g_list_append (parentList,
		       (gpointer) searchForCategory (parents, VECTOR));
    }
  initName ();
}

repere::repere (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
geometricObject (FALSE, figureList)
{
  /* FIXME: build repere from a xml tree */
  initName ();
}

void
repere::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == true && force == FALSE)
      || !exist)
    return;
  area.drawLine (style, O, O + ir);
  area.drawLine (style, O, O + jr);
  /* FIXME: draw graduation on the axes */
}

void
repere::update (drgeoDrawable & area)
{
  gdouble d;

  exist = parentExist ();
  if (!exist)
    return;

  O = ((point *) getFirst)->getCoordinate ();
  if (type == THREE_PTS)
    {
      point *p1, *p2;
      p1 = (point *) getN (1);
      p2 = (point *) getN (2);
      ir = p1->getCoordinate () - O;
      jr = p2->getCoordinate () - O;
    }
  else
    {
      ir = ((direction *) getN (1))->getDirection ();
      jr = ((direction *) getN (1))->getDirection ();
    }
  d = ir ^ jr;
  if (ABS (d) < EPSILON)
    {
      exist = FALSE;
      return;
    }
  i.set (-jr.getX () / d, ir.getX () / d);
  j.set (jr.getY () / d, -ir.getY () / d);
  /* FIXME: 
     We should now check out the figures who belong
     to this reference frame and update their coordinates
   */
}

gboolean
repere::overObject (drgeoPoint & mouse, drgeoDrawable & area)
{
  return overLine (mouse, O, ir, area.getRange ())
    || overLine (mouse, O, jr, area.getRange ());
}

void
repere::move (drgeoVector & t)
{
  point *item;
  item = (point *) g_list_nth_data (parentList, 0);
  item->move (t);
  if (type == THREE_PTS)
    {
      item = (point *) g_list_nth_data (parentList, 1);
      item->move (t);
      item = (point *) g_list_nth_data (parentList, 2);
      item->move (t);
    }
}

void
repere::initName ()
{
  int l;
  geometricObject *item0, *item1, *item2;

  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this reference frame %1")) + 1;
  item0 = (geometricObject *) g_list_nth_data (parentList, 0);
  item1 = (geometricObject *) g_list_nth_data (parentList, 1);
  item2 = (geometricObject *) g_list_nth_data (parentList, 2);
  if (strlen (item1->getName ()) != 0 && strlen (item1->getName ()) != 0
      && strlen (item1->getName ()) != 0)
    {
      g_free (name);
      name = g_strdup_printf ("(%s,%s,%s)", item0->getName (),
			      item1->getName (), item2->getName ());
    }
  l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this reference frame %1"));
  strinsmsg (typeName, name, "%1");
}

void
repere::save (xmlNodePtr tree, liste_elem & figureList)
{
  /* FIXME: Write the save code */
}

drgeoPoint & repere::repereToWorld (drgeoPoint & p)
{
  static drgeoPoint
    m;
  m = vectorRepereToWorld (p) + O;
  return m;
}

drgeoPoint & repere::worldToRepere (drgeoPoint & p)
{
  static drgeoPoint
    m;
  m = vectorWorldToRepere (p - O);
  return m;
}

drgeoVector & repere::vectorWorldToRepere (drgeoVector & v)
{
  static drgeoVector
    ve;
  /* be careful there as the result from * is static */
  ve = v.getX () * i;
  ve += v.getY () * j;
  return ve;
}

drgeoVector & repere::vectorRepereToWorld (drgeoVector & v)
{
  static drgeoVector
    ve;
  ve = v.getX () * ir;
  ve += v.getY () * jr;
  return ve;
}
