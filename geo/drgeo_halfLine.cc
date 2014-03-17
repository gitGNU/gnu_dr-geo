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

#include "config.h"
#include <math.h>
#include "drgeo_halfLine.h"
#include "drgeo_repere.h"
#include "drgeo_point.h"
#include "liste_elem.h"
#include "drgeo_drawable.h"

halfLine::halfLine (liste_elem & parents, halfLineType type, gboolean createdFromMacro, liste_elem * figureList):
direction (parents, (transformationType) type, createdFromMacro, figureList)
{
  style.color = preferedColor (":halfLineColor");
  style.thick = preferedThickness (":halfLineStyle");

  this->type = (gint) type;
  category = HALF_LINE;
  switch (type)
    {
    case HALFLINE_2PTS:
      /* POINT - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (1));
      parentList = g_list_append (parentList, (gpointer) parents.lire (2));
      break;
    }
  initName ();
}

halfLine::halfLine (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
direction (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("halfLine::halfLine missing mandatory attribute\n");
  else
    {
      category = HALF_LINE;
      if (!strcmp (attr, "Rotation"))
	type = ROTATION;
      else if (!strcmp (attr, "Symmetry"))
	type = SYMMETRY;
      else if (!strcmp (attr, "Reflexion"))
	type = REFLEXION;
      else if (!strcmp (attr, "Translation"))
	type = TRANSLATION;
      else if (!strcmp (attr, "Scale"))
	type = SCALE;
      else if (!strcmp (attr, "2pts"))
	type = HALFLINE_2PTS;
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
halfLine::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  area.drawHalfLine (style, origin, u);
}

void
halfLine::update (drgeoDrawable & area)
{
  exist = parentExist ();
  if (!exist)
    return;
  if ((type & TRANSFORMATION) != 0)
    direction::update (area);
  else
    {
      switch (type)
	{
	case HALFLINE_2PTS:
	  origin = ((point *) getFirst)->getCoordinate ();
	  u = ((point *) getLast)->getCoordinate () - origin;
	  break;
	}
    }
  if (u.getX () == 0 && u.getY () == 0)
    exist = FALSE;
}

gboolean
halfLine::overObject (drgeoPoint & mouse, gdouble range)
{
  if (!exist)
    return FALSE;
  return ABS (u ^ (mouse - origin)) / u.norm () <= range
    && ((mouse - origin) * u) >= 0;
}

void
halfLine::move (drgeoVector & t)
{
  switch (type)
    {
    case HALFLINE_2PTS:
      ((geometricObject *) getFirst)->move (t);
      ((geometricObject *) getLast)->move (t);
      break;
    }
}

void
halfLine::initName ()
{
  int l;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this half-line %1")) + 1;
  switch (type)
    {
    case HALFLINE_2PTS:
      geometricObject * item0, *item1;
      item0 = (geometricObject *) getFirst;
      item1 = (geometricObject *) getLast;
      if (!item0->getName () || !item1->getName ())
	break;
      if (strlen (item0->getName ()) == 0 || strlen (item1->getName ()) == 0)
	break;
      g_free (name);
      name = g_strdup_printf ("[%s%s)", item0->getName (), item1->getName ());
      break;
    }
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this half-line %1"));
  strinsmsg (typeName, name, "%1");
}

void
halfLine::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "halfLine", NULL);
  switch (type)
    {
    case ROTATION:
      save_attribute (item, this, "Rotation");
      break;
    case SCALE:
      save_attribute (item, this, "Scale");
      break;
    case SYMMETRY:
      save_attribute (item, this, "Symmetry");
      break;
    case REFLEXION:
      save_attribute (item, this, "Reflexion");
      break;
    case TRANSLATION:
      save_attribute (item, this, "Translation");
      break;
    case HALFLINE_2PTS:
      save_attribute (item, this, "2pts");
      break;
    }
  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}
void
halfLine::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate half-line:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed half-line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric half-line:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed half-line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric half-line:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed half-line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate half-line:: %s"), name);
      description[1] = g_strdup_printf (_("Translated half-line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale half-line:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled half-line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    case HALFLINE_2PTS:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Half-line defined by two points:: %s"), name);
      description[1] = g_strdup_printf (_("Origin %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Point %s"), getNthName (1));
      break;
    }
}

drgeoPoint & halfLine::getPointAt (gdouble abscissa)
{
  static drgeoPoint
    a;
  if (abscissa == 1)
    {
      a = u * abscissa;
      /* point at infinit */
      if (a.getX () > 0)
	a.setX (PINFINIT);
      else
	a.setX (NINFINIT);
      if (a.getY () < 0)
	a.setY (PINFINIT);
      else
	a.setY (NINFINIT);
    }
  else
    {
      abscissa *= M_PI_2;
      /* compute the coordinate */
      a = origin + tan (abscissa) * (u / u.norm ());
    }
  return a;
}

drgeoPoint & halfLine::getClosestPoint (drgeoPoint & p)
{
  static drgeoPoint
    a;
  gdouble
    ma;
  ma = (p - origin) * (u / u.norm ());
  if (ma < 0)
    ma = 0;
  a = origin + ma * (u / u.norm ());
  return a;
}

gdouble
halfLine::getAbscissa (drgeoPoint & p)
{
  return atan ((p - origin) * (u / u.norm ())) * M_2_PI;
}

gboolean
halfLine::getIntersection (geometricObject & c, drgeoPoint & p, gint k)
{
  gboolean intersection;
  switch (c.getCategory ())
    {
    case LINE:
      intersection = directionIntersection ((direction &) c, p);
      return intersection && overObject (p, 1.0);
      break;
    case HALF_LINE:
      intersection = directionIntersection ((direction &) c, p);
      return intersection && overObject (p, 1.0) && c.overObject (p, 1.0);
      break;
    case SEGMENT:
      return c.getIntersection (*((geometricObject *) this), p, 0);
      break;
    case CIRCLE:
      return c.getIntersection (*((geometricObject *) this), p, k);
      break;
    case ARC_CIRCLE:
      return c.getIntersection (*((geometricObject *) this), p, k);
      break;
    }
}
