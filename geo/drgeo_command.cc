/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2000
 * hilaire@ext.cri74.org 
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

#include "drgeo_command.h"
#include "traite.h"
#include "drgeo_numeric.h"
#include "drgeo_script.h"

static void
replaceParentKey (xmlNodePtr xml, gpointer oldkey, gpointer newkey);

drgeoCommand::drgeoCommand (drgeoFigure * figure)
{
  this->theFigure = figure;
}

drgeoCommand::~drgeoCommand ()
{
}

/* Remove item command */

drgeoRemoveItemCommand::drgeoRemoveItemCommand (drgeoFigure * figure, geometricObject * rmItem):
drgeoCommand (figure)
{
  xmlDocPtr xml;
  geometricObject *item;
  GList *parent;
  gpointer tkey;
  liste_elem *toDelete;

  xmlBackup = NULL;
  key.vide ();

  /* build the xml structure for each item to delete */
  toDelete = theFigure->dependantItems (rmItem);
  toDelete->init_lire ();
  while ((item = (geometricObject *) toDelete->lire (0)) != NULL)
    {
      xml = xmlNewDoc (BAD_CAST "1.0");
      xml->xmlRootNode = xmlNewDocNode (xml, NULL, BAD_CAST "undo", NULL);
      item->save (xml->xmlRootNode, *(theFigure->getFigureList ()));
      xmlBackup = g_list_append (xmlBackup, (gpointer) xml);
      /* insert in the hash table the parents of these items */
      parent = item->getParentList ();
      parent = g_list_first (parent);
      while (parent)
	{
	  /* is the parent already referenced in the hash */
	  tkey = theFigure->getKeyFromValueOnHash ((gpointer) parent->data);
	  if (tkey == NULL)
	    {
	      /* if not, reference it, note: key is then the
	         same as in the xml backup, good. */
	      theFigure->addUndoHash (parent->data, parent->data);
	    }
	  else
	    {
	      /* the key is not the same as in the backup,
	         need to update the xml backup */
	      replaceParentKey (xml->xmlRootNode->xmlChildrenNode->xmlChildrenNode, parent->data,
				tkey);
	    }
	  parent = g_list_next (parent);
	}
      /* we first check if this item is in the hash table,
         in this case we use its key otherwise we create a new
         one */
      tkey = theFigure->getKeyFromValueOnHash ((gpointer) item);
      if (tkey == NULL)
	{
	  tkey = (gpointer) item;
	  /* item not in hash, add it */
	  theFigure->addUndoHash (tkey, tkey);
	}
      key.ajoute (tkey);
    }
  delete (toDelete);
}

drgeoRemoveItemCommand::~drgeoRemoveItemCommand ()
{
  GList *xmlItems;

  xmlItems = g_list_first (xmlBackup);
  while (xmlItems != NULL)
    {
      xmlFreeDoc ((xmlDocPtr) (xmlItems->data));
      xmlItems = g_list_next (xmlItems);
    }
  g_list_free (xmlBackup);
}

void
drgeoRemoveItemCommand::execute ()
{
  gpointer tkey, item;
  /* delete the items */
  key.init_lire ();
  while ((tkey = (key.lire (0))) != NULL)
    {
      item = theFigure->getValueFromKeyOnHash (tkey);
      if (item == NULL)
	g_printerr ("In drgeoRemoveItemCommand::execute: item is NULL\n");
      else
	{
	  theFigure->getFigureList ()->supprime (item);
	  delete ((geometricObject *) item);
	}
    }
  theFigure->getDrawable ()->updateDescription ();
}

void
drgeoRemoveItemCommand::unexecute ()
{
  GList *itemList;
  geometricObject *item;
  gpointer keyItem;

  /* rebuild the items from the xml record */
  itemList = g_list_first (xmlBackup);
  key.init_lire ();

  while (itemList != NULL)
    {
      item =
	theFigure->xmlBuildItem (((xmlDocPtr) (itemList->data))->xmlRootNode->xmlChildrenNode,
				 theFigure->getUndoHash ());
      itemList = g_list_next (itemList);
      /* check if the created item was referenced in the
         hash table (ie it's a parent somewhere in the undo
         pipe), then update its value in the hash table */
      keyItem = (gpointer) key.lire (0);
      theFigure->updateUndoHash (keyItem, (gpointer) item);
      theFigure->addToFigureListNoRedraw (item, TRUE);
      theFigure->getDrawable ()->addDescriptionItem (item);
    }
}


/* Create item command */

