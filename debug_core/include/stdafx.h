#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define DEBUG_MESSAGE_PREFIX	"[   DEBUG   ]"
#define ERROR_MESSAGE_PREFIX	"[   ERROR   ]"
#define WARN_MESSAGE_PREFIX		"[   WARN    ]"
#define INFO_MESSAGE_PREFIX     "[   INFO    ]"

// Exit codes
#define OK		0		// Code to return to the operating system to indicate successful program termination
#define ERROR	-1		// Code to return to the operating system to indicate an error condition

typedef enum { FALSE, TRUE } BOOL;


#endif //__STDAFX_H__
