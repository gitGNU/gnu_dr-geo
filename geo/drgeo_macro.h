#ifndef DRGEO_MACRO_H
#define DRGEO_MACRO_H

#include <glib.h>

#include "drgeo_figure.h"
#include "macro.h"


enum drgeoMacroMode
{
  drgeoMacroStartMode,
  drgeoMacroInputMode,
  drgeoMacroOutputMode,
  drgeoMacroDescriptionMode,
  drgeoMacroFinishMode
};

class drgeoMacroBuilder
{
  // This class is used to create new macros.

private:
  char *name, *description;
  class drgeoFigure *figure;
  class liste_elem input, output;
  drgeoMacroMode mode;

  // the macro and its kernels
  // return the position of the item kernel in the macro-construction
  // reurn -1 if i can't compute the kernel (this item doesn't have
  // parent in the macro tree
  int kernel_position (macro & M, geometricObject * fig);


public:
  // Constructor.
    drgeoMacroBuilder (drgeoFigure * figure);
   ~drgeoMacroBuilder ();
  void setMode (drgeoMacroMode mode);
  drgeoMacroMode getMode ();
  // Macro input parameters management.
  void addInput (geometricObject * item);
  void clearInput ();

  // Macro output parameters management.
  void addOutput (geometricObject * item);
  void clearOutput ();

  // Add to the current list (input or output).
  void add (geometricObject * item);

  // Name and description of the macro about to be created.
  void setName (const gchar *name);
  gchar *getName ();
  void setDescription (gchar *description);
  gchar *getDescription ();

  // Check that everything is correct, before going for the macro
  // creation.
  bool checkParameters ();

  // Macro creation.
  macro *createMacro ();

};

class drgeoMacroPlayer
{
private:
  class drgeoFigure * figure;
  class macro *selectedMacro;
  // we use the drgeoFigure::selection list to store
  // the selected element
public:
    bool itemsUnderMouseForMacro (drgeoPoint & mouse,
				  liste_elem & underMouse);
    drgeoMacroPlayer (drgeoFigure * figure);
   ~drgeoMacroPlayer ();
  bool setMacro (const char *macroName);
  void setDescription (gchar *description);
  gchar *getDescription ();
  void add (geometricObject * item);
};

#endif
