/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
 * hilaire.fernandes@iname.com 
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


#ifndef DRGEO_XML_H
#define DRGEO_XML_H

#include "../xmlinclude.h"


xmlNodePtr xml_search_next (xmlNodePtr node, const char *name);
xmlNodePtr xml_search_child (xmlNodePtr node, const char *name);
char *xml_value_get (xmlNodePtr node, const char *name);
int xml_get_value_char (xmlNodePtr node, const char *name, char *val);
int xml_get_value_int (xmlNodePtr node, const char *name, int *val);
int xml_get_value_double (xmlNodePtr node, const char *name, double *val);
void xml_set_int_prop (xmlNodePtr node, const char *prop, int value);
void xml_set_double_prop (xmlNodePtr node, const char *prop, double value);
#endif /* DRGEO_XML_H */
