/************************************************************************
Programma pelaths	: AirportManagement.c
Syggrafeas			: Georgios Kamaras
Skopos				: skeleton main for askhsh 3
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>               /* for gettimeofday() */
#include <math.h>										/* for pow() */

#include "TSEvr.h"
#include "Evretirio.h"

/* functions for the core exercise's objectives, in order to shorten main()'s length */
void obj2Insertion(EvrPtr E, char *filename);
void obj3Search(EvrPtr E, char *filename);
void obj4SearchFromFound(EvrPtr E, char *filename);
void obj5PrintAlphabetically(EvrPtr E, char *filename);

int main(int argc, char *argv[])
{
	EvrPtr E = NULL;		// our pointer for accessing EvrNode
	int destr;					// variable for the return value of desctruction function, for error diagnosing
							/* PART 1 */
	/* OBJECTIVE 1: CREATION */
	E = EvrConstruct(5000);
	/* OBJECTIVE 2: INSERTION */
	obj2Insertion(E, "Linux Input/ICAO_Random_Input-Linux.txt");
	/* OBJECTIVE 3: SEARCH FROM ICAO_SEARCH-Linux.txt */
	obj3Search(E, "Linux Input/ICAO_FOUND-Linux.txt");
	/* OBJECTIVE 4: SEARCH FROM ICAO_FOUND-Linux.txt */
	obj4SearchFromFound(E, "Linux Input/ICAO_FOUND-Linux.txt");
	/* OBJECTIVE 5: PRINT ALPHABETICALLY AT AIRPORTS.txt */
	obj5PrintAlphabetically(E, "Linux Input/AIRPORTS.txt");
	/* OBJECTIVE 6: DESTRUCTION */
	destr = EvrDestruct(&E);
	if(!destr) printf("There was an error at destroying the EvrNode or the tree\n"); // if error, tell us
							/* PART 2 */
	printf("\nNow, we will do all the above, but with input from ICAO_Sorted_Input-Linux.txt\n\n");
	/* OBJECTIVE 7: ALL THE ABOVE, BUT WITH SORTED INPUT */
	E = EvrConstruct(5000);			/* 1: Creation */
	obj2Insertion(E, "Linux Input/ICAO_Sorted_Input-Linux.txt");	/* 2: Insertion */
	obj3Search(E, "Linux Input/ICAO_FOUND_Sorted-Linux.txt");		/* 3: Search */
	obj4SearchFromFound(E, "Linux Input/ICAO_FOUND_Sorted-Linux.txt");		/* 4: Search from the ones already found */
	obj5PrintAlphabetically(E, "Linux Input/AIRPORTS_Sorted.txt");		/* 5: Print alphabetically */
	destr = EvrDestruct(&E);		/* 6: Destruction */
	if(!destr) printf("There was an error at destroying the EvrNode or the tree\n"); // if error, tell us

	getchar();		// user must press a key for the program to terminate
	return 0;
}

