/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
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

#include "xml.h"
#include "macro.h"
#include "drgeo_figure.h"
#include "drgeo_point.h"
#include "drgeo_vector.h"
#include "drgeo_line.h"
#include "drgeo_halfLine.h"
#include "drgeo_segment.h"
#include "drgeo_circle.h"
#include "drgeo_arcCircle.h"
#include "drgeo_angle.h"
#include "drgeo_numeric.h"
#include "drgeo_equation.h"
#include "drgeo_locus.h"
#include "drgeo_polygon.h"
#include "drgeo_script.h"
#include "traite.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;


/* Helper function to load/save a XML macro */
static void xmlSaveKernel (xmlNodePtr tree, noyau_macro * kernel);
static bool xmlLoadKernel (xmlNodePtr tree, macro * mac);

// Definition des fonctions membres de la classe macro
int
macro::

ajoute (int *parent, int type, objectCategory category,
	void *objet, int flag, void * extra)
{
  courant->fig = objet;
  courant->liste_parent = parent;
  courant->type = type;
  courant->category = category;
  courant->param_f = flag;
  courant->extra = extra;
  courant->suivant = new (noyau_macro);
  courant = courant->suivant;
  nb_noyau++;
  return (nb_noyau);
}

int
macro::position (void *fig)
{
  int position = 0;
  noyau_macro *p;
  p = debut;
  while (p->suivant != NULL)
    {
      position++;
      if (p->fig == fig)
	break;
      p = p->suivant;
    }
  if (p->fig == fig)
    return (position);
  return (0);
}

void
macro::init_lire (void)
{
  lecture = debut;
}

noyau_macro *
macro::lire (int pos)
{
  noyau_macro *noyau;
  if (pos == 0)
    {
      if (lecture != NULL)
	{
	  noyau = lecture;
	  lecture = lecture->suivant;
	  return (noyau);
	}
      else
	{
	  lecture = debut->suivant;
	  return (debut);
	}
    }
  else if (pos > 0)
    {
      noyau = debut;
      for (int a = 1; a != pos; a++)
	{
	  noyau = noyau->suivant;
	  if (noyau == NULL)
	    return (NULL);
	}
      return (noyau);
    }
  else
    return (NULL);
}

void
macro::vide (void)
{
// vide la liste chainee
  noyau_macro *p, *q;
  p = debut->suivant;
  while (p != NULL)
    {
      q = p->suivant;
      delete[]p->liste_parent;
      delete (p);
      p = q;
    }
  lecture = courant = debut;
  debut->fig = NULL;
  debut->suivant = NULL;
  nb_noyau = 0;
}

// The following method is applying the macro construction to a list
// of input parameters.  Its result is a boolean indicating wether the
// macro was applied successfully or not.

bool
macro::play (drgeoFigure * aFigure)
{
  int a, nb;
  noyau_macro *n;
  liste_elem parentList;
  liste_elem *input;
  gboolean createdFromMacro;

  input = aFigure->getSelection ();
  nb = input->nb_elem;
  init_lire ();
  input->init_lire ();
  while (nb-- != 0)
    {
      // Copy the address of the input parameters.
      lire (0)->fig = input->lire (0);
    }

  // Number of intermediate and final items to be created.
  nb = nb_noyau - param_i;
  while (nb-- != 0)
    {
      n = lire (0);
      if (!n->param_f)
	createdFromMacro = TRUE;
      else
	createdFromMacro = FALSE;
      n->fig = (void *) createItem (findParent (n->liste_parent, 
						parentList),
				    n, createdFromMacro,
				    aFigure->getFigureList ());
    }

  nb = nb_noyau;
  for (a = param_i + 1; a <= nb; a++)
    {
      (aFigure->getFigureList ())->ajoute (lire (a)->fig);
    }
  aFigure->updateItems ();
  return TRUE;
}

liste_elem & macro::findParent (int *l, liste_elem & parentList)
{
  int
    nb,
    a;
  nb = l[0];
  parentList.vide ();
  for (a = 1; a <= nb; a++)
    parentList.ajoute (lire (l[a])->fig);
  return parentList;
}

