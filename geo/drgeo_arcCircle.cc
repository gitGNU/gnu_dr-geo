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

#include "drgeo_arcCircle.h"
#include "drgeo_value.h"
#include "drgeo_repere.h"
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "liste_elem.h"
#include "drgeo_drawable.h"


arcCircle::arcCircle (liste_elem & parents, arcCircleType type, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  style.color = preferedColor (":arcCircleColor");
  style.thick = preferedThickness (":arcCircleStyle");

  this->type = (gint) type;
  category = ARC_CIRCLE;
  switch (type)
    {
    case ROTATION:
    case SCALE:
      /* ARCCIRCLE - POINT - VALUE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								ARC_CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VALUE));
      break;
    case SYMMETRY:
      /* ARCCIRCLE - POINT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								ARC_CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      break;
    case REFLEXION:
      /* ARCCIRCLE - AXE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								ARC_CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								LINE));
      break;
    case TRANSLATION:
      /* ARCCIRCLE - VECTOR */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								ARC_CIRCLE));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VECTOR));
      break;
    case ARCCIRCLE_3PTS:
      /* POINT - POINT - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (1));
      parentList = g_list_append (parentList, (gpointer) parents.lire (2));
      parentList = g_list_append (parentList, (gpointer) parents.lire (3));
      break;
    }
  initName ();
}

arcCircle::arcCircle (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
geometricObject (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("arcCircle::arcCircle missing mandatory attribute\n");
  else
    {
      category = ARC_CIRCLE;
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
      else if (!strcmp (attr, "3pts"))
	type = ARCCIRCLE_3PTS;
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
arcCircle::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  area.drawArc (style, center, radius, origin, length);
}

void
arcCircle::update (drgeoDrawable & area)
{
  static drgeoPoint a, b, c, u, v, m1, m2;
  gdouble ab;
  exist = parentExist ();
  if (!exist)
    return;
  if (type & TRANSFORMATION)
    {
      center = ((arcCircle *) getFirst)->getCenter ();
      radius = ((arcCircle *) getFirst)->getRadius ();
      origin = ((arcCircle *) getFirst)->getOrigin ();
      length = ((arcCircle *) getFirst)->getLength ();
    }
  switch (type)
    {
    case ROTATION:
      center = center.rotatePoint (((point *) getN (1))->getCoordinate (),
				   ((value *) getN (2))->getValue ());
      origin += ((value *) getN (2))->getValue ();
      break;
    case SCALE:
      center = (center - ((point *) getN (1))->getCoordinate ())
	* ((value *) getLast)->getValue ()
	+ ((point *) getN (1))->getCoordinate ();
      radius *= ABS (((value *) getLast)->getValue ());
      break;
    case SYMMETRY:
      center = 2 * ((point *) getN (1))->getCoordinate () - center;
      origin += M_PI;
      break;
    case REFLEXION:
      a.set (cos (origin), sin (origin));
      a = center + a * radius;
      a = a.reflexionPoint (((direction *) getN (1))->getOrigin (),
			    ((direction *) getN (1))->getDirection ());
      center = center.reflexionPoint (((direction *) getN (1))->getOrigin (),
				      ((direction *) getN (1))->
				      getDirection ());
      a = a - center;
      origin = atan2 (a.getY (), a.getX ());

      length = -length;
      break;
    case TRANSLATION:
      center += ((direction *) getN (1))->getDirection ();
      break;
    case ARCCIRCLE_3PTS:
      /* compute the centeras the intersection of two mediators(?) */
      a = ((point *) getN (0))->getCoordinate ();
      b = ((point *) getN (1))->getCoordinate ();
      c = ((point *) getN (2))->getCoordinate ();
      m1 = (a + b) / 2;
      m2 = (b + c) / 2;
      u = (a - b).normal ();
      v = (b - c).normal ();
      exist = sectionDirectionDirection (m1, u, m2, v, center);
      if (!exist)
	break;
      a -= center;
      b -= center;
      c -= center;
      radius = a.norm ();
      origin = atan2 (a.getY (), a.getX ());
      ab = atan2 (b.getY (), b.getX ()) - atan2 (a.getY (), a.getX ());
      length = atan2 (c.getY (), c.getX ()) - atan2 (a.getY (), a.getX ());
      if (length == 0)
	{
	  exist = FALSE;
	  break;
	}
      if (length < 0 && (ab < length || ab > 0))
	length += 2 * M_PI;
      else if (length > 0 && (ab > length || ab < 0))
	length -= 2 * M_PI;
      break;
    }
}

gboolean
arcCircle::overObject (drgeoPoint & mouse, gdouble range)
{
  gdouble am, d;
  if (!exist)
    return FALSE;
  am = atan2 ((mouse - center).getY (), (mouse - center).getX ()) - origin;
  d = ABS ((mouse - center).norm () - radius);
  if (d <= range)
    {
      if (length > 0)
	{
	  if (am < 0)
	    am += 2 * M_PI;
	  return length >= am;
	}
      else if (length < 0)
	{
	  if (am > 0)
	    am -= 2 * M_PI;
	  return length <= am;
	}
    }
  return FALSE;
}

