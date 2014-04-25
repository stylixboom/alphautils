/*
 * lapwrap.cpp
 *
 *  Lapack wrapper for lapack library through lapacke c wrapper
 *  Modified from ransac lapwrap.h to support recent lapack 3.4.2 (lapacke instead of lapack) by Siriwat K.
 *
 *  Created on: March 19, 2014
 *      Author: Siriwat Kasamwattanarote
 */

/* Library of tools wrapping LAPACK utilities and making their usage a bit more comfortable. */

#include "lapwrap.h"

/* DGESVD prototype (LAPACK) */
//extern void dgesvd_( char* jobu, char* jobvt, lapack_int* m, lapack_int* n, double* a,
//                lapack_int* lda, double* s, double* u, lapack_int* ldu, double* vt, lapack_int* ldvt,
//                double* work, lapack_int* lwork, lapack_int* info );

/* Standard (=FULL) SVD */
//char char_all[] = "All";
int lap_SVD (double *d, double *a, double *u, lapack_int m, double *vt, lapack_int n) {
	lapack_int lda = m, ldu = m, ldvt = n, info, lwork;
	double wkopt;
	double *work;
	/* Query and allocate the optimal workspace */
	lwork = -1;
	info = LAPACKE_dgesvd_work(LAPACK_ROW_MAJOR, 'a', 'a', m, n, a, lda, d, u, ldu, vt, ldvt, &wkopt, lwork);
	//dgesvd_( char_all, char_all, &m, &n, a, &lda, d, u, &ldu, vt, &ldvt, &wkopt, &lwork, &info );
	lwork = (lapack_int) wkopt;
	work = (double *) malloc ( lwork*sizeof(double) );
	/* Compute SVD */
	info = LAPACKE_dgesvd_work(LAPACK_ROW_MAJOR, 'a', 'a', m, n, a, lda, d, u, ldu, vt, ldvt, work, lwork);
	//dgesvd_( char_all, char_all, &m, &n, a, &lda, d, u, &ldu, vt, &ldvt, work, &lwork, &info );
	free(work);
	if (info != 0) {
		return 1;
	} else {
		return 0;
	}
}


/* DSYEV prototype */
//extern void dsyev_( char* jobz, char* uplo, lapack_int* n, double* a, lapack_int* lda,
//                double* w, double* work, lapack_int* lwork, lapack_int* info );

/* Eigen-decomposition */
char char_vector[] = "Vectors";
char char_upper[] = "Upper";
int lap_eig(double *a, double *ev, lapack_int n) {
	lapack_int lda = n, info, lwork;
	double wkopt;
	double *work;
	/* Query and allocate the optimal workspace */
	lwork = -1;
	info = LAPACKE_dsyev_work(LAPACK_ROW_MAJOR, 'v', 'u', n, a, lda, ev, &wkopt, lwork);
	//dsyev_( char_vector, char_upper, &n, a, &lda, ev, &wkopt, &lwork, &info );
	lwork = (lapack_int) wkopt;
	work = (double *) malloc ( lwork*sizeof(double) );
	/* Solve eigenproblem */
	info = LAPACKE_dsyev_work(LAPACK_ROW_MAJOR, 'v', 'u', n, a, lda, ev, work, lwork);
	//dsyev_( char_vector, char_upper, &n, a, &lda, ev, work, &lwork, &info );
	free(work);
	if( info != 0 ) {
		return 1;
	} else {
		return 0;
	}
}
//;)

