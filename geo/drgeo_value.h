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

#ifndef DRGEO_VALUE_H
#define DRGEO_VALUE_H

#include "drgeo_string.h"

class value:public string
{
public:
  value (drgeoPoint & p, gboolean createdFromMacro, liste_elem * figureList);
  value (gboolean createdFromMacro, liste_elem * figureList);
  value (xmlNodePtr tree, liste_elem * figureList);
  ~value ()
  {
  };
  void initName ();
  gdouble getValue ();
  void setValue (gdouble val);
protected:
  gdouble val;
};

#endif /* DRGEO_VALUE_H */
