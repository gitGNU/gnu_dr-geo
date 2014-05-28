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

#include "drgeo_numeric.h"
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "drgeo_circle.h"
#include "drgeo_arcCircle.h"
#include "drgeo_repere.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_drawable.h"

int numericPrecision;

numeric::numeric (drgeoPoint & p, liste_elem & parents, numericType type, gboolean createdFromMacro, liste_elem * figureList):
value (p, createdFromMacro, figureList)
{
  style.color = preferedColor (":scalarColor");

  category = NUMERIC;
  this->type = (gint) type;
  parents.init_lire ();
  switch (type)
    {
    case SEGMENT_LENGTH:
      /* SEGMENT */
    case VECTOR_NORM:
      /* VECTOR */
    case CIRCLE_PERIMETER:
      /* CIRCLE */
    case LINE_SLOPE:
      /* LINE */
    case ARC_CIRCLE_LENGTH:
      /* ARC_CIRCLE */
    case POINT_ABSCISSA:
    case POINT_ORDINATE:
    case VECTOR_ABSCISSA:
    case VECTOR_ORDINATE:
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      break;
    case DISTANCE_PT_CIRCLE:
      /* POINT - CIRCLE */
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								CIRCLE));
      break;
    case DISTANCE_PT_LINE:
      /* POINT - LINE - */
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								LINE));
      break;
    case DISTANCE_2PTS:
      /* POINT - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      break;
    }
  initName ();
}

numeric::numeric (drgeoPoint & p, gdouble val, numericType type, gboolean createdFromMacro):
value (p, createdFromMacro, (liste_elem *) NULL)
{
  style.color = preferedColor (":scalarColor");

  category = NUMERIC;
  this->type = (gint) type;
  switch (type)
    {
    case FREE_VALUE:
      this->val = val;
      break;
    }
  initName ();
}

numeric::numeric (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
value (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;
  double val = 0;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("numeric::numeric missing mandatory attribute\n");
  else
    {
      category = NUMERIC;
      if (!strcmp (attr, "segment_length"))
	type = SEGMENT_LENGTH;
      else if (!strcmp (attr, "vector_norm"))
	type = VECTOR_NORM;
      else if (!strcmp (attr, "distance_2pts"))
	type = DISTANCE_2PTS;
      else if (!strcmp (attr, "distance_pt_circle"))
	type = DISTANCE_PT_CIRCLE;
      else if (!strcmp (attr, "distance_pt_line"))
	type = DISTANCE_PT_LINE;
      else if (!strcmp (attr, "circle_perimeter"))
	type = CIRCLE_PERIMETER;
      else if (!strcmp (attr, "slope"))
	type = LINE_SLOPE;
      else if (!strcmp (attr, "arc_length"))
	type = ARC_CIRCLE_LENGTH;
      else if (!strcmp (attr, "value"))
	{
	  type = FREE_VALUE;
	  xml_get_value_double (tree, "value", &val);
	  this->val = val;
	}
      else if (!strcmp (attr, "pt_abscissa"))
	type = POINT_ABSCISSA;
      else if (!strcmp (attr, "pt_ordinate"))
	type = POINT_ORDINATE;
      else if (!strcmp (attr, "vector_abscissa"))
	type = VECTOR_ABSCISSA;
      else if (!strcmp (attr, "vector_ordinate"))
	type = VECTOR_ORDINATE;
      /* get the coordinate on screen of this item */
      xml_get_value_double (tree, "x", &val);
      p.setX (val);
      xml_get_value_double (tree, "y", &val);
      p.setY (val);
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
numeric::update (drgeoDrawable & area)
{
  gchar *tmp;
  static drgeoVector p1, p2, u, m;
  exist = parentExist ();
  if (!exist)
    return;
  switch (type)
    {
    case SEGMENT_LENGTH:
    case VECTOR_NORM:
      setValue (((direction *) getFirst)->getDirection ().norm ());
      break;
    case DISTANCE_2PTS:
      setValue ((((point *) getFirst)->getCoordinate () -
		 ((point *) getLast)->getCoordinate ()).norm ());
      break;
    case DISTANCE_PT_CIRCLE:
      m = ((point *) getFirst)->getCoordinate ();
      u = ((circle *) getLast)->getCenter () - m;
      if (!sectionCircleLine (m, u, ((circle *) getLast)->getCenter (),
			      ((circle *) getLast)->getRadius (), 1, p1))
	{
	  setValue (((circle *) getLast)->getRadius ());
	  break;
	}
      sectionCircleLine (m, u, ((circle *) getLast)->getCenter (),
			 ((circle *) getLast)->getRadius (), -1, p2);
      if ((p1 - m).norm () > (p2 - m).norm ())
	setValue ((p2 - m).norm ());
      else
	setValue ((p1 - m).norm ());
      break;
    case DISTANCE_PT_LINE:
      p1 = ((point *) getFirst)->getCoordinate ();
      m = ((direction *) getLast)->getOrigin ();
      u = ((direction *) getLast)->getDirection ();
      u = u.normal () / u.norm ();
      setValue (ABS ((p1 - m) * u));
      break;
    case CIRCLE_PERIMETER:
      setValue (2 * M_PI * ((circle *) getFirst)->getRadius ());
      break;
    case LINE_SLOPE:
      u = ((direction *) getFirst)->getDirection ();
      if (u.getX () == 0)
	{
	  exist = FALSE;
	  break;
	}
      setValue (u.getY () / u.getX ());
      break;
    case ARC_CIRCLE_LENGTH:
      setValue (ABS (((arcCircle *) getFirst)->getLength ())
		* ((arcCircle *) getFirst)->getRadius ());
      break;
    case POINT_ABSCISSA:
      setValue (((point *) getFirst)->getCoordinate ().getX ());
      break;
    case POINT_ORDINATE:
      setValue (((point *) getFirst)->getCoordinate ().getY ());
      break;
    case VECTOR_ABSCISSA:
      setValue (((direction *) getFirst)->getDirection ().getX ());
      break;
    case VECTOR_ORDINATE:
      setValue (((direction *) getFirst)->getDirection ().getY ());
      break;
    }

  if (name ? strlen (name) : 0)
    {
      tmp = g_strdup_printf ("%%s = %%.%df", numericPrecision);
      setString (g_strdup_printf (tmp, name, val));
      g_free (tmp);
    }
  else
    {
      tmp = g_strdup_printf ("%%.%df", numericPrecision);            
      setString (g_strdup_printf (tmp, val));
      g_free (tmp);
    }
}

