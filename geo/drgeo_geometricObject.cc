/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Free Software Foundation  2000-2002
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
#include <string.h>

#include "drgeo_geometricObject.h"
#include "liste_elem.h"
#include "drgenius_config.h"

static char *colorName[DRGEO_NUMBER_COLOR] = {
  "Black", "DarkGrey", "Grey", "White", "DarkGreen", "Green", "DarkBlue",
    "Blue",
  "Bordeaux", "Red", "Orange", "Yellow"
};

static char *thicknessName[3] = { "Dashed", "Normal", "Thick" };
static char *styleName[5] =
  { "Round", "Cross", "Rectangular", "RoundEmpty", "RectangularEmpty" };

geometricObject::
geometricObject (gboolean createdFromMacro, liste_elem * figureList)
{
  parentList = NULL;
  typeName = name = NULL;
  description = NULL;
  type = NO_TYPE;

  if (createdFromMacro)
    style.mask = alway;
  else
    style.mask = no;
  style.color = drgeoColorBlack;
  stick = FALSE;
  exist = TRUE;

  this->figureList = figureList;

  /* there reference frame by default is the world system */
  R = NULL;
}

geometricObject::geometricObject (xmlNodePtr tree, liste_elem * figureList)
{
  gchar *attr;
  /* set defaut value of the object */
  parentList = NULL;
  typeName = name = NULL;
  description = NULL;
  type = NO_TYPE;
  stick = FALSE;
  exist = TRUE;
  this->figureList = figureList;
  /* there reference frame by default is the world system */
  R = NULL;

  /* adjust color */
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "color");
  if (attr != NULL)
    {
      if (!strcmp (attr, "Black"))
	style.color = drgeoColorBlack;
      else if (!strcmp (attr, "DarkGrey"))
	style.color = drgeoColorDarkGrey;
      else if (!strcmp (attr, "Grey"))
	style.color = drgeoColorGrey;
      else if (!strcmp (attr, "White"))
	style.color = drgeoColorWhite;
      else if (!strcmp (attr, "DarkGreen"))
	style.color = drgeoColorDarkGreen;
      else if (!strcmp (attr, "Green"))
	style.color = drgeoColorGreen;
      else if (!strcmp (attr, "DarkBlue"))
	style.color = drgeoColorDarkBlue;
      else if (!strcmp (attr, "Blue"))
	style.color = drgeoColorBlue;
      else if (!strcmp (attr, "Bordeaux"))
	style.color = drgeoColorBordeaux;
      else if (!strcmp (attr, "Red"))
	style.color = drgeoColorRed;
      else if (!strcmp (attr, "Orange"))
	style.color = drgeoColorOrange;
      else if (!strcmp (attr, "Yellow"))
	style.color = drgeoColorYellow;
      g_free (attr);
    }

  /* adjust thickness */
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "thickness");
  if (attr != NULL)
    {
      if (!strcmp (attr, "Invisible"))	/*FIXME: obselete */
	style.mask = yes;
      else if (!strcmp (attr, "Dashed"))
	style.thick = drgeoThicknessDashed;
      else if (!strcmp (attr, "Normal"))
	style.thick = drgeoThicknessNormal;
      else if (!strcmp (attr, "Thick"))
	style.thick = drgeoThicknessLarge;
      g_free (attr);
    }

  /* adjust shape - style */
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "style");
  if (attr != NULL)
    {
      if (!strcmp (attr, "Round"))
	style.pointShape = drgeoPointRound;
      else if (!strcmp (attr, "Cross"))
	style.pointShape = drgeoPointX;
      else if (!strcmp (attr, "Rectangular"))
	style.pointShape = drgeoPointRec;
      else if (!strcmp (attr, "RectangularEmpty"))
	style.pointShape = drgeoPointRecEmpty;
      else if (!strcmp (attr, "RoundEmpty"))
	style.pointShape = drgeoPointRoundEmpty;
      g_free (attr);
    }

  /* adjust filled attribute */
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "filled");
  if (attr != NULL)
    {
      if (!strcmp (attr, "True"))
	style.fill = TRUE;
      else if (!strcmp (attr, "False"))
	style.fill = FALSE;
      g_free (attr);
    }

  /* adjust mask attribute */
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "masked");
  if (attr != NULL)
    {
      if (!strcmp (attr, "True"))
	style.mask = yes;
      else if (!strcmp (attr, "False"))
	style.mask = no;
      else if (!strcmp (attr, "Alway"))
	style.mask = alway;
      g_free (attr);
    }

  /* adjust the name */
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "name");
  if (attr != NULL)
    {
      if (strlen (attr) != 0)
	name = g_strdup (attr);
      g_free (attr);
    }
}


geometricObject::~geometricObject ()
{
  gint nb;

  // free the description
  updateDescription ();
  g_list_free (parentList);
  g_free (name);
  g_free (typeName);
}

