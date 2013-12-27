/*
 * =====================================================================================
 *
 *       Filename:  lcf.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/05/2013 09:51:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include    "lcf.h"
# include "selfassert.h"
#include    <ctype.h>
#include    "memutil.h"
#include    <math.h>
#include    <string.h>
#include "util.h"
static mpz_t  root_num, root_den, root_prec, root_base, root_remNum, root_times, root_temp, root_tempB, root_cir,root_mul,coeff_sum,preSum;
static  mpz_t *absA;
static  mpz_t *tempCoeff;

static  mpq_t root_up,root_max, root_lb, root_tempUp,root_qtemp,root_qtempB,root_qtimes,root_left,root_right, root_LEFT,root_RIGHT;

static  int *locate;

static  unsigned long int* timeused;

static  char* str_buffer;
static  int str_cap;

static int  root_init=0;      /* 0 denote that the system varibales hasn't been initialization */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  initVars
 *  Description:
 * =====================================================================================
 */
void
initVars (  )
{

    if(!root_init){
        root_init=1;
        int pow=100;
        mpz_init(root_times);
        mpz_init(root_prec);
        mpz_init(root_base);
        mpz_init(root_num);
        mpz_init(root_den);
        mpz_init(root_remNum);
        mpz_init(root_temp);
        mpz_init(root_tempB);
        mpz_init(root_cir);
        mpz_init(root_mul);
        mpz_init(coeff_sum);
        mpz_init(preSum);
        mpz_set_si(root_base,10);
        mpz_set_si(root_cir,16);
        mpz_pow_ui(root_prec,root_base,pow);
        mpq_init(root_up);
        mpq_init(root_lb);
        mpq_init(root_max);
        mpq_init(root_tempUp);
        mpq_init(root_qtemp);
        mpq_init(root_qtempB);
        mpq_init(root_qtimes);
        mpq_init(root_left);
        mpq_init(root_right);
        mpq_init(root_LEFT);
        mpq_init(root_RIGHT);

        str_cap=512;
        str_buffer  =(char* ) malloc_d (1024* sizeof(char) );


    }
}       /* -----  end of function initVars  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  clearVars
 *  Description:
 * =====================================================================================
 */
void
clearVars (  ){
    if(root_init){
        root_init=0;
        mpz_clear(root_times);
        mpz_clear(root_prec);
        mpz_clear(root_base);
        mpz_clear(root_num);
        mpz_clear(root_den);
        mpz_clear(root_remNum);
        mpz_clear(root_temp);
        mpz_clear(root_tempB);
        mpz_clear(root_cir);
        mpz_clear(root_mul);
        mpz_clear(coeff_sum);
        mpz_clear(preSum);
        mpq_clear(root_up);
        mpq_clear(root_lb);
        mpq_clear(root_max);
        mpq_clear(root_tempUp);
        mpq_clear(root_qtemp);
        mpq_clear(root_qtempB);
        mpq_clear(root_qtimes);
        mpq_clear(root_left);
        mpq_clear(root_right);
        mpq_clear(root_LEFT);
        mpq_clear(root_RIGHT);
        free(str_buffer );

    }

}       /* -----  end of function clearVars  ----- */
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  readMpz
 *  Description:
 * =====================================================================================
 */
int
readMpz ( mpz_t v,const char *str, int *start ){

    if(str[*start]=='\0'){
        fprintf ( stderr, "\n aread reach the last chacter\n" );
        return 0;
    }

    int end=*start+1;
    while(str[end]!='\0'&&str[end]!=','){       /* use ',' to seperaton different coeffs */
        if(!isdigit(str[end]))  {

            fprintf ( stderr, "\nineteger format error %s\n",str+*start );
            return 0;
        }

        end++;
    }

    char *temp;

    temp    =(char*) malloc_d ( sizeof(char)*(end-(*start)+1) );


    strncpy(temp, str+*start,end-*start);
    temp[end-*start]='\0';

    mpz_set_str(v,temp,10);
    free(temp);
    *start=end+1;                               /* modify start location */

    return 1;
}       /* -----  end of function readMpz  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  nroot
 *  Description:  x^n=-b/a; solve the positive root.
 *  a ,b is mpq_t
 * =====================================================================================
 */
void
nroot_1 ( mpq_t re, mpq_t a, mpq_t b, unsigned long int used, unsigned long int n )
{
    mpz_ui_pow_ui(root_times,2,used);
    mpq_set_z(root_qtimes,root_times);
    mpq_neg(root_qtempB,b);
    mpq_mul(root_qtempB,root_qtempB,root_qtimes);
    mpq_div(root_qtemp,root_qtempB,a);
    mpq_get_num(root_num,root_qtemp);
    mpq_get_den(root_den,root_qtemp);
    mpz_mul(root_num,root_num,root_prec);
    mpz_mul(root_den,root_den,root_prec);
    mpz_rootrem(root_num,root_remNum,root_num,n);
    if(mpz_sgn(root_remNum)>0){
        mpz_add_ui(root_num,root_num,1);
    }

    mpz_root(root_den,root_den,n);
    mpq_set_num(re,root_num);
    mpq_set_den(re,root_den);

}


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  nroot
 *  Description:   x^n=-b/a; solve the positive root.
 *
 *  a, b is mpz_t

 * =====================================================================================
 */
void
nroot_2 ( mpq_t re, mpz_t a, mpz_t b,  unsigned long int used, unsigned long int n )
{
/*  mpz_mul(root_num,b,root_times);*/
    mpz_mul_2exp(root_num,b,used);
    /*-----------------------------------------------------------------------------
     *elimination the both diver
     *-----------------------------------------------------------------------------*/
    mpz_rootrem(root_num,root_remNum,root_num,n);
    if(mpz_sgn(root_remNum)>0){
        mpz_add_ui(root_num,root_num,1);
    }
    mpz_root(root_den,a,n);
    mpq_set_num(re,root_num);
    mpq_set_den(re,root_den);

}       /* -----  end of function nroot  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  root
 *  Description:
 * =====================================================================================
 */
void
root (mpq_t re, mpq_t q, unsigned long int mul, unsigned long int n, int r )
{
    mpq_get_num(root_num,q);
    mpq_get_den(root_den,q);
    mpz_abs(root_num,root_num);
    mpz_abs(root_den,root_den);

    mpz_mul_2exp(root_num,root_num,mul);
    mpz_mul_2exp(root_den,root_den,mul);

    if((mpq_sgn(q)==1&&r)||(mpq_sgn(q)==-1&&!r)){
        mpz_rootrem(root_num,root_remNum,root_num,n);
        if(mpz_sgn(root_remNum)>0){
            mpz_add_ui(root_num,root_num,1);
        }
        mpz_root(root_den,root_den,n);
    }else{
        mpz_rootrem(root_den,root_remNum,root_den,n);
        if(mpz_sgn(root_remNum)>0){
            mpz_add_ui(root_den,root_den,1);
        }
        mpz_root(root_num,root_num,n);

    }
    mpq_set_num(re,root_num);
    mpq_set_den(re,root_den);
    if(mpq_sgn(q)==-1){
        mpq_neg(re,re);
    }

}       /* -----  end of function root  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  pow
 *  Description:
 * =====================================================================================
 */
void
pow_Q (mpq_t re, mpq_t q,unsigned long int n )
{
    mpq_get_num(root_num,q);
    mpq_get_den(root_den,q);
    mpz_pow_ui(root_num,root_num,n);
    mpz_pow_ui(root_den,root_den,n);
    mpq_set_num(re,root_num);
    mpq_set_den(re,root_den);

}       /* -----  end of function pow  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  copySearchPoly
 *  Description:
 * =====================================================================================
 */
SearchPoly*
copySearchPoly (const  SearchPoly *rhs ){

    SearchPoly* re;

    re  =(SearchPoly *) malloc_d ( sizeof(SearchPoly) );


    re->times=rhs->times;
    re->deg=rhs->deg;
    re->size=re->deg+1;

    re->coeff   =(mpz_t *) malloc_d (re->size* sizeof(mpz_t) );



    re->sign    =(int *) malloc_d ( re->size*sizeof(int) );


    re->index   =(int*) malloc_d (re->size* sizeof(int) );

    mpz_init(re->a);
    mpz_set(re->a,rhs->a);
    mpz_init(re->b);
    mpz_set(re->b,rhs->b);
    mpz_init(re->c);
    mpz_set(re->c, rhs->c);
    mpz_init(re->d);
    mpz_set(re->d, rhs->d);

    re->ch=rhs->ch;

    int i;

    for ( i = 0; i < re->size; i += 1 ) {
        mpz_init(re->coeff[i]);
        re->index[i]=i;
    }


    for ( i = 0; i < re->size; i += 1 ) {
        mpz_set(re->coeff[i], rhs->coeff[rhs->index[i]]);
        re->sign[i]=rhs->sign[rhs->index[i]];
    }

    return re;
}       /* -----  end of function copySearchPoly  ----- */



