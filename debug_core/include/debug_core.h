/*
 * debug.h
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG_MESSAGE_PREFIX	"[   DEBUG   ]"
#define ERROR_MESSAGE_PREFIX	"[   ERROR   ]"
#define WARN_MESSAGE_PREFIX		"[   WARN    ]"
#define INFO_MESSAGE_PREFIX     "[   INFO    ]"

/**
 * Exit code to be returned from processes upon successful completion; operating system/standard
 * library functions also (generally, as a rule) return this to indicate success.
 */
#define OK		0		// Code to return to the operating system to indicate successful program termination

/**
 * Exit code to be returned from processes when an error condition has arisen; some operating system/standard
 * library functions also return this upon failure, although generally, as a rule, these functions return some
 * other nonzero value.
 */
#define ERROR	-1		// Code to return to the operating system to indicate an error condition

/**
 * Data type to encode a one-or-zero, true-or-false value.
 */
typedef enum { FALSE, TRUE } BOOL;

/**
 * Turns this library's messages on or off.
 * @param enabled TRUE to enable this library; FALSE to turn all messages off.
 * @remarks This library is enabled by default.
 */
void toggle_debug(BOOL enabled);

/**
 * @name log_info
 * @details Logs a formatted [INFO] message to standard output.
 * @param message The message to be displayed, with any printf-compatible format specifiers that may be applicable.
 * @param vargs Format arguments.
 */
void log_info(const char* message, ...);
void log_warning(const char* message, ...);
void log_error(const char* message, ...);
void log_debug(const char* message, ...);

#endif /* __DEBUG_H__ */