void
numeric::initName ()
{
  int l;
  gchar vide[1] = { 0 };
  geometricObject *item0, *item1;
  gchar *parentName = NULL;
  if (style.mask == alway)
    return;
  g_free (typeName);
  l = 1;
  switch (type)
    {
    case ARC_CIRCLE_LENGTH:
    case SEGMENT_LENGTH:
      item0 = (geometricObject *) getFirst;
      l += strlen (_("%1's length"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	parentName = g_strdup (item0->getName ());
      if (parentName)
	l += strlen (parentName);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("%1's length"));
      strinsmsg (typeName, parentName, "%1");
      break;
    case VECTOR_NORM:
      item0 = (geometricObject *) getFirst;
      l += strlen (_("%1's magnitude"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	parentName = g_strdup (item0->getName ());
      if (parentName)
	l += strlen (parentName);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("%1's magnitude"));
      strinsmsg (typeName, parentName, "%1");
      break;
    case CIRCLE_PERIMETER:
      item0 = (geometricObject *) getFirst;
      l += strlen (_("%1's perimeter"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	parentName = g_strdup (item0->getName ());
      if (parentName)
	l += strlen (parentName);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("%1's perimeter"));
      strinsmsg (typeName, parentName, "%1");
      break;
    case LINE_SLOPE:
      item0 = (geometricObject *) getFirst;
      l += strlen (_("the slope of %1"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	parentName = g_strdup (item0->getName ());
      if (parentName)
	l += strlen (parentName);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("the slope of %1"));
      strinsmsg (typeName, parentName, "%1");
      break;
    case DISTANCE_PT_CIRCLE:
    case DISTANCE_PT_LINE:
    case DISTANCE_2PTS:
      item0 = (geometricObject *) getFirst;
      item1 = (geometricObject *) getLast;
      l += strlen (_("the distance between %1 and %2"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	{
	  l += strlen (item0->getName ());
	}
      if (item1->getName ()? strlen (item1->getName ()) : 0)
	{
	  l += strlen (item1->getName ());
	}
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("the distance between %1 and %2"));
      if (item0->getName ())
	strinsmsg (typeName, item0->getName (), "%1");
      else
	strinsmsg (typeName, vide, "%1");
      if (item1->getName ())
	strinsmsg (typeName, item1->getName (), "%2");
      else
	strinsmsg (typeName, vide, "%2");
      break;
    case POINT_ABSCISSA:
    case VECTOR_ABSCISSA:
      item0 = (geometricObject *) getFirst;
      l += strlen (_("the abscissa of %1"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	parentName = g_strdup (item0->getName ());
      if (parentName)
	l += strlen (parentName);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("the abscissa of %1"));
      strinsmsg (typeName, parentName, "%1");
      break;
    case POINT_ORDINATE:
    case VECTOR_ORDINATE:
      item0 = (geometricObject *) getFirst;
      l += strlen (_("the ordinate of %1"));
      if (item0->getName ()? strlen (item0->getName ()) : 0)
	parentName = g_strdup (item0->getName ());
      if (parentName)
	l += strlen (parentName);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("the ordinate of %1"));
      strinsmsg (typeName, parentName, "%1");
      break;
    default:
      typeName = g_strdup (_("this value"));
      break;
    }
  g_free (parentName);
}

void
numeric::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item, subitem;
  GList *parent;
  gchar *content;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "numeric", NULL);
  switch (type)
    {
    case SEGMENT_LENGTH:
      save_attribute (item, this, "segment_length");
      break;
    case VECTOR_NORM:
      save_attribute (item, this, "vector_norm");
      break;
    case DISTANCE_2PTS:
      save_attribute (item, this, "distance_2pts");
      break;
    case DISTANCE_PT_CIRCLE:
      save_attribute (item, this, "distance_pt_circle");
      break;
    case DISTANCE_PT_LINE:
      save_attribute (item, this, "distance_pt_line");
      break;
    case CIRCLE_PERIMETER:
      save_attribute (item, this, "circle_perimeter");
      break;
    case LINE_SLOPE:
      save_attribute (item, this, "slope");
      break;
    case ARC_CIRCLE_LENGTH:
      save_attribute (item, this, "arc_length");
      break;
    case FREE_VALUE:
      save_attribute (item, this, "value");
      content = g_strdup_printf ("%lf", this->val);
      subitem = xmlNewChild (item, NULL, BAD_CAST "value", BAD_CAST content);
      g_free (content);
      break;
    case POINT_ABSCISSA:
      save_attribute (item, this, "pt_abscissa");
      break;
    case POINT_ORDINATE:
      save_attribute (item, this, "pt_ordinate");
      break;
    case VECTOR_ABSCISSA:
      save_attribute (item, this, "vector_abscissa");
      break;
    case VECTOR_ORDINATE:
      save_attribute (item, this, "vector_ordinate");
      break;
    }

  content = g_strdup_printf ("%lf", p.getX ());
  subitem = xmlNewChild (item, NULL, BAD_CAST "x", BAD_CAST content);
  g_free (content);
  content = g_strdup_printf ("%lf", p.getY ());
  subitem = xmlNewChild (item, NULL, BAD_CAST "y", BAD_CAST content);
  g_free (content);

  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}

void
numeric::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case FREE_VALUE:
      description = new gchar *[1];
      description[0] = g_strdup_printf (_("Free value:: %s"), name);
      break;
    case SEGMENT_LENGTH:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Segment's length:: %s"), name);
      description[1] = g_strdup_printf (_("Segment %s"), getNthName (0));
      break;
    case VECTOR_NORM:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Vector's norm:: %s"), name);
      description[1] = g_strdup_printf (_("Vector %s"), getNthName (0));
      break;
    case CIRCLE_PERIMETER:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Circle's perimeter:: %s"), name);
      description[1] = g_strdup_printf (_("Circle %s"), getNthName (0));
      break;
    case LINE_SLOPE:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Line's slope:: %s"), name);
      description[1] = g_strdup_printf (_("Line %s"), getNthName (0));
      break;
    case ARC_CIRCLE_LENGTH:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Arc-circle's length:: %s"), name);
      description[1] = g_strdup_printf (_("Arc-circle %s"), getNthName (0));
      break;
    case POINT_ABSCISSA:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Point's abscissa:: %s"), name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      break;
    case POINT_ORDINATE:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Point's ordinate:: %s"), name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      break;
    case VECTOR_ABSCISSA:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Vector's abscissa:: %s"), name);
      description[1] = g_strdup_printf (_("Vector %s"), getNthName (0));
      break;
    case VECTOR_ORDINATE:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Vector's ordinate:: %s"), name);
      description[1] = g_strdup_printf (_("Vector %s"), getNthName (0));
      break;
    case DISTANCE_PT_CIRCLE:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Distance between a point and a circle:: %s"),
			 name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Circle %s"), getNthName (1));
      break;
    case DISTANCE_PT_LINE:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Distance between a point and a line:: %s"), name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Line %s"), getNthName (1));
      break;
    case DISTANCE_2PTS:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Distance between two points:: %s"), name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Point %s"), getNthName (1));
      break;
    }
}


void * numeric::
getExtra ()
{
  void *p = NULL;

  if (type == FREE_VALUE)
    {
      p = g_malloc (sizeof (double));
      * (double *) p = getValue ();      
    }
  return p;
}
