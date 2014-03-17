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
#include "drgeo_point.h"
#include "drgeo_direction.h"
#include "drgeo_value.h"
#include "drgeo_repere.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_drawable.h"
#include "drgenius_config.h"

point::point (drgeoPoint & p, liste_elem & parents, pointType type, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  style.color = preferedColor (":pointColor");
  style.pointShape = preferedShape (":pointShape");
  style.thick = preferedThickness (":pointSize");
  this->type = (gint) type;
  switch (type)
    {
    case FREE:
      this->p = p;
      category = FREE_PT;
      break;
    case ON_CURVE:
      category = FREE_PT_ON_CURVE;
      parents.init_lire ();
      /* CURVE */
      parentList = g_list_append (parentList, (gpointer) parents.lire (0));
      this->p = ((geometricObject *) getFirst)->getClosestPoint (p);
      abscissa = ((geometricObject *) getFirst)->getAbscissa (p);
      break;
    default:
      /* we should no be there */
      break;
    }
  initName ();
}

point::point (gdouble a, liste_elem & parents, 
	      gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  style.color = preferedColor (":pointColor");
  style.pointShape = preferedShape (":pointShape");
  style.thick = preferedThickness (":pointSize");
  type = (gint) ON_CURVE;
  category = FREE_PT_ON_CURVE;
  parents.init_lire ();
  parentList = g_list_append (parentList, (gpointer) parents.lire (0));
  /* normalize it */
  if (a < 0)
    a = 0;
  else if (a > 1)
    a = 1;
  abscissa = a;
  p = ((geometricObject *) getFirst)-> getPointAt (abscissa);  
    initName ();
}


point::point (liste_elem & parents, pointType type, gboolean createdFromMacro, liste_elem * figureList, gint k):
geometricObject (createdFromMacro, figureList)
{
  style.color = preferedColor (":pointColor");
  style.pointShape = preferedShape (":pointShape");
  style.thick = preferedThickness (":pointSize");
  this->type = (gint) type;
  category = CONSTRAINED_PT;
  extra = k;
  switch (type)
    {
    case ROTATION:
    case SCALE:
      /* the object parentList will contain in this order
         POINT (point to be transformed) - POINT - VALUE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parentList =
	g_list_append (parentList,
		       (gpointer) searchForCategory (parents, POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VALUE));
      break;
    case SYMMETRY:
      /* POINT - POINT */
    case COORDINATE:
      /* VALUE - VALUE */
    case INTERSECTION:
      /* CURVE - CURVE */
    case MIDDLE_2PTS:
      /* POINT - POINT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (1));
      parentList = g_list_append (parentList, (gpointer) parents.lire (2));
      break;
    case REFLEXION:
      /* POINT - LINE */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								LINE));
      break;
    case TRANSLATION:
      /* POINT - VECTOR */
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								POINT));
      parents.init_lire ();
      parentList = g_list_append (parentList,
				  (gpointer) searchForCategory (parents,
								VECTOR));
      break;
    case MIDDLE_SEGMENT:
      /* SEGMENT */
      parentList = g_list_append (parentList, (gpointer) parents.lire (1));
      break;
    }
  initName ();
}

