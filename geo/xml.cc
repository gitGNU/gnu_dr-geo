/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes 1997-1999
 * hilaire.fernandes@iname.com  
 * Portion of code copyright to their respective owner :
 * Daniel Veillard <Daniel.Veillard@w3.org>
 * Miguel de Icaza <miguel@gnu.org>
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

#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "xml.h"

/*
 * Search the next the element name in the node list. 
 */

xmlNodePtr
xml_search_next (xmlNodePtr node, const char *name)
{
  while (node != NULL)
    {
      if (strcmp ((char *) node->name, name) == 0)
	return node;
      node = node->next;
    }
  return NULL;
}




/*
 * Search a child by name, if needed go down the tree to find it. 
 */

xmlNodePtr
xml_search_child (xmlNodePtr node, const char *name)
{
  xmlNodePtr ret;
  xmlNodePtr child;

  if (node == NULL)
    return NULL;
  child = node->xmlChildrenNode;
  while (child != NULL)
    {
      if (strcmp ((char *) child->name, name) == 0)
	return child;
      child = child->next;
    }
  child = node->xmlChildrenNode;
  while (child != NULL)
    {
      ret = xml_search_child (child, name);
      if (ret != NULL)
	return ret;
      child = child->next;
    }
  return NULL;
}

char *
xml_value_get (xmlNodePtr node, const char *name)
{
  char *ret;
  xmlNodePtr child;

  ret = (char *) xmlGetProp (node, BAD_CAST name);
  if (ret != NULL)
    return ret;
  child = node->xmlChildrenNode;

  while (child != NULL)
    {
      if (!strcmp ((char *) child->name, name))
	{
	  /*
	   * !!! Inefficient, but ...
	   */
	  ret = (char *) xmlNodeGetContent (child);
	  if (ret != NULL)
	    return (ret);
	}
      child = child->next;
    }

  return NULL;
}

int
xml_get_value_char (xmlNodePtr node, const char *name, char *val)
{
  char *ret;
  int c;
  int res;

  ret = xml_value_get (node, name);
  if (ret == NULL)
    return (0);
  res = sscanf (ret, "%d", &c);
  free (ret);

  if (res == 1)
    {
      *val = (signed char) c;
      return 1;
    }
  return 0;
}

int
xml_get_value_int (xmlNodePtr node, const char *name, int *val)
{
  char *ret;
  int i;
  int res;

  ret = xml_value_get (node, name);
  if (ret == NULL)
    return (0);
  res = sscanf (ret, "%d", &i);
  free (ret);

  if (res == 1)
    {
      *val = i;
      return 1;
    }
  return 0;
}

int
xml_get_value_double (xmlNodePtr node, const char *name, double *val)
{
  int res;
  char *ret;

  ret = xml_value_get (node, name);
  if (ret == NULL)
    return (0);
  res = sscanf (ret, "%lf", val);
  free (ret);

  return (res == 1);
}

void
xml_set_int_prop (xmlNodePtr node, const char *prop, int value)
{
  gchar *content;

  content = g_strdup_printf ("%d", value);
  xmlSetProp (node, BAD_CAST prop, BAD_CAST content);
  g_free (content);
}

void
xml_set_double_prop (xmlNodePtr node, const char *prop, double value)
{
  gchar *content;

  content = g_strdup_printf ("%f", value);
  xmlSetProp (node, BAD_CAST prop, BAD_CAST content);
  g_free (content);
}