geometricObject *
macro::createItem (liste_elem & parentList, noyau_macro * n,
		   gboolean createdFromMacro, liste_elem * figureList)
{
  geometricObject *fig = NULL;
  drgeoPoint p (0, 0);
  g_printerr ("Create Item %d\n", n->category);
  switch (n->category)
    {
    case CONSTRAINED_PT:
      if (n->extra != NULL)
	fig = new point (parentList, (pointType) n->type,
			 createdFromMacro, figureList, 
			 *((int *) n->extra));
      else
	fig = new point (parentList, (pointType) n->type,
			 createdFromMacro, figureList, 0);
      break;
    case FREE_PT:
      g_print ("%lf,%lf\n",*((double *) n->extra),
	       *((double *) n->extra + 1)); 
      p.setX (*((double *) n->extra));
      p.setY (*((double *) n->extra + 1));
      fig = new point (p, parentList, (pointType) n->type,
		       createdFromMacro, figureList);
      break;
    case FREE_PT_ON_CURVE:
      fig = new point ( *((double *) n->extra), parentList,		
		       createdFromMacro, figureList);
      break;
    case LINE:
      fig = new line (parentList, (lineType) n->type,
		      createdFromMacro, figureList);
      break;
    case HALF_LINE:
      fig = new halfLine (parentList, (halfLineType) n->type,
			  createdFromMacro, figureList);
      break;
    case SEGMENT:
      fig = new segment (parentList, (segmentType) n->type,
			 createdFromMacro, figureList);
      break;
    case VECTOR:
      fig = new vector (parentList, (vectorType) n->type,
			createdFromMacro, figureList);
      break;
    case CIRCLE:
      fig = new circle (parentList, (circleType) n->type,
			createdFromMacro, figureList);
      break;
    case ARC_CIRCLE:
      fig = new arcCircle (parentList, (arcCircleType) n->type,
			   createdFromMacro, figureList);
      break;
    case LOCUS:
      fig = new locus (parentList, createdFromMacro, figureList);
      break;
    case REPERE:
/*
		fig = new repere (parentList, (repereType) n->type, 
				  createdFromMacro, figureList);
*/
      break;
    case ANGLE:
      fig = new angle (p, parentList, (angleType) n->type,
		       createdFromMacro, figureList);
      break;
    case NUMERIC:
      if ((numericType) n->type == FREE_VALUE)
	fig = new numeric (p, *((double *) n->extra), 
			   (numericType) n->type, createdFromMacro);
      else
	fig = new numeric (p, parentList, (numericType) n->type,
			   createdFromMacro, figureList);
      break;
    case EQUATION:
      fig = new equation (p, parentList, (equationType) n->type,
			  createdFromMacro, figureList);
      break;
    case POLYGON:
      fig = new polygon (parentList, (polygonType) n->type,
			 createdFromMacro, figureList);
      break;
    case SCRIPT:
      fig = new script (p, parentList, (scriptType) n->type,
			(gchar *) n->extra,createdFromMacro, figureList);
      break;
    }
  return fig;
}

// ------------------------
// Implementation of Macro Registry.

drgeoMacroRegistry *
  drgeoMacroRegistry::registry =
  NULL;

drgeoMacroRegistry::drgeoMacroRegistry ()
{
  // Do something here?
}

drgeoMacroRegistry::~drgeoMacroRegistry ()
{
  registry = NULL;
}

drgeoMacroRegistry *
drgeoMacroRegistry::get ()
{
  // Make sure only one instance is created.
  if (registry == NULL)
    {
      registry = new drgeoMacroRegistry;
    }
  return (registry);
}

