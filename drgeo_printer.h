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
/*  A printer as found on the system.					*/
/************************************************************************/

#ifndef	DRGEO_PRINTER_H
#define	DRGEO_PRINTER_H

#define	APPprinterPIPE		1
#define	APPprinterTMPFILE	2
#define	APPprinterTOFILE	3

typedef struct PrintDestination
{
  char *pdCommand;
  char *pdPrinterName;
  int pdPrintKind;
  int pdPercentCount;
  int pdCommandLength;
} PrintDestination;

/************************************************************************/
/*  Routine declarations.						*/
/************************************************************************/

extern int utilPrinterGetPrinters (int *pPrinterCount,
				   int *pDefaultPrinter,
				   PrintDestination ** pDestinations,
				   const char *customCommand,
				   const char *customName);

#endif /* DRGEO_PRINTER */
