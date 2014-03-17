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

#ifndef DRGEO_COMMAND_H
#define DRGEO_COMMAND_H

#include <gtk/gtk.h>

enum drgeoAttributeType
{
  drgeoColor,
  drgeoSize,
  drgeoShape,
  drgeoName,
  drgeoFill,
  drgeoValue,
  drgeoVisible,
  drgeoScript
};


#include "drgeo_figure.h"


/* Abstract class of the command */
class drgeoCommand
{
public:
  virtual void execute () = 0;
  virtual void unexecute () = 0;
    virtual ~ drgeoCommand ();
protected:
    drgeoCommand (drgeoFigure * figure);
  drgeoFigure *theFigure;
};

/* Command to remove a list of item from a figure */
class drgeoRemoveItemCommand:public drgeoCommand
{
public:
  drgeoRemoveItemCommand (drgeoFigure * figure, geometricObject * rmItem);
  ~drgeoRemoveItemCommand ();
  void execute ();
  void unexecute ();
private:
    GList * xmlBackup;		/* a list of xmlNodePtr */
  liste_elem key;
};

/* Command to build a new item */
class drgeoCreateItemCommand:public drgeoCommand
{
public:
  drgeoCreateItemCommand (drgeoFigure * figure, geometricObject * buildItem);
  ~drgeoCreateItemCommand ();
  void execute ();
  void unexecute ();
private:
    xmlDocPtr xml;
  gpointer key;
};

/* Command to move an item */

class drgeoMoveItemCommand:public drgeoCommand
{
public:
  drgeoMoveItemCommand (drgeoFigure * figure, geometricObject * item,
			drgeoVector & t);
  void execute ();
  void unexecute ();
private:
    drgeoVector t;
  gpointer key;
};

/* Command to set the attrtibute of an item */

class drgeoSetItemAttributeCommand:public drgeoCommand
{
public:
  drgeoSetItemAttributeCommand (drgeoFigure * figure, geometricObject * item,
				drgeoAttributeType attr, gpointer value);
  ~drgeoSetItemAttributeCommand ();
  void execute ();
  void unexecute ();
private:
    drgeoAttributeType attr;
  gpointer key, backValue;
};

#endif /* DRGEO_COMMAND_H */