point::point (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
geometricObject (tree, figureList)
{
  gchar *attr;
  xmlNodePtr item;
  void *obj;
  double val;

  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("point::point missing mandatory attribute\n");
  else
    {
      category = CONSTRAINED_PT;
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
      else if (!strcmp (attr, "Coordinate"))
	type = COORDINATE;
      else if (!strcmp (attr, "Free"))
	{
	  type = FREE;
	  category = FREE_PT;
	  val = 0;
	  xml_get_value_double (tree, "x", &val);
	  p.setX (val);
	  val = 0;
	  xml_get_value_double (tree, "y", &val);
	  p.setY (val);
	}
      else if (!strcmp (attr, "On_curve"))
	{
	  type = ON_CURVE;
	  category = FREE_PT_ON_CURVE;
	  abscissa = 0;		/* be nice with fool */
	  xml_get_value_double (tree, "value", &abscissa);
	}
      else if (!strcmp (attr, "Intersection"))
	{
	  type = INTERSECTION;
	  xml_get_value_int (tree, "extra", &extra);
	}
      else if (!strcmp (attr, "Middle_segment"))
	type = MIDDLE_SEGMENT;
      else if (!strcmp (attr, "Middle_2pts"))
	type = MIDDLE_2PTS;
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
point::update (drgeoDrawable & area)
{
  exist = parentExist ();
  if (!exist)
    return;
  if (type & TRANSFORMATION)
    p = ((point *) getFirst)->getCoordinate ();
  switch (type)
    {
    case ROTATION:
      p = p.rotatePoint (((point *) getN (1))->getCoordinate (),
			 ((value *) getN (2))->getValue ());

      break;
    case SYMMETRY:
      p = 2 * ((point *) getN (1))->getCoordinate () - p;
      break;
    case REFLEXION:
      p = p.reflexionPoint (((direction *) getN (1))->getOrigin (),
			    ((direction *) getN (1))->getDirection ());
      break;
    case TRANSLATION:
      p += ((direction *) getN (1))->getDirection ();
      break;
    case SCALE:
      p = (p - ((point *) getN (1))->getCoordinate ())
	* ((value *) getLast)->getValue ()
	+ ((point *) getN (1))->getCoordinate ();
      break;
    case COORDINATE:
      p.set (((value *) getFirst)->getValue (),
	     ((value *) getLast)->getValue ());
      break;
    case FREE:
      break;
    case ON_CURVE:
      p = ((geometricObject *) getFirst)->getPointAt (abscissa);
      break;
    case INTERSECTION:
      exist =
	((geometricObject *) getFirst)->
	getIntersection (*((geometricObject *) getLast), p, extra);
      break;
    case MIDDLE_SEGMENT:
      p = ((direction *) getFirst)->getOrigin () +
	((direction *) getFirst)->getDirection () / 2;
      break;
    case MIDDLE_2PTS:
      p = (((point *) getFirst)->getCoordinate ()
	   + ((point *) getLast)->getCoordinate ()) / 2;
      break;
    }
}

void
point::move (drgeoVector & t)
{
  switch (type)
    {
    case FREE:
      p += t;
      break;
    case ON_CURVE:
      p += t;
      p = ((geometricObject *) getFirst)->getClosestPoint (p);
      abscissa = ((geometricObject *) getFirst)->getAbscissa (p);
      break;
    default:
      /* don't move the point */
      break;
    }
}

void
point::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;
  gchar *content;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "point", NULL);

  if (type != FREE && type != ON_CURVE)
    {
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
	case COORDINATE:
	  save_attribute (item, this, "Coordinate");
	  break;
	case INTERSECTION:
	  save_attribute (item, this, "Intersection");
	  /* save the extra attribute */
	  content = g_strdup_printf ("%d", extra);
	  xmlSetProp (item, BAD_CAST "extra", BAD_CAST content);
	  g_free (content);
	  break;
	case MIDDLE_SEGMENT:
	  save_attribute (item, this, "Middle_segment");
	  break;
	case MIDDLE_2PTS:
	  save_attribute (item, this, "Middle_2pts");
	  break;
	}
      while (parent)
	{
	  xmlAddParent (item, (geometricObject *) parent->data);
	  parent = g_list_next (parent);
	}
    }
  else
    {
      xmlNodePtr subitem;
      if (type == FREE)
	{
	  save_attribute (item, this, "Free");
	  content = g_strdup_printf ("%lf", p.getX ());
	  subitem = xmlNewChild (item, NULL, BAD_CAST "x", BAD_CAST content);
	  g_free (content);
	  content = g_strdup_printf ("%lf", p.getY ());
	  subitem = xmlNewChild (item, NULL, BAD_CAST "y", BAD_CAST content);
	  g_free (content);
	}
      else
	{
	  save_attribute (item, this, "On_curve");
	  content = g_strdup_printf ("%lf", abscissa);
	  subitem =
	    xmlNewChild (item, NULL, BAD_CAST "value", BAD_CAST content);
	  g_free (content);
	  xmlAddParent (item, (geometricObject *) parent->data);
	}
    }
}