SearchPoly * createSearchPoly2f(const char * file)
{
    FILE *fin=fopen(file,"r");
    if(NULL==fin){
        PRINT_VALUE("error open file");
        exit(1);

    }
    enum STATE
    {
        VAR=1,
        COEF=2,
        INDICES=3,
    };


    enum STATE state=COEF;
    //  1 var, 2 coef,3 indices

    int bufSize=fsize(fin);
    int termSize=0;

    char buffer[bufSize];

    char contBuff[bufSize];
    int i,j,k;
    int size=10;

    int *exp=(int*)malloc_d(size*sizeof(int));

    mpz_t * tempCoef=(mpz_t *) malloc_d(size*sizeof(mpz_t));

    for(i=0;i<size;i++){
        mpz_init(tempCoef[i]);
        mpz_set_si(tempCoef[i],1);
        exp[i]=0;

    }
    j=0;

    bufSize=fread(contBuff,1,bufSize,fin);
    
    i=0;
    k=0;
    while(i<bufSize){
        if(' '!=contBuff[i])                /* remove space */
            contBuff[k++]=contBuff[i];
        i++;
    }
    contBuff[k]='\0';
    bufSize=k;
   
    k=0;
    i=0;

    while(i<bufSize){// remove comment

		if('#'==contBuff[i]) break;
		i++;
        
    }
	contBuff[i]='\0';
	bufSize=i+1;
//	printf ( "%s\n",contBuff );
	i=0;
    
    k=0;
    while(i<bufSize){//remove newline
        if('\n'!=contBuff[i] ) contBuff[k++]=contBuff[i];
        i++;
    }
    // printf("%d\n",k);
    
    contBuff[k]='\0';
    
    i=0;
    k=0;
    j=0;
    

    while('\0'!=contBuff[i]&& contBuff[i]!=';' ){

        if(COEF==state){
            if(0==j){
                if('-'==contBuff[i])
                    buffer[j++]=contBuff[i++];
                else if('+'==contBuff[i]) i++;

            }

            while(isdigit(contBuff[i])){
                buffer[j++]=contBuff[i++];
            }
            if('\0'!=contBuff[i]){

                state=VAR;

                buffer[j]='\0';

                if (j==1&& '-'==buffer[0]){
                    mpz_set_si(tempCoef[termSize],-1);

                }else  {
//                      printf("%s\n",buffer);

                    mpz_set_str(tempCoef[termSize],buffer,10);

                }

                if('*'==contBuff[i]) i++;


            }

        }
        if (VAR==state){
            if(isalpha(contBuff[i]))
                exp[termSize]=1;


            while(isalpha(contBuff[i]))i++;
            if(contBuff[i]!='\0'){
                j=0;

                state =INDICES;
            }


        }
        if (INDICES==state){
            if('^'==contBuff[i]){
                i++;
                while(isdigit(contBuff[i])) buffer[j++]=contBuff[i++];
                if('\0'!=contBuff[i] &&j>0 ){
                    buffer[j]='\0';


                    exp[termSize]=atoi(buffer);
                    state=COEF;

                }

            }else {

                state=COEF;

            }
            if(COEF==state){

                termSize++;

                if(termSize>= size){
                    size*=2;
                    tempCoef=(mpz_t*)realloc_d(tempCoef, size*sizeof(mpz_t));
                    exp=(int*) realloc_d(exp, size*sizeof(int));
                    for(k=termSize;k<size;k++){
                        mpz_init(tempCoef[k]);
                        mpz_set_si(tempCoef[k],1);
                        exp[k]=0;
                    }
                }
                j=0;

            }

        }

    }
//    if(';'==contBuff[i]) break;


    //free(buffer);

    fclose(fin);

    int s=1;
    int temp;


    while(s){

        s=0;

        for( i=termSize-1;i>=1;i--){;

            if(exp[i]>exp[i-1]){
                s=1;
                temp=exp[i];
                exp[i]=exp[i-1];
                exp[i-1]=temp;
                mpz_swap(tempCoef[i],tempCoef[i-1]);
            }

        }
    }


    SearchPoly *re;
    re=(SearchPoly *) malloc_d(sizeof(SearchPoly));

    re->deg=exp[0];
    re->times=re->deg;

    re->size=re->deg+1;
    re->coeff=(mpz_t*) malloc(re->size *sizeof(mpz_t));
    re->sign=(int*) calloc_d(re->size, sizeof(int));
    re->index=(int*)malloc_d(re->size*sizeof(int));
    int *index=re->index;
    int *sign=re->sign;
    mpz_t * coeff=re->coeff;
    mpz_init(re->a);
    mpz_set_si(re->a,1);
    mpz_init(re->b);
    mpz_init(re->c);
    mpz_init(re->d);
    mpz_set_si(re->d,1);

    for(i=0;i<re->size; i++){
        mpz_init(coeff[i]);

        index[i]=i;
    }

    re->ch=0;
    int pre=1;
    int indice=0;

    int last=re->deg;
    int cha;

    for(int i=0;i<termSize;i++){

        if(0==i){
            if(-1==mpz_sgn(tempCoef[0])) pre=0;
        }
        indice=exp[i];


        if(re->times>1){
            cha=last-indice;
            last=indice;

            re->times=gcdint(re->times,cha);
        }

        mpz_set(re->coeff[indice], tempCoef[i]);

        if(1==mpz_sgn(tempCoef[i])){
            sign[indice]=1;
            if(!pre){
                pre=1;
                re->ch++;
            }
        }
        else if (-1==mpz_sgn(tempCoef[i])){
            sign[indice]=-1;
            if(pre){
                pre=0;
                re->ch++;
            }
        }

    }

    if(re->times>1){
        for(i=0;i< (re->deg)/(re->times)+1;i++){
            sign[i]=sign[i*(re->times)];
            mpz_set(coeff[i],coeff[i*(re->times)]);
        }

        (re->deg)/=(re->times);


    }

    free(exp);
    for(i=0;i<size;i++)
        mpz_clear(tempCoef[i]);

    free(tempCoef);
    return re;



}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  createSeachPoly
 *  Description:
 * =====================================================================================
 */
SearchPoly*
createSeachPoly ( const int n, const int * exp, const char* cf ){

    assert(n>0);

    SearchPoly *re;
    re  =(SearchPoly*) malloc_d ( sizeof(SearchPoly) );

    re->deg=exp[0];
    re->times=re->deg;

    re->size=re->deg+1;

    re->coeff   =(mpz_t *) malloc_d (re->size* sizeof(mpz_t) );

    re->sign    =(int *) calloc_d ( re->size, sizeof(int) ); /* all set 0 */
    re->index   =(int*) malloc_d (re->size* sizeof(int) );


    int *index=re->index;
    int * sign=re->sign;
    mpz_t *coeff=re->coeff;

    mpz_init(re->a);
    mpz_set_si(re->a,1);
    mpz_init(re->b);
    mpz_init(re->c);
    mpz_init(re->d);
    mpz_set_si(re->d,1);

    int i;

    for ( i = 0; i < re->size; i += 1 ) {
        mpz_init(coeff[i]);
        index[i]=i;
    }

    re->ch=0;
    int pre=1;
    int indice=0;
    int start=0;
    int last=re->deg;
    int cha;

    int tt;


    for ( i = 0; i < n; i += 1 ) {

        mpz_t temp;
        mpz_init(temp);
        tt=readMpz(temp,cf,&start);
        if(0==tt){

            return NULL;
        }

        if(0==i){
            if(-1==mpz_sgn(temp)) pre=0;
        }

        indice=exp[i];
        if(re->times>1){
            cha=last-indice;
            last=indice;
            re->times=gcdint(re->times,cha);
        }

        mpz_set(re->coeff[indice],temp);

        if(1==mpz_sgn(temp)){
            sign[indice]=1;
            if(!pre){
                pre=1;
                re->ch++;
            }
        }else if(-1==mpz_sgn(temp)){
            sign[indice]=-1;
            if(pre){
                pre=0;
                re->ch++;
            }
        }
        mpz_clear(temp);


    }

    if(re->times>1){

        for ( i = 0; i < (re->deg)/(re->times)+1; i += 1 ) {
            sign[i]=sign[i*(re->times)];
            mpz_set(coeff[i], coeff[i*(re->times)]);

        }
        (re->deg)/=(re->times);

    }


    return re;
}       /* -----  end of function createSeachPoly  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  delSearchPoly
 *  Description:
 * =====================================================================================
 */
