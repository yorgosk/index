/************************************************************************
Arxeio: Evretirio.h
Syggrafeas: Georgios Kamaras
*************************************************************************/
#ifndef __Evr__
#define __Evr__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>   // for the assertions

#include "TSEvr.h"

typedef struct EvrNode *EvrPtr;

EvrPtr EvrConstruct(int MaxSize);
int EvrDestruct(EvrPtr *E);

int EvrInsert(EvrPtr E, TStoixeiouEvr Data);
int EvrSearch(EvrPtr E, keyType key, int * found);
int EvrPrintAll(EvrPtr E, FILE *out, int *counter);

#endif
