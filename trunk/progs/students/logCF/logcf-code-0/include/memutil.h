/*
 * =====================================================================================
 *
 *       Filename:  memutil.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/07/2013 05:26:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include    <stdio.h>
#include    <stdlib.h>
#ifndef  MEMUTIL_INC
#define  MEMUTIL_INC
/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

    void* malloc_d(const size_t );

    void* realloc_d(void *, const size_t );
    void* calloc_d(size_t num, size_t size);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif   /* ----- #ifndef MEMUTIL_INC  ----- */

