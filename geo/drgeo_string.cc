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

#include "drgeo_string.h"
#include "drgeo_drawable.h"

string::string (drgeoPoint & p, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  str = NULL;
  this->p = p;
  size.set (0, 0);
}

string::string (drgeoPoint & p, gchar * str, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  this->str = g_strdup (str);
  this->p = p;
  size.set (0, 0);
}

string::string (gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  str = NULL;
  size.set (0, 0);
}

string::string (xmlNodePtr tree, liste_elem * figureList):
geometricObject (tree, figureList)
{
  str = NULL;
  size.set (0, 0);
}

string::~string ()
{
  g_free (str);
}

void
string::draw (drgeoDrawable & area, char force)
{
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;
  size.set (area.stringWidth (str), area.stringHeight (str));
  area.drawText (p, str, style.color);
}

gboolean
string::overObject (drgeoPoint & mouse, gdouble range)
{
  if (!exist)
    return FALSE;
  return ((mouse >= p) && (mouse <= (p + size)));
}

void
string::move (drgeoVector & t)
{
  p += t;
}

gchar *
string::getString ()
{
  return str;
}

void
string::setString (gchar * str)
{
  g_free (this->str);
  this->str =str;
}