drgeoCreateItemCommand::drgeoCreateItemCommand (drgeoFigure * figure, geometricObject * buildItem):
drgeoCommand (figure)
{
  GList *parent;
  gpointer tmpkey;

  key = NULL;
  xml = xmlNewDoc (BAD_CAST "1.0");
  xml->xmlRootNode = xmlNewDocNode (xml, NULL, BAD_CAST "undo", NULL);
  buildItem->save (xml->xmlRootNode, *(theFigure->getFigureList ()));
  /* insert in the hash table the parents of this item */
  parent = buildItem->getParentList ();
  parent = g_list_first (parent);
  while (parent)
    {
      /* is the parent already referenced in the hash */
      tmpkey = theFigure->getKeyFromValueOnHash ((gpointer) parent->data);
      if (tmpkey == NULL)
	{
	  /* if not, reference it, note: key is then the
	     same as in the xml backup, good. */
	  theFigure->addUndoHash (parent->data, parent->data);
	}
      else
	{
	  /* the key is not the same as in the backup,
	     need to update the xml backup */
	  g_printerr ("parent on hash with key %X\n", tmpkey);
	  replaceParentKey (xml->xmlRootNode->xmlChildrenNode->xmlChildrenNode, parent->data, tmpkey);
	}
      parent = g_list_next (parent);
    }
  delete (buildItem);
}

drgeoCreateItemCommand::~drgeoCreateItemCommand ()
{
  xmlFreeDoc (xml);
  /* remove the ref in the hash table of the item created by
     this command */
  /* WE have to keep the key in the hash because of the
     we may need the reference of this key */
//      theFigure->removeKeyFromHash (key);
}

void
drgeoCreateItemCommand::execute ()
{
  geometricObject *item;

  item = theFigure->xmlBuildItem (xml->xmlRootNode->xmlChildrenNode,
				  theFigure->getUndoHash ());
  theFigure->addToFigureListNoRedraw (item, TRUE);
  theFigure->getDrawable ()->addDescriptionItem (item);
  /* update any value matching the previous instantiation
     reference of this item in case it was a parents somewhere
     in the undo/redo pipe */
  if (key == NULL)
    {
      /* this is the first instantiation of this item, we
         need to add the key in the hash, this item
         reference can be altered by an eventual futur
         removeCommand */
      g_printerr ("First instantiation of item with key %X\n", item);
      key = item;
      theFigure->addUndoHash (key, key);
    }
  else
    theFigure->updateUndoHash (key, (gpointer) item);
}

void
drgeoCreateItemCommand::unexecute ()
{
  geometricObject *item;

  item = (geometricObject *) theFigure->getValueFromKeyOnHash (key);
  theFigure->getFigureList ()->supprime ((void *) item);
  theFigure->getDrawable ()->updateDescription ();
  delete (item);
}

/* Move item command */
drgeoMoveItemCommand::drgeoMoveItemCommand (drgeoFigure * figure, geometricObject * item, drgeoVector & t):
drgeoCommand (figure)
{
  this->t = t;
  key = theFigure->getKeyFromValueOnHash ((gpointer) item);
  if (key == NULL)
    {
      key = (gpointer) item;
      /* item not in hash, add it */
      theFigure->addUndoHash (key, key);
    }
}

void
drgeoMoveItemCommand::execute ()
{
  geometricObject *item;

  item = (geometricObject *) theFigure->getValueFromKeyOnHash (key);
  item->move (t);
  theFigure->updateItems ();
}

void
drgeoMoveItemCommand::unexecute ()
{
  geometricObject *item;

  item = (geometricObject *) theFigure->getValueFromKeyOnHash (key);
  item->move (-t);
  theFigure->updateItems ();
}

/* Set attribute command */
drgeoSetItemAttributeCommand::drgeoSetItemAttributeCommand (drgeoFigure * figure, geometricObject * item, drgeoAttributeType attr, gpointer value):
drgeoCommand (figure)
{

  this->attr = attr;
  this->backValue = value;
  key = theFigure->getKeyFromValueOnHash ((gpointer) item);
  if (key == NULL)
    {
      key = (gpointer) item;
      /* item not in hash, add it */
      theFigure->addUndoHash (key, key);
    }
}

