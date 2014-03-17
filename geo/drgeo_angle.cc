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
#include <math.h>

#include "drgeo_angle.h"
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "drgeo_repere.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_drawable.h"

angle::angle (drgeoPoint & p, liste_elem & parents, angleType type, gboolean createdFromMacro, liste_elem * figureList):
value (p, createdFromMacro, figureList)
{
  style.color = preferedColor (":angleColor");
  style.thick = preferedThickness (":angleStyle");

  this->type = (unsigned int) type;
  category = ANGLE;
  parents.init_lire ();
  switch (type)
    {
    case ANGLE_3PTS:
      /* POINT - POINT (sommet) - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      /* Go through to get the two next parameters */
    case ANGLE_2VEC:
      /* VECTOR - VECTOR */
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      break;
    }
  initName ();
}

angle::angle (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
value (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;
  double aval = 0;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("angle::angle missing mandatory attribute\n");
  else
    {
      category = ANGLE;
      if (!strcmp (attr, "3pts"))
	type = ANGLE_3PTS;
      else if (!strcmp (attr, "vectors"))
	{
	  type = ANGLE_2VEC;
	  /* get the coordinate on screen of this item */
	  xml_get_value_double (tree, "x", &val);
	  p.setX (val);
	  xml_get_value_double (tree, "y", &val);
	  p.setY (val);
	}
      /* get the parent ref */
      item = xml_search_next (tree->xmlChildrenNode, "parent");
      while (item)
	{
	  if ((obj = xmlInsertParent (item, itemIdToAdress)) == NULL)
	    exit (1);		/* FIXME: implement a nicer way */
	  parentList = g_list_append (parentList, obj);
	  item = xml_search_next (item->next, "parent");
	}
    }
  g_free (attr);
  initName ();
}

void
angle::draw (drgeoDrawable & area, char force)
{
  static drgeoVector centre, u, v, s;
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  if (type == ANGLE_3PTS)
    {
      gdouble l;
      centre = ((point *) getN (1))->getCoordinate ();
      u = ((point *) getFirst)->getCoordinate () - centre;
      v = ((point *) getLast)->getCoordinate () - centre;
      if (ABS (val - M_PI_2) > EPSILON)
	{
	  /* draw an arc */
	  area.drawArc (style, centre, area.pixelToWorld (15),
			origin, length);
	}
      else
	{
	  /* draw a square */
	  u = area.pixelToWorld (10) * u / u.norm ();
	  v = area.pixelToWorld (10) * v / v.norm ();
	  u += centre;
	  s = u + v;
	  v += centre;
	  area.drawSegment (style, u, s);
	  area.drawSegment (style, v, s);
	}
      /* update the position of the string */
      l = area.stringWidth (str);
      p.setX (area.pixelToWorld (30) * cos (origin + length / 2) - l / 2);
      l = area.stringHeight (str);
      p.setY (area.pixelToWorld (30) * sin (origin + length / 2) - l / 2);
      p += centre;
    }
  value::draw (area, force);
}

void
angle::update (drgeoDrawable & area)
{
  static drgeoVector u, v, centre;
  gdouble ps;
  exist = parentExist ();
  if (!exist)
    return;
  switch (type)
    {
    case ANGLE_3PTS:
      centre = ((point *) getN (1))->getCoordinate ();
      u = ((point *) getFirst)->getCoordinate () - centre;
      v = ((point *) getLast)->getCoordinate () - centre;
      break;
    case ANGLE_2VEC:
      u = ((direction *) getFirst)->getDirection ();
      v = ((direction *) getLast)->getDirection ();
      break;
    }
  if ((u.getX () == .0 && u.getY () == .0)
      || (v.getX () == .0 && v.getY () == .0))
    {
      exist = FALSE;
      return;
    }

  switch (type)
    {
    case ANGLE_3PTS:
      setValue (ABS (atan2 (u ^ v, u * v)));
      origin = atan2 (u.getY (), u.getX ());
      if (origin < 0)
	origin += 2 * M_PI;
      ps = atan2 (v.getY (), v.getX ());
      if (ps < 0)
	ps += 2 * M_PI;
      length = acos (cos (ps - origin));
      if (ps - origin > M_PI || (ps - origin < 0 && ps - origin > -M_PI))
	length = -length;
      break;
    case ANGLE_2VEC:
      setValue (atan2 (u ^ v, u * v));
      break;
    }

  setString (g_strdup_printf ("%.0f°", val * 180 * M_1_PI));
}

void
angle::move (drgeoVector & t)
{
  switch (type)
    {
    case ANGLE_3PTS:
      break;
    case ANGLE_2VEC:
      p += t;
      break;
    }
}

void
angle::initName ()
{
  int l;
  geometricObject *item0, *item1, *item2;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this angle %1")) + 1;
  switch (type)
    {
    case ANGLE_3PTS:
      item0 = (geometricObject *) getN (0);
      item1 = (geometricObject *) getN (1);
      item2 = (geometricObject *) getN (2);
      if (!item0->getName () || !item1->getName () || !item2->getName ())
	break;
      if (strlen (item0->getName ()) == 0 || strlen (item1->getName ()) == 0
	  || strlen (item2->getName ()) == 0)
	break;
      g_free (name);
      name = g_strdup_printf ("%s%s%s", item0->getName (),
			      item1->getName (), item2->getName ());
      break;
    case ANGLE_2VEC:
      item0 = (geometricObject *) getN (0);
      item1 = (geometricObject *) getN (1);
      if (!item0->getName () || !item1->getName ())
	break;
      if (strlen (item0->getName ()) == 0 || strlen (item1->getName ()) == 0)
	break;
      g_free (name);
      name = g_strdup_printf ("(%s,%s)", item0->getName (),
			      item1->getName ());
      break;
    }
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this angle %1"));
  strinsmsg (typeName, name, "%1");
}

void
angle::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item, subitem;
  GList *parent;
  gchar *content;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "angle", NULL);
  switch (type)
    {
    case ANGLE_3PTS:
      save_attribute (item, this, "3pts");
      break;
    case ANGLE_2VEC:
      save_attribute (item, this, "vectors");
      content = g_strdup_printf ("%lf", p.getX ());
      subitem = xmlNewChild (item, NULL, BAD_CAST "x", BAD_CAST content);
      g_free (content);
      content = g_strdup_printf ("%lf", p.getY ());
      subitem = xmlNewChild (item, NULL, BAD_CAST "y", BAD_CAST content);
      g_free (content);
      break;
    }
  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}

void
angle::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ANGLE_3PTS:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Geometric angle:: %s"), name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Vertex %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Point %s"), getNthName (2));
      break;
    case ANGLE_2VEC:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Oriented angle:: %s"), name);
      description[1] = g_strdup_printf (_("Vector %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Vector %s"), getNthName (1));
      break;
    }
}
