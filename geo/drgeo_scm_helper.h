/*
 *  Dr Geo 
 * (C) Copyright Hilaire Fernandes  2003
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

#ifndef DRGEO_SCM_HELPER_H
#define DRGEO_SCM_HELPER_H

#include <gtk/gtk.h>
#include <guile/gh.h>
#include "drgeo_drgeoVector.h"

drgeoPoint & scmCoord2drgeoPoint (SCM x, SCM y);
drgeoVector & scmList2drgeoVector (SCM l);
drgeoVector & scmVector2drgeoVector (SCM v);
SCM drgeoPoint2scmList (drgeoPoint & p);

/* very basic error handler */
SCM standard_handler (void *data, SCM tag, SCM throw_args);
static void dgs_error_cb (GtkWidget * widget, gpointer data);

#endif   /* DRGEO_SCM_HELPER_H */