void
delSearchPoly ( SearchPoly *p ){
    mpz_clear(p->a);
    mpz_clear(p->b);
    mpz_clear(p->c);
    mpz_clear(p->d);

    int i;

    for ( i = 0; i < p->size; i += 1 ) {
        mpz_clear(p->coeff[i]);
    }
    free(p->sign);
    p->sign=NULL;

    free(p->index);
    p->index=NULL;
    free(p->coeff);
    p->coeff=NULL;
    free(p);

}       /* -----  end of function delSearchPoly  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  printPoly
 *  Description:
 * =====================================================================================
 */
void
printPoly (const  SearchPoly * p ){

    const int* index=p->index;
    const MPZ_P coeff=(MPZ_P)p->coeff;
    int i;
    printf ( "poly: \n" );
    for ( i = 0; i < p->deg+1; i += 1 ) {
        if(0!=mpz_sgn(coeff[index[p->deg-i]])){
            if(i==0)
                gmp_printf ( "%Zd*x^%d ",coeff[index[p->deg-i]],p->deg-i );
            else if(i==p->deg){
                if (1==mpz_sgn(coeff[index[p->deg-i]])){

                    gmp_printf ( "+%Zd ",coeff[index[p->deg-i]] );
                }else{

                    gmp_printf ( "%Zd ",coeff[index[p->deg-i]] );
                }


            }
            else{
                if (1==mpz_sgn(coeff[index[p->deg-i]])){

                    gmp_printf ( "+%Zd*x^%d ",coeff[index[p->deg-i]],p->deg-i );
                }else{

                    gmp_printf ( "%Zd*x^%d ",coeff[index[p->deg-i]],p->deg-i );
                }
            }
        }

    }
    printf ( "\n" );

}       /* -----  end of function printPoly  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  oneSepZeros
 *  Description:   when ch=2 invoke this function checking wether 1 seperaton roots
 * return 0 label  1 is one root, 1 one is a seperaton
 * -1 is no zeros

 * =====================================================================================
 */
int
oneSepZeros (const  SearchPoly *p ){

    const int deg=p->deg;
    const int *sign=p->sign;
    const int *index=p->index;
    const MPZ_P coeff=(MPZ_P)p->coeff;

    int start=deg;
    int end=0; /*ch==2  */

    const int hSign=sign[index[deg]];

    int i,j;
    while(sign[index[--start]]+hSign!=0);
    while(sign[index[++end]]+hSign!=0);

    mpz_set(coeff_sum,coeff[index[deg]]);

    i=deg-1;
    j=start;
    while(i>start&&j>=end-1){
        if(mpz_sgn(coeff_sum)==hSign){
            if(j==end-1) return -1;
            mpz_add(coeff_sum,coeff_sum,coeff[index[j--]]);
        }else{
            mpz_add(coeff_sum,coeff_sum,coeff[index[i--]]);
        }
    }

    i=end-1;

    while(i>=-1&&j>=end-1){
        if(mpz_sgn(coeff_sum)==hSign){
            if(j==end-1) return -1;
            mpz_add(coeff_sum,coeff_sum,coeff[index[j--]]);
        }else{
            if(-1==i ){
                if(mpz_sgn(coeff_sum)==-hSign) return 1;
                i--;
            }
            else{
                mpz_add(coeff_sum,coeff_sum,coeff[index[i--]]);
            }
        }
    }

/*  if(j>end) return 1;
 *
 *
 *  if(i>=-1&& mpz_sgn(coeff_sum)==-hSign) return 1;
 */

    if (i<-1&& j==end-1&& mpz_sgn(coeff_sum)==0) return 0;

    return -1;

}       /* -----  end of function oneSepZeros  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  hasPosRoot
 *  Description:  when ch==2 invoke this function. when can assert poly has
 *  not positive root return 0 otherwise return 1.
 *  The sum of abs negtive coeff less then the  sum of abs of  front coeffs
 *  and less
 *  then sum of abs of back  coeffs then this poly has no postive root.
 * =====================================================================================
 */
int
hasPosRoot (const SearchPoly *p ){
    int start=0;
    int end=p->deg;
    int i,j;

    const int deg=p->deg;
    const int *sign=p->sign;
    const int *index=p->index;
    const MPZ_P coeff=(MPZ_P)p->coeff;


    while(sign[index[++start]]+sign[index[deg]]!=0);
    while(sign[index[--end]]+sign[index[deg]]!=0);

    mpz_set_si(coeff_sum,0);

    if(sign[index[deg]]==1){

        i=deg;
        j=end;
        while(i>=end&&j>=start-1){

            if(mpz_sgn(coeff_sum)<0){
                mpz_add(coeff_sum,coeff_sum,coeff[index[i]]);
                i--;
            }else{
                mpz_add(coeff_sum,coeff_sum, coeff[index[j]]);
                j--;
            }

        }
        if(i==end-1) return 1;

        mpz_set_si(coeff_sum,0);
        i=0;
        j=end;
        while(i<=start&&j>=start-1){

            if(mpz_sgn(coeff_sum)<0){
                mpz_add(coeff_sum,coeff_sum,coeff[index[i]]);
                i++;
            }else{
                mpz_add(coeff_sum,coeff_sum, coeff[index[j]]);
                j--;
            }

        }
        if(i==start+1) return 1;

    }else{

        i=deg;
        j=end;
        while(i>=end&&j>=start-1){

            if(mpz_sgn(coeff_sum)>0){
                mpz_add(coeff_sum,coeff_sum,coeff[index[i]]);
                i--;
            }else{
                mpz_add(coeff_sum,coeff_sum, coeff[index[j]]);
                j--;
            }

        }
        if(i==end-1) return 1;
        mpz_set_si(coeff_sum,0);

        i=0;
        j=end;
        while(i<=start&&j>=start-1){

            if(mpz_sgn(coeff_sum)>0){
                mpz_add(coeff_sum,coeff_sum,coeff[index[i]]);
                i++;
            }else{
                mpz_add(coeff_sum,coeff_sum, coeff[index[j]]);
                j--;
            }

        }
        if(i==start+1) return 1;

    }

    return 0;

}       /* -----  end of function hasPosRoot  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  mapOne
 *  Description:
 * =====================================================================================
 */
void
mapOne ( SearchPoly *p ){

    mpz_t tempA,tempC;
    mpz_init(tempA);
    mpz_init(tempC);
    mpz_set(tempA,p->a);
    mpz_set(tempC,p->c);
    mpz_set(p->a,p->b);
    mpz_add(p->b,tempA,p->b);
    mpz_set(p->c,p->d);
    mpz_add(p->d,tempC,p->d);
    mpz_clear(tempA);
    mpz_clear(tempC);

}       /* -----  end of function mapOne  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  stepOne
 *  Description:
 * =====================================================================================
 */
void
stepOne ( SearchPoly *p ){

    negOrder(p );
    mpz_swap(p->a,p->b);
    mpz_swap(p->c,p->d);
    subsAddOne(p);

}       /* -----  end of function stepOne  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  negOrder
 *  Description:
 * =====================================================================================
 */
void
negOrder ( SearchPoly *p ){
    int i,temp;
    const int deg=p->deg;
    int *index=p->index;
    for ( i = 0; i < (deg+1)/2; i += 1 ) {
        temp=index[i];
        index[i]=index[deg-i];
        index[deg-i]=temp;
    }

}       /* -----  end of function negOrder  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  decrease
 *  Description:
 * =====================================================================================
 */
void
decrease ( SearchPoly *p ){
    p->deg--;
    int i;
    int *index=p->index;
    for ( i = 0; i < p->deg+1; i += 1 ) {
        index[i]=index[i+1];
    }

}       /* -----  end of function decrease  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  negsubs
 *  Description:
 * =====================================================================================
 */
