/************************************************************************
Arxeio: TSEvr.h
Syggrafeas: Georgios Kamaras
*************************************************************************/
#ifndef __TSEvr__
#define __TSEvr__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char keyType[5];

typedef struct data {
	keyType ICAO;
	char IATA[4];
	char *City;
	char *Name;
	char *Country;
} TStoixeiouEvr;

int TSEvr_setValue (TStoixeiouEvr *target, TStoixeiouEvr source);

int TSEvr_readValue (FILE *from,  TStoixeiouEvr *Elem);

int TSEvr_writeValue(FILE *to, TStoixeiouEvr Elem);

#endif