/* OBJECTIVE 2: INSERTION */
void obj2Insertion(EvrPtr E, char *filename)
{
	int inserted = 0, not_inserted = 0, ret;	// counters and variable to check the returned result of the insertion's function
	int line = 0, phase1, phase2, phase3, phase4;	// line's (insertion file's elements) counter and printing phases for time evaluation
	char ch;		// the read character's variable
	TStoixeiouEvr Elem;		// temporary element to pass to the readValue function
	FILE *fp;			// the file from which we read
	/* for time-counting */
	struct timeval t_init, t_fin, t_mid, t1, t2;
	double elapsedTime, curTime, averageTime;
	/* calculate printing phases for time evaluation */
	phase1 = pow(2, 9) - 1;
	phase2 = pow(2, 10) - 1;
	phase3 = pow(2, 11) - 1;
	phase4 = pow(2, 12) - 1;
	/* start timer */
	gettimeofday(&t_init, NULL);

	fp = fopen(filename, "r");		// open file for reading

	while( (ch = fgetc(fp)) != EOF )		// loop until we reach the end of file
	{
		if( ch == '\n' ) ch = fgetc(fp);	// if new-line-character take next character
		if( ch != EOF )			// if we haven't reach the end of file
		{
			ungetc(ch, fp);		// we still need to input that character
			/* start current insertion's timer */
			gettimeofday(&t1, NULL);
			/* insert */
			TSEvr_readValue(fp, &Elem);		// read element
			ret = EvrInsert(E, Elem);			// attempt to insert the just-read element
			/* stop current insertion's timer */
			gettimeofday(&t2, NULL);
			/* compute the current insertion's time in millisec */
			curTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
			curTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
			line++;			// increase line's (insertion file's elements) counter
			if(ret)		// if inserted
			{
				inserted++;	// increase the inserted's counter
				/* if we have reached one time-report-point */
				if( line == phase1 || line == phase2 || line == phase3 || line == phase4 )
				{
					/* time-now, so we can compute the current elapsed time */
					gettimeofday(&t_mid, NULL);
					/* compute the elapsed time in millisec */
					elapsedTime = (t_mid.tv_sec - t_init.tv_sec) * 1000.0;      // sec to ms
					elapsedTime += (t_mid.tv_usec - t_init.tv_usec) / 1000.0;   // us to ms
					/* on-screen message */
					printf("Inserted %d elements, at a time of %g msec\n", line, elapsedTime);
				}
			}
			else		// if not inserted
			{
				not_inserted++;	// increase the not-inserted's counter
				/* on-screen message */
				printf("Insertion error at %4d input line!\tDiagnose: Most probably the element already exists!\n", line);
			}
		}
	}
	fclose(fp);		// close file for reading

	/* compute the average insertion's time */
	averageTime = curTime / (inserted + not_inserted);

	/* stop timer */
	gettimeofday(&t_fin, NULL);
	/* compute the elapsed time in millisec */
	elapsedTime = (t_fin.tv_sec - t_init.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t_fin.tv_usec - t_init.tv_usec) / 1000.0;   // us to ms

	/* screen output for the results of the insertion */
	printf("Inserted %d data in total, at a time of %g msec , with average insertion time %g msec\n", inserted, elapsedTime, averageTime);
}

/* OBJECTIVE 3: SEARCH FROM ICAO_SEARCH-Linux.txt */
void obj3Search(EvrPtr E, char *filename)
{
	int found, exist = 0, not_exist = 0;	// variable for the returned result of search function and existence counters
	char ch;		// the read character's variable
	keyType key;		// temporary key to pass to the search function
	FILE *sinp, *soutp;		// the files for search-input and search-output (respectively)
	/* for time-counting */
	struct timeval t_init, t_fin, t1, t2;
	double elapsedTime, curTime, averageTime;
	/* start timer */
	gettimeofday(&t_init, NULL);

	sinp = fopen("Linux Input/ICAO_SEARCH-Linux.txt", "r");	// open file for reading
	soutp = fopen(filename, "w");				// open file for writing

	while( (ch = fgetc(sinp)) != EOF )		// loop until we reach the end of file
	{
		if( ch == '\n' ) ch = fgetc(sinp);	// if new-line-character take next character
		if( ch != EOF )			// if we haven't reach the end of file
		{
			ungetc(ch, sinp);		// we still need to input that character
			fscanf(sinp, "%[^\n]%*c", key);		// read key's value
			/* start current search's timer */
			gettimeofday(&t1, NULL);
			/* search */
			EvrSearch(E, key, &found);
			/* stop current search's timer */
			gettimeofday(&t2, NULL);
			/* compute the current search's time in millisec */
			curTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
			curTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

			if(found)		// if key exists
			{
				fprintf(soutp, "%s\n", key);		// write key to search-output's file
				exist++;		// increase "exist" counter
			}
			else not_exist++;	// if key doesn't exist increase "not_exist" counter
		}
	}
	fclose(sinp);		// close search-input file
	fclose(soutp);	// close search-output file

	/* compute the average search's time */
	averageTime = curTime / (exist + not_exist);

	/* stop timer */
	gettimeofday(&t_fin, NULL);
	/* compute the elapsed time in millisec */
	elapsedTime = (t_fin.tv_sec - t_init.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t_fin.tv_usec - t_init.tv_usec) / 1000.0;   // us to ms

	/* screen output for the results of the search */
	printf("\nFound: %d ICAOs\nNot Found: %d ICAOs\nAt a time of %g msec , with average search time %g msec\n", exist, not_exist, elapsedTime, averageTime);
}

