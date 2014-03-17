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
#include "drgeo_equation.h"
#include "drgeo_direction.h"
#include "drgeo_circle.h"
#include "drgeo_drawable.h"

equation::equation (drgeoPoint & p, liste_elem & parents, equationType type, gboolean createdFromMacro, liste_elem * figureList):
string (createdFromMacro, figureList)
{
  category = EQUATION;
  this->type = (gint) type;
  parents.init_lire ();
  switch (type)
    {
    case LINE_EQUATION:
      /* LINE */
    case CIRCLE_EQUATION:
      /* CIRCLE */
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      break;
    }
  initName ();
}


equation::equation (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
string (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  double val = 0;
  void *obj;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("equation::equation missing mandatory attribute\n");
  else
    {
      category = EQUATION;
      if (!strcmp (attr, "line"))
	type = LINE_EQUATION;
      else if (!strcmp (attr, "circle"))
	type = CIRCLE_EQUATION;
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
equation::update (drgeoDrawable & area)
{
  static drgeoPoint p, u;
  exist = parentExist ();
  if (!exist)
    return;
  g_free (str);
  switch (type)
    {
    case LINE_EQUATION:
      u = ((direction *) getFirst)->getDirection ();
      p = ((direction *) getFirst)->getOrigin ();
      if (ABS (u.getX ()) < EPSILON)
	{
	  if (p.getX () != 0)
	    str = g_strdup_printf ("x = %.2f", p.getX ());
	  else
	    str = g_strdup ("x = 0");
	}
      else
	{
	  gdouble s, b;

	  s = u.getY () / u.getX ();
	  b = p.getY () - p.getX () * s;
	  if (ABS (s) < EPSILON)
	    {
	      if (b == 0)
		str = g_strdup ("y = 0");
	      else
		str = g_strdup_printf (" y = %.2f", b);
	    }
	  else
	    {
	      if (b == 0)
		str = g_strdup_printf ("y = %.2f x", s);
	      else
		str = g_strdup_printf ("y = %.2f x + %.2f", s, b);
	    }
	}
      break;
    case CIRCLE_EQUATION:
      {
	gdouble r;
	r = ((circle *) getFirst)->getRadius ();
	r *= r;
	p = ((circle *) getFirst)->getCenter ();
	if (ABS (p.getX ()) < EPSILON)
	  {
	    if (ABS (p.getY ()) < EPSILON)
	      str = g_strdup_printf ("x² + y² = %.2f", r);
	    else
	      str = g_strdup_printf ("x² + (y + %.2f)² = %.2f",
				     -p.getY (), r);
	  }
	else
	  {
	    if (ABS (p.getY ()) < EPSILON)
	      str = g_strdup_printf ("(x + %.2f)² + y² = %.2f",
				     -p.getX (), r);
	    else
	      str = g_strdup_printf
		("(x + %.2f)² + (y + %.2f)² = %.2f",
		 -p.getX (), -p.getY (), r);
	  }
      }
      break;
    }
}

void
equation::initName ()
{
  int l;
  gchar vide[1] = { 0 };
  geometricObject *item;
  if (style.mask == alway)
    return;
  l = 1;
  switch (type)
    {
    case LINE_EQUATION:
    case CIRCLE_EQUATION:
      item = (geometricObject *) getFirst;
      l += strlen (_("the equation of %1"));
      if (item->getName ()? strlen (item->getName ()) : 0)
	{
	  g_free (name);
	  name = g_strdup (item->getName ());
	}
      if (name)
	l += strlen (name);
      typeName = (gchar *) g_malloc (l);
      strcpy (typeName, _("the equation of %1"));
      strinsmsg (typeName, name, "%1");
      break;
    }
}

void
equation::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item, subitem;
  GList *parent;
  gchar *content;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "equation", NULL);
  switch (type)
    {
    case LINE_EQUATION:
      save_attribute (item, this, "line");
      break;
    case CIRCLE_EQUATION:
      save_attribute (item, this, "circle");
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
equation::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case LINE_EQUATION:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Line's equation:: %s"), name);
      description[1] = g_strdup_printf (_("Line %s"), getNthName (0));
      break;
    case CIRCLE_EQUATION:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Circle's equation:: %s"), name);
      description[1] = g_strdup_printf (_("Circle %s"), getNthName (0));
      break;
    }
}