void
arcCircle::move (drgeoVector & t)
{
  switch (type)
    {
    case ARCCIRCLE_3PTS:
      ((geometricObject *) getN (0))->move (t);
      ((geometricObject *) getN (1))->move (t);
      ((geometricObject *) getN (2))->move (t);
      break;
    }
}

void
arcCircle::initName ()
{
  int l;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this arc %1")) + 1;
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this arc %1"));
  strinsmsg (typeName, name, "%1");
}

void
arcCircle::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "arcCircle", NULL);
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
    case ARCCIRCLE_3PTS:
      save_attribute (item, this, "3pts");
      break;
    }
  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}

void
arcCircle::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate arc-circle:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed arc-circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric arc-circle:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed arc-circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric arc-circle:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed arc-circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate arc-circle:: %s"), name);
      description[1] = g_strdup_printf (_("Translated arc-circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale arc-circle:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled arc-circle %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    case ARCCIRCLE_3PTS:
      description = new gchar *[4];
      description[0] =
	g_strdup_printf (_("Arc-circle defined by three points:: %s"), name);
      description[1] = g_strdup_printf (_("Extremity %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Point %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Extremity %s"), getNthName (2));
      break;
    }
}

drgeoPoint & arcCircle::getCenter ()
{
  return center;
}

gdouble
arcCircle::getRadius ()
{
  return radius;
}

gdouble
arcCircle::getOrigin ()
{
  return origin;
}

gdouble
arcCircle::getLength ()
{
  return length;
}



drgeoPoint & arcCircle::getPointAt (gdouble abscissa)
{
  static drgeoPoint
    p;
  p.set (cos (origin + abscissa * length), sin (origin + abscissa * length));
  p = center + radius * p;
  return p;
}

drgeoPoint & arcCircle::getClosestPoint (drgeoPoint & p)
{
  static drgeoPoint
    m;
  gdouble
    am;
  m = (p - center);
  m = radius * m / m.norm ();
  am = atan2 (m.getY (), m.getX ()) - origin;
  if (length > 0)
    {
      if (am < 0)
	am += 2 * M_PI;
      if (length < am)
	am = length;
    }
  else if (length < 0)
    {
      if (am > 0)
	am -= 2 * M_PI;
      if (length > am)
	am = length;
    }
  p.set (cos (origin + am), sin (origin + am));
  p = center + radius * p;
}

gdouble
arcCircle::getAbscissa (drgeoPoint & p)
{
  static drgeoPoint m;
  gdouble am;
  m = p - center;
  am = atan2 (m.getY (), m.getX ()) - origin;
  if (length > 0)
    {
      if (am < 0)
	am += 2 * M_PI;
      if (am > length)
	am = length;
    }
  else if (length < 0)
    {
      if (am > 0)
	am -= 2 * M_PI;
      if (am < length)
	am = length;
    }
  return am / length;
}

gboolean
arcCircle::getIntersection (geometricObject & c, drgeoPoint & p, gint k)
{
  static drgeoPoint c1c2, g;
  gboolean intersection;
  switch (c.getCategory ())
    {
    case LINE:
      intersection = sectionCircleLine
	(((direction &) c).getOrigin (),
	 ((direction &) c).getDirection (),
	 center, radius, (k == 0) ? -1 : 1, p);
      return intersection && overObject (p, 1.0);
      break;
    case HALF_LINE:
    case SEGMENT:
      intersection = sectionCircleLine
	(((direction &) c).getOrigin (), ((direction &) c).getDirection (),
	 center, radius, (k == 0) ? -1 : 1, p);
      return intersection && c.overObject (p, 1.0) && overObject (p, 1.0);
      break;
    case CIRCLE:
      {
	gdouble r2, x;

	c1c2 = ((arcCircle &) c).getCenter () - center;
	r2 = ((arcCircle &) c).getRadius ();
	x = -(r2 * r2 - radius * radius - c1c2.squareNorm ())
	  / (2 * c1c2.squareNorm ());
	g = x * c1c2;
	g += center;
	c1c2 = c1c2.normal ();
	intersection = sectionCircleLine
	  (g, c1c2, center, radius, (k == 0) ? -1 : 1, p);
	return intersection && overObject (p, 1.0);
	break;
      }
    case ARC_CIRCLE:
      {
	gdouble r2, x;

	c1c2 = ((arcCircle &) c).getCenter () - center;
	r2 = ((arcCircle &) c).getRadius ();
	x = -(r2 * r2 - radius * radius - c1c2.squareNorm ())
	  / (2 * c1c2.squareNorm ());
	g = x * c1c2;
	g += center;
	c1c2 = c1c2.normal ();
	intersection = sectionCircleLine
	  (g, c1c2, center, radius, (k == 0) ? -1 : 1, p);
	return intersection && overObject (p, 1.0) && c.overObject (p, 1.0);
	break;
      }
    }
}