bool
drgeoMacroRegistry::load (xmlNodePtr macroXml)
{
  xmlNodePtr tree, subtree;
  gchar *name, *description, *tmp;
  int pi, pf, parentQty, *parent, category, parentNumber;
  macro *mac;

  if (strcmp ((char *) macroXml->name, "macro"))
    {
      fprintf (stderr,
	       "drgeoMacroRegistry::load : invalid element type %s, 'macro' expected\n",
	       macroXml->name);
      return false;
    }
  name = (gchar *) xml_value_get (macroXml, "name");
  description = (gchar *) xml_value_get (macroXml, "description");

  /* Count how many input parameter we have there */
  if ((tree = xml_search_child (macroXml, "input")) == NULL)
    {
      fprintf (stderr,
	       "drgeoMacroRegistry::load : no input element in macro %s\n", name);
      return false;
    }
  if ((tree = xml_search_child (tree, "kernel")) == NULL)
    {
      fprintf (stderr,
	       "drgeoMacroRegistry::load : no kernel element in input element in macro %s\n", name);
      return false;
    }
  pi = 0;
  while (tree != NULL)
    {
      pi++;
      tree = xml_search_next (tree->next, "kernel");
    }

  /* Count how many output parameter we have there */
  if ((tree = xml_search_child (macroXml, "intermediate")) == NULL)
    {
      fprintf (stderr,
	       "drgeoMacroRegistry::load : no intermediate element in macro %s\n", name);
      return false;
    }
  if ((tree = xml_search_child (tree, "kernel")) == NULL)
    {
      fprintf (stderr,
	       "drgeoMacroRegistry::load : no kernel element in intermediate element in macro %s\n", name);
      return false;
    }
  pf = 0;
  while (tree != NULL)
    {
      tmp = (char *) xmlGetProp (tree, BAD_CAST "final");
      if (!strcmp (tmp, "True"))
	pf++;
      g_free (tmp);
      tree = xml_search_next (tree->next, "kernel");
    }

  mac = new macro (name, description, pi, pf);
  /* rebrowse the input element and insert them in the macro
     we alreay know input element exist and there is 
     at least one kernel element in it */
  tree = xml_search_child (macroXml, "input");
  tree = xml_search_child (tree, "kernel");
  while (tree != NULL)
    {
      if (!xml_get_value_int (tree, "category", &category))
	{
	  fprintf (stderr,
		   "drgeoMacroRegistry::load : no category in input kernel in macro %s\n", name);
	  delete mac;
	  g_free (name);
	  g_free (description);
	  return false;
	}
      mac->ajoute (NULL, 0, (objectCategory) category, NULL, FALSE, NULL);
      tree = xml_search_next (tree->next, "kernel");
    }


  /* browse the intermediate element and insert them in the macro */
  tree = xml_search_child (macroXml, "intermediate");
  tree = xml_search_child (tree, "kernel");
  if (!xmlLoadKernel (tree, mac) || (tree == NULL))
    {
      fprintf (stderr,
	       "drgeoMacroRegistry::load : error building the intermediate kernel in macro %s\n", name);
      delete mac;
      g_free (name);
      g_free (description);
      return false;
    }
  this->add (mac);
  mdi->addMacroMenuItem (name, description);
  g_free (name);
  g_free (description);
  return true;
}

bool
drgeoMacroRegistry::save (const gchar *macroName, xmlNodePtr macroXml)
{
  macro *mac;
  xmlNodePtr tree, subtree, subsubtree, elem;
  int nb;
  char *attr;
  noyau_macro *kernel;
  if ((mac = find (macroName)) == NULL)
    {
      fprintf (stderr, "The macro %s does'nt exist\n", macroName);
      return FALSE;
    }
  tree = xmlNewChild (macroXml, NULL, BAD_CAST "macro", NULL);
  xmlSetProp (tree, BAD_CAST "name", BAD_CAST macroName);

  // save the description if not void
  if (strlen (mac->getDescription ()) != 0)
    xmlNewChild (tree, NULL, BAD_CAST "description",
		 BAD_CAST (mac->getDescription ()));

  // Init the macro to read its kernels content
  mac->init_lire ();
  nb = mac->getInputParameterNumber ();

  // save the input item
  subtree = xmlNewChild (tree, NULL, BAD_CAST "input", NULL);

  while (nb-- != 0)
    {
      elem = xmlNewChild (subtree, NULL, BAD_CAST "kernel", NULL);
      attr = g_strdup_printf ("%d", (int) mac->lire (0)->category);
      xmlSetProp (elem, BAD_CAST "category", BAD_CAST attr);
      g_free (attr);
    }

  // save the intermediate item (invisible & visible)
  nb = mac->getKernelNumber () - mac->getInputParameterNumber ();
  if (nb > 0)
    subtree = xmlNewChild (tree, NULL, BAD_CAST "intermediate", NULL);
  while (nb != 0)
    {
      kernel = mac->lire (0);
      nb--;
      xmlSaveKernel (subtree, kernel);
    }
  return TRUE;
}