void
negsubs ( SearchPoly *p ){
    const int deg=p->deg;
    int *sign=p->sign;
    int *index=p->index;
    mpz_t *coeff=p->coeff;
    int i;
    for ( i = 0; i < deg+1; i += 1 ) {
        if(i%2==1&&sign[index[i]]!=0){
            sign[index[i]]*=-1;
            mpz_neg(coeff[index[i]],coeff[index[i]]);

        }

    }
    int pre=1;
    if(sign[index[deg]]==-1) pre=0;
    p->ch=0;

    for ( i = deg-1; i >= 0; i -= 1 ) {
        if(sign[index[i]]==1){
            if(!pre){
                pre=1;
                p->ch++;
            }
        }
        else if( sign[index[i]]==-1){
            if(pre){
                pre=0;
                p->ch++;

            }
        }
    }

}       /* -----  end of function negsubs  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  value
 *  Description:   return 1 positive -1 negtive 0 zero
 * =====================================================================================
 */
int
value (const  SearchPoly *p, const mpq_t v ){
    int i;
    const int deg=p->deg;
    const MPZ_P coeff=(MPZ_P)p->coeff;
    const int* index=p->index;

    mpq_set_z(root_qtemp,coeff[index[deg]]);

    for ( i = deg-1; i >= 0; i -= 1 ) {
        mpq_mul(root_qtemp,root_qtemp,v);
        mpq_set_z(root_qtempB,coeff[index[i]]);
        mpq_add(root_qtemp,root_qtemp,root_qtempB);
    }

    return mpq_sgn(root_qtemp);

}       /* -----  end of function value  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  hasZeroRoot
 *  Description:
 * =====================================================================================
 */
int
hasZeroRoot ( const SearchPoly *p ){

    return p->sign[p->index[0]]==0;

}       /* -----  end of function hasZeroRoot  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  isRoot
 *  Description:
 * =====================================================================================
 */
int
isRoot ( SearchPoly *p, const unsigned long int value ){
    int i=0;
    int pre=1;

    int *index=p->index;
    int *sign=p->sign;
    mpz_t *coeff=p->coeff;

    if(!mpz_divisible_ui_p(coeff[index[0]],value))
        return 0;

    mpz_divexact_ui(tempCoeff[0],coeff[index[0]],value);
    mpz_neg(tempCoeff[0],tempCoeff[0]);

/*  mpz_set(tempCoeff[deg-1],coeff[index[deg]]);*/


    for ( i = 1; i < p->deg; i += 1 ) {

        mpz_sub(root_base, tempCoeff[i-1],coeff[index[i]]);
        if(!mpz_divisible_ui_p(root_base,value)) return 0;
        mpz_divexact_ui(tempCoeff[i],root_base,value);
    }

    mpz_sub(root_base,coeff[index[p->deg]],tempCoeff[p->deg-1]);
    if(mpz_sgn(root_base)!=0) return 0;

    for ( i = 0; i < p->deg; i += 1 ) {
        mpz_set(coeff[index[i]],tempCoeff[i]);
        sign[index[i]]=mpz_sgn(tempCoeff[i]);
    }
    p->ch=0;
    if(sign[index[p-> deg-1]]==-1) pre=0;

    for ( i = p->deg-2; i >= 0; i -= 1 ) {
        if(sign[index[i]]==1&&!pre ){
            pre=1;
            p->ch++;
        }
        else if(sign[index[i]]==-1&&pre){
            pre=0;
            p->ch++;
        }

    }
    p->deg--;                                      /* decrease degree */

/*  printf ( "ok\n" );*/
    return 1 ;

}       /* -----  end of function isRoot  ----- */
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  zeroRoot
 *  Description:
 * =====================================================================================
 */
Root*
zeroRoot ( const SearchPoly *p ){

    Root* re=createRoot();
    mpq_set_num(re->left,p->b);
    mpq_set_den(re->left,p->d);
    mpq_set(re->right,re->left);
    /*          printf ( "%f\n",mpq_get_d(re->left) );*/
    return re;

}       /* -----  end of function zeroRoot  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  root_1
 *  Description:   root \in (min(a/c,b/d), max(a/c,b/d))
 *                Note is interval is an open interval.
 * =====================================================================================
 */
Root*
root_1 ( const SearchPoly *p ){

    Root *re=createRoot();

    mpq_set_num(re->right,p->b);
    mpq_set_den(re->right,p->d);
    if(mpz_sgn(p->c)==0){
        mpq_get_num(root_num,root_max);
        mpq_get_den(root_den,root_max);
        mpz_mul(root_num,p->a,root_num);
        mpz_mul(root_remNum,p->b,root_den);
        mpz_add(root_num,root_num,root_remNum);
        mpz_mul(root_den,p->d,root_den);
        mpq_set_num(re->left,root_num);
        mpq_set_den(re->left,root_den);
    }
    else {
        mpq_set_num(re->left,p->a);
        mpq_set_den(re->left,p->c);
    }

    if(mpq_cmp(re->left,re->right)>0){
        mpq_swap(re->left,re->right);

    }

    return re;


}       /* -----  end of function root_1  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  root_2
 *  Description:
 * =====================================================================================
 */
Root*
root_2 ( const SearchPoly* p,const unsigned long int value ){

    Root *re=createRoot();
    mpz_mul_ui(root_num,p->a,value);
    mpz_add(root_num,root_num,p->b);
    mpz_mul_ui(root_den,p->c,value);
    mpz_add(root_den,root_den,p->d);
    mpq_set_num(re->left,root_num);
    mpq_set_den(re->left,root_den);
    mpq_set(re->right,re->left);

    return re ;

}       /* -----  end of function root_2  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  leftOneRoot
 *  Description:
 * =====================================================================================
 */
Root*
leftOneRoot ( SearchPoly *p ){

    Root *re=createRoot();

    mpq_set_num(re->right,p->b);
    mpq_set_den(re->right,p->d);
    mpz_add(p->b,p->a,p->b);
    mpz_add(p->d,p->c,p->d);

    mpq_set_num(re->left,p->b);
    mpq_set_den(re->left,p->d);

    if(mpq_cmp(re->left,re->right)>0){
        mpq_swap(re->left,re->right);

    }
    return re ;

}       /* -----  end of function leftOneRoot  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  subsAdd
 *  Description: x<-x+a
 * =====================================================================================
 */
void
subsAdd ( SearchPoly *p , mpz_t step, const int shift ){

    subsTimes(p,step,shift);
    subsAddOne(p);
}       /* -----  end of function subsAdd  ----- */




/*
 * ===  FUNCTION  ======================================================================
 *         Name:  subsAddOne
 *  Description:   x<-x+1
 * =====================================================================================
 */
void
subsAddOne ( SearchPoly *p ){

    const int deg=p->deg;
    int * index=p->index;
    int *sign=p->sign;
    mpz_t * coeff=p->coeff;

    int j;
    int i;
    i=deg;
    int start;
    start=deg-1;

    mpz_add(p->b,p->b,p->a);
    mpz_add(p->d,p->d,p->c);
/*  printf ( "a %f b %f c %f f %f \n",mpz_get_d(a),mpz_get_d(b),mpz_get_d(c),mpz_get_d(d) );*/

    for ( i = 0; i < deg+1; i += 1 ) {
        mpz_set(tempCoeff[i],coeff[index[i]]);
    }

    int pre;
    pre=1;

    if(sign[index[deg]]==1){
        while(sign[index[start]]!=-1){
            sign[index[start]]=1;
            start--;
        }
    }
    else{
        pre=0;
        while(sign[index[start]]!=1 ){
            sign[index[start]]=-1;
            start--;
        }

    }

    for (i=0 ; i<start+1; i += 1 ) {

        for ( j = deg; j>i; j -= 1 ) {
            mpz_add(tempCoeff[j-1],tempCoeff[j-1],tempCoeff[j]);

        }

    }

    p->ch=0;
    int sgn;

    for (i=start ; i>=0; i -= 1 ) {

        sgn=mpz_sgn(tempCoeff[i]);

        if(sgn>0){
            sign[index[i]]=1;
            if(!pre){
                pre=1;
                p->ch++;
            }
        }else if (sgn<0){
            sign[index[i]]=-1;
            if(pre){
                pre=0;
                p->ch++;
            }
        }
        else
            sign[index[i]]=0;
    }

    if(p->ch==1||p->ch==0){
        return;
    }


    for ( i = start+1; i < deg; i += 1 ) {

        for ( j = deg; j>i; j -= 1 ) {
            mpz_add(tempCoeff[j-1],tempCoeff[j-1],tempCoeff[j]);

        }
    }


    for ( i = 0; i < deg+1; i += 1 ) {
        mpz_set(coeff[index[i]],tempCoeff[i]);
/*      printf ( "%d\n",mpz_sizeinbase(tempCoeff[i],2)/32 );*/

    }

}       /* -----  end of function subsAddOne  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  update
 *  Description:
 * =====================================================================================
 */
