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

FILE* g_fpLog;
FILE* g_fpErrorLog;

void close_log_file() {
	/* Close the g_fLog file handle, if and only if it is not
	 * currently referencing the standard output or is a NULL value
	 * already.
	 */
	if (g_fpLog != NULL && g_fpLog != stdout) {
		fclose(g_fpLog);
		g_fpLog = NULL;
	}

	/* Close the g_fErrorLog file handle, if and only if it is not
	 * already referencing standard error, or if it is non-NULL. */
	if (g_fpErrorLog != NULL && g_fpErrorLog != stderr) {
		fclose(g_fpErrorLog);
		g_fpErrorLog = NULL;
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
	return g_fpErrorLog;
}

FILE* get_log_file_handle() {
	return g_fpLog;
}

void set_error_log_file(FILE* fpErrorLogFile) {
	// Check whether the FILE pointer given to us is non-NULL;
	// if it's NULL, then stop.
	if (fpErrorLogFile == NULL) {
			g_fpErrorLog = stderr;
			return;
	}
	// Set the global error log file handle to the value passed.
	// Callers are responsible for ensuring that the FILE pointer
	// passed is a handle to a file that is currently open for
	// reading, writing, and/or appending.
	g_fpErrorLog = fpErrorLogFile;
}

void set_log_file(FILE* fpLogFile) {
	// Check whether the FILE pointer given to us is non-NULL;
	// if it's NULL, then stop.
	if (fpLogFile == NULL) {
		g_fpLog = stdout;
		return;
	}

	// Set the global log file handle to the value passed.
	// Callers are responsible for ensuring that the FILE pointer
	// passed is a handle to a file that is currently open for
	// reading, writing, and/or appending.
	g_fpLog = fpLogFile;
}

void set_log_file_path(const char* path) {
	if (path == NULL || strlen(path) == 0 || path[0] == '\0')
		return;

	g_fpLog = g_fpErrorLog = fopen(path, "a+");
	if (g_fpLog == NULL) {
		g_fpLog = stdout;
		g_fpLog = stderr;
		return;
	}
}

void toggle_debug(BOOL enabled) {
	mute = !enabled;
}

void log_info(const char* message, ...) {
	if (mute == TRUE)
		return;

	if (get_log_file_handle() == NULL)
		set_log_file(stdout);

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

	if (get_log_file_handle() == NULL)
		set_log_file(stdout);

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

	if (get_error_log_file_handle() == NULL)
		set_error_log_file(stderr);

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

	if (get_log_file_handle() == NULL)
		set_log_file(stdout);

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
