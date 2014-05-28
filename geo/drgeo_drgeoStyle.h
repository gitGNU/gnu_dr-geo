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


#ifndef DRGEO_DRGEOSTYLE_H
#define DRGEO_DRGEOSTYLE_H

#include <glib.h>
#include "couleur.h"

// it's important to start the indice at 0
// this is used as array index
enum drgeoThicknessType
{
  drgeoThicknessDashed = 0,
  drgeoThicknessNormal = 1,
  drgeoThicknessLarge = 2
};

enum drgeoPointType
{
  drgeoPointRound = 0,
  drgeoPointX = 1,
  drgeoPointRec = 2,
  drgeoPointRoundEmpty = 3,
  drgeoPointRecEmpty = 4
};

enum masked
{
  no = 0,
  yes = 1,
  alway = 2
};

#ifdef __cplusplus
// This style class is used to store the style of figure items.
class drgeoStyle
{
public:
  drgeoColorType color;
  drgeoThicknessType thick;
  drgeoPointType pointShape;
  bool fill;
  masked mask;
  gchar *name;
    drgeoStyle ()
  {
    color = drgeoColorBlack;
    thick = drgeoThicknessNormal;
    pointShape = drgeoPointX;
    fill = (0 == 1);
    mask = no;
    name = NULL;
  }
};
#endif /* __cplusplus */

#endif /* DRGEO_DRGEOSTYLE_H */
