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

#include <errno.h>
#include <libintl.h>
#include "define.h"
#include "drgeo_scm_helper.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;

static gboolean DGSerror = FALSE;

/********************/
/* Helper functions */
/********************/
drgeoPoint &
scmCoord2drgeoPoint (SCM x, SCM y)
{
  static drgeoPoint t;
  t.set (scm_to_double (x), scm_to_double (y));
  return t;
}

drgeoPoint &
scmList2drgeoVector (SCM l)
{
  static drgeoVector t;
  t.set (scm_to_double (scm_car (l)), scm_to_double (scm_cadr (l)));
  return t;
}

drgeoVector &
scmVector2drgeoVector (SCM v)
{
  static drgeoVector t;
  static gdouble gv[2];

  for (int i=0, n=scm_c_array_length(v); i<n; ++i) 
  { 
    gv[i] = scm_to_double(scm_c_array_ref_1(v, i));
  }
  t.set (gv[0], gv[1]);
  return t;
}


SCM
drgeoPoint2scmList (drgeoPoint & p)
{
  static SCM x, y;
  x = scm_make_real (p.getX ());
  y = scm_make_real (p.getY ());
  return scm_list_n (x, y, SCM_UNDEFINED);
}



SCM
standard_handler (void *data, SCM tag, SCM throw_args)
{
  GtkWidget *dialog;

  /* is there pending error message in dialog box ? */
  if (DGSerror)
    return SCM_BOOL_F;

  DGSerror = TRUE;

  dialog = gtk_message_dialog_new (mdi->mainWindow (),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,
				   GTK_BUTTONS_CLOSE,
				   _("Error! Just got the following error tag: %s\n"),
				    SCM_CHARS(tag) , g_strerror (errno));

  g_signal_connect_swapped (GTK_OBJECT (dialog), "response",
			    G_CALLBACK (gtk_widget_destroy),
			    GTK_OBJECT (dialog));

  /* Called when the user acknowledge the error message */
  gtk_signal_connect (GTK_OBJECT (dialog), "unrealize",
		      GTK_SIGNAL_FUNC (dgs_error_cb), NULL);


  gtk_widget_show_all (dialog);


  return SCM_BOOL_F;
}

static void
dgs_error_cb (GtkWidget * widget, gpointer data)
{
  /* the user closed the error message box */
  DGSerror = FALSE;
}
