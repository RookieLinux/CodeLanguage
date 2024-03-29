/* $Id$
 *
 * sd_xplatform.h
 * 
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __sd_xplatform_h
#define __sd_xplatform_h

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if !defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#else
#include <time.h>
#include <io.h> /* needed for _access  */
#include <windows.h>
#include <winsock.h>
#include <process.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDINT_H
#       include <stdint.h>
#define  XP_UINT64 uint64_t
#define  XP_INT64 int64_t
#else
#ifndef _WIN32
#define  XP_UINT64 unsigned long long
#define  XP_INT64 long long
#else
#define  XP_UINT64 DWORD64
#define  XP_INT64 __int64
#endif
#endif

//#include <log4c/defs.h>
//#include <sd/defs.h>
#include "../log4c/defs.h"
#include "../sd/defs.h"

/*extern int sd_optind; */
LOG4C_DATA int sd_optind; 

extern void getopt_reset(void); 

extern int sd_getopt(int argc, char *const *argv, const char *opts);

#ifdef _WIN32
#define SD_GETOPT(a,b,c) sd_getopt(a,b,c)
#define SD_OPTIND sd_optind
#else
#define SD_GETOPT(a,b,c) getopt(a,b,c)
#define SD_OPTIND optind
#endif


#if defined(_WIN32) && !defined(__MINGW32__) && !defined(__MINGW64__)

#define SD_GETTIMEOFDAY(a,b) sd_gettimeofday(a,b)
extern int sd_gettimeofday(LPFILETIME lpft, void* tzp);
#else
#define SD_GETTIMEOFDAY(a,b) gettimeofday(a,b)

#if !defined(HAVE_GMTIME_R) || !HAVE_DECL_GMTIME_R
#define gmtime_r(a, b) sd_gmtime_r((a), (b))
struct tm *sd_gmtime_r(const time_t *timep, struct tm *result);
#endif

#if !defined(HAVE_LOCALTIME_R) || !HAVE_DECL_LOCALTIME_R
#define localtime_r(a, b) sd_localtime_r((a), (b))
struct tm *sd_localtime_r(const time_t *timep, struct tm *result);
#endif

#endif /* _WIN32 && !__MINGW32__ && !__MINGW64__ */

#ifdef _WIN32
#define FILE_SEP "\\"
#else
#define FILE_SEP "/"
#endif

#ifdef _WIN32
#define SD_ACCESS_READ(a) _access(a,04)
#else
#define SD_ACCESS_READ(a) access(a,R_OK)
#endif

int sd_stat_ctime(const char* path, time_t* time);
#define SD_STAT_CTIME(path, time) sd_stat_ctime(path, time)

#ifndef _WIN32
#define DIFF_CMD  "/usr/bin/diff -q"
#else
#define DIFF_CMD  "comp.exe"
#endif

#ifdef _WIN32
#if defined(__MINGW32__) || defined(__MINGW64__)
#define snprintf sd_snprintf
#define vsnprintf sd_vsnprintf
int sd_snprintf(char *str, size_t size, const char *format, ...) SD_ATTRIBUTE((format(printf, 3, 4)));
int sd_vsnprintf(char *str, size_t size, const char *format, va_list ap);
#else
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif
#if !defined(HAVE_ALLOC_H) && defined(HAVE_MALLOC_H) && !defined(alloca)
#define alloca _alloca
#endif
#define strncasecmp strnicmp
#define strcasecmp stricmp
#define YY_NO_UNISTD_H
#if !defined(HAVE_SLEEP) || !HAVE_DECL_SLEEP
#define sleep(x) Sleep(x*1000)
#endif
#endif


/* Maybe should be using this for to mean
* MS compiler #if defined(_MSC_VER) 
*/
#ifdef _WIN32
#define pthread_t HANDLE
#define pthread_mutex_t HANDLE
#define pthread_attr_t DWORD
#define THREAD_FUNCTION DWORD (WINAPI *)(void *)

/*
* This one not obvious: you would have naturally thought of mapping to
* CreateThread()--turns out that to be safe using CRT functions
* you need to use _begintheadex().  
* cf. http://msdn2.microsoft.com/en-us/library/7t9ha0zh.aspx
*  http://groups.google.com/group/comp.os.ms-windows.programmer.win32/browse_thread/thread/86d8624e7ee38c5d/f947ac76cd10f397?lnk=st&q=when+to+use+_beginthreadex&rnum=1#f947ac76cd10f397
* 
*/
#define pthread_create(thhandle,attr,thfunc,tharg) \
  (int)((*thhandle=(HANDLE)_beginthreadex(NULL,0,(THREAD_FUNCTION)thfunc,tharg,0,NULL))==NULL)
#define pthread_join(thread, result) \
  ((WaitForSingleObject((thread),INFINITE)!=WAIT_OBJECT_0) || !CloseHandle(thread))
#define pthread_exit() _endthreadex(0)
#define pthread_cancel(thread) TerminateThread(thread,0)

#define pthread_mutex_init(pobject,pattr) (*pobject=CreateMutex(NULL,FALSE,NULL))
#define pthread_mutex_lock(pobject) WaitForSingleObject(*pobject,INFINITE)
#define pthread_mutex_unlock(pobject) ReleaseMutex(*pobject)

#define pthread_mutex_destroy(pobject) CloseHandle(*pobject)

#endif


#ifdef __HP_cc
#define inline __inline
#endif 

#endif
