/* Dr Geo an interactive geometry software
 * (C) Copyright Mark de Does 2003
 * (C) Copyright Hilaire Fernandes  2004
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


/************************************************************************/
/*									*/
/*  Utility functions for printing.					*/
/*									*/
/************************************************************************/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drgeo_printer.h"
// #include "appUtilConfig.h"

/************************************************************************/
/*									*/
/*  Find out what printers are available on the system.			*/
/*									*/
/*  0)  Compare for qsort()ing by name.					*/
/*  1)  BSD/Linux style.						*/
/*  2)  AIX style. ( Thanks to Marc Selis from axi.nl )			*/
/*  3)  SysV/LPRng style.						*/
/*									*/
/*  NOTE: Before the CUPS, all commands were tried. Now that CUPS	*/
/*	implements them both on the same machine, the first one that	*/
/*	returns any printers is assumed to return them all.		*/
/*									*/
/************************************************************************/

/*  0  */
static int
utilPrinterCompareNames (const void *vpd1, const void *vpd2)
{
  const PrintDestination *pd1 = (const PrintDestination *) vpd1;
  const PrintDestination *pd2 = (const PrintDestination *) vpd2;

  return strcmp (pd1->pdPrinterName, pd2->pdPrinterName);
}

/*  2,3 */
static int
utilPrinterSetLpCommand (PrintDestination * pd, const char *name, int l)
{
  pd->pdCommand = (char *) malloc (l + 12);
  if (!pd->pdCommand)
    {
      g_printerr ("No printer command\n");
      // LXDEB (l, pd->pdCommand);
      return -1;
    }
  sprintf (pd->pdCommand, "lp -s -d '%s'", name);
  pd->pdPrintKind = APPprinterPIPE;
  pd->pdPrinterName = (char *) malloc (l + 1);
  if (!pd->pdPrinterName)
    {
      g_printerr ("No printer command\n");
      // LXDEB (l, pd->pdPrinterName);
      return -1;
    }

  pd->pdCommandLength = strlen (pd->pdCommand);
  pd->pdPercentCount = 0;

  strcpy (pd->pdPrinterName, name);

  return 0;
}

/*  1  */
static int
utilPrinterGetLprPrinters (const char *command,
			   int *pDefaultPrinter,
			   int *pPrinterCount,
			   PrintDestination ** pDestinations)
{
  char scratch[250 + 1];

  int l;
  FILE *f;
  int found = 0;

  PrintDestination *fresh;
  int count = *pPrinterCount;
  const char *defaultPrinterName;

  f = popen (command, "r");
  if (!f)
    {
      return 0;
    }

  while (fgets (scratch, 250, f))
    {
      l = strlen (scratch);
      if (l > 0 && scratch[l - 1] == '\n')
	{
	  scratch[l - 1] = '\0';
	}

      if (scratch[0] == '\t' || scratch[l - 2] != ':')
	{
	  continue;
	}

      scratch[l - 2] = '\0';

      fresh = (PrintDestination *) realloc (*pDestinations,
					    (count +
					     1) * sizeof (PrintDestination));
      if (!fresh)
	{
	  g_printerr ("Cannot realloc\n");
	  // XDEB (fresh);
	  return -1;
	}
      *pDestinations = fresh;

      fresh += count;

      fresh->pdCommand = (char *) malloc (l + 23);
      if (!fresh->pdCommand)
	{
      g_printerr ("No printer command\n");
      // LXDEB (l, fresh->pdCommand);
	  return -1;
	}
#	if 0
        Has the advantage that there is no maximum on the file size.
	Has the disadvantage that it does not work on SuSE Linux 6.0
	sprintf (fresh->pdCommand, "lpr -r -s -P '%s' '%s'", scratch, "%s");
#	else
      sprintf (fresh->pdCommand, "lpr -r -P '%s' '%s'", scratch, "%s");
#	endif
      fresh->pdPrintKind = APPprinterTMPFILE;
      fresh->pdPrinterName = strdup (scratch);
      if (!fresh->pdPrinterName)
	{
	  g_printerr ("No printer command\n");
	  // LXDEB (l, fresh->pdPrinterName);
	  return -1;
	}

      fresh->pdCommandLength = strlen (fresh->pdCommand);
      fresh->pdPercentCount = 1;

      found++;
      count++;
    }

  pclose (f);

  qsort (*pDestinations, count, sizeof (PrintDestination),
	 utilPrinterCompareNames);

  defaultPrinterName = getenv ("PRINTER");
  if (defaultPrinterName)
    {
      PrintDestination *pd = *pDestinations;

      for (l = count - found; l < count; pd++, l++)
	{
	  if (!strcmp (defaultPrinterName, pd->pdPrinterName))
	    {
	      *pDefaultPrinter = l;
	      break;
	    }
	}
    }

  *pPrinterCount = count;
  return found;
}

