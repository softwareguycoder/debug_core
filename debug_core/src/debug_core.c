/*

 * debug.c
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 *      Contains routines for debugging, such as logging etc.
 *      NOTE!  This code is NOT thread-safe!  Use the debug_threadsafe library
 *      in a multithreaded logging environment.  The debug_threadsafe library has
 *      dependencies on threading_core and mutex_core; however, if you're using that
 *      specific library, it's assumed you're using those as dependencies anyway.
 */

#include "stdafx.h"
#include "debug_core.h"

#define LOG_FORMAT		"%s %s %s\n"		// Format for fprintf to log messages
#define TIMESTRLEN		1024				// Size of buffer for time string

BOOL mute = FALSE;

// File handles to support writing logs to a file

FILE* g_fpLog;             // File handle for logging messages other than errors
FILE* g_fpErrorLog;                         // File handle for logging errors

pthread_mutex_t* g_pLogFileMutex;            // Mutex for accessing the log file

void GetLoggingMutex() {
	if (NULL == g_pLogFileMutex) {
		return;
	}

	int nResult = pthread_mutex_lock(g_pLogFileMutex);
	if (OK != nResult) {
		fprintf(stderr,
				"debug_core: Failed to obtain lock on mutex used for logging.\n");
		exit(ERROR);
	}
}

void ReleaseLoggingMutex() {
	if (NULL == g_pLogFileMutex) {
		return;
	}

	int nResult = pthread_mutex_unlock(g_pLogFileMutex);
	if (OK != nResult) {
		fprintf(stderr, "debug_core: Failed to release the log file mutex.\n");
		exit(ERROR);
	}
}

/**
 * @brief Checks whether the global hLogFileMutex handle has a valid value; if not, re-initializes it.
 */
void CreateLogFileMutexIfNotExists() {

	if (NULL != g_pLogFileMutex) {
		return;
	}

	/* If the global g_pLogFileMutex reference is invalid, then create a new mutex handle to be
	 used for all subsequent calls */
	pthread_mutex_t* pMutex = (pthread_mutex_t*) malloc(
			sizeof(pthread_mutex_t));
	if (pMutex == NULL) {
		fprintf(stderr, "Failed to allocate mutex for logging.\n");
		exit(ERROR);
	}

	// Call pthread_mutex_init.  This version of CreateMutex just passes a
	// mutex handle for the function to initialize with NULL for the attributes.
	int nResult = pthread_mutex_init(pMutex, NULL);
	if (OK != nResult) {
		// Cleanup the mutex handle if necessary
		if (pMutex != NULL) {
			free(pMutex);
			pMutex = NULL;
		}

		fprintf(stderr, "Failed to allocate mutex for logging.\n");
		exit(ERROR);
	}

	g_pLogFileMutex = pMutex;
}

/**
 * @brief Obtains a lock on the file mutex before performing the operation of closing the file handle.
 * @param fp FILE pointer providing a handle to the file to be closed.
 * @remarks This function refuses to operate if fp is NULL, or if it is stdin/stdout/stderr.
 */
void InterlockedCloseFile(FILE* fp) {
	CreateLogFileMutexIfNotExists();

	GetLoggingMutex();
	{
		if (fp == NULL) {
			ReleaseLoggingMutex();
			return;
		}
		if (fp == stdout) {
			ReleaseLoggingMutex();
			return;
		}
		if (fp == stderr) {
			ReleaseLoggingMutex();
			return;
		}

		fclose(fp);
	}
	ReleaseLoggingMutex();
}

void close_log_file_handles() {
	/* Close the g_fpLog file handle, if and only if it is not
	 * currently referencing the standard output or is a NULL value
	 * already. */
	InterlockedCloseFile(g_fpLog);

	/* Close the g_fpErrorLog file handle, if and only if it is not
	 * already referencing standard error, or if it is non-NULL. */
	InterlockedCloseFile(g_fpErrorLog);
}