gboolean
geometricObject::dependOn (geometricObject * object)
{
  GList *tmp;
  tmp = g_list_first (parentList);
  /* First, check for direct dependence */
  while (tmp)
    {
      if ((geometricObject *) tmp->data == object)
	return TRUE;
      tmp = g_list_next (tmp);
    }

  /* Next, go up in the parent items to check for relative
     dependence */
  tmp = g_list_first (parentList);
  while (tmp)
    {
      if (((geometricObject *) tmp->data)->dependOn (object))
	return TRUE;
      tmp = g_list_next (tmp);
    }
  /* we get there, no dependence at all */
  return FALSE;
}

int
geometricObject::parents (liste_elem & parent)
{
  GList *tmp;
  tmp = g_list_first (parentList);
  parent.vide ();
  while (tmp != NULL)
    {
      parent.ajoute ((void *) tmp->data);
      tmp = g_list_next (tmp);
    }
  return parent.nb_elem;
}

void
geometricObject::save (xmlNodePtr tree, liste_elem & figureList)
{

}

void
geometricObject::updateDescription ()
{
  gint nb;

  if (description == NULL)
    return;
  nb = g_list_length (parentList) + 1;
  while (nb--)
    g_free (description[nb]);
  delete[]description;
  description = NULL;
}

gchar **
geometricObject::getDescription ()
{
  return description;
}

gboolean
geometricObject::parentExist ()
{
  GList *parent;
  parent = g_list_first (parentList);
  while (parent != NULL)
    {
      if (!((geometricObject *) parent->data)->objectExist ())
	return FALSE;
      parent = g_list_next (parent);
    }
  return TRUE;
}

gboolean
geometricObject::objectExist ()
{
  return exist;
}

objectCategory
geometricObject::getCategory ()
{
  return category;
}

gchar *
geometricObject::getName ()
{
  return name;
}

void
geometricObject::setName (gchar * name)
{
  g_free (this->name);
  if (name == NULL)
    this->name = NULL;
  else
    this->name = g_strdup (name);
}

gchar *
geometricObject::getTypeName ()
{
  return typeName;
}

gint
geometricObject::getType ()
{
  return type;
}

void *
geometricObject::getExtra ()
{
  return NULL;
}

GList *
geometricObject::getParentList ()
{
  return parentList;
}

repere *
geometricObject::getRepere ()
{
  return R;
}

void
geometricObject::setRepere (repere * R)
{
  this->R = R;
}

drgeoStyle & geometricObject::getStyle ()
{
  return style;
}

masked
geometricObject::getMask ()
{
  return style.mask;
}

void
geometricObject::setMask (masked mask)
{
  style.mask = mask;
}

drgeoColorType
geometricObject::preferedColor (gchar *attr)
{
  gchar *str;  

  str = drgeoConfigGet (attr);
  if (!strcmp (str, "Black"))
    return drgeoColorBlack;
  else if (!strcmp (str, "Darkgrey"))
    return drgeoColorDarkGrey;
  else if (!strcmp (str, "Grey"))
    return drgeoColorGrey;
  else if (!strcmp (str, "White"))
    return drgeoColorWhite;
  else if (!strcmp (str, "DarkGreen"))
    return drgeoColorDarkGreen;
  else if (!strcmp (str, "Green"))
    return drgeoColorGreen;
  else if (!strcmp (str, "Darkblue"))
    return drgeoColorDarkBlue;
  else if (!strcmp (str, "Blue"))
    return drgeoColorBlue;
  else if (!strcmp (str, "Bordeaux"))
    return drgeoColorBordeaux;
  else if (!strcmp (str, "Red"))
    return drgeoColorRed;
  else if (!strcmp (str, "Orange"))
    return drgeoColorOrange;
  else if (!strcmp (str, "Yellow"))
    return drgeoColorYellow;
  else
    return drgeoColorBlack;
}

drgeoThicknessType
geometricObject::preferedThickness (gchar *attr)
{
  gchar *str;  

  str = drgeoConfigGet (attr);
  if (!strcmp (str, "Small"))
    return drgeoThicknessDashed;
  else if (!strcmp (str, "Normal"))
    return drgeoThicknessNormal;
  else if (!strcmp (str, "Large"))
    return drgeoThicknessLarge;
}

gboolean
overLine (drgeoPoint & at, drgeoPoint & p, drgeoVector & v, gdouble range)
{
  if (p == at)
    return TRUE;
  if (v.getX () == 0 && v.getY () == 0)
    return FALSE;
  return (ABS ((p - at) ^ v)) < range;
}


/* 
   this function insert the message 'msg' at the insertion point mark
   in the string 'dest' */
char *
strinsmsg (gchar *dest, gchar *msg, gchar *mark)
{
  gchar *ins;
  // find the mark in dest
  ins = strstr (dest, mark);
  if (ins == NULL)
    return dest;
  // remove the mark, don't forget the 0 end string mark
  memmove (ins, ins + strlen (mark), 
	   strlen (ins  + strlen (mark)) + 1);
  // we can insert the message msg
  if (msg)
    {
      memmove (ins + strlen (msg), ins, strlen (ins) + 1);
      strncpy (ins, msg, strlen (msg));
    }
  return dest;
}