/*  2  */
static int
utilPrinterGetAixPrinters (const char *command,
			   int *pDefaultPrinter,
			   int *pPrinterCount,
			   PrintDestination ** pDestinations)
{
  char scratch[250 + 1];

  int l;
  FILE *f;
  int found = 0;

  PrintDestination *fresh;
  int count = *pPrinterCount;
  const char *defaultPrinterName;

  f = popen (command, "r");
  if (!f)
    {
      return 0;
    }

  if (fgets (scratch, 250, f) && fgets (scratch, 250, f))
    {
      while (fgets (scratch, 250, f))
	{
	  char *s = strchr (scratch, ' ');
	  if (!s)
	    {
	      continue;
	    }
	  *s = '\0';
	  l = s - scratch;

	  fresh = (PrintDestination *) realloc (*pDestinations,
						(count +
						 1) *
						sizeof (PrintDestination));
	  if (!fresh)
	    {
	      g_printerr ("fresh is null\n");
	      //XDEB (fresh);
	      return -1;
	    }
	  *pDestinations = fresh;

	  if (utilPrinterSetLpCommand (fresh + count, scratch, l))
	    {
	      g_printerr ("Can not set printer command\n");
	      // LDEB (l);
	      return -1;
	    }

	  found++;
	  count++;
	}
    }

  pclose (f);

  qsort (*pDestinations, count, sizeof (PrintDestination),
	 utilPrinterCompareNames);

  defaultPrinterName = getenv ("LPDEST");
  if (defaultPrinterName)
    {
      PrintDestination *pd = *pDestinations;

      for (l = count - found; l < count; pd++, l++)
	{
	  if (!strcmp (defaultPrinterName, pd->pdPrinterName))
	    {
	      *pDefaultPrinter = l;
	      break;
	    }
	}
    }

  *pPrinterCount = count;
  return found;
}

/*  3  */
static int
utilPrinterGetLpPrinters (const char *command,
			  int *pDefaultPrinter,
			  int *pPrinterCount,
			  PrintDestination ** pDestinations)
{
  char scratch[250 + 1];

  int l;
  FILE *f;
  int found = 0;

  PrintDestination *fresh;
  int count = *pPrinterCount;
  const char *defaultPrinterName;

  /*  2  */
  f = popen (command, "r");
  if (!f)
    {
      return 0;
    }

  while (fgets (scratch, 250, f))
    {
      char *s = strchr (scratch, ' ');
      if (!s)
	{
	  continue;
	}
      *s = '\0';
      l = s - scratch;

      fresh = (PrintDestination *) realloc (*pDestinations,
					    (count +
					     1) * sizeof (PrintDestination));
      if (!fresh)
	{
	  g_printerr ("fresh is null\n");
	  //  XDEB (fresh);
	  return -1;
	}
      *pDestinations = fresh;

      if (utilPrinterSetLpCommand (fresh + count, scratch, l))
	{
	  g_printerr ("Can not set printer command\n");
	  // LDEB (l);
	  return -1;
	}

      found++;
      count++;
    }

  pclose (f);

  qsort (*pDestinations, count, sizeof (PrintDestination),
	 utilPrinterCompareNames);

  defaultPrinterName = getenv ("LPDEST");
  if (defaultPrinterName)
    {
      PrintDestination *pd = *pDestinations;

      for (l = count - found; l < count; pd++, l++)
	{
	  if (!strcmp (defaultPrinterName, pd->pdPrinterName))
	    {
	      *pDefaultPrinter = l;
	      break;
	    }
	}
    }

  *pPrinterCount = count;
  return found;
}

int
utilPrinterGetPrinters (int *pPrinterCount,
			int *pDefaultPrinter,
			PrintDestination ** pDestinations,
			const char *customCommand, const char *customName)
{
  PrintDestination *pd = (PrintDestination *) 0;
  int count = 0;
  int defaultPrinter = -1;

  /*  1  */
  if (count == 0)
    {
      utilPrinterGetLprPrinters ("( lpc status ) 2>/dev/null",
				 &defaultPrinter, &count, &pd);
    }

  /*  1b  */
  if (count == 0)
    {
      utilPrinterGetLprPrinters ("( /usr/sbin/lpc status ) 2>/dev/null",
				 &defaultPrinter, &count, &pd);
    }

  /*  2  */
  if (count == 0)
    {
      utilPrinterGetAixPrinters ("( enq -As ) 2>/dev/null",
				 &defaultPrinter, &count, &pd);
    }

  /*  3  */
  if (count == 0)
    {
      utilPrinterGetLpPrinters ("( lpstat -a ) 2>/dev/null",
				&defaultPrinter, &count, &pd);
    }

  if (defaultPrinter < 0 && count > 0)
    {
      defaultPrinter = 0;
    }

  if (customName || customCommand)
    {
      if (!customName)
	{
	  g_printerr ("No custom\n");
	  // XXDEB (customName, customCommand);
	}
      if (!customCommand)
	{
	  g_printerr ("No custom\n");
	  // XXDEB (customName, customCommand);
	}
    }

  if (customName && customCommand)
    {
      const char *p;
      PrintDestination *fresh;

      fresh = (PrintDestination *) realloc (pd,
					    (count +
					     1) * sizeof (PrintDestination));
      if (!fresh)
	{
	  g_printerr ("fresh is null\n");
	  //  XDEB (fresh);
	  return -1;
	}
      pd = fresh;

      fresh += count;

      fresh->pdCommand = strdup (customCommand);
      fresh->pdPrinterName = strdup (customName);

      fresh->pdCommandLength = strlen (customCommand);
      fresh->pdPercentCount = 0;

      p = customCommand;
      while (*p)
	{
	  if (p[0] == '%' && p[1] == 'f')
	    {
	      fresh->pdPercentCount++;
	    }
	  p++;
	}

      if (fresh->pdPercentCount > 0)
	{
	  fresh->pdPrintKind = APPprinterTMPFILE;
	}
      else
	{
	  fresh->pdPrintKind = APPprinterPIPE;
	}

      if (fresh->pdCommand && fresh->pdPrinterName)
	{
	  defaultPrinter = count++;
	}
      else
	{
	  g_printerr ("No command or printer name\n");
	  // XXDEB (fresh->pdCommand, fresh->pdPrinterName);
	}
    }

  *pPrinterCount = count;
  *pDefaultPrinter = defaultPrinter;
  *pDestinations = pd;

  return 0;
}