/* OBJECTIVE 4: SEARCH FROM ICAO_FOUND-Linux.txt */
void obj4SearchFromFound(EvrPtr E, char *filename)
{
	int found, exist = 0, not_exist = 0;	// variable for the returned result of search function and existence counters
	char ch;		// the read character's variable
	keyType key;		// temporary key to pass to the search function
	FILE *fp, *fout;		// the files for search-input and search-output (respectively)
	/* for time-counting */
	struct timeval t_init, t_fin, t1, t2;
	double elapsedTime, curTime, averageTime;
	/* start timer */
	gettimeofday(&t_init, NULL);

	fp = fopen(filename, "r");	// open file for reading
	if( !strcmp(filename, "Linux Input/ICAO_FOUND-Linux.txt") )	// if we are on the unsorted part of the exercise
		fout = fopen("Linux Input/ICAO_FOUND_2-Linux.txt", "w");	// open file for writing
	else	// if we are on the sorted part of the exercise
		fout = fopen("Linux Input/ICAO_FOUND_Sorted_2-Linux.txt", "w");	// open file for writing

	while( (ch = fgetc(fp)) != EOF )		// loop until we reach the end of file
	{
		if( ch == '\n' ) ch = fgetc(fp);	// if new-line-character take next character
		if( ch != EOF )			// if we haven't reach the end of file
		{
			ungetc(ch, fp);		// we still need to input that character
			fscanf(fp, "%[^\n]%*c", key);		// read key's value
			/* start current search's timer */
			gettimeofday(&t1, NULL);
			/* search */
			EvrSearch(E, key, &found);
			/* stop current search's timer */
			gettimeofday(&t2, NULL);
			/* compute the current search's time in millisec */
			curTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
			curTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

			if(found)		// if key exists
			{
				fprintf(fout, "%s\n", key);		// write key to search-output's file
				exist++;		// increase "exist" counter
			}
			else not_exist++;	// if key doesn't exist increase "not_exist" counter
		}
	}
	fclose(fp);			// close search-input file
	fclose(fout);		// close search-output file

	/* compute the average search's time */
	averageTime = curTime / (exist + not_exist);

	/* stop timer */
	gettimeofday(&t_fin, NULL);
	/* compute the elapsed time in millisec */
	elapsedTime = (t_fin.tv_sec - t_init.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t_fin.tv_usec - t_init.tv_usec) / 1000.0;   // us to ms

	/* screen output for the results of the search */
	printf("\nFound: %d ICAOs\nNot Found: %d ICAOs\nAt a time of %g msec , with average search time %g msec\n", exist, not_exist, elapsedTime, averageTime);
}

/* OBJECTIVE 5: PRINT ALPHABETICALLY AT AIRPORTS.txt */
void obj5PrintAlphabetically(EvrPtr E, char *filename)
{
	int counter = 0;	// elements to be printed
	FILE *out;
	/* for time-counting */
	struct timeval t_init, t_fin;
	double elapsedTime;

	/* start timer */
	gettimeofday(&t_init, NULL);

	out = fopen(filename, "w");			// open file for writing
	EvrPrintAll(E, out, &counter);	// do the printing
	fclose(out);			// close file for writing

	/* stop timer */
	gettimeofday(&t_fin, NULL);
	/* compute the elapsed time in millisec */
	elapsedTime = (t_fin.tv_sec - t_init.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t_fin.tv_usec - t_init.tv_usec) / 1000.0;   // us to ms

	/* screen output for the results of the alphabetical printing */
	printf("\nPrinted %d data alphabetically in %s , at a time of %g msec\n\n", counter, filename, elapsedTime);
}
