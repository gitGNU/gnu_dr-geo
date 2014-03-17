/*
 *  Dr Genius interactive geometry software
 * (C) 2001,2002 Copyright FSF 
 * Author: Hilaire Fernandes  2003
 * 
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

#include <guile/gh.h>
#include <glib.h>

void
drgeoInit ()
{
  SCM ret;
  // Code to handle user configurations
  ret = gh_load (DRGEO_SCMDIR"/drgeo_config.scm");
  // The Dr. Geo Scheme interface
  ret = gh_load (DRGEO_SCMDIR"/drgeo_scm_interface.scm");
  // The Scheme interface constants and synonymes and their
  // translations in other national language
  ret = gh_load (DRGEO_SCMDIR"/drgeo_scm_interface_constant.scm");
  ret = gh_load (DRGEO_SCMDIR"/drgeo_scm_interface_constant_fr.scm");
  ret = gh_load (DRGEO_SCMDIR"/drgeo_scm_interface_constant_sr.scm");
  ret = gh_load (DRGEO_SCMDIR"/drgeo_scm_interface_constant_es.scm");
  ret = gh_load (DRGEO_SCMDIR"/drgeo_scm_interface_constant_nl.scm");
}

