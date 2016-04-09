/************************************************************************
Arxeio: Evretirio.c
Syggrafeas: Georgios Kamaras
*************************************************************************/
#include "Evretirio.h"
#include "TSDDA.h"

#define Simple 1 /* 0 for AVL - any Evr integer for Simple  */

/* Oi diafores tvn ylopoihsevn Simple kai AVL einai mikres - mporeite na xrhsimopoihsete
   thn  domh #if ... #else ...#endif gia na diaforopihsete tis dyo ylopoihseis  */

#if (Simple)
#include "BST/ch8_BSTpointer.h" /* gia Simple Ylopoihsh */
#else
#include "AVL/ch8_AVLpointer.h" /* gia AVL Ylopoihsh */
#endif


struct EvrNode{
	TStoixeiouEvr *DataArray; /* array of size MaxSize */
	int Index;              /* index of first available element in array */
	typos_deikti TreeRoot;     /* Root of DDA */
} EvrNode;


/* assistive function for printing */
/* Recursive sub-program. Traverses with inorded method a BST (or an AVL BST).*/
void endodiataksi(EvrPtr E, typos_deikti riza, int * counter, FILE *to)
{
	assert(E != NULL);	// check and stop if E == NULL
	assert(to != NULL);	// check and stop if to == NULL
	assert(counter != NULL);	// check and stop if counter == NULL

	TStoixeiouDDA periexomeno;	// temporary element to assist writeValue function

#if (Simple)
		if (!Tree_keno(riza))		// if tree is not empty
		{
			endodiataksi(E, Tree_apaidi(riza), counter, to); // left sub-tree
			Tree_periexomeno(riza, &periexomeno);	// get element's value
			TSEvr_writeValue(to, E->DataArray[periexomeno.arrayIndex]);	// write element's value at output file
			(*counter)++;		// increase printed elements' counter
			endodiataksi(E, Tree_dpaidi(riza), counter, to); // right sub-tree
		}
#else
		if (!AVLTree_keno(riza))		// if tree is not empty
		{
			endodiataksi(E, AVLTree_apaidi(riza), counter, to); // left sub-tree
			AVLTree_periexomeno(riza, &periexomeno);	// get element's value
			TSEvr_writeValue(to, E->DataArray[periexomeno.arrayIndex]);	// write element's value at output file
			(*counter)++;		// increase printed elements' counter
			endodiataksi(E, AVLTree_dpaidi(riza), counter, to); // right sub-tree
		}
#endif
}

EvrPtr EvrConstruct(int MaxSize){
/* Oi diafores tvn ylopoihsevn Simple kai AVL einai mikres -
	mporeite na xrhsimopoihsete thn  domh #if ... #else ...#endif */
	assert(MaxSize > 0);		// check and stop if MaxSize <= 0

	EvrPtr Evr;		// a pointer to EvrNode that is about to be created and returned by the function
	if( (Evr = malloc(sizeof(EvrNode))) == NULL )		// dynamically allocate memory for our EvrNode
	{														//...and if there is a malloc problem
		printf("Memory allocation problem!\n");		// report it...
		return NULL;										//...and then return
	}

	typos_deikti TreeRoot;	// a pointer to the root of the tree that is about to be created as a part if EvrNode creation

#if (Simple)
		printf("use simple BST\n");		// which type of tree we use
		Tree_dimiourgia(&TreeRoot);		// create tree
#else
		printf("use AVL BST\n");		// which type of tree we use
		AVLTree_dimiourgia(&TreeRoot);		// create tree
#endif

	Evr->TreeRoot = TreeRoot;			// "link" just-created-tree's rood with the respective attribute of our EvrNode

	if( (Evr->DataArray = malloc(MaxSize * sizeof(TStoixeiouEvr))) == NULL )	// dynamically allocate memory for our tree's data-array
	{														//...and if there is a malloc problem
		printf("Memory allocation problem!\n");		// report it...
		return NULL;										//...and then return
	}

	Evr->Index = 0;			// initiate our tree's insertion-index, with 0 (zero), because our tree is currently empty

	assert(Evr != NULL);	// check and stop if E == NULL

	return Evr;			// return the pointer to the created EvrNode
}

