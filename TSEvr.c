/************************************************************************
Arxeio: TSEvr.c
Syggrafeas: Georgios Kamaras
*************************************************************************/
#include "TSEvr.h"

int TSEvr_setValue (TStoixeiouEvr *target, TStoixeiouEvr source)
{
	*target = source;	// setting the value
	/* if setting has been done correctly, then we get "1" as a return value (else 0) */
	return (!strcmp(target->ICAO, source.ICAO) &&
		!strcmp(target->IATA, source.IATA) &&
		!strcmp(target->City, source.City) &&
		!strcmp(target->Name, source.Name) &&
		!strcmp(target->Country, source.Country));
}

int TSEvr_readValue (FILE *from,  TStoixeiouEvr *Elem)
{
	int r1, r2, r3, r4, r5;		// returned values from fscanf()s' are going to be stored here
	char buf1[5], buf2[4], buf3[100], buf4[100], buf5[100];	// buffers for reading...
																										// ...the values for the various element's attributes

	r1 = fscanf(from, "%[^%]%*c", buf1);	// get ICAO value
	strcpy(Elem->ICAO, buf1);							// copy ICAO value to the element

	r2 = fscanf(from, "%[^%]%*c", buf2);	// get IATA value
	strcpy(Elem->IATA, buf2);							// copy IATA value to the element

	r3 = fscanf(from, "%[^%]%*c", buf3);	// get City
	Elem->City = malloc(100 * sizeof(char));	// allocate memory for element's City
	strcpy(Elem->City, buf3);							// copy City to the element

	r4 = fscanf(from, "%[^%]%*c", buf4);	// get Name
	Elem->Name = malloc(100 * sizeof(char));	// allocate memory for element's Name
	strcpy(Elem->Name, buf4);							// copy Name to the element

	r5 = fscanf(from, "%[^%]%*c", buf5);	// get Country
	Elem->Country = malloc(100 * sizeof(char));	// allocate memory for element's Country
	strcpy(Elem->Country, buf5);							// copy Country to the element

	return (r1 && r2 && r3 && r4 && r5);		// current function's result depends on fscanf()s' results
}

int TSEvr_writeValue(FILE *to, TStoixeiouEvr Elem)
{
	int result;
	/* print element's value */
	result = fprintf(to, "%s\t| %s\t| %s\t| %s\t| %s\n", Elem.ICAO, Elem.IATA, Elem.City, Elem.Name, Elem.Country);

	if(result > 0) return 1;        // no error
	else return 0;                  // some error
}
