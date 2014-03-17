/* Dr Genius an interactive geometry software
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

#include "drgenius_view.h"

drgeniusView::drgeniusView (drgeniusViewType type, gboolean save)
{
  this->p_name = NULL;
  this->p_filename = NULL;
  this->p_supportsSave = save;
  this->p_childType = type;    
  this->p_toolbar = NULL;
}

drgeniusView::
~drgeniusView ()
{
  g_free (p_name);
  g_free (p_filename);
}

gboolean
drgeniusView::save (gchar *filename)
{
  if (p_supportsSave)
    {
      g_free (this->p_filename);
      this->p_filename = filename;
      // update the name on the view
      return TRUE;
    }
  return FALSE;
}

gboolean
drgeniusView::undo (gint n)
{
  return false;
}

gboolean
drgeniusView::redo (gint n)
{
  return true;
}

gboolean
drgeniusView::undoActive ()
{
  return false;
}

gboolean
drgeniusView::redoActive ()
{
  return false;
}

void 
drgeniusView::setToolbar (GtkWidget *w)
{
  if (this->p_toolbar)
    gtk_object_destroy (GTK_OBJECT (this->p_toolbar));
  this->p_toolbar = w;
}

GtkWidget *
drgeniusView::toolbar ()
{
  return p_toolbar;
}

gboolean
drgeniusView::supportsSave ()
{
  return p_supportsSave;
}

drgeniusViewType
drgeniusView::childType ()
{
  return p_childType;
}

gchar *
drgeniusView::filename ()
{
  return p_filename;
}

gchar *
drgeniusView::name ()
{
  return p_name;
}

void 
drgeniusView::setName (const gchar *name)
{
  g_free (p_name);
  p_name = g_strdup (name);
}

void 
drgeniusView::setFilename (const gchar *filename)
{
  g_free (p_filename);
  p_filename = g_strdup (filename);
}
