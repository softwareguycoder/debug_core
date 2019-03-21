/*

 * debug.c
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 *      Contains routines for debugging, such as logging etc.
 */

#include "stdafx.h"
#include "debug_core.h"

#define LOG_FORMAT		"%s %s %s\n"		// Format for fprintf to log messages

BOOL mute = FALSE;

FILE* g_fLog = stdout;
FILE* g_fErrorLog = stderr;

void close_log_file() {
	if (g_fLog != NULL && g_fLog != stdout) {
		fclose(g_fLog);
	}

	if (g_fErrorLog != NULL && g_fErrorLog != stderr) {
		fclose(g_fErrorLog);
	}
}

/**
 * @name get_current_time_string
 * @brief Returns the string obtained by calling asctime.
 * @remarks Utilized by the calls to fprintf that write log messages.  This function
 * is only available internally to this source file.
 */
const char* get_current_time_string() {
	// time_t is arithmetic time type
	time_t now;

	// Obtain current time
	// time() returns the current time of the system as a time_t value
	time(&now);

	// Format the curernt time as a string and return it
	return ctime(&now);
}

FILE* get_error_log_file_handle() {
	return g_fErrorLog;
}

FILE* get_log_file_handle() {
	return g_fLog;
}

void set_error_log_file(FILE* fErrorLogFile) {
	// Check whether the FILE pointer given to us is non-NULL;
	// if it's NULL, then stop.
	if (fErrorLogFile == NULL) {
		return;
	}

	// Set the global error log file handle to the value passed.
	// Callers are responsible for ensuring that the FILE pointer
	// passed is a handle to a file that is currently open for
	// reading, writing, and/or appending.
	g_fErrorLog = fErrorLogFile;
}

void set_log_file(FILE* fLogFile) {
	// Check whether the FILE pointer given to us is non-NULL;
	// if it's NULL, then stop.
	if (fLogFile == NULL) {
		return;
	}

	// Set the global log file handle to the value passed.
	// Callers are responsible for ensuring that the FILE pointer
	// passed is a handle to a file that is currently open for
	// reading, writing, and/or appending.
	g_fLog = fLogFile;
}

void set_log_file_path(const char* path) {
	if (path == NULL || strlen(path) == 0 || path[0] == '\0')
		return;

	g_fLog = g_fErrorLog = fopen(path, "a+");
	if (g_fLog == NULL) {
		g_fLog = stdout;
		g_fLog = stderr;
		return;
	}
}

void toggle_debug(BOOL enabled) {
	mute = !enabled;
}

void log_info(const char* message, ...) {
	if (mute == TRUE)
		return;

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	fprintf(get_log_file_handle(),
			LOG_FORMAT, get_current_time_string(),
			INFO_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_warning(const char* message, ...) {
	if (mute == TRUE)
		return;

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	fprintf(get_log_file_handle(),
			LOG_FORMAT, get_current_time_string(),
			WARN_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_error(const char* message, ...) {
	if (mute == TRUE)
		return;

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	/* Send errors to error log file handle */
	fprintf(get_error_log_file_handle(),
		LOG_FORMAT, get_current_time_string(),
		ERROR_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_debug(const char* message, ...) {
	if (mute == TRUE)
		return;

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	fprintf(get_log_file_handle(),
		LOG_FORMAT, get_current_time_string(),
		DEBUG_MESSAGE_PREFIX, buf);

	va_end(args);
}
