/*
 * =====================================================================================
 *
 *       Filename:  root.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 06:53:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  ROOT_INC
#define  ROOT_INC

#include	"pointvector.h"
#include	"pointlist.h"
#include	<gmp.h>
/*-----------------------------------------------------------------------------
 *  left<=right
 *  root \in  [left,right]
 *-----------------------------------------------------------------------------*/

struct root {
	mpq_t left;
	mpq_t right;

};				/* ----------  end of struct root  ---------- */

typedef struct root Root;


Root * createRoot(void );
Root * createRoot_1 (const mpq_t left,const  mpq_t right  );

void delRoot(Root *r);

void negSwap(Root *const root);

int rootCompare(Root *,Root *);

void sortRoot_L(PointList *list);

void sortRoot_V(PointVector *);

int gcdint(int a, int b);

char*  getStr_L(  PointList * ,char * , int *);

void printRoots(const PointList *);


#endif   /* ----- #ifndef ROOT_INC  ----- */