/**
 * @name get_current_time_string
 * @brief Returns the string obtained by calling asctime.
 * @remarks Utilized by the calls to fprintf that write log messages.  This function
 * is only available internally to this source file.
 */
char* get_current_time_string() {
	char* s = NULL;

	// time_t is arithmetic time type
	time_t now;

	// Obtain current time
	// time() returns the current time of the system as a time_t value
	time(&now);

	s = (char*) malloc(TIMESTRLEN * sizeof(char));

	// Format the curernt time as a string and return it
	//strftime(s, TIMESTRLEN, "%A, %B %d %Y ", localtime(&now));
	strftime(s, TIMESTRLEN, "%c", localtime(&now));

	return s;
}

FILE* get_error_log_file_handle() {
	return g_fpErrorLog;
}

FILE* get_log_file_handle() {
	return g_fpLog;
}

void set_error_log_file(FILE* fpErrorLogFile) {
	CreateLogFileMutexIfNotExists();

	if (NULL == g_pLogFileMutex) {
		return;
	}

	GetLoggingMutex();
	{
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
	ReleaseLoggingMutex();
}

void set_log_file(FILE* fpLogFile) {
	CreateLogFileMutexIfNotExists();

	if (NULL == g_pLogFileMutex) {
		return;
	}

	GetLoggingMutex();
	{
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
	ReleaseLoggingMutex();
}

void set_log_file_path(const char* path) {
	CreateLogFileMutexIfNotExists();

	if (NULL == g_pLogFileMutex) {
		return;
	}

	GetLoggingMutex();
	{
		/* Check path to ensure it's not blank; if it is, then stop since we have
		 nothing to do. */
		if (path == NULL || strlen(path) == 0 || path[0] == '\0')
			return;

		/* Attempt to open a new file for reading, writing, and appending.  Create the file
		 if it does not already exist.  For now, we will direct all logging and error outputs
		 to the same file. */
		g_fpLog = g_fpErrorLog = fopen(path, "a+");
		if (g_fpLog == NULL) {
			g_fpLog = stdout;
			g_fpErrorLog = stderr;
			return;
		}
	}
	ReleaseLoggingMutex();
}

void toggle_debug(BOOL enabled) {
	mute = !enabled;
}

void write_log(FILE* fp, const char* prefix, const char* buf) {
	CreateLogFileMutexIfNotExists();

	if (NULL == g_pLogFileMutex) {
		return;
	}

	GetLoggingMutex();
	{
		if (fp == NULL) {
			return;
		}

		if (prefix == NULL || prefix[0] == '\0' || strlen(prefix) == 0) {
			return;
		}

		if (buf == NULL || buf[0] == '\0' || strlen(buf) == 0) {
			return;
		}

		char* timestring = get_current_time_string();

		fprintf(fp, LOG_FORMAT, timestring, prefix, buf);
		fflush(fp);


		free(timestring);
		timestring = NULL;
	}
	ReleaseLoggingMutex();
}

void log_info(const char* message, ...) {
	if (mute == TRUE)
		return;

	if (get_log_file_handle() == NULL)
		set_log_file(stdout);

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0' || strlen(message) == 0) {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	write_log(get_log_file_handle(), INFO_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_warning(const char* message, ...) {
	if (mute == TRUE)
		return;

	if (get_log_file_handle() == NULL)
		set_log_file(stdout);

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0' || strlen(message) == 0) {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	write_log(get_log_file_handle(), WARN_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_error(const char* message, ...) {
	if (mute == TRUE)
		return;

	if (get_error_log_file_handle() == NULL)
		set_error_log_file(stderr);

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0' || strlen(message) == 0) {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	write_log(get_error_log_file_handle(), ERROR_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_debug(const char* message, ...) {
	if (mute == TRUE)
		return;

	if (get_log_file_handle() == NULL)
		set_log_file(stdout);

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0' || strlen(message) == 0) {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	write_log(get_log_file_handle(), DEBUG_MESSAGE_PREFIX, buf);

	va_end(args);
}
