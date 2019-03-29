#ifndef __DEBUG_CORE_STDAFX_H__
#define __DEBUG_CORE_STDAFX_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

typedef enum {
	FALSE, TRUE
} BOOL;

#endif //__DEBUG_CORE_STDAFX_H__
