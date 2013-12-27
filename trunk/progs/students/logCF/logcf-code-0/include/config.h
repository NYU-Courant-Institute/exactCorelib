/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/05/2013 04:01:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liyun Dai (pku), dlyun2009@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  CONFIG_INC
#define  CONFIG_INC

#define	DEFAULT_CAP  10			/*  */

#define	 RESCALE 1.2	/*  */

#ifdef _WIN32
#  ifdef CLOGLIB_EXPORT
#    define CLOGLIB __declspec(dllexport)
#  elif defined(CLOGLIB_IMPORT)
#    define CLOGLIB __declspec(dllimport)
#  else
#    define CLOGLIB
#  endif
#else
#  define CLOGLIB
#endif

#endif   /* ----- #ifndef CONFIG_INC  ----- */
