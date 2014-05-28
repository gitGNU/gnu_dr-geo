/*
 *  Dr Genius 
 * (C) Copyright Hilaire Fernandes  2001
 * hilaire@ofset.org 
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

#ifndef DRGEO_SCM_API_H
#define DRGEO_SCM_API_H

#include <guile/gh.h>


#ifdef __cplusplus
extern "C"
{
#endif				/* __cplusplus */


/* Point */
  SCM drgeo_scm_getAbscissa (SCM object);
  SCM drgeo_scm_setAbscissa (SCM object, SCM x);
  SCM drgeo_scm_getCoordinates (SCM object);
  SCM drgeo_scm_setCoordinates (SCM object, SCM coord);

/* Direction */
  SCM drgeo_scm_getUnit (SCM object);
  SCM drgeo_scm_getNormal (SCM object);


/* Line, half-line, segment, vector */
  SCM drgeo_scm_getSlope (SCM object);

/* vector */
  SCM drgeo_scm_getNorm (SCM object);


/* segment, circle, arc-circle */
  SCM drgeo_scm_getLength (SCM object);

/* circle, arc-circle */
  SCM drgeo_scm_getCenter (SCM object);
  SCM drgeo_scm_getRadius (SCM object);


/* Number */
  SCM drgeo_scm_getValue (SCM object);
  SCM drgeo_scm_setValue (SCM object, SCM v);

/* Angle */
  SCM drgeo_scm_getAngle (SCM objetct);

/* Others */
  SCM drgeo_scm_move (SCM object, SCM t);

#ifdef __cplusplus
}
#endif				/* __cplusplus */



#endif				/* DRGEO_SCM_API_H */
