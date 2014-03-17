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

#ifndef DRGEO_GEOMETRICOBJECT_H
#define DRGEO_GEOMETRICOBJECT_H

#include <gtk/gtk.h>
#include <libintl.h>
#include "xml.h"
#include "drgeo_drgeoVector.h"
#include "drgeo_drgeoStyle.h"
#include "define.h"

/* Some macro to ease the acces to parameters */
#define getFirst (g_list_first (parentList)->data)
#define getN(n) g_list_nth_data (parentList, n)
#define getLast (g_list_last (parentList)->data)
#define getNthName(n) (((geometricObject *)  g_list_nth_data (parentList, n))->getName ())

class liste_elem;
class drgeoDrawable;
class repere;

class geometricObject
{
  friend class list_elem;
public:
  geometricObject (gboolean createdFromMacro, liste_elem * figureList);
  geometricObject (xmlNodePtr tree, liste_elem * figureList);
  virtual ~ geometricObject ();
  virtual void draw (drgeoDrawable & area, char force) = 0;
  virtual void update (drgeoDrawable & area) = 0;
  virtual gboolean overObject (drgeoPoint & mouse, gdouble range) = 0;
  virtual void move (drgeoVector & t) = 0;
  gboolean dependOn (geometricObject * object);
  int parents (liste_elem & parent);
  virtual void initName () = 0;
  virtual void save (xmlNodePtr tree, liste_elem & figureList) = 0;
  gchar *getName ();
  void setName (gchar * name);
  gchar *getTypeName ();
  virtual void updateDescription ();
  gchar **getDescription ();
  gboolean parentExist ();
  gboolean objectExist ();
  objectCategory getCategory ();
  gint getType ();
  virtual void * getExtra ();
  GList *getParentList ();
  repere *getRepere ();
  void setRepere (repere * R);
  drgeoStyle & getStyle ();
  masked getMask ();
  void setMask (masked mask);
  drgeoColorType preferedColor (gchar *attr);
  drgeoThicknessType preferedThickness (gchar *attr);
  /* FIXME: durty 
     virtual function for curve object */
  virtual drgeoPoint & getPointAt (gdouble abscissa)
  {
  };
  virtual drgeoPoint & getClosestPoint (drgeoPoint & p)
  {
  };
  virtual gdouble getAbscissa (drgeoPoint & p)
  {
  };
  virtual gboolean getIntersection (geometricObject & c,
				    drgeoPoint & p, gint k)
  {
  };
protected:
  liste_elem * figureList;
  drgeoStyle style;		/* the display style */
  /* parent of the object, and specific data list */
  GList *parentList;
  /* generic name ("this point %1", etc.) and the name ("A") 
     so we get "this point A" in typeName */
  gchar *typeName, *name;
  /* The description of the object:
     . 1st string is the description of the category of the object itself
     . the following descriptions are the category name of its parents
     (in the same other as in parentList) */
  gchar **description;
  objectCategory category;
  gint type;
  /* does object exist */
  gboolean exist;
  /* is object sticked  */
  gboolean stick;
  /* the reference frame to which belong this object */
  repere *R;
};

gboolean overLine (drgeoPoint & at, drgeoPoint & p, drgeoVector & v,
		   gdouble range);
GList *searchForCategory (GList * list, objectCategory category);
geometricObject *searchForCategory (liste_elem & list,
				    objectCategory category);
char *strinsmsg (gchar *dest, gchar *msg, gchar *mark);

gboolean sectionCircleLine (drgeoPoint & A, drgeoVector & u,
			    drgeoPoint & C, gdouble r, int s, drgeoPoint & I);
gboolean sectionDirectionDirection (drgeoPoint & A, drgeoVector & u,
				    drgeoPoint & B, drgeoVector & v,
				    drgeoPoint & I);

char save_attribute (xmlNodePtr tree, geometricObject * fig, gchar * type);
void xmlAddParent (xmlNodePtr tree, geometricObject * fig);
void *xmlInsertParent (xmlNodePtr parent, GHashTable * itemIdToAddress);


#endif /* DRGEO_GEOMETRICOBJECT_H */
