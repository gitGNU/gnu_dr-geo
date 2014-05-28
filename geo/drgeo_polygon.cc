/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Free Software Foundation 2001
 * Auhtor: hilaire@ofset.org 
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
#include "drgeo_polygon.h"
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "drgeo_value.h"
#include "drgeo_repere.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_drawable.h"

polygon::polygon (liste_elem & parents, polygonType type, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  gpointer item;

  style.color = preferedColor (":polygonColor");

  this->type = (gint) type;
  category = POLYGON;
  switch (type)
    {
    case POLYGON_NPTS:
      /* the object parentList will contain
         a list of POINT object */
      parents.init_lire ();
      while (item = (gpointer) parents.lire (0))
	parentList = g_list_append (parentList, gpointer (item));
      break;
    case ROTATION:
    case SCALE:
      /* the object parentList will contain in this order              
         POLYGON (point to be transformed) - POINT - VALUE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POLYGON));
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
      /* POLYGON - POINT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POLYGON));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      break;
    case TRANSLATION:
      /* POLYGON - VECTOR */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POLYGON));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VECTOR));
      break;
    case REFLEXION:
      /* POLYGON - LINE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POLYGON));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								LINE));
      break;
    }

  if (type & TRANSFORMATION)
    numberVertex = ((polygon *) getFirst)->getNumberVertex ();
  else
    numberVertex = g_list_length (parentList);

  // The number of vertex does not change during the all life of a vertex
  vertices = new drgeoPoint[numberVertex];

  initName ();
}

polygon::polygon (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
geometricObject (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("point::point missing mandatory attribute\n");
  else
    {
      category = POLYGON;
      if (!strcmp (attr, "npts"))
	type = POLYGON_NPTS;
      else if (!strcmp (attr, "Symmetry"))
	type = SYMMETRY;
      else if (!strcmp (attr, "Reflexion"))
	type = REFLEXION;
      else if (!strcmp (attr, "Translation"))
	type = TRANSLATION;
      else if (!strcmp (attr, "Scale"))
	type = SCALE;
      else if (!strcmp (attr, "Rotation"))
	type = ROTATION;

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

  if (type & TRANSFORMATION)
    numberVertex = ((polygon *) getFirst)->getNumberVertex ();
  else
    numberVertex = g_list_length (parentList);

  // The number of vertex does not change during the all life of a vertex
  vertices = new drgeoPoint[numberVertex];

  initName ();
}

polygon::~polygon ()
{
  delete[]vertices;
}

void
polygon::update (drgeoDrawable & area)
{
  gint nb;
  drgeoPoint c, *prior;
  drgeoVector d;
  gdouble k;

  exist = parentExist ();
  if (!exist)
    return;
  if (type & TRANSFORMATION)
    {
      prior = ((polygon *) getFirst)->getVertices ();
      switch (type)
	{
	case ROTATION:
	  c = ((point *) getN (1))->getCoordinate ();
	  k = ((value *) getN (2))->getValue ();
	  for (nb = 0; nb < numberVertex; nb++)
	    vertices[nb] = prior[nb].rotatePoint (c, k);
	  break;
	case SYMMETRY:
	  c = ((point *) getN (1))->getCoordinate ();
	  for (nb = 0; nb < numberVertex; nb++)
	    vertices[nb] = 2 * c - prior[nb];
	  break;
	case REFLEXION:
	  d = ((direction *) getN (1))->getDirection ();
	  c = ((direction *) getN (1))->getOrigin ();
	  for (nb = 0; nb < numberVertex; nb++)
	    vertices[nb] = prior[nb].reflexionPoint (c, d);
	  break;
	case TRANSLATION:
	  d = ((direction *) getN (1))->getDirection ();
	  for (nb = 0; nb < numberVertex; nb++)
	    vertices[nb] = prior[nb] + d;
	  break;
	case SCALE:
	  c = ((point *) getN (1))->getCoordinate ();
	  k = ((value *) getN (2))->getValue ();
	  for (nb = 0; nb < numberVertex; nb++)
	    vertices[nb] = (prior[nb] - c) * k + c;
	  break;
	}
    }
  else
    {
      for (nb = 0; nb < numberVertex; nb++)
	vertices[nb] = ((point *) getN (nb))->getCoordinate ();
    }
}

void
polygon::move (drgeoVector & t)
{
  GList *parent;
  switch (type)
    {
    case POLYGON_NPTS:
      parent = g_list_first (parentList);
      while (parent != NULL)
	{
	  ((geometricObject *) parent->data)->move (t);
	  parent = g_list_next (parent);
	}
      break;
    }
}

void
polygon::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "polygon", NULL);

  switch (type)
    {
    case POLYGON_NPTS:
      save_attribute (item, this, "npts");
      break;
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
    }
  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}

void
polygon::updateDescription ()
{
  gint nb;
  geometricObject::updateDescription ();

  switch (type)
    {
    case POLYGON_NPTS:
      description = new gchar *[g_list_length (parentList) + 1];
      description[0] = g_strdup_printf (_("Polygon:: %s"), name);
      for (nb = 0; nb < g_list_length (parentList); nb++)
	description[nb + 1] = g_strdup_printf (_("Point %s"),
					       getNthName (nb));
      break;
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate polygon:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed polygon %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric polygon:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed polygon %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric polygon:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed polygon %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate polygon:: %s"), name);
      description[1] = g_strdup_printf (_("Translated polygon %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale polygon:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled polygon %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    }
}

void
polygon::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  area.drawPolygon (style, vertices, numberVertex);
}

gboolean
polygon::overObject (drgeoPoint & mouse, gdouble range)
{
  drgeoVector u;
  drgeoPoint origin;
  gint nb;

  if (!exist)
    return FALSE;
  for (nb = 0; nb < numberVertex - 1; nb++)
    {
      origin = vertices[nb];
      u = vertices[nb + 1] - origin;
      if (ABS (u ^ (mouse - origin)) / u.norm () <= range
	  && ((mouse - origin) * u) >= 0
	  && ((mouse - origin) * u) <= u.squareNorm ())
	return TRUE;
    }

  // Check with the segment formed by the first and last point
  origin = vertices[0];
  u = vertices[numberVertex - 1] - origin;
  if (ABS (u ^ (mouse - origin)) / u.norm () <= range
      && ((mouse - origin) * u) >= 0
      && ((mouse - origin) * u) <= u.squareNorm ())
    return TRUE;
  return FALSE;
}

void
polygon::initName ()
{
  int l, nb;
  gboolean autoName = TRUE;


  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this polygon %1")) + 1;

  nb = g_list_length (parentList);
  while (nb-- && autoName)
    if (!getNthName (nb))
      autoName = FALSE;
    else if (strlen (getNthName (nb)) == 0)
      autoName = FALSE;
  if (autoName)
    {
      gchar *tmp;
      g_free (name);
      name = g_strdup ("");
      for (nb = 0; nb < g_list_length (parentList); nb++)
	{
	  tmp = g_strconcat (name, getNthName (nb), NULL);
	  g_free (name);
	  name = tmp;
	}
    }

  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this polygon %1"));
  strinsmsg (typeName, name, "%1");
}

drgeoPoint *
polygon::getVertices ()
{
  return vertices;
}

gint
polygon::getNumberVertex ()
{
  return numberVertex;
}
