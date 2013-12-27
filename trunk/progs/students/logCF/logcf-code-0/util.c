/*
 * =====================================================================================
 *
 *       Filename:  util.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 07:08:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
# include "selfassert.h"
#include	"util.h"

void swap(void *v[], int i, int j){
	if(i==j) return ;
	void * temp;
	temp=v[i];
	v[i]=v[j];
	v[j]=temp;
}

/*-----------------------------------------------------------------------------
 *  qsort: sort v[left]...v[right] into increasing order
 *
 *-----------------------------------------------------------------------------*/
void qsort_d(void *v[], int left, int right, int (*comp)(void *, void *)){
	
	int i, last;

	/*-----------------------------------------------------------------------------
	 *  do nothing if arrat contains fewer than two elements
	 *-----------------------------------------------------------------------------*/
	if(left>=right)
	  return ;

	swap(v,left,(left+right)/2);

	last=left;

	for ( i = left+1; i <= right; i ++ ) 
	  if((*comp)(v[i],v[left])<0)
		swap(v,++last,i);
	swap(v,left,last);
	qsort_d(v,left,last-1,comp);
	qsort_d(v,last+1,right,comp);

}

int fsize(FILE *fp)
{

    int prev=ftell(fp);
    fseek(fp,0L,SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET);
    return sz;
}



