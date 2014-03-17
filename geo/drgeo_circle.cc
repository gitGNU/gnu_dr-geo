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

#include "drgeo_circle.h"
#include "drgeo_value.h"
#include "drgeo_repere.h"
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "liste_elem.h"
#include "drgeo_drawable.h"

circle::circle (liste_elem & parents, circleType type, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  style.color = preferedColor (":circleColor");
  style.thick = preferedThickness (":circleStyle");

  this->type = (gint) type;
  category = CIRCLE;
  switch (type)
    {
    case ROTATION:
    case SCALE:
      /* CIRCLE - POINT - VALUE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								CIRCLE));
      parentList =
	g_list_append (parentList,
		       (gpointer) searchForCategory (parents, POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VALUE));
      break;
    case SYMMETRY:
      /* CIRCLE - POINT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      break;
    case REFLEXION:
      /* CIRCLE - AXE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								LINE));
      break;
    case TRANSLATION:
      /* CIRCLE - VECTOR */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VECTOR));
      break;
    case CIRCLE_2PTS:
      /* POINT - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (1));
      parentList = g_list_append (parentList, (gpointer) parents.lire (2));
      break;
    case CIRCLE_SEGMENT:
      /* POINT - SEGMENT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								SEGMENT));
      break;
    case CIRCLE_VALUE:
      /* POINT - VALUE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VALUE));
      break;
    }
  initName ();
}

circle::circle (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
geometricObject (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("circle::circle missing mandatory attribute\n");
  else
    {
      category = CIRCLE;
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
	type = CIRCLE_2PTS;
      else if (!strcmp (attr, "segment"))
	type = CIRCLE_SEGMENT;
      else if (!strcmp (attr, "radius"))
	type = CIRCLE_VALUE;
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
circle::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  area.drawCircle (style, center, radius);
}

void
circle::update (drgeoDrawable & area)
{
  exist = parentExist ();
  if (!exist)
    return;
  if (type & TRANSFORMATION)
    {
      center = ((circle *) getFirst)->getCenter ();
      radius = ((circle *) getFirst)->getRadius ();
    }
  switch (type)
    {
    case ROTATION:
      center = center.rotatePoint (((point *) getN (1))->getCoordinate (),
				   ((value *) getN (2))->getValue ());
      break;
    case SCALE:
      center = (center - ((point *) getN (1))->getCoordinate ())
	* ((value *) getLast)->getValue ()
	+ ((point *) getN (1))->getCoordinate ();
      radius *= ABS (((value *) getLast)->getValue ());
      break;
    case SYMMETRY:
      center = 2 * ((point *) getN (1))->getCoordinate () - center;
      break;
    case REFLEXION:
      center = center.reflexionPoint (((direction *) getN (1))->getOrigin (),
				      ((direction *) getN (1))->
				      getDirection ());
      break;
    case TRANSLATION:
      center += ((direction *) getN (1))->getDirection ();
      break;
    case CIRCLE_2PTS:
      center = ((point *) getFirst)->getCoordinate ();
      radius = (center - ((point *) getLast)->getCoordinate ()).norm ();
      break;
    case CIRCLE_SEGMENT:
      center = ((point *) getFirst)->getCoordinate ();
      radius = ((direction *) getLast)->getDirection ().norm ();
      break;
    case CIRCLE_VALUE:
      center = ((point *) getFirst)->getCoordinate ();
      radius = ((value *) getLast)->getValue ();
      break;
    }
}

gboolean
circle::overObject (drgeoPoint & mouse, gdouble range)
{
  if (!exist)
    return FALSE;
  return ABS ((center - mouse).norm () - radius) <= range;
}

void
circle::move (drgeoVector & t)
{
  switch (type)
    {
    case CIRCLE_2PTS:
      ((geometricObject *) getLast)->move (t);
      /* go through to move center(getFirst) */
    case CIRCLE_SEGMENT:
    case CIRCLE_VALUE:
      ((geometricObject *) getFirst)->move (t);
      break;
    }
}

void
circle::initName ()
{
  int l;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this circle %1")) + 1;
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this circle %1"));
  strinsmsg (typeName, name, "%1");
}

void
circle::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "circle", NULL);
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
    case CIRCLE_2PTS:
      save_attribute (item, this, "2pts");
      break;
    case CIRCLE_SEGMENT:
      save_attribute (item, this, "segment");
      break;
    case CIRCLE_VALUE:
      save_attribute (item, this, "radius");
      break;
    }
  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}
void
circle::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate circle:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric circle:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric circle:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate circle:: %s"), name);
      description[1] = g_strdup_printf (_("Translated circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale circle:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    case CIRCLE_2PTS:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Circle defined by its center and a point:: %s"),
			 name);
      description[1] = g_strdup_printf (_("Center %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Point %s"), getNthName (1));
      break;
    case CIRCLE_SEGMENT:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Circle defined by its center and a segment:: %s"),
			 name);
      description[1] = g_strdup_printf (_("Center %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Radius from segment %s"),
					getNthName (1));
      break;
    case CIRCLE_VALUE:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Circle defined by its center and radius:: %s"),
			 name);
      description[1] = g_strdup_printf (_("Center %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Radius %s"), getNthName (1));
      break;
    }
}

drgeoPoint & circle::getCenter ()
{
  return center;
}

gdouble
circle::getRadius ()
{
  return radius;
}

drgeoPoint & circle::getPointAt (gdouble abscissa)
{
  static drgeoPoint
    p;
  p.set (cos (2 * M_PI * abscissa), sin (2 * M_PI * abscissa));
  p = center + radius * p;
  return p;
}

drgeoPoint & circle::getClosestPoint (drgeoPoint & p)
{
  static drgeoPoint
    m;
  m = (p - center);
  m = center + radius * m / m.norm ();
  return m;
}

gdouble
circle::getAbscissa (drgeoPoint & p)
{
  static drgeoPoint m;
  gdouble a;
  m = p - center;
  a = atan2 (m.getY (), m.getX ());
  if (a < 0)
    a += 2 * M_PI;
  return a * 0.5 * M_1_PI;
}

gboolean
circle::getIntersection (geometricObject & c, drgeoPoint & p, gint k)
{
  gboolean intersection;
  switch (c.getCategory ())
    {
    case LINE:
      return sectionCircleLine
	(((direction &) c).getOrigin (), ((direction &) c).getDirection (),
	 center, radius, (k == 0) ? -1 : 1, p);
      break;
    case HALF_LINE:
    case SEGMENT:
      intersection = sectionCircleLine
	(((direction &) c).getOrigin (), ((direction &) c).getDirection (),
	 center, radius, (k == 0) ? -1 : 1, p);
      return intersection && c.overObject (p, 1.0);
      break;
    case CIRCLE:
      {
	static drgeoPoint c1c2, g;
	gdouble r2, x;

	c1c2 = ((circle &) c).getCenter () - center;
	r2 = ((circle &) c).getRadius ();
	x = -(r2 * r2 - radius * radius - c1c2.squareNorm ())
	  / (2 * c1c2.squareNorm ());
	g = x * c1c2;
	g += center;
	c1c2 = c1c2.normal ();
	return sectionCircleLine
	  (g, c1c2, center, radius, (k == 0) ? -1 : 1, p);
	break;
      }
    case ARC_CIRCLE:
      return c.getIntersection (*((geometricObject *) this), p, k);
      break;
    }
}
