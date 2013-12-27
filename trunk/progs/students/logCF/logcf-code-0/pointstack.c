/*
 * =====================================================================================
 *
 *       Filename:  pointstack.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 03:00:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"pointstack.h"

#include	<stdlib.h>
#include "memutil.h"
#include	<stdio.h>
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createStack
 *  Description:  
 * =====================================================================================
 */
PointStack*
createStack (void (* del)(void *)  ){
	PointStack *re;

	re	=(PointStack*) malloc_d ( sizeof( PointStack) );
	
	re->capacity=DEFAULT_CAP;

	
	re->data	=(void **) malloc_d ( DEFAULT_CAP*sizeof(void*) );
	
	re->size=-1;
	re->delFun=del;

	return re;
}		/* -----  end of function createStack  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  reScale_S
 *  Description:  
 * =====================================================================================
 */
void
reScale_S ( PointStack *p ){
	p->capacity*=RESCALE+2;
	p->data	=(void **) realloc (p->data,DEFAULT_CAP*sizeof(void*) );
	if ( p->data==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

}		/* -----  end of function reScale_S  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rdelStack
 *  Description:  
 * =====================================================================================
 */
void
delStack ( PointStack *p ){
	int i;
	for ( i = 0; i < p->size; i += 1 ) {
		p->delFun(p->data[i]);
	}
	free(p->data);
	free(p);
}		/* -----  end of function rdelStack  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isEmpty_S
 *  Description:  
 * =====================================================================================
 */
int
isEmpty_S ( PointStack *p ){

	return p->size<0;
}		/* -----  end of function isEmpty_S  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  push_S
 *  Description:  
 * =====================================================================================
 */
void
push_S ( PointStack *p, void * elem ){
	if(p->size==p->capacity) reScale_S(p);

	p->size++;
	p->data[p->size]=elem;

}		/* -----  end of function push_S  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  top_S
 *  Description:  
 * =====================================================================================
 */
void*
top_S ( PointStack *p ){

	if(p->size<0) return NULL;
	else return p->data[p->size];
}		/* -----  end of function top_S  ----- */




/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pop_S
 *  Description:  
 * =====================================================================================
 */
void
pop_S ( PointStack *p ){
	if(p->size>=0)
	  p->size--;

}		/* -----  end of function pop_S  ----- */
