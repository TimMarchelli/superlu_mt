/*! \file
Copyright (c) 2003, The Regents of the University of California, through
Lawrence Berkeley National Laboratory (subject to receipt of any required 
approvals from U.S. Dept. of Energy) 

All rights reserved. 

The source code is distributed under BSD license, see the file License.txt
at the top-level directory.
*/

/*
 * -- SuperLU routine (version 2.0) --
 * Univ. of California Berkeley, Xerox Palo Alto Research Center,
 * and Lawrence Berkeley National Lab.
 * November 15, 1997
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "slu_mt_sdefs.h"

#define TLEN	80

static void dumptitle();

void
sreadmt(int_t *m, int_t *n, int_t *nonz, float **nzval, int_t **rowind, int_t **colptr)
{
/*
 * Output parameters
 * =================
 *   (a,asub,xa): asub[*] contains the row subscripts of nonzeros
 *	in columns of matrix A; a[*] the numerical values;
 *	row i of A is given by a[k],k=xa[i],...,xa[i+1]-1.
 *
 */
    int    i, k, nnz;
    int    lasta;
    char   title[TLEN];
    float *a;
    int    *asub;
    int    *xa;
    
    /* 	Matrix format:
     *         up to 60 characters           title
     *         integer nrow                  number of rows
     *         integer ncol                  number of cols
     *         integer nonz	             number of nonzeros
     *         (for each column:)
     *               integer nnz             number of nzs in col
     *                       integer index   row index of nonzero
     *                       real value      value of nonzero
     */

    lasta = 0;
    dumptitle(title);
    printf("%s\n", title);

    scanf("%d%d%d", m, n, nonz);
    sallocateA(*n, *nonz, nzval, rowind, colptr); /* Allocate storage */
    a    = *nzval;
    asub = *rowind;
    xa   = *colptr;

    for (i = 0; i < *n; i++) {
	scanf("%d", &nnz);
	xa[i] = lasta;
        for (k = 0; k < nnz; k++) {
	    scanf("%d%f\n", &asub[lasta], &a[lasta]);
	    --asub[lasta];	/* C convention: 0-based indexing */
            lasta++;
        }
    }
    if ( *nonz < lasta ) {
	fprintf(stderr, "nnz inconsistent: *nonz %d, lasta %d\n",*nonz,lasta);
	exit(-1);
    }
    
    xa[*n] = lasta--;

#ifdef CHK_INPUT
    for (i = 0; i < *n; i++) {
	printf("Col %d, xa %d\n", i, xa[i]);
	for (k = xa[i]; k < xa[i+1]; k++)
	    printf("%d %16.11e\n", asub[k], a[k]);
    }
#endif

}

/* Eat up the rest of the current line */
static
void dumptitle(char *title)
{
    int c, i = 0;
	  
    while ((c = getchar()) != '\n') title[i++] = c;
    title[i] = '\0';
}
