/*
 * =====================================================================================
 *
 *       Filename:  root.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/04/2013 08:31:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"root.h"
#include	"pointlist.h"
#include    "memutil.h"
#include	<string.h>
#include	"pointvector.h"
#include	<assert.h>
#include	<stdio.h>

#include	<stdlib.h>
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createRoot_1
 *  Description:  
 * =====================================================================================
 */
Root*
createRoot ( void ){
	Root *re;

	re	=(Root*) malloc_d ( sizeof(Root) );
	
	mpq_init(re->left);
	mpq_init(re->right);

	return re;
}		/* -----  end of function createRoot_1  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  createRoot_1
 *  Description:  
 * =====================================================================================
 */
Root*
createRoot_1 (const mpq_t left,const  mpq_t right ){

	Root *re=createRoot();
	mpq_set(re->left,left);
	mpq_set(re->right,right);
	return re;
}		/* -----  end of function createRoot_1  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delRoot
 *  Description:  
 * =====================================================================================
 */
void
delRoot ( Root *r )
{
	if(r==NULL) return;
	mpq_clear(r->left);
	mpq_clear(r->right);
	free(r);

}		/* -----  end of function delRoot  ----- */

void negSwap(Root *const  root){

	
	mpq_swap(root->left,root->right);
	mpq_neg(root->left,root->left);
	mpq_neg(root->right,root->right);

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rootCompare
 *  Description:  
 * =====================================================================================
 */

 int rootCompare( Root *  a,  Root * b  ){
	return mpq_cmp(a->left,b->left);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sortRoot
 *  Description:  
 * =====================================================================================
 */
void
sortRoot_L( PointList* list ){

	sortList(list,(int (*)(void *, void *)) rootCompare);

}		/* -----  end of function sortRoot  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sortRoot_V
 *  Description:  
 * =====================================================================================
 */
void
sortRoot_V ( PointVector *v ){

	 sortVector(v, (int (*)(void *, void *)) rootCompare);
}		/* -----  end of function sortRoot_V  ----- */

int gcdint(int a, int b){
	assert(a>=0&&b>=0);
	if(a<0||b<0) return 0;
	if(a==0) return b;
	if (b==0) return a;
	if(a==1) return 1;
	if(b==1) return 1;
	if(b>a){
		int temp=a;
		a=b;
		b=temp;
	}
	int c=a%b;
	while(c!=0){
		a=b;
		b=c;
		c=a%b;
	}

	return b;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getStr_L
 *  Description:  
 * =====================================================================================
 */
char*
getStr_L ( PointList *list ,char *re, int *cap ){

	int rescale=2;
	int len;
	int size=0;

/* 	char *re;
 * 
 * 	re	=(char*) malloc ( cap*sizeof(char) );
 * 	if ( re==NULL ) {
 * 		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
 * 		exit (EXIT_FAILURE);
 * 	}
 * 
 */
	

	re[0]='\0';
	if(NULL== list->head){
		strcat(re,"[ ]");
		return re;
	}

	re=strcat(re,"[");

	PointElem* elem=list->head;

	while(elem!=NULL){

		mpq_canonicalize(((Root*)elem->value)->left);
		mpq_canonicalize(((Root*)elem->value)->right);
		
		if(elem!=list->head)
			re=strcat(re,", [");
		else 
			re=strcat(re,"[");
		size=strlen(re);


		char * lefts=mpq_get_str(NULL,10,((Root*)elem->value)->left);
		len=strlen(lefts);
		if(size+len+10>(*cap)){

			(*cap)=(*cap)*rescale+size+len+10-(*cap);
			re=(char*) realloc_d(re, (*cap)*sizeof(char));

		
		}
		re=strcat(re,lefts);
		free(lefts);
		re=strcat(re,", ");
		size=strlen(re);
		char * rights=mpq_get_str(NULL,10,((Root*)elem->value)->right);
		len=strlen(rights);
		if(size+len+10>(*cap)){

			(*cap)=(*cap)*rescale+size+len+10-(*cap);
			re=(char*) realloc_d(re, (*cap)*sizeof(char));
        }
		re=strcat(re,rights);
		free(rights);

		re=strcat(re,"]");

		elem=elem->next;
	}
	re=strcat(re,"]");
	return re;


}		/* -----  end of function getStr_L  ----- */

void printRoots(const PointList *list)
{
    
//	int rescale=2;
//	int len;
//	int size=0;
	

	//re[0]='\0';
	if(NULL== list->head){
		printf("[ ]\n");
		return ;
	}
    int num=0;
    

	printf("[");

	PointElem* elem=list->head;

	while(elem!=NULL){
        num++;
        mpq_canonicalize(((Root*)elem->value)->left);
		mpq_canonicalize(((Root*)elem->value)->right);
		
		if(elem!=list->head)
			printf(", [");
		else 
			printf("[");
		//size=strlen(re);


		char * lefts=mpq_get_str(NULL,10,((Root*)elem->value)->left);
		
		printf("%s",lefts);
		free(lefts);
		printf(", ");
		char * rights=mpq_get_str(NULL,10,((Root*)elem->value)->right);
		
		printf("%s",rights);
		free(rights);

		printf("]");

		elem=elem->next;
	}
	printf("]\n");
    printf("%d real roots\n",num);
    
	


}

