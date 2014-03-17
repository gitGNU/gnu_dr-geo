/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1998
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

#ifndef var_decl_h
#define var_decl_h

int objet_issue_macro_construction;


#define NB_MODE_MAX 10
#define NB_OBJET_PAR_MODE_MAX 5

// Description des modes de creations
objectCategory mode1[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {CURVE, NO_OBJECT}
};
//
objectCategory mode2[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, NO_OBJECT},
  {POINT, VALUE, REPERE, NO_OBJECT},
  {EQUATION, REPERE, NO_OBJECT}
};
//
objectCategory mode3[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, NO_OBJECT}
};
//
objectCategory mode4[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, NO_OBJECT}
};
//
objectCategory mode5[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, NO_OBJECT},
  {POINT, SEGMENT, NO_OBJECT},
  {POINT, VALUE, NO_OBJECT}
};
//
objectCategory mode6[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {CURVE, CURVE, NO_OBJECT}
};
//
objectCategory mode7[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, DIRECTION, NO_OBJECT}
};
//
objectCategory mode8[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, DIRECTION, NO_OBJECT}
};
//
objectCategory mode9[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {LINE, POINT, NO_OBJECT},
  {LINE, LINE, NO_OBJECT},
  {LINE, HALF_LINE, NO_OBJECT},
  {LINE, SEGMENT, NO_OBJECT},
  {LINE, VECTOR, NO_OBJECT},
  {LINE, CIRCLE, NO_OBJECT},
  {LINE, ARC_CIRCLE, NO_OBJECT},
  {LINE, POLYGON, NO_OBJECT}
};
//
objectCategory mode10[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, NO_OBJECT},
  {POINT, LINE, NO_OBJECT},
  {POINT, HALF_LINE, NO_OBJECT},
  {POINT, SEGMENT, NO_OBJECT},
  {POINT, CIRCLE, NO_OBJECT},
  {POINT, ARC_CIRCLE, NO_OBJECT},
  {POINT, VECTOR, NO_OBJECT},
  {POINT, POLYGON, NO_OBJECT}
};
//
objectCategory mode11[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {VECTOR, POINT, NO_OBJECT},
  {VECTOR, LINE, NO_OBJECT},
  {VECTOR, HALF_LINE, NO_OBJECT},
  {VECTOR, SEGMENT, NO_OBJECT},
  {VECTOR, CIRCLE, NO_OBJECT},
  {VECTOR, ARC_CIRCLE, NO_OBJECT},
  {VECTOR, POLYGON, NO_OBJECT}
};
//
objectCategory mode12[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {VALUE, POINT, POINT, NO_OBJECT},
  {VALUE, LINE, POINT, NO_OBJECT},
  {VALUE, HALF_LINE, POINT, NO_OBJECT},
  {VALUE, SEGMENT, POINT, NO_OBJECT},
  {VALUE, VECTOR, POINT, NO_OBJECT},
  {VALUE, CIRCLE, POINT, NO_OBJECT},
  {VALUE, ARC_CIRCLE, POINT, NO_OBJECT},
  {VALUE, POLYGON, POINT, NO_OBJECT}
};
//
objectCategory mode13[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {SEGMENT, NO_OBJECT},
  {VECTOR, NO_OBJECT},
  {POINT, POINT, NO_OBJECT},
  {POINT, CIRCLE, NO_OBJECT},
  {POINT, LINE, NO_OBJECT},
  {CIRCLE, NO_OBJECT},
  {LINE, NO_OBJECT},
  {ARC_CIRCLE, NO_OBJECT}
};
//
objectCategory mode14[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, POINT, NO_OBJECT},
  {VECTOR, VECTOR, NO_OBJECT}
};
//
objectCategory mode15[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, NO_OBJECT},
  {VECTOR, NO_OBJECT},
  {LINE, NO_OBJECT},
  {CIRCLE, NO_OBJECT}
};
//
objectCategory mode16[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {VALUE, VALUE, NO_OBJECT}
};
//
objectCategory mode17[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {SEGMENT, NO_OBJECT},
  {POINT, POINT, NO_OBJECT}
};
//
objectCategory mode18[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {FREE_PT_ON_CURVE, CONSTRAINED_PT, NO_OBJECT}
};
//
objectCategory mode19[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {POINT, POINT, POINT, NO_OBJECT}
};
//
objectCategory mode20[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {VALUE, POINT, POINT, NO_OBJECT},
  {VALUE, LINE, POINT, NO_OBJECT},
  {VALUE, HALF_LINE, POINT, NO_OBJECT},
  {VALUE, SEGMENT, POINT, NO_OBJECT},
  {VALUE, VECTOR, POINT, NO_OBJECT},
  {VALUE, CIRCLE, POINT, NO_OBJECT},
  {VALUE, ARC_CIRCLE, POINT, NO_OBJECT},
  {VALUE, POLYGON, POINT, NO_OBJECT}
};
// This a dummy mode for the polygon object
objectCategory mode21[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX] = {
  {},
};

#endif