void
point::updateDescription ()
{
  geometricObject::updateDescription ();

  switch (type)
    {
    case ROTATION:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Rotate point:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed point %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Angle %s"), getNthName (2));
      break;
    case SYMMETRY:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric point:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed point %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      break;
    case REFLEXION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Symmetric point:: %s"), name);
      description[1] = g_strdup_printf (_("Transformed point %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Reflexion axe %s"),
					getNthName (1));
      break;
    case TRANSLATION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Translate point:: %s"), name);
      description[1] = g_strdup_printf (_("Translated point %s"),
					getNthName (0));
      description[2] = g_strdup_printf (_("Vector of translation %s"),
					getNthName (1));
      break;
    case SCALE:
      description = new gchar *[4];
      description[0] = g_strdup_printf (_("Scale point:: %s"), name);
      description[1] = g_strdup_printf (_("Scaled point %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Center %s"), getNthName (1));
      description[3] = g_strdup_printf (_("Factor %s"), getNthName (2));
      break;
    case COORDINATE:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Coordinate point:: %s"), name);
      description[1] = g_strdup_printf (_("Abscissa %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Ordinate %s"), getNthName (1));
      break;
    case FREE:
      description = new gchar *[1];
      description[0] = g_strdup_printf (_("Free point:: %s"), name);
      break;
    case ON_CURVE:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Free point on curve:: %s"), name);
      description[1] = g_strdup_printf (_("Curve %s"), getNthName (0));
      break;
    case INTERSECTION:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Intersection point:: %s"), name);
      description[1] = g_strdup_printf (_("Curve %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Curve %s"), getNthName (1));
      break;
    case MIDDLE_SEGMENT:
      description = new gchar *[2];
      description[0] = g_strdup_printf (_("Middle of segment:: %s"), name);
      description[1] = g_strdup_printf (_("Segment %s"), getNthName (0));
      break;
    case MIDDLE_2PTS:
      description = new gchar *[3];
      description[0] = g_strdup_printf (_("Middle of two points:: %s"), name);
      description[1] = g_strdup_printf (_("Point %s"), getNthName (0));
      description[2] = g_strdup_printf (_("Point %s"), getNthName (1));
      break;
    }
}

void
point::draw (drgeoDrawable & area, char force)
{
  drgeoVector t (area.pixelToWorld (5), area.pixelToWorld (5));

  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  area.drawPoint (style, p);
  area.drawText (p + t, name, drgeoColorBlack);
}

gboolean
point::overObject (drgeoPoint & mouse, gdouble range)
{
  drgeoVector n;
  if (!exist)
    return FALSE;
  if ((p - mouse).norm () >= range)
    return FALSE;
  return TRUE;
}

void
point::initName ()
{
  int l;
  if (style.mask == alway)
    return;
  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this point %1")) + 1;
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this point %1"));
  strinsmsg (typeName, name, "%1");
}

drgeoPoint & point::getCoordinate ()
{
  return p;
}

void
point::setCoordinate (drgeoPoint & p)
{
  this->p = p;
}


void
point::setAbscissa (gdouble abscissa)
{
  this->abscissa = abscissa;
  if (this->abscissa < 0)
    this->abscissa = 0;
  else if (this->abscissa > 1)
    this->abscissa = 1;
}

gdouble
point::getAbscissa ()
{
  return abscissa;
}

void *
point::getExtra ()
{
  void *p;
  switch (type)
    {
    case INTERSECTION:
      p = g_malloc (sizeof (int));
      * (int *) p = extra;
      break;
    case ON_CURVE:
      p = g_malloc (sizeof (double));
      * (double *) p = getAbscissa ();
      break;
    case FREE:
      p = g_malloc (2 * sizeof(double));
      * (double *) p = getCoordinate().getX();
      * (((double *) p) + 1) = getCoordinate().getY();
      break;
    default:
      p = NULL;
    }
  return p; 
}

drgeoPointType
point::preferedShape (gchar *attr)
{
  gchar *str;  

  str = drgeoConfigGet (attr);
  if (!strcmp (str, "Round"))
    return drgeoPointRound;
  else if (!strcmp (str, "Cross"))
    return drgeoPointX;
  else if (!strcmp (str, "Squarre"))
    return drgeoPointRec;
  else if (!strcmp (str, "RoundEmpty"))
    return drgeoPointRoundEmpty;
  else if (!strcmp (str, "SquarreEmpty"))
    return drgeoPointRecEmpty;
}

