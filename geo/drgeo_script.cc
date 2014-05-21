/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2000-2001
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

#include <libguile.h>
#include "config.h"
#include "drgeo_script.h"
#include "drgeo_drawable.h"
#include "drgeo_scm_helper.h"
#include "drgenius_mdi.h"

extern int numericPrecision;

script::script (drgeoPoint & p, liste_elem & parents, scriptType type, gchar * scriptCode, gboolean createdFromMacro, liste_elem * figureList):
value (p, createdFromMacro, figureList)
{
  gpointer item;

  style.color = preferedColor (":scalarColor");

  category = SCRIPT;
  this->type = (gint) type;
  switch (type)
    {
    case SCRIPT_NITEMS:
      /* parentList contain a list of geometric object, cannot
         be void */
      parents.init_lire ();
      while (item = (gpointer) parents.lire (0))
	parentList = g_list_append (parentList, gpointer (item));
      break;
    }
  this->scriptCode = NULL;
  setScript (scriptCode);
  initName ();
}

script::script (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
value (tree, figureList)
{
  gchar *attr, *tmp;
  xmlNodePtr item;
  void *obj;
  double val;

  this->scriptCode = NULL;
  attr = (gchar *) xmlGetProp (tree, BAD_CAST "type");
  if (!attr)
    /* FIXME: do something there,
       mandatory attribute missing */
    g_printerr ("script::script missing mandatory attribute\n");
  else
    {
      category = SCRIPT;
      if (!strcmp (attr, "nitems"))
	type = SCRIPT_NITEMS;
      /* get the parent ref. */
      item = xml_search_next (tree->xmlChildrenNode, "parent");
      while (item)
	{
	  if ((obj = xmlInsertParent (item, itemIdToAdress)) == NULL)
	    exit (1);		/* FIXME: implement a nicer way */
	  parentList = g_list_append (parentList, obj);
	  item = xml_search_next (item->next, "parent");
	}
      /* get the script code itself */
      tmp = xml_value_get (tree, "code");
      setScript (tmp);
      g_free (tmp);
    }
  val = 0;
  xml_get_value_double (tree, "x", &val);
  p.setX (val);
  val = 0;
  xml_get_value_double (tree, "y", &val);
  p.setY (val);

  g_free (attr);
  initName ();
}

script::~script ()
{
  gchar *tmpScript;

  g_free (scriptCode);

  tmpScript = g_strdup_printf ("(undefine script%d)", this);
  scm_c_eval_string (tmpScript);
  g_free (tmpScript);
  //      scm_gc();
}

void
script::update (drgeoDrawable & area)
{
  SCM ret;
  gchar *tmpScript, *tmp1, *tmp2, *tmp;
  gint nb;

  exist = parentExist ();
  if (!exist)
    return;

  /* build the call sequence */
  tmpScript = g_strdup_printf ("(script%d", this);
  for (nb = 0; nb < g_list_length (parentList); nb++)
    {
      tmp1 = g_strdup_printf (" %d", g_list_nth_data (parentList, nb));
      tmp2 = g_strconcat (tmpScript, tmp1, NULL);
      g_free (tmpScript);
      g_free (tmp1);
      tmpScript = tmp2;
    }
  tmp1 = g_strconcat (tmpScript, ")", NULL);
  g_free (tmpScript);
  ret = scm_internal_catch(SCM_BOOL_T, thunk, (void *)tmp1, standard_handler, NULL);
  g_free (tmp1);
  /* there is no gh_ equivalent in the guile interface,
     therefore I use the internal stuff */
  if (SCM_NFALSEP (scm_real_p (ret)))
    {
      setValue (scm_to_double (ret));
      if (name ? strlen (name) : 0)
	{
	  tmp = g_strdup_printf ("%%s = %%.%df",numericPrecision);      
	  setString (g_strdup_printf (tmp, name, val));
	  g_free (tmp);
	}
      else
	{
	  tmp = g_strdup_printf ("%%.%df",numericPrecision);      
	  setString (g_strdup_printf (tmp, val));
	  g_free (tmp);
	}      
    }
  else if (scm_is_string (ret))
    setString (scm_to_locale_string (ret));
  else
    setString (g_strdup (_("Unprintable result")));
}

void
script::initName ()
{
  int l;

  if (style.mask == alway)
    return;

  if (typeName != NULL)
    g_free (typeName);
  l = strlen (_("this script %1")) + 1;
  if (name)
    l += strlen (name);
  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("this script %1"));
  strinsmsg (typeName, name, "%1");
}

void
script::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item, subitem;
  GList *parent;
  gchar *content;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "script", NULL);
  switch (type)
    {
    case SCRIPT_NITEMS:
      save_attribute (item, this, "nitems");
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
  /* save the script code */
  if (strlen (scriptCode) != 0)
    xmlNewChild (item, NULL, BAD_CAST "code", BAD_CAST (scriptCode));
}

void
script::updateDescription ()
{
  gint nb;
  geometricObject::updateDescription ();

  switch (type)
    {
    case SCRIPT_NITEMS:
      //FIXME: implement a virtual getDescription method for the
      // geometricObject class, cleaner way!!!
      description = new gchar *[g_list_length (parentList) + 1];
      description[0] = g_strdup_printf (_("Script:: %s"), name);
      for (nb = 0; nb < g_list_length (parentList); nb++)
	description[nb + 1] = g_strdup_printf (_("Object %s"),
					       getNthName (nb));

      break;
    }
}

gchar *
script::getScript ()
{
  return scriptCode;
}

void
script::setScript (gchar * scriptCode)
{
  gchar *tmpScript, *tmp1, *tmp2;
  gint nb;

  g_free (this->scriptCode);
  this->scriptCode = g_strdup (scriptCode);

  /* define a scm procedure to hold the user script */
  tmpScript = g_strdup_printf ("(define (script%d", this);
  for (nb = 1; nb <= g_list_length (parentList); nb++)
    {
      tmp1 = g_strdup_printf (" a%d", nb);
      tmp2 = g_strconcat (tmpScript, tmp1, NULL);
      g_free (tmp1);
      g_free (tmpScript);
      tmpScript = tmp2;
    }
  tmp1 = g_strconcat (tmpScript, ")", scriptCode, ")", NULL);
  g_free (tmpScript);
  /* define the procedure in the Guile interpretor */
  scm_internal_catch(SCM_BOOL_T, thunk, (void *)tmp1, standard_handler, NULL);
  g_free (tmp1);
}

void *
script::getExtra ()
{
  return (void *) g_strdup (scriptCode);
}