void
update ( SearchPoly *p ){
    int * index=p->index;
    int *sign=p->sign;
    mpz_t * coeff=p->coeff;
    const int deg=p->deg;

    int i=0;

    for ( i = 0; i < deg+1; i += 1 ) {
        sign[index[i]]=mpz_sgn(coeff[index[i]]);
    }
    int pre=1;
    if(sign[index[deg]]==-1) pre=0;


    p->ch=0;
    for ( i = deg-1; i >= 0; i -= 1 ) {
        if(sign[index[i]]==1&&!pre){
            pre=1;
            p->ch++;
        }else if(sign[index[i]]==-1&&pre){
            pre=0;
            p->ch++;
        }
    }

}       /* -----  end of function update  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  delFac_1
 *  Description:
 * =====================================================================================
 */
void
delFac_1 (SearchPoly *p, const unsigned long int fac  ){

    int i;
    int pre=1;

    int *index=p->index;
    int * sign=p->sign;
    mpz_t * coeff=p->coeff;

    mpz_set(tempCoeff[p->deg-1],coeff[index[p->deg]]);

    for ( i = p->deg-2; i >= 0; i -= 1 ) {
        mpz_mul_ui(root_base,tempCoeff[i+1],fac);
        mpz_add(tempCoeff[i],coeff[index[i+1]],root_base);

    }

    for ( i = 0; i < p->deg; i += 1 ) {
        mpz_set(coeff[index[i]],tempCoeff[i]);
        sign[index[i]]=mpz_sgn(tempCoeff[i]);
    }

    p->ch=0;
    if(sign[index[p->deg-1]]==-1) pre=0;

    for ( i = p->deg-2; i >= 0; i -= 1 ) {
        if(sign[index[i]]==1&&!pre ){
            pre=1;
            p->ch++;
        }
        else if(sign[index[i]]==-1&&pre){
            pre=0;
            p->ch++;
        }

    }
    p->deg--;                                      /* decrease degree */

}       /* -----  end of function delFac_1  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  delFac_2
 *  Description:  modify sign
 * =====================================================================================
 */
void
delFac_2 ( SearchPoly *p, const mpq_t v  ){
    int* index=p->index;
    int* sign=p->sign;
    mpz_t * coeff=p->coeff;

    int pre=1;

    mpq_get_num(root_num,v);
    mpq_get_den(root_den,v);
    mpz_divexact(tempCoeff[p->deg-1],coeff[index[ p->deg]],root_den);
    int i;

    for ( i = p->deg-2; i >= 0; i -= 1 ) {
        mpz_mul(root_base,root_num,tempCoeff[i+1]);
        mpz_add(root_base,root_base,coeff[index[i+1]]);
        mpz_divexact(tempCoeff[i],root_base,root_den);
    }

    for ( i = 0; i < p->deg; i += 1 ) {
        mpz_set(coeff[index[i]],tempCoeff[i]);
        sign[index[i]]=mpz_sgn(tempCoeff[i]);
    }

    p->ch=0;
    if(sign[index[p->deg-1]]==-1) pre=0;

    for ( i = p->deg-2; i >= 0; i -= 1 ) {
        if(sign[index[i]]==1&&!pre ){
            pre=1;
            p->ch++;
        }
        else if(sign[index[i]]==-1&&pre){
            pre=0;
            p->ch++;
        }

    }

    p->deg--;

}       /* -----  end of function delFac_2  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  subTimes
 *  Description:
 * =====================================================================================
 */
void
subsTimes ( SearchPoly *p , const mpz_t m ,const int shift ){

    int i;
    mpz_mul(p->a,p->a,m);
    mpz_mul(p->c,p->c,m);
    mpz_t times;
    mpz_init(times);
    mpz_set_si(times,1);

    const int deg=p->deg;
    int *index=p->index;
    mpz_t * coeff=p->coeff;


    if(shift>0){
        if(mpz_divisible_p(coeff[index[0]],m)!=0){
            for ( i = 2; i < deg+1; i += 1 ) {
                mpz_mul_2exp(coeff[index[i]],coeff[index[i]],(i-1)*shift);
            }

            mpz_divexact(coeff[index[0]],coeff[index[0]],m);
            return ;

        }else{

            for ( i = 1; i < deg+1; i += 1 ) {
                mpz_mul_2exp(coeff[index[i]],coeff[index[i]],i*shift);
            }

        }


    }else{

        if(mpz_divisible_p(coeff[index[0]],m)!=0){

            for ( i = 2; i < deg+1; i += 1 ) {
                mpz_mul(times,times,m);
                mpz_mul(coeff[index[i]],coeff[index[i]],times);
            }
            mpz_divexact(coeff[index[0]],coeff[index[0]],m);
            return ;

        }else{

            for ( i = 1; i < deg+1; i += 1 ) {
                mpz_mul(times,times,m);
                mpz_mul(coeff[index[i]],coeff[index[i]],times);
            }


        }
    }

    mpz_gcd(times,m,coeff[index[0]]);
    if(mpz_cmp_ui(times,1)>0){

        for ( i = 0; i < deg+1; i += 1 ) {
            mpz_divexact(coeff[index[i]],coeff[index[i]],times);
        }
    }

}       /* -----  end of function subTimes  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  subsDiv
 *  Description:
 * =====================================================================================
 */
void
subsDiv ( SearchPoly *p, const mpz_t m ){

    int i;
    const int deg=p->deg;
    int* index=p->index;
    mpz_t *coeff=p->coeff;
    mpz_t times;
    mpz_init(times);
    mpz_set_si(times,1);

    for ( i = deg-1; i>=0; i -= 1 ) {
        mpz_mul(times,times,m);
        mpz_mul(coeff[index[i]],coeff[index[i]],times);
    }
    mpz_divexact(p->a,p->a,m);
    mpz_divexact(p->c,p->c,m);
    mpz_clear(times);


}       /* -----  end of function subsDiv  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  lmt_lb
 *  Description:
 * =====================================================================================
 */
int
lmt_lb ( SearchPoly *p ){
    negOrder(p);
    int re=lmt_up(p);
    if(mpq_sgn(root_up)==0){
        mpq_set_si(root_lb,0,1);
    }else
        mpq_inv(root_lb,root_up);
    negOrder(p);
    return re;

}       /* -----  end of function lmt_lb  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  lmt_up
 *  Description:
 * =====================================================================================
 */
