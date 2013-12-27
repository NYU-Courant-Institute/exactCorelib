/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/05/2013 07:06:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */
#include "selfassert.h"

#ifndef  UTIL_INC
#define  UTIL_INC

/*  qsort: sort v[left]...v[right] into increasing order */
void qsort_d(void *v[], int left, int right, int (*comp)(void *, void *));

void swap(void *v[], int i, int j);

int fsize(FILE *fp);

#endif   /* ----- #ifndef UTIL_INC  ----- */