void
drgeoMacroRegistry::add (macro * aMacro)
{
  macro *oldMacro;

  if (oldMacro = find (aMacro->getName ()))
    {
      list.supprime ((void *) oldMacro);
    }
  list.ajoute ((void *) aMacro);
}

macro *
drgeoMacroRegistry::find (const gchar *aName)
{
  macro *aMacro = NULL;
  int nb;

  // Search a list for the corresponding macro name.
  list.init_lire ();
  nb = list.nb_elem;
  while (nb-- != 0)
    {
      aMacro = (macro *) list.lire (0);
      if (strcmp (aMacro->getName (), aName) == 0)
	{
	  return (aMacro);
	}
    }
  // Not found a macro with such a name.
  return (NULL);
}

macro *
drgeoMacroRegistry::first ()
{
  macro *aMacro = NULL;

  // Read the first item in the list.
  if (list.nb_elem >= 1)
    {
      aMacro = (macro *) list.lire (1);
    }
  index = 1;
  return (aMacro);
}

bool
drgeoMacroRegistry::isDone ()
{
  // Is the end near?
  if (index > list.nb_elem)
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
}

macro *
drgeoMacroRegistry::next ()
{
  macro *aMacro = NULL;

  // NEXT!
  index++;
  if (index <= list.nb_elem)
    {
      aMacro = (macro *) list.lire (index);
    }
  return (aMacro);
}

static void
xmlSaveKernel (xmlNodePtr tree, noyau_macro * kernel)
{
  int n, *parent;
  gchar *attr;
  xmlNodePtr subtree, elem;

  subtree = xmlNewChild (tree, NULL, BAD_CAST "kernel", NULL);
  attr = g_strdup_printf ("%d", kernel->type);
  xmlSetProp (subtree, BAD_CAST "type", BAD_CAST attr);
  g_free (attr);
  attr = g_strdup_printf ("%d", (int) kernel->category);
  xmlSetProp (subtree, BAD_CAST "category", BAD_CAST attr);
  g_free (attr);
  if (kernel->extra != NULL)
    {
      switch (kernel->category)
	{
	case CONSTRAINED_PT:
	  // extra argument for intersection point
	  attr = g_strdup_printf ("%d", *((int *) kernel->extra));
	  break;
	case SCRIPT:
	  // extra argument for script
	  attr = g_strdup_printf ("%s", (gchar *) kernel->extra);
	  break;
	case FREE_PT_ON_CURVE:
	  // extra argument for free point on curve is its abscissa
	  attr = g_strdup_printf ("%lf", *((double *) kernel->extra));
	  break;
	case FREE_PT:
	  // extra argument for free point is its coordinates
	  attr = g_strdup_printf ("%lf;%lf", *((double *) kernel->extra),
				  *((double *)kernel->extra + 1));	  
	  break;
	case NUMERIC:
	  // extra argument for free value is its value
	  if (kernel->type == FREE_VALUE)
	    {
	      attr = g_strdup_printf ("%lf", *((double *) kernel->extra));
	      break;
	    }
	  // Fall through, this is what we want
	default:
	  g_printerr ("macro::xmlSaveKernel kernel->extra value to a not handled category object\n");
	  attr = NULL;
	}
      xmlSetProp (subtree, BAD_CAST "extra", BAD_CAST attr);
      g_free (attr);
    }
  if (kernel->param_f != TRUE)
    xmlSetProp (subtree, BAD_CAST "final", BAD_CAST "False");
  else
    xmlSetProp (subtree, BAD_CAST "final", BAD_CAST "True");
  // Let's save the parent ref of this kernel
  parent = kernel->liste_parent;
  n = *parent++;
  while (n-- != 0)
    {
      attr = g_strdup_printf ("%d", *parent++);
      elem = xmlNewChild (subtree, NULL, BAD_CAST "parent", NULL);
      xmlSetProp (elem, BAD_CAST "ref", BAD_CAST attr);
      g_free (attr);
    }
}


