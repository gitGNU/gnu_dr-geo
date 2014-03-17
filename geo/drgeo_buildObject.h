/*
 *  Dr Genius 
 * (C) Copyright Hilaire Fernandes  2000
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

#ifndef DRGEO_BUILDOBJECT_H
#define DRGEO_BUILDOBJECT_H

#include "drgeo_geometricObject.h"

GList *buildFreePoint (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildMiddlePoint (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildIntersectionPoint (int cas, drgeoPoint & at,
			       drgeoFigure * figure);
GList *buildCoordinatePoint (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildLine (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildHalfLine (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildSegment (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildVector (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildCircle (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildArcCircle (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildLocus (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildPolygon (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildParallelLine (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildOrthogonalLine (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildReflexion (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildSymmetry (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildTranslation (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildRotation (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildScale (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildNumeric (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildAngle (int cas, drgeoPoint & at, drgeoFigure * figure);
GList *buildCoordinateAndEquation (int cas, drgeoPoint & at,
				   drgeoFigure * figure);
GList *buildScript (int cas, drgeoPoint & at, drgeoFigure * figure);

#endif /* DRGEO_BUILDOBJECT_H */
