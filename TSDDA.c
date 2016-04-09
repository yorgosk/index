/************************************************************************
Arxeio: TSDDA.c
Syggrafeas: Georgios Kamaras
*************************************************************************/
#include "TSDDA.h"

int TSDDA_setValue (TStoixeiouDDA *target, TStoixeiouDDA source)
{
	*target = source;	// setting the value
	/* if setting has been done correctly, then we get "1" as a return value (else 0) */
	return !strcmp(target->key, source.key) && (target->arrayIndex == source.arrayIndex);
}

int TSDDA_setKey(keyType * key, keyType Val)
{
	strcpy(*key, Val); // setting the value
	/* if setting has been done correctly, then we get "1" as a return value (else 0) */
	return !strcmp(*key, Val);
}

int TSDDA_setIndex(int * Index, int Val)
{
	*Index = Val;
	/* if setting has been done correctly, then we get "1" as a return value (else 0) */
	return *Index == Val;
}

int TSDDA_iso(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	return !strcmp(s1.key, s2.key);	// this way we can find if keys are equal alphabetically
}

int TSDDA_mikrotero(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	return strcmp(s1.key, s2.key) < 0;	// this way we can find if key s1 is less than s2 alphabetically
}

int TSDDA_megalytero(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	return strcmp(s1.key, s2.key) > 0;	// this way we can find if key s1 is greater than s2 alphabetically
}

/* The Following are already Implemented using Above */

int TSDDA_mikrotero_iso(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	return TSDDA_mikrotero(s1,s2) || TSDDA_iso(s1,s2);
}

int TSDDA_megalytero_iso(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	return TSDDA_megalytero(s1,s2) || TSDDA_iso(s1,s2);
}

int TSDDA_diaforo(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
	return !TSDDA_iso(s1,s2);
}
