/* Geo an interactive geometry software
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

#ifndef drgeo_dialog_h
#define drgeo_dialog_h

#include "drgeo_figure.h"

#ifdef __cplusplus
extern "C"
{
#endif				/* __cplusplus */
  void get_edited_value ();
#ifdef __cplusplus
}

#endif				/* __cplusplus */
typedef struct
{
  drgeoPoint mouse;
  drgeoFigure *figure;
} DialogData;
#endif