GList *
searchForCategory (GList * list, objectCategory category)
{
  /* search for the next object belonging to category */
  /* Return NULL if not find */
  while (list != NULL)
    {
      if (((geometricObject *) list->data)->getCategory () & category)
	break;
      list = g_list_next (list);
    }
  return list;
}

geometricObject *
searchForCategory (liste_elem & list, objectCategory category)
{
  /* search for the next object belonging to category */
  /* Return NULL if not find */
  geometricObject *item;
  item = (geometricObject *) list.lire (0);
  while (item != NULL)
    {
      if (item->getCategory () & category)
	break;
      item = (geometricObject *) list.lire (0);
    }
  return item;
}

gboolean
sectionCircleLine (drgeoPoint & A, drgeoVector & u,
		   drgeoPoint & C, gdouble r, int s, drgeoPoint & I)
{
  gdouble a, b, c, d, k;

  a = u * u;
  b = u * (A - C);
  c = (A - C).squareNorm () - r * r;
  d = b * b - a * c;
  if (d < 0 || a == 0)
    return FALSE;
  k = (-b + s * sqrt (d)) / a;
  I = A + k * u;
  return TRUE;
}

gboolean
sectionDirectionDirection (drgeoPoint & A, drgeoVector & u,
			   drgeoPoint & B, drgeoVector & v, drgeoPoint & I)
{
  gdouble det, c1, c2;

  det = u ^ v;
  if (det == 0)
    return FALSE;
  c1 = A ^ u;
  c2 = B ^ v;

  I.set ((-c1 * v.getX () + c2 * u.getX ()) / det,
	 (u.getY () * c2 - v.getY () * c1) / det);
  return TRUE;
}

char
save_attribute (xmlNodePtr tree, geometricObject * fig, gchar * type)
{
  gchar *attr;
  int a;


  attr = g_strdup_printf ("%X", GPOINTER_TO_UINT (fig));
  xmlSetProp (tree, BAD_CAST "id", BAD_CAST attr);
  g_free (attr);

  xmlSetProp (tree, BAD_CAST "type", BAD_CAST type);

  a = fig->getStyle ().color;
  if (a < drgeoColorBlack)
    a = drgeoColorBlack;
  if (a > drgeoColorYellow)
    a = drgeoColorYellow;
  xmlSetProp (tree, BAD_CAST "color", BAD_CAST colorName[a]);

  a = fig->getStyle ().thick;
  if (a <= drgeoThicknessDashed)
    a = drgeoThicknessDashed;
  if (a > drgeoThicknessLarge)
    a = drgeoThicknessLarge;
  xmlSetProp (tree, BAD_CAST "thickness", BAD_CAST thicknessName[a]);

  a = fig->getStyle ().pointShape;
  if (a < drgeoPointRound)
    a = drgeoPointRound;
  if (a > drgeoPointRecEmpty)
    a = drgeoPointRecEmpty;
  xmlSetProp (tree, BAD_CAST "style", BAD_CAST styleName[a]);

  xmlSetProp (tree, BAD_CAST "filled",
	      BAD_CAST ((fig->getStyle ().fill) ? "True" : "False"));
  if (fig->getMask () == yes)
    {
      xmlSetProp (tree, BAD_CAST "masked", BAD_CAST "True");
    }
  else if (fig->getMask () == no)
    {
      xmlSetProp (tree, BAD_CAST "masked", BAD_CAST "False");
    }
  else
    {
      xmlSetProp (tree, BAD_CAST "masked", BAD_CAST "Alway");
    }
  xmlSetProp (tree, BAD_CAST "name", BAD_CAST fig->getName ());
  return TRUE;
}

void
xmlAddParent (xmlNodePtr tree, geometricObject * fig)
{
  gchar *attr;
  xmlNodePtr subtree;

  subtree = xmlNewChild (tree, NULL, BAD_CAST "parent", NULL);
  attr = g_strdup_printf ("%X", GPOINTER_TO_UINT (fig));
  xmlSetProp (subtree, BAD_CAST "ref", BAD_CAST attr);
  g_free (attr);
}

void *
xmlInsertParent (xmlNodePtr parent, GHashTable * itemIdToAddress)
{
  gchar *id;
  void *parentAddress;

  if (parent == NULL)
    return NULL;

  id = (char *) xmlGetProp (parent, BAD_CAST "ref");
  if (id == NULL)
    {
      fprintf (stderr,
	       "xmlInsertParent : can't find ref attribute in parent element\n");
      return NULL;
    }
  parentAddress = g_hash_table_lookup (itemIdToAddress, id);
  if (parentAddress == NULL)
    fprintf (stderr,
	     "xmlInsertParent : can't find the key %s in the hash table\n",
	     id);
  g_free (id);
  return parentAddress;
}
