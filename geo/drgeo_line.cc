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
#include "drgeo_line.h"
#include "drgeo_repere.h"
#include "drgeo_point.h"
#include "liste_elem.h"
#include "drgeo_drawable.h"

line::line (liste_elem & parents, lineType type, gboolean createdFromMacro, liste_elem * figureList):
direction (parents, (transformationType) type, createdFromMacro, figureList)
{
  style.color = preferedColor (":lineColor");
  style.thick = preferedThickness (":lineStyle");

  this->type = (gint) type;
  category = LINE;
  switch (type)
    {
    case LINE_2PTS:
      /* POINT - POINT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parentList =
	g_list_append (parentList,
		       (gpointer) searchForCategory (parents, POINT));
      break;
    case LINE_PT_VA:
      /* POINT - VALUE */
      break;
    case LINE_PAR_DIRECTION:
      /* POINT - DIRECTION */
    case LINE_PER_DIRECTION:
      /* POINT - DIRECTION */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								DIRECTION));
      break;
    }
  initName ();
}

line::line (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
direction (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("line::line missing mandatory attribute\n");
  else
    {
      category = LINE;
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
	type = LINE_2PTS;
      else if (!strcmp (attr, "pt_slope"))
	type = LINE_PT_VA;
      else if (!strcmp (attr, "parallel"))
	type = LINE_PAR_DIRECTION;
      else if (!strcmp (attr, "perpendicular"))
	type = LINE_PER_DIRECTION;
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
line::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  area.drawLine (style, origin, u);
}

void
line::update (drgeoDrawable & area)
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
	case LINE_2PTS:
	  origin = ((point *) getFirst)->getCoordinate ();
	  u = ((point *) getLast)->getCoordinate () - origin;
	  break;
	case LINE_PT_VA:
	  break;
	case LINE_PAR_DIRECTION:
	  origin = ((point *) getFirst)->getCoordinate ();
	  u = ((direction *) getLast)->getDirection ();
	  break;
	case LINE_PER_DIRECTION:
	  origin = ((point *) getFirst)->getCoordinate ();
	  u = ((direction *) getLast)->getNormal ();
	  if (ABS (u * (((direction *) getLast)->getOrigin () - origin)) >
	      EPSILON)
	    {
	      if (u * (((direction *) getLast)->getOrigin () - origin) < 0)
		u = -1 * u;
	    }
	  /* line origin == origin point, compute another point 
	     but in this case no matter, sens will never change */
	  break;
	}
    }
  if (u.getX () == 0 && u.getY () == 0)
    exist = FALSE;
}

gboolean
line::overObject (drgeoPoint & mouse, gdouble range)
{
  if (!exist)
    return FALSE;
  return ABS (u ^ (mouse - origin)) / u.norm () <= range;
}

void
line::move (drgeoVector & t)
{
  switch (type)
    {
    case LINE_2PTS:
      ((geometricObject *) getFirst)->move (t);
      ((geometricObject *) getLast)->move (t);
      break;
    case LINE_PT_VA:
    case LINE_PAR_DIRECTION:
    case LINE_PER_DIRECTION:
      ((geometricObject *) getFirst)->move (t);
      break;
    }
  //      update ();
}

void
line::initName ()
{
  int l;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this line %1")) + 1;
  switch (type)
    {
    case LINE_2PTS:
      geometricObject * item0, *item1;
      item0 = (geometricObject *) getFirst;
      item1 = (geometricObject *) getLast;
      if (!item0->getName () || !item1->getName ())
	break;
      if (strlen (item0->getName ()) == 0 || strlen (item1->getName ()) == 0)
	break;
      g_free (name);
      name = g_strdup_printf ("(%s%s)", item0->getName (), item1->getName ());
      break;
    }
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this line %1"));
  strinsmsg (typeName, name, "%1");
  style.name = name;
}

void
line::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "line", NULL);
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
    case LINE_2PTS:
      save_attribute (item, this, "2pts");
      break;
    case LINE_PT_VA:
      save_attribute (item, this, "pt_slope");
      break;
    case LINE_PAR_DIRECTION:
      save_attribute (item, this, "parallel");
      break;
    case LINE_PER_DIRECTION:
      save_attribute (item, this, "perpendicular");
      break;
    }
  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}

void
line::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate line:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric line:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric line:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate line:: %s"), name);
      description[1] = g_strdup_printf (_("Translated line %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale line:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled line %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    case LINE_2PTS:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Line going through two points:: %s"), name);
      description[1] = g_strdup_printf (_("Point  %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Point %s"), getNthName (1));
      break;
    case LINE_PT_VA:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Line defined by a point and its slope:: %s"),
			 name);
      description[1] = g_strdup_printf (_("Point  %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Slope %s"), getNthName (1));
      break;
    case LINE_PAR_DIRECTION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Parallel line:: %s"), name);
      description[1] = g_strdup_printf (_("Point  %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Direction %s"), getNthName (1));
      break;
    case LINE_PER_DIRECTION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Perpendicular line:: %s"), name);
      description[1] = g_strdup_printf (_("Point  %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Direction %s"), getNthName (1));
      break;
    }
}

drgeoPoint & line::getPointAt (gdouble abscissa)
{
  static drgeoPoint
    a;
  /* Fit to [- 0.5 ; 0.5] */
  abscissa -= 0.5;
  if (abscissa == .5 || abscissa == -.5)
    {
      a = u * abscissa;
      /* point at infinit */
      if (a.getX () > 0)
	a.setX (PINFINIT);
      else
	a.setX (NINFINIT);
      if (a.getY () > 0)
	a.setY (PINFINIT);
      else
	a.setY (NINFINIT);
    }
  else
    {
      abscissa *= M_PI;
      /* compute the coordinate */
      a = origin + tan (abscissa) * (u / u.norm ());
    }
  return a;
}

drgeoPoint & line::getClosestPoint (drgeoPoint & p)
{
  static drgeoPoint
    a;
  a = origin + ((p - origin) * (u / u.norm ())) * (u / u.norm ());
  return a;
}

gdouble
line::getAbscissa (drgeoPoint & p)
{
  return atan ((p - origin) * (u / u.norm ())) * M_1_PI + 0.5;
}

gboolean
line::getIntersection (geometricObject & c, drgeoPoint & p, gint k)
{
  switch (c.getCategory ())
    {
    case LINE:
      return directionIntersection ((direction &) c, p);
      break;
    case HALF_LINE:
    case SEGMENT:
      return c.getIntersection (*((geometricObject *) this), p, 0);
      break;
    case CIRCLE:
      return c.getIntersection (*((geometricObject *) this), p, k);
      break;
    case ARC_CIRCLE:
      return c.getIntersection (*((geometricObject *) this), p, k);
      break;
    case LOCUS:
      return c.getIntersection (*((geometricObject *) this), p, k);      
    }
}

