/*
 *  Dr Geo an interactive geometry software
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

#include "stdio.h"
#include "mode_obj.h"


// Definition des fonctions membres des classes de creation d'objets
int
buildParametersListForGeometricObject::
inserer_figure (liste_elem & selection, geometricObject * new_fig)
{
  //Insertion d'un objet new_fig dans la liste de selection selection :
  //si l'objet est deja dans la selection, on le supprime de la selection
  //sinon on l'insere
  //Retourne k>=0 si la selection permet la creation d'un category d'objet
  //k correspond a la position du mode de creation de la figure
  //-1 si necessite encore des selections
  char a, b, c, possible, possible1, position, pos;
  objectCategory category_fig;
  category_fig = new_fig->getCategory ();
  if (selection.position ((void *) new_fig) == 0)
    {
      // l'objet n'est pas dans la selection  on regarde s'il convient
      if (utilisable (new_fig))
	{
	  position = selection.ajoute ((void *) new_fig);
	  for (a = 0, c = 0, possible1 = FALSE; a < nb_mode; a++)
	    {
	      // parcours les  modes

	      b = 0;
	      possible = FALSE;
	      while (buildMode[a][b] != NO_OBJECT)
		{
		  // regardre ds un mode les category d'objet
		  if ((buildMode[a][b] & category_fig) != 0
		      && mode[a][b] == NO_OBJECT
		      && cas_possible[a] != NOT_VALID)
		    {
		      possible = possible1 = TRUE;
		      mode[a][b] = (objectCategory) position;
		      break;
		    }
		  b++;
		}
	      if (!possible)
		cas_possible[a] = NOT_VALID;
	      // erreur si l'objet ne convient pas

	      c += cas_possible[a];
	      // VALIDE=1
	      // il y a des cas_possible supprime

	      if (cas_possible[a] != NOT_VALID)
		pos = a;
	      // position du dernier cas valide

	    }
	  if (!possible1)
	    {
	      selection.supprime ((void *) new_fig);
	      return (-1);
	    }
	  if (c == 1)
	    {
	      // Il   n'y a plus qu'un mode de creation possible, on doit verifier
	      // si tout les elem necessaires a ce mode ont ete selectionnes
	      possible = TRUE;
	      b = 0;
	      while (buildMode[pos][b] != NO_OBJECT)
		{
		  if (mode[pos][b] == NO_OBJECT)
		    {
		      possible = FALSE;
		      break;
		    }
		  b++;
		}
	      if (possible)
		return (pos);
	    }
	  else
	    {
	      // verifie parmi les modes encore valides si un n'a pas sa selection complete
	      for (a = 0; a < nb_mode; a++)
		{
		  if (cas_possible[a] == NOT_VALID)
		    continue;
		  b = 0;
		  possible = TRUE;
		  while (buildMode[a][b] != NO_OBJECT)
		    {
		      if (mode[a][b] == NO_OBJECT)
			{
			  possible = FALSE;
			  break;
			}
		      b++;
		    }
		  if (possible)
		    return a;
		}
	    }
	}
      return (-1);
    }
  else
    {
      // l'objet est  dans la liste, on le supprime
      // On reaffiche l'objet dans sa couleur original puisqu'il etait clignotant
      // new_fig->dessine (current_screen, FALSE);
      selection.supprime ((void *) new_fig);
      for (a = 0; a < nb_mode; a++)
	{
	  cas_possible[a] = VALID;
	  b = 0;
	  while (buildMode[a][b] != NO_OBJECT)
	    {
	      mode[a][b] = NO_OBJECT;
	      b++;
	    }
	}
      selection.init_lire ();
      for (position = 1; position < selection.nb_elem; position++)
	{
	  category_fig =
	    ((geometricObject *) selection.lire (0))->getCategory ();
	  for (a = 0; a < nb_mode; a++)
	    {
	      b = 0;
	      possible = FALSE;
	      while (buildMode[a][b] != NO_OBJECT)
		{
		  if ((buildMode[a][b] & category_fig) != 0
		      && mode[a][b] == NO_OBJECT
		      && cas_possible[a] != NOT_VALID)
		    {
		      possible = TRUE;
		      mode[a][b] = (objectCategory) position;
		      break;
		    }
		  b++;
		}
	      if (!possible)
		cas_possible[a] = NOT_VALID;
	    }
	}
      return (-1);
    }
}
int
buildParametersListForGeometricObject::utilisable (geometricObject * fig)
{
  //test si le category de figure passe en parametre convient pour la creation
  //de la figure en cours
  char a, b;
  objectCategory category_fig;
  category_fig = fig->getCategory ();
  for (a = 0; a < nb_mode; a++)
    {
      b = 0;
      while (buildMode[a][b] != NO_OBJECT)
	{
	  if ((buildMode[a][b] & category_fig) != 0
	      && mode[a][b] == NO_OBJECT && cas_possible[a] != NOT_VALID)
	    return (true);
	  b++;
	}
    }
  return (false);
}

int
buildParametersListForGeometricObject::
utilisable1 (geometricObject * fig, liste_elem & selection)
{
  //test si le category de figure passe en parametre convient pour 
  //la creation de la figure en cours,
  //ceci par rapport aux modes de creations initiaux et non actuel
  //l'objet n'est pas dans la selection on regarde s'il convient
  if (selection.position ((void *) fig) == 0 && utilisable (fig))
    return TRUE;
  else
    return FALSE;
}

// buildParametersListForPolygon methods
int
buildParametersListForPolygon::
inserer_figure (liste_elem & selection, geometricObject * new_fig)
{
  /* Description of the method at 
     buildParametersListForGeometricObject::inserer_figure
   */
  objectCategory category_fig;
  gint pos;

  category_fig = new_fig->getCategory ();
  if ((category_fig & POINT) == 0)
    return -1;
  pos = selection.position ((void *) new_fig);
  if (pos == 0)
    selection.ajoute ((void *) new_fig);
  else if (pos == 1)
    {
      // The user close the polygon selecting the 1st point,
      // let's build the polygon
      return 0;
    }
  return -1;
}

int
buildParametersListForPolygon::utilisable (geometricObject * fig)
{
  objectCategory category_fig;

  category_fig = fig->getCategory ();
  if (category_fig & POINT)
    return TRUE;
  else
    return FALSE;
}

int
buildParametersListForPolygon::
utilisable1 (geometricObject * fig, liste_elem & selection)
{
  if (utilisable (fig) &&
      (selection.position ((void *) fig) == 0
       || selection.position ((void *) fig) == 1))
    return TRUE;
  return FALSE;
}

// buildParametersListForScript methods
int
buildParametersListForScript::
inserer_figure (liste_elem & selection, geometricObject * new_fig)
{
  /* Description of the method at 
     buildParametersListForGeometricObject::inserer_figure
   */
  gint pos;

  pos = selection.position ((void *) new_fig);
  if (pos == 0)
    selection.ajoute ((void *) new_fig);
  else if (pos == 1)
    {
      // The user close the selection while selection the 1st object,
      // let's build the script
      return 0;
    }
  return -1;
}

int
buildParametersListForScript::utilisable (geometricObject * fig)
{
  /* every object is useable with script */
  return TRUE;
}

int
buildParametersListForScript::
utilisable1 (geometricObject * fig, liste_elem & selection)
{
  /* useable if 1st in list or not in list at all */
  if (selection.position ((void *) fig) == 0
      || selection.position ((void *) fig) == 1)
    return TRUE;
  return FALSE;
}