drgeoSetItemAttributeCommand::~drgeoSetItemAttributeCommand ()
{
  switch (attr)
    {
    case drgeoColor:
      g_free ((drgeoColorType *) backValue);
      break;
    case drgeoSize:
      g_free ((drgeoThicknessType *) backValue);
      break;
    case drgeoShape:
      g_free ((drgeoPointType *) backValue);
      break;
    case drgeoFill:
      g_free ((bool *) backValue);
      break;
    case drgeoName:
      g_free ((gchar *) backValue);
      break;
    case drgeoValue:
      g_free ((gdouble *) backValue);
      break;
    case drgeoVisible:
      g_free ((masked *) backValue);
      break;
    case drgeoScript:
      g_free ((gchar *) backValue);
      break;
    }
}

void
drgeoSetItemAttributeCommand::execute ()
{
  geometricObject *item;

  item = (geometricObject *) theFigure->getValueFromKeyOnHash (key);
  switch (attr)
    {
    case drgeoColor:
      {
	drgeoColorType tmp;
	tmp = item->getStyle ().color;
	item->getStyle ().color = *((drgeoColorType *) backValue);
	*((drgeoColorType *) backValue) = tmp;
	break;
      }
    case drgeoSize:
      {
	drgeoThicknessType tmp;
	tmp = item->getStyle ().thick;
	item->getStyle ().thick = *((drgeoThicknessType *) backValue);
	*((drgeoThicknessType *) backValue) = tmp;
	break;
      }
    case drgeoShape:
      {
	drgeoPointType tmp;
	tmp = item->getStyle ().pointShape;
	item->getStyle ().pointShape = *((drgeoPointType *) backValue);
	*((drgeoPointType *) backValue) = tmp;
	break;
      }
    case drgeoFill:
      {
	bool tmp;
	tmp = item->getStyle ().fill;
	item->getStyle ().fill = *((bool *) backValue);
	*((bool *) backValue) = tmp;
	break;
      }
    case drgeoName:
      {
	gchar *tmp;
	liste_elem *itemList;
	gint nb;
	geometricObject *item1;
	
	if (!item->getName ())
	  {
	    // item's current name is void
	    item->setName ((gchar *) backValue);
	    g_free ((gchar *) backValue);
	    backValue = NULL;
	  }
	else
	  {
	    tmp = g_strdup (item->getName ());
	    item->setName ((gchar *) backValue);
	    g_free ((gchar *) backValue);
	    backValue = (gpointer) tmp;
	  }
	itemList = theFigure->getFigureList ();
	nb = itemList->nb_elem;
	itemList->init_lire ();
	while (nb-- != 0)
	  {
	    item1 = (geometricObject *) itemList->lire (0);
	    item1->initName ();
	  }
	theFigure->getDrawable ()->updateDescription ();
	break;
      }
    case drgeoValue:
      {
	gdouble tmp;
	tmp = ((numeric *) item)->getValue ();
	((value *) item)->setValue (*((gdouble *) backValue));
	*((gdouble *) backValue) = tmp;
	((numeric *) item)->update (*theFigure->getDrawable ());
	theFigure->updateItems ();
	break;
      }
    case drgeoVisible:
      {
	masked tmp;
	tmp = item->getStyle ().mask;
	item->getStyle ().mask = *((masked *) backValue);
	*((masked *) backValue) = tmp;
	break;
      }
    case drgeoScript:
      {
	gchar *tmp;
	tmp = g_strdup (((script *) item)->getScript ());
	((script *) item)->setScript ((gchar *) backValue);
	g_free ((gchar *) backValue);
	backValue = (gpointer) tmp;
	((script *) item)->update (*theFigure->getDrawable ());
	theFigure->updateItems ();
	break;
      }

    }
}

void
drgeoSetItemAttributeCommand::unexecute ()
{
  execute ();
}

/* help function */
static void
replaceParentKey (xmlNodePtr xml, gpointer oldkey, gpointer newkey)
{
  xmlNodePtr tree;
  gchar *p, *oldkeys, *newkeys;

  oldkeys = g_strdup_printf ("%X", GPOINTER_TO_UINT (oldkey));
  newkeys = g_strdup_printf ("%X", GPOINTER_TO_UINT (newkey));

  tree = xml_search_next (xml, "parent");
  while (tree)
    {
      p = (char *) xmlGetProp (tree, BAD_CAST "ref");
      if (!strcmp (p, oldkeys))
	{
	  xmlSetProp (tree, BAD_CAST "ref", BAD_CAST newkeys);
	  g_printerr ("Find parent with ref. %s\n", p);
	  g_free (p);
	  break;
	}
      g_free (p);
      tree = xml_search_next (tree->next, "parent");
    }
  g_free (oldkeys);
  g_free (newkeys);
}