static bool
xmlLoadKernel (xmlNodePtr tree, macro * mac)
{
  int *parent, type, category, parentQty, pi, parentNumber;
  void * extra;
  int paramf;
  char *str;
  xmlNodePtr subtree;

  while ((tree = xml_search_next (tree, "kernel")) != NULL)
    {
      if (!xml_get_value_int (tree, "type", &type)
	  || !xml_get_value_int (tree, "category", &category))
	return false;
      if ((str = xml_value_get (tree, "final")) == NULL)
	return false;
      if (!strcmp (str, "True"))
	paramf = true;
      else
	paramf = false;
      g_free (str);

      // eventually fetch the extra element
      extra = (void *) xml_value_get (tree, "extra");
      if (extra != NULL)
	{
	  switch ((objectCategory) category)
	    {
	    case CONSTRAINED_PT:
	      int * tmp;
	      tmp = (int *) g_malloc (sizeof (int));
	      xml_get_value_int (tree, "extra", tmp);
	      g_free (extra);
	      extra = (void *) tmp;
	      break;
	    case FREE_PT_ON_CURVE:
	      double *x;
	      x = (double *) g_malloc (sizeof (double));
	      xml_get_value_double (tree, "extra", x);
	      g_free (extra);
	      extra = (void *) x;
	      break;
	    case FREE_PT:
	      double *coordinate;
	      coordinate = (double *) g_malloc (2 * sizeof(double));
	      if (sscanf ((const char *) extra,"%lf;%lf", coordinate, coordinate + 1) != 2)
		{
		  *coordinate = 0.0;
		  *(coordinate + 1) = 0.0;
		  g_printerr ("macro::xmlLoadKernel cannot scan coordinate of free point\n");
		}
	      g_free (extra);
	      extra = (void *) coordinate;
	      break;
  	    case SCRIPT:
	      // Nothing to be done to fetch the extra value for script
	      break;
	    case NUMERIC:
	      if (type == FREE_VALUE)
		{
		  double *val;
		  val = (double *) g_malloc (sizeof (double));
		  xml_get_value_double (tree, "extra", val);
		  g_free (extra);
		  extra = (void *) val;
		  break;
		}
	      // just fall through, this is what we want
	    default:
	      // default, set extra to NULL
	      g_free (extra);
	      extra = NULL;
	      g_printerr ("macro::xmlLoadKernel extra value to a not handled category object\n");
	      break;
	    }
	}

      subtree = xml_search_child (tree, "parent");
      parentQty = 0;
      while (subtree != NULL)
	{
	  subtree = xml_search_next (subtree->next, "parent");
	  parentQty++;
	}
      if (parentQty == 0 &&
	  (objectCategory) category != FREE_PT &&
	  category != SCRIPT &&
	  !(category == NUMERIC && type == FREE_VALUE))
	/* all intermediate item but FREE_PT, FREE_VALUE, SCRIPT must
	   have at least one parent */
	return false;
      parent = new int[parentQty + 1];
      parent[0] = parentQty;
      pi = 1;
      subtree = xml_search_child (tree, "parent");
      while (parentQty-- > 0)
	{
	  if (!xml_get_value_int (subtree, "ref", &parentNumber))
	    return false;
	  parent[pi++] = parentNumber;
	  subtree = xml_search_next (subtree->next, "parent");
	}
      mac->ajoute (parent, type, (objectCategory) category, NULL,
		   paramf, extra);
      tree = tree->next;
    }
  return true;
}