int
lmt_up (const  SearchPoly *p ){

    const int deg=p->deg;
    const int *sign=p->sign;
    const int *index=p->index;
    const MPZ_P coeff=p->coeff;

    int i,j;
    int negMax;
    int last;
    int start;
    start=deg;
    int lastNeg;
    lastNeg=-1;
    int returnOne;
    int base;
    const int hSign=sign[index[deg]];
    base=1;
    returnOne=0;
    mpq_set_si(root_up,0,1);
    if(deg==0)
        return 0 ;

    for(i=0;i<deg+1;i+=1){
        timeused[i]=1;
        locate[i]=i;
        mpz_abs(absA[i],coeff[index[i]]);
    }

    while(sign[index[++lastNeg]]+hSign!=0);

    while(sign[index[--start]]+hSign!=0);

    negMax=start;
    /*-----------------------------------------------------------------------------
     *  if polynomial's root_up is greater than 1 we return 2
     *-----------------------------------------------------------------------------*/

    mpq_set_si(root_up,2,1);

    mpz_set(coeff_sum,absA[deg]);
    mpz_add(preSum,coeff_sum,coeff_sum);
/*  printf ( "%f  \n",mpz_get_d(preSum) );*/

    i=deg-1;                                    /* pos */
    j=start;                                    /* neg */
    last=start;
    while(i>=lastNeg-1&&j>=lastNeg-1){
        if(mpz_sgn(coeff_sum)<0){
            while(i>last&&sign[index[i]]!=hSign)i--;
            if(i==last){
                returnOne=1;
                mpz_add(preSum,preSum,absA[last]);

                if(i==0||sign[index[i-1]]!=hSign||mpz_sgn(preSum)<0){
                    return 0;
                }
                mpz_add(coeff_sum,coeff_sum,absA[i-1]);
                if(mpz_sgn(coeff_sum)<0)
                    return 0;
                i--;

            }
            else mpz_add(coeff_sum,coeff_sum,absA[i]);
            i--;

        }else{
            if(j==lastNeg-1){
                j=lastNeg-2;
                break;
            }
            while(j>=lastNeg&&sign[index[j]]+hSign!=0)j--;
            mpz_sub(coeff_sum,coeff_sum,absA[j]);
            last=j;
            if(mpz_cmp(absA[negMax],absA[j])<=0) negMax=j;

            j--;
        }

        mpz_mul_2exp(preSum,coeff_sum,1);
    }


    if(returnOne){

        mpq_set_si(root_up,1,1);
        return 0;
    }


    /*-----------------------------------------------------------------------------
     *  the largest root less than 1/base
     *-----------------------------------------------------------------------------*/

/*      printf ( "%d %d\n",deg,lastNeg );*/



    i=deg;
    while(i==deg){
        i=deg-1;
        j=start;
        mpz_set(coeff_sum,absA[deg]);
        while(i>=lastNeg-1&&j>=lastNeg-1){
            if(mpz_sgn(coeff_sum)<0){
                while(i>j&&sign[index[i]]!=hSign)i--;
                if(i==j) break;
                mpz_mul_2exp(preSum,absA[i],(deg-i)*base);
                mpz_add(coeff_sum,coeff_sum,preSum);
                i--;
            }else{
                if(j==lastNeg-1){
                    j=lastNeg-2;
                    break;
                }
                while(j>lastNeg&&sign[index[j]]+hSign!=0)j--;
                mpz_mul_2exp(preSum,absA[j],(deg-j)*base);
                mpz_sub(coeff_sum,coeff_sum,preSum);
                j--;

            }
        }
        if(j==lastNeg-2){
            base++;
            if(base>=30)break;

            i=deg;
        }

    }


    base--;
    int re=base;
    base=(int)(pow(2,base)+0.1);

    mpq_set_si(root_up,1,base);
    return re;

}       /* -----  end of function lmt_up  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  merageRatAndIrratRoot
 *  Description:
 * =====================================================================================
 */
void
merageRatAndIrratRoot (SearchPoly *p , PointList *reRoots , PointVector * ratRoot, PointList * roots  ){

    int i=0;
    int size=size_V(ratRoot );

    int midSign=2;
    int frontSign=2;

    Root* front=NULL;
    while(i<size&&!isEmpty_L(roots )){

        if(frontSign==2)front=(Root*)front_L(roots );

        if(mpq_cmp(front->left,((Root*)at_V(ratRoot,i))->left)>0){ /* rat left......right */
            push_back_L(reRoots, at_V (ratRoot,i));
            i++;
            midSign=2;

        }else if(mpq_cmp(((Root*) at_V(ratRoot,i))->left,front->right)>0){ /* left.....right rat */
            push_back_L(reRoots, front);
            pop_front_L(roots);
            frontSign=2;

        }else if(mpq_equal(front->left,((Root*)at_V( ratRoot,i))->left)){ /* left(rat)....right */
            int rightSign=value(p, front->right);

            mpq_add(root_LEFT,front->left,front->right);
            mpq_div_2exp(root_LEFT,root_LEFT,1);

            while (value(p, root_LEFT)==rightSign){
                mpq_add(root_LEFT,front->left,root_LEFT);
                mpq_div_2exp(root_LEFT,root_LEFT,1);
            }
            mpq_set(front->left,root_LEFT);

            push_back_L(reRoots, at_V(ratRoot,i));
            i++;
            midSign=2;


        }else if(mpq_equal( ((Root*)at_V(ratRoot,i))->left,front->right)){ /* left.....right(rat) */

            int leftSign=value(p, front->left);
            mpq_add(root_RIGHT,front->left,front->right);
            mpq_div_2exp(root_RIGHT,root_RIGHT,1);
            while (value(p, root_RIGHT)==leftSign){
                mpq_add(root_RIGHT,root_RIGHT,front->right);
                mpq_div_2exp(root_RIGHT,root_RIGHT,1);
            }
            mpq_set(front->right,root_RIGHT);

            push_back_L(reRoots, front);
            pop_front_L(roots);
            frontSign=2;

        }
        else{                                   /* left...rat....right */
            if(frontSign==2) frontSign=value(p, front->left);
            if(midSign==2)  midSign=value(p, ((Root*)at_V(ratRoot,i))->left);
            if(midSign==frontSign){

                mpq_set(front->left,((Root*)at_V(ratRoot,i))->left);

                int rightSign=value(p, front->right);
                mpq_add(root_LEFT,front->left,front->right);
                mpq_div_2exp(root_LEFT,root_LEFT,1);
                while (value(p, root_LEFT)==rightSign){
                    mpq_add(root_LEFT,front->left,root_LEFT);
                    mpq_div_2exp(root_LEFT,root_LEFT,1);
                }
                mpq_set(front->left,root_LEFT);

                push_back_L(reRoots, at_V(ratRoot,i));
                i++;
                midSign=2;

            }else{

                mpq_set(front->right, ((Root*)at_V( ratRoot,i))->right);

                int leftSign=value(p, front->left);
                mpq_add(root_RIGHT,front->left,front->right);
                mpq_div_2exp(root_RIGHT,root_RIGHT,1);
                while (value(p, root_RIGHT)==leftSign){
                    mpq_add(root_RIGHT,root_RIGHT,front->right);
                    mpq_div_2exp(root_RIGHT,root_RIGHT,1);
                }
                mpq_set(front->right,root_RIGHT);

                push_back_L(reRoots, front);
                pop_front_L(roots );
                frontSign=2;
                push_back_L(reRoots, at_V(ratRoot,i));
                i++;
                midSign=2;
            }


        }


    }
    for ( ; i < size; i += 1 ) {
        push_back_L(reRoots, at_V(ratRoot,i));
    }
    while(!isEmpty_L(roots )){

        push_back_L(reRoots,front_L(roots ));
        pop_front_L(roots );
    }


}       /* -----  end of function merageRatAndIrratRoot  ----- */
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  refinePrec
 *  Description:   refinement precsion
 *          have a bug when times>1
 * =====================================================================================
 */
void
refinePrec (SearchPoly* p, PointList* reRoots, mpq_t prec ){
    mpq_t intv;
    mpq_t mid;
    mpq_init(intv);
    mpq_init(mid);

    int leftsign;
    int val;
    PointElem *it=reRoots->head;
    while(it!=NULL){

        leftsign=value(p, ((Root*)(it->value))->left);
        mpq_set(mid,((Root*)(it->value))->left);

        mpq_sub(intv,((Root*)(it->value))->right,((Root*)(it->value))->left);
        /*-----------------------------------------------------------------------------
         *  assert mpq_eq(mid,*it->left)
         *-----------------------------------------------------------------------------*/
        while ( mpq_cmp(intv,prec)>0){

            mpq_div_2exp(intv,intv,1);
            mpq_add(mid,mid,intv);
            val=value (p, mid);
            if(0==val){
                mpq_set(((Root*)(it->value))->left,mid);
                mpq_set(((Root*)(it->value))->right,mid);
                break;
            }
            else if(value(p, mid)==leftsign){
                mpq_set(((Root*)(it->value))->left,mid);
            }else {
                mpq_set(((Root*)(it->value))->right,mid);
                mpq_sub(mid,mid,intv);

            }
        }

        it=it->next;

    }

    mpq_clear(intv);
    mpq_clear(mid);

}       /* -----  end of function refinePrec  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  initdata
 *  Description:
 * =====================================================================================
 */
void
initdata ( const int deg ){
    int i;
    absA    =(mpz_t*) malloc_d ( (deg+1)*sizeof(mpz_t) );


    tempCoeff   =(mpz_t*) malloc_d ( (deg+1)*sizeof(mpz_t ) );



    locate  = (int *)malloc_d ( sizeof(int)*(deg+1) );


    timeused    = ( unsigned long int*)malloc_d ( sizeof(long int)*(deg+1) );


    for ( i = 0; i < deg+1; i += 1 ) {
        mpz_init(tempCoeff[i]);
        mpz_init(absA[i]);
    }

}       /* -----  end of function initdata  ----- */


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  cleardata
 *  Description:
 * =====================================================================================
 */
