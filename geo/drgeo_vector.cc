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

#include <math.h>

#include "config.h"
#include "drgeo_vector.h"
#include "drgeo_repere.h"
#include "drgeo_point.h"
#include "liste_elem.h"
#include "drgeo_drawable.h"

vector::vector (liste_elem & parents, vectorType type, gboolean createdFromMacro, liste_elem * figureList):
direction (parents, (transformationType) type, createdFromMacro, figureList)
{
  style.color = preferedColor (":vectorColor");
  style.thick = preferedThickness (":vectorStyle");

  this->type = (gint) type;
  category = VECTOR;
  switch (type)
    {
    case VECTOR_2PTS:
      /* POINT - POINT */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parentList =
	g_list_append (parentList,
		       (gpointer) searchForCategory (parents, POINT));
      origin = ((point *) getFirst)->getCoordinate ();
      break;
    default:
      /* orgin of a transformed vector, set only once */
      origin = ((vector *) getFirst)->getOrigin ();
    }
  initName ();
}

vector::vector (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
direction (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;
  gdouble val = 0;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("vector::vector missing mandatory attribute\n");
  else
    {
      category = VECTOR;
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
	type = VECTOR_2PTS;
      /* get the origin of the vector representer */
      xml_get_value_double (tree, "Ox", &val);
      origin.setX (val);
      xml_get_value_double (tree, "Oy", &val);
      origin.setY (val);
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
vector::draw (drgeoDrawable & area, char force)
{
  static drgeoVector v;
  static drgeoPoint p;
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  p = origin + u;
  area.drawSegment (style, origin, p);

  v = -area.pixelToWorld (10) * u / u.norm ();
  area.drawSegment (style, p, p + v.rotate (M_PI / 7));
  area.drawSegment (style, p, p + v.rotate (-M_PI / 7));
}

void
vector::update (drgeoDrawable & area)
{
  exist = parentExist ();
  if (!exist)
    return;
  if ((type & TRANSFORMATION) != 0)
    direction::update (area);
  else
    {
      /* VECTOR_2PTS */
      u = ((point *) g_list_nth_data (parentList, 1))->getCoordinate () -
	((point *) g_list_nth_data (parentList, 0))->getCoordinate ();
    }
  if (u.getX () == 0 && u.getY () == 0)
    exist = FALSE;
}

gboolean
vector::overObject (drgeoPoint & mouse, gdouble range)
{
  drgeoPoint m;

  if (!exist)
    return FALSE;
  m = mouse - origin;
  return (ABS (u ^ m) / u.norm () <= range
	  && m * u >= 0 && m * u <= u.squareNorm ());
}

void
vector::move (drgeoVector & t)
{
  /* a vector does'nt depend on any position, 
     so it is freely moveable */
  origin += t;
}

void
vector::initName ()
{
  int l;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this vector %1")) + 1;
  switch (type)
    {
    case VECTOR_2PTS:
      geometricObject * item0, *item1;
      item0 = (geometricObject *) getFirst;
      item1 = (geometricObject *) getLast;
      if (!item0->getName () || !item1->getName ())
	break;
      if (strlen (item0->getName ()) == 0 || strlen (item1->getName ()) == 0)
	break;
      g_free (name);
      name = g_strdup_printf ("%s%s", item0->getName (), item1->getName ());
      break;
    }
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this vector %1"));
  strinsmsg (typeName, name, "%1");
}

void
vector::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item, subitem;
  GList *parent;
  gchar *content;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "vector", NULL);
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
    case VECTOR_2PTS:
      save_attribute (item, this, "2pts");
      break;
    }

  content = g_strdup_printf ("%lf", origin.getX ());
  subitem = xmlNewChild (item, NULL, BAD_CAST "Ox", BAD_CAST content);
  g_free (content);
  content = g_strdup_printf ("%lf", origin.getY ());
  subitem = xmlNewChild (item, NULL, BAD_CAST "Oy", BAD_CAST content);
  g_free (content);

  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}
void
vector::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate vector:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed vector %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric vector:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed vector %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric vector:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed vector %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate vector:: %s"), name);
      description[1] = g_strdup_printf (_("Translated vector %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale vector:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled vector %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    case VECTOR_2PTS:
      description = new gchar *[3];
      description[0] =
	g_strdup_printf (_("Vector defined by two points:: %s"), name);
      description[1] = g_strdup_printf (_("Origin %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Extremity %s"), getNthName (1));
      break;
    }
}
