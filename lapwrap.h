/*
 * lapwrap.h
 *
 *  Lapack wrapper for lapack library through lapacke c wrapper
 *  Modified from ransac lapwrap.h to support recent lapack 3.4.2 (lapacke instead of lapack) by Siriwat K.
 *
 *  Created on: March 19, 2014
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once

#include <lapacke.h>

#ifndef __LAPWRAP_H__
#define __LAPWRAP_H__


//typedef ptrdiff_t lapack_int;

/* Library of tools wrapping LAPACK utilities and making their usage a bit more comfortable.
   All the matrices are stored row-wise! */

/* Standard (=FULL) SVD */
/* prototype is similar to the one in CCMATH, but V is returned transposed
Compute the singular value transformation A = U*S*V^T.

     int lap_SVD(double *d,double *a,double *u,int m,double *vt,int n)
       d = pointer to double array of dimension n
           (output = singular values of A)
       a = pointer to store of the m by n input matrix A
           (A is altered by the computation)
       u = pointer to store for m by m orthogonal matrix U
       vt= pointer to store for n by n orthogonal matrix V^T
       m = number of rows in A
       n = number of columns in A (m>=n required)
       return value: status flag with:
               0 -> success
               1 -> failed to converge. */
int lap_SVD (double *d, double *a, double *u, lapack_int m, double *vt, lapack_int n);


/* Eigen-decomposition
     Compute the eigenvalues and eigenvectors of a real symmetric
     matrix A.

     void eigen(double *a,double *ev,int n)
     double *a_eivec,*eival; int n;
       a_eivec = pointer to store for symmetric n by n input
           matrix A. The computation overloads this with an
           orthogonal matrix of eigenvectors E.
       eival = pointer to the array of the output eigenvalues
       n = dimension parameter (dim(a_eivec)= n*n, dim(eival)= n)
       return value: status flag with:
               0 -> success
               1 -> failed to converge

     The input and output matrices are related by

          A = E*D*E~ where D is the diagonal matrix of eigenvalues
          D[i,j] = ev[i] if i=j and 0 otherwise.

     The columns of E are the eigenvectors. */
int lap_eig(double *a_eivec,double *eival, lapack_int n);

#endif /* __LAPWRAP_H__ */
//;)