void
cleardata ( const int deg ){
    int i;
    for ( i = 0; i < deg+1; i += 1 ) {
        mpz_clear(tempCoeff[i]);
        mpz_clear(absA[i]);
    }
    free(absA);
    free(tempCoeff);

    free (locate);
    locate  = NULL;
    free (timeused);
    timeused    = NULL;

}       /* -----  end of function cleardata  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  delTimes
 *  Description:     ---------------------------------------------------------------
 *  subvariable x^n-> x
 *-------------------------------------------

 * =====================================================================================
 */
void
subvariable (SearchPoly *p, PointVector *ratRoot, PointList* reRoots ,const unsigned long int times  ){
    int i;
    unsigned long int  mul=8;
    int backSign=2;
    int state=1;

    Root *temp;
    clear_V(ratRoot);

    v_insertBack_L(ratRoot, reRoots);
    clear_L(reRoots);


    for ( i = 0; i <  size_V(ratRoot); i += 1 ) {
        if( mpq_sgn(((Root*) at_V(ratRoot,i))->left)==0&&mpq_sgn(((Root*) at_V(ratRoot,i))->right)==0){ /* [0,0] is a spercial in interval */
            push_back_L(reRoots, at_V(ratRoot,i));
            continue;

        }

        mul=8;
        state=1;
        if(mpq_equal(((Root*) at_V(ratRoot,i))->left,((Root*) at_V(ratRoot,i))->right))state=0;

        root(root_left,((Root*) at_V(ratRoot,i))->left,mul,times,0);
        root(root_right,((Root*) at_V(ratRoot,i))->right,mul,times,!state);

//          if(i==0&&state){                     The first interval's left point must less or equal to  the orignal left point
//
//              root(root_left,ratRoot[i]->left,mul,times,!state);
//}


        if(i>0){
            if(mpq_cmp(root_left,((Root*) at_V(ratRoot,i-1))->right)<0) mpq_set(root_left,((Root*) at_V(ratRoot,i-1))->right);

        }
        if(i+1<size_V (ratRoot )){

            backSign=value(p, ((Root*) at_V(ratRoot,i))->right);

            pow_Q(root_RIGHT,root_right,times);
            while(mpq_cmp(root_RIGHT,((Root*) at_V(ratRoot,i+1))->right)>0||(value(p, root_RIGHT)+backSign==0&&backSign!=0)){
                mul<<=1;

                root(root_right,((Root*) at_V(ratRoot,i))->right,mul,times,!state);

                pow_Q(root_RIGHT,root_right,times);

            }

        }else {                    /* The last interval' right point must greater or equal to the orignal right point  */

            root(root_right,((Root*) at_V(ratRoot,i))->right,mul,times,1);

        }

//        gmp_printf("%Qx ,  %Qx\n",root_left,root_right);

        mpq_set(((Root*) at_V(ratRoot,i))->left,root_left);
        mpq_set(((Root*) at_V(ratRoot,i))->right,root_right);
        push_back_L(reRoots, (at_V(ratRoot,i)));
        if(times%2==0){

            temp=createRoot();
            mpq_set(temp->left,root_left);
            mpq_set(temp->right,root_right);
            negSwap(temp);
            push_front_L (reRoots, temp);
        }

    }

}       /* -----  end of function delTimes  ----- */
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  kiou_up
 *  Description:
 * =====================================================================================
 */
void
kiou_up (const  SearchPoly *p ){

    const int deg=p->deg;
    const int* sign=p->sign;
    const int* index=p->index;
    const MPZ_P coeff= p->coeff;

    mpq_set_si(root_max,0,1);

    int i;
    int used=1;
    if(deg==0)
        return ;

    for(i=0;i<deg+1;i+=1){
        if(sign[index[i]]<0){
            mpz_neg(absA[i],coeff[index[i]]);
        }else if(sign[index[i]]>0){
            mpz_set(absA[i],coeff[index[i]]);
        }
    }
    if (sign[index[deg]]==1){                   /* header coeff is posiive */

        for ( i = deg-1; i >= 0; i -= 1 ) {
            if(sign[index[i]]==-1){
                nroot_2(root_tempUp,absA[deg],absA[i],
                        used,deg-i);
                if(mpq_cmp(root_max,root_tempUp)<0){
                    mpq_set(root_max,root_tempUp);
                }
                used++;

            }
        }
    }else{
        for ( i = deg-1; i >= 0; i -= 1 ) {
            if(sign[index[i]]==1){
                nroot_2(root_tempUp,absA[deg],absA[i],used,deg-i);
                if(mpq_cmp(root_max,root_tempUp)<0){
                    mpq_set(root_max,root_tempUp);
                }
                used++;
            }
        }

    }

}       /* -----  end of function kiou_up  ----- */



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  vas_cont
 *  Description:
 * =====================================================================================
 */
