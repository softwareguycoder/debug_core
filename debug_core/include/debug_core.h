/*
 * debug.h
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

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
