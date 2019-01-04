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