void
vas_cont (PointList *re, PointVector *ratRoot, SearchPoly *poly  ){

    SearchPoly *first;
    SearchPoly *second;
    first=copySearchPoly(poly);

/*  printPoly(poly);
//  first->print();*/
    int s=first->ch;

    int s1,s2,r,shfit;

    unsigned long int value;
    int lose;
    int oneSep;
    kiou_up(first);
//  printf ( "max %f\n",mpq_get_d(root_max) );
    if(s==0){
		delSearchPoly(first);
        return  ;
	}
    else if (s==1){
        mpq_t zero;
        mpq_init(zero);
        push_back_L(re, createRoot_1(zero,root_max) );
        mpq_clear(zero);
        delSearchPoly(first);
        return ;

    }
    PointStack*  searchStack =createStack((void (*)(void *)) delSearchPoly );

    push_S(searchStack,first);

    while (!isEmpty_S(searchStack) ) {

        first=(SearchPoly*) top_S(searchStack);

        pop_S(searchStack );

        s=first->ch;

        shfit=lmt_lb(first);                        /* first's low bound of positive root */

        if(mpq_sgn(root_lb)==0){
            delSearchPoly(first);
            continue;
        }

        mpz_set_q(root_temp ,root_lb);          /* root_temp is the integer lower bound of the positive root */


        /*-----------------------------------------------------------------------------
         *  if root_temp or root_lb is too large we set x<-root_temp*x.
         *-----------------------------------------------------------------------------*/
        if(mpz_cmp(root_temp,root_cir)>0){      /* p's smallest postive root greater than root_cir */

            subsTimes(first, root_temp,shfit);
            mpz_set_si(root_temp,1);            /* the postive root lower bound become 1 */

        }

        if(mpz_cmp_si(root_temp,1)==0){

            subsAddOne(first );

            if(hasZeroRoot(first )){          /* 0 is the root of first->p */

                decrease(first );
                push_back_V (ratRoot, zeroRoot(first ));

                value=1;
                lose=0;

                while(first->ch>1&&lose<sqrt(first->deg)){

                    if(isRoot(first, value)){
                        push_back_V(ratRoot, root_2(first , value ));
                        lose--;

                    }
                    else lose++;

                    value++;
                }

            }

            s=first->ch;
            if(s==0){
                delSearchPoly(first);
                /*interval first  has no root*/

                continue;

            }else if(s==1){                     /* interval first has only one root */

                push_back_L(re, root_1(first ) );

                delSearchPoly(first);

                continue;
            }else if(s==2){

                oneSep=oneSepZeros(first );

                if(oneSep==0){

                    push_back_L(re, root_1(first ) );
                    push_back_V (ratRoot, root_2 (first, 1));

                    delSearchPoly(first);
                    continue;


                }
                else if(oneSep==1){
                    push_back_L(re, leftOneRoot(first ));

                    push_back_L(re, root_1(first ));

                    delSearchPoly(first);
                    continue;

                }

            }


        }
        else if(mpz_cmp_si(root_temp,1)>0){     /* if the least positive root greater   1 */

            subsAdd(first, root_temp,shfit);

            if(hasZeroRoot(first )){          /* 0 is the root of first->p */

                decrease(first );

                push_back_V (ratRoot, zeroRoot(first ));

                value=1;
                lose=0;

                while(first->ch>1&&lose<sqrt(first->deg)){

                    if(isRoot(first, value)){

                        push_back_V (ratRoot, root_2(first, value));
                        lose--;

                    }
                    else lose++;

                    value++;

                }

//              ratRoot.sort(Root);


            }

            s=first->ch;

            if(s==0){

                delSearchPoly(first);
                /*interval first  has no real root*/

                continue;

            }else if(s==1){                     /* interval first has only one root */

                push_back_L(re,root_1(first ) );

                delSearchPoly(first);

                continue;
            }
            else if(s==2){
                oneSep=oneSepZeros(first );
                if(oneSep==0){

                    push_back_L(re,root_1( first) );
                    push_back_V (ratRoot, root_2 (first, 1));

                    delSearchPoly(first);
                    continue;

                }

                else if(oneSep==1){

                    push_back_L(re,leftOneRoot(first ) );
                    push_back_L(re,root_1(first ) );
                    delSearchPoly(first);
                    continue;
                }


            }

        }

        /*-----------------------------------------------------------------------------
         *  find all the positive roots in [1,\infty)
         *-----------------------------------------------------------------------------*/

        second= copySearchPoly(first);        /* This is the orginal polynomial we need. */

        r=0;

        subsAddOne(first );

        if(hasZeroRoot(first )){

            decrease(first );

            push_back_V(ratRoot ,zeroRoot(first ));
            delFac_1(second, 1);
            r=1;

            value=1;
            lose=0;

            while( first->ch>0&&lose<sqrt(first->deg)){

                if(isRoot(first, value)){

                    push_back_V(ratRoot, root_2(first, value));
                    lose--;

                    delFac_1(second, value+1);
                    r++;

                }
                else lose++;

                value++;
            }



        }

        s1=first->ch;                  /* s1 first signchange, s2 second signchange */

        s2=s-s1-r;

        if(s2>1){                               /* (0,1) possibly  has more than 1 */

            /*-----------------------------------------------------------------------------
             *  second=(x+1)^deg *second(1/(x+1));
             *  [1,0)->[0,+\infty)
             *-----------------------------------------------------------------------------*/
            stepOne(second );

            if(hasZeroRoot(second )){          /* the orignal polynomial second has 1 as root, and this case is considered in adove */
                decrease(second );
            }
            s2=second->ch;
        }
        else if(s2==1){
            mapOne(second );
        }
        if(s2==2){
            oneSep=oneSepZeros(second );

            if(oneSep==0){

                push_back_L(re,root_1(second ) );
                push_back_V(ratRoot, root_2(second, 1));
                s2=0;

            }
            else if(oneSep==1){

                push_back_L(re,leftOneRoot(second ));

                push_back_L(re,root_1(second ));
                s2=0;
            }

        }

        if(s2>s1)
        {
            int t=s2;
            s2=s1;
            s1=t;
            SearchPoly * ts=second;
            second=first;
            first=ts;
        }

        if (s1==0){

            delSearchPoly(first);
            delSearchPoly(second);
            continue;
        }
        if(s1==1){

            push_back_L(re,root_1(first ));

            delSearchPoly(first);

        }
        else if(s1==2 ){
            oneSep=oneSepZeros(first );

            if(oneSep==0){
                push_back_L(re,root_1(first ) );
                push_back_V(ratRoot, root_2(first, 1));

                delSearchPoly(first);

            }
            else if(oneSep==1){

                push_back_L(re,leftOneRoot(first ));

                push_back_L(re,root_1(first ));
                delSearchPoly(first);
            }
            else push_S ( searchStack, first);


        }else push_S (searchStack, first);

        if(s2==0){

            delSearchPoly(second);
            continue;

        }
        else if(s2==1){

            push_back_L(re,root_1(second ));
            delSearchPoly(second);
            continue;


        }else if(s2==2 ){
            oneSep=oneSepZeros(second );
            if(oneSep==0){
                push_back_L(re,root_1(second ));
                push_back_V(ratRoot, root_2(second, 1));

                delSearchPoly(second);
                continue;

            }
            else if(oneSep==1){

                push_back_L(re,leftOneRoot(second ));
                push_back_L(re,root_1(second ));

                delSearchPoly(second);
                continue;
            }
            else push_S (searchStack, second);

        } else push_S(searchStack, second);

    }

    int i;
    int sz=(int)size_V(ratRoot );

    for ( i = 0; i < sz; i += 1 ) {
        delFac_2 (poly,  ((Root*) at_V(ratRoot,i))->left);
    }

    if(sz>0){
        update(poly );

    }
    delStack(searchStack);


}       /* -----  end of function vas_cont  ----- */


PointList* LCF(SearchPoly *p, mpq_t prec)
{

    PointVector* ratRoot=createVector( (void (*)(void *)) delRoot);

    PointVector* posRat=createVector( (void (*)(void *))  delRoot);

    PointVector* negRat=createVector((void (*)(void *))  delRoot);

    PointList* negroots=createList((void (*)(void *))  delRoot);

    PointList* posroots=createList((void (*)(void *))  delRoot);

    PointList* reRoots=createList((void (*)(void *))  delRoot);

    int i;

    Root *temp;

    initVars();

//  printPoly(p);

    if(hasZeroRoot(p )){                          /* zero root */
        mpq_t zero1;
        mpq_init(zero1);
        Root *r=createRoot_1(zero1,zero1);
        mpq_clear(zero1);
        push_back_V(ratRoot, r);
        decrease(p);
    }

    const int deg=p->deg;

    initdata(deg);

    vas_cont(posroots, posRat,p);                       /* postitive roots */

    insertBack_V(ratRoot, posRat);
    clear_V(posRat);
    delVector(posRat);

    if( p->times%2==1){
        negsubs(p);
        vas_cont( negroots, negRat,p);                    /* negative roots */
        negsubs(p);

        for ( i = 0; i < size_V(negRat); i += 1 ) {
            negSwap((Root*)at_V(negRat,i));
            push_back_V(ratRoot ,at_V(negRat,i) );
        }
        clear_V(negRat);

        //  if(posRat.size()>0)p.negsubs();

        while ( !isEmpty_L(negroots) ) {

            temp=(Root*) front_L(negroots );
            pop_front_L(negroots);
            negSwap(temp);
            push_front_L(posroots, temp);

        }
        clear_L(negroots);

    }


    delVector(negRat);
    delList(negroots);

    sortRoot_V(ratRoot);

    sortRoot_L(posroots);

//  printf ( "size %d\n",size );

    merageRatAndIrratRoot (p , reRoots , ratRoot, posroots  );

    clear_V(ratRoot);

    clear_L(posroots);
    delList(posroots);

    PointElem *it=reRoots->head;
    while(it!=NULL){

        mpq_canonicalize(((Root*)(it->value))->left);
        mpq_canonicalize(((Root*)(it->value))->right);
        it=it->next;
    }

    const unsigned long int times=p->times;
    /*-----------------------------------------------------------------------------
     *  refinement precsion
     *  have a bug when times>1
     *-----------------------------------------------------------------------------*/
    if(mpq_sgn(prec)==1){
        if(times>1) pow_Q(prec,prec,times);
        refinePrec(p, reRoots,prec);

    }


/*-----------------------------------------------------------------------------
 *  subvariable x^n-> x
 *-----------------------------------------------------------------------------*/

    if(times>1){

        subvariable (p, ratRoot, reRoots , times );

    }
    cleardata(deg);

    clear_V(ratRoot);
    delVector(ratRoot);
    return  reRoots;

}


/*-----------------------------------------------------------------------------
 *  exp cf come from a square free polynomial
 n is the number of term, exp is the array of expoent, cf is coeff rep. preStr is
 precision
 *-----------------------------------------------------------------------------*/
CLOGLIB char* lcf(const int n, const int *exp, const char *cf , int *renum ,const char * precStr ){

//  printf ( "%d   %s \n",n,cf );

    mpq_t prec;
    mpq_init(prec);
    mpq_set_str(prec, precStr,10);

    SearchPoly *p=createSeachPoly(n,exp,cf);
    PointList *reRoots=LCF(p,prec);

    *renum=size_L(reRoots);
    str_buffer= getStr_L(reRoots,str_buffer,&str_cap );
    mpq_clear(prec);
    delList(reRoots);
    delSearchPoly(p);
    return  str_buffer;

}

CLOGLIB void lcfbyf(const char* file ,const char * precStr)
{
    mpq_t prec;
    mpq_init(prec);
    mpq_set_str(prec, precStr,10);
    SearchPoly *p=createSearchPoly2f(file);

//    printPoly(p);

    PointList *reRoots=LCF(p,prec);
    printRoots(reRoots);

    mpq_clear(prec);
    delList(reRoots);
    delSearchPoly(p);


}


