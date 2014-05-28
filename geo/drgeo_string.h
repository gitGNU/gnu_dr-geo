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

#ifndef DRGEO_STRING_H
#define DRGEO_STRING_H

#include "drgeo_geometricObject.h"

class string:public geometricObject
{
public:
  string (drgeoPoint & p, gboolean createdFromMacro, liste_elem * figure);
  string (drgeoPoint & p, gchar * str, gboolean createdFromMacro,
	  liste_elem * figure);
  string (gboolean createdFromMacro, liste_elem * figure);
  string (xmlNodePtr tree, liste_elem * figure);
  ~string ();
  void draw (drgeoDrawable & area, char force);
  gboolean overObject (drgeoPoint & mouse, gdouble range);
  void move (drgeoVector & t);
  gchar *getString ();
  void setString (gchar * str);
protected:
    drgeoPoint p;
  gchar *str;
  /* dimension of the displayed text */
  drgeoPoint size;
};

#endif /* DRGEO_STRING_H */