int EvrInsert(EvrPtr E, TStoixeiouEvr Data)
{
	assert(E != NULL);	// check and stop if E == NULL

	int error = 0;
	TStoixeiouDDA Stoixeio;

	strcpy(Stoixeio.key, Data.ICAO);		// key
	Stoixeio.arrayIndex = E->Index;		// index in the array

	TSEvr_setValue(&(E->DataArray[E->Index]), Data);	// insert element in the array
	E->Index++;			// index of next available element in array

#if (Simple)
		Tree_eisagogi(&(E->TreeRoot), Stoixeio, &error);
#else
		int ypsilotero;
		AVLTree_eisagogi(&(E->TreeRoot), Stoixeio, &ypsilotero, &error);
#endif

	return !error;	// return if there was error in the insertion (return 0) or not (return 1)
}

int EvrSearch(EvrPtr E, keyType key, int * found)
{
	assert(E != NULL);	// check and stop if E == NULL
	assert(found != NULL);	// check and stop if found == NULL

	typos_deikti deiktis;			// pointer for the searched tree element (if its found)
	TStoixeiouDDA stoixeio;		// temporary tree-element to assist us with the search and it's related functions

	strcpy(stoixeio.key, key);	// store the key that we search as temporary element's key
	stoixeio.arrayIndex = -1;		// not neccessary to be filled, but better than living it blank

#if (Simple)
		Tree_anazitisi(E->TreeRoot, stoixeio, &deiktis, found);		// search the tree for the element with the requested key
#else
		AVLTree_anazitisi(E->TreeRoot, stoixeio, &deiktis, found);		// search the tree for the element with the requested key
#endif

	assert( *found == 0 || *found == 1 );	// check and stop if *found != 0 AND *found != 1

	return !(deiktis == NULL);	// returns 0 if not found and 1 if found
}

int EvrPrintAll(EvrPtr E, FILE *out, int * counter){
	assert(E != NULL);	// check and stop if E == NULL
	assert(out != NULL);	// check and stop if out == NULL
	assert(counter != NULL);	// check and stop if counter == NULL

	typos_deikti a = E->TreeRoot, d = E->TreeRoot;	// pointers for left and right sub-tree respectively
	int ret;		// variable for the returned value of the writeValue function

	assert( a != NULL || d != NULL);	// check and stop if a == NULL AND d == NULL

	TStoixeiouDDA periexomeno;		// temporary tree-element to assist us with the printing and it's related functions
/* endo-diadromh - use recursion with apaidi-depaidi */
/* example of use */
#if (Simple)
		if(!Tree_keno(a))		// if tree is not empty
		{
			a = Tree_apaidi(a); // left sub-tree
			endodiataksi(E, a, counter, out);	// call function for the inorded printing of the left sub-tree
			Tree_periexomeno(E->TreeRoot, &periexomeno);	// get element's value
			ret = TSEvr_writeValue(out, E->DataArray[periexomeno.arrayIndex]);	// write element's value at output file
			(*counter)++;		// increase printed elements' counter
			d = Tree_dpaidi(d); // right sub-tree
			endodiataksi(E, d, counter, out);	// call function for the inorded printing of the right sub-tree
		}
#else
		if(!AVLTree_keno(a))		// if tree is not empty
		{
			a = AVLTree_apaidi(a); // left sub-tree
			endodiataksi(E, a, counter, out);	// call function for the inorded printing of the left sub-tree
			AVLTree_periexomeno(E->TreeRoot, &periexomeno);	// get element's value
			ret = TSEvr_writeValue(out, E->DataArray[periexomeno.arrayIndex]);	// write element's value at output file
			(*counter)++;		// increase printed elements' counter
			d = AVLTree_dpaidi(d); // right sub-tree
			endodiataksi(E, d, counter, out);	// call function for the inorded printing of the right sub-tree
		}
#endif

	return ret;		// return the result of printing attemp of the "first" (/"top") tree's element
}

int EvrDestruct(EvrPtr *E)
{
	assert(*E != NULL);	// check and stop if *E == NULL

	int i;

	for(i = 0; i < (*E)->Index; i++)
	{
		free( (*E)->DataArray[i].City );
		free( (*E)->DataArray[i].Name );
		free( (*E)->DataArray[i].Country );
	}
	free((*E)->DataArray);

#if (Simple)
		printf("destroy simple BST\n");
		Tree_katastrofi(&((*E)->TreeRoot));
#else
		printf("destroy AVL BST\n");
		AVLTree_katastrofi(&((*E)->TreeRoot));
#endif

	if( (*E)->TreeRoot != NULL ) return 0;	// unsuccessful destruction
	free(*E);
	*E = NULL;

	assert(*E == NULL);	// check and stop if *E != NULL

	return 1;
}
