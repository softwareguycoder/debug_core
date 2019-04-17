/*

 * debug_core.c
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

#define LOG_BUFFER_SIZE	1024
#define LOG_FORMAT		"%s %s %s\n"		// Format for fprintf to log messages
#define TIMESTRLEN		32				// Size of buffer for time string

BOOL g_bIsMute = FALSE;

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
		fp = NULL;
	}
	ReleaseLoggingMutex();
}

void CloseLogFileHandles() {
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
char* GetCurrentTimeString() {
	char* pszResult = NULL;

	// time_t is arithmetic time type
	time_t tNow;

	// Obtain current time
	// time() returns the current time of the system as a time_t value
	time(&tNow);

	pszResult = (char*) malloc((TIMESTRLEN + 1) * sizeof(char));

	// Format the curernt time as a string and return it
	//strftime(s, TIMESTRLEN, "%A, %B %d %Y ", localtime(&now));
	strftime(pszResult, TIMESTRLEN + 1, "%c", localtime(&tNow));

	return pszResult;
}

FILE* GetErrorLogFileHandle() {
	return g_fpErrorLog;
}

FILE* GetLogFileHandle() {
	return g_fpLog;
}

void SetErrorLogFileHandle(FILE* fpErrorLogFile) {
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

void SetLogFileHandle(FILE* fpLogFile) {
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

void SetLogFilePath(const char* pszPath) {
	CreateLogFileMutexIfNotExists();

	if (NULL == g_pLogFileMutex) {
		return;
	}

	GetLoggingMutex();
	{
		/* Check path to ensure it's not blank; if it is, then stop since we have
		 nothing to do. */
		if (pszPath == NULL || strlen(pszPath) == 0 || pszPath[0] == '\0')
			return;

		/* Attempt to open a new file for reading, writing, and appending.  Create the file
		 if it does not already exist.  For now, we will direct all logging and error outputs
		 to the same file. */
		g_fpLog = g_fpErrorLog = fopen(pszPath, "a+");
		if (g_fpLog == NULL) {
			g_fpLog = stdout;
			g_fpErrorLog = stderr;
			return;
		}
	}
	ReleaseLoggingMutex();
}

void ToggleDebug(BOOL bEnabled) {
	g_bIsMute = !bEnabled;
}

void WriteToLog(FILE* fp, const char* pszPrefix, const char* pszBuffer) {
	if (fp == NULL) {
		return;
	}

	CreateLogFileMutexIfNotExists();

	if (NULL == g_pLogFileMutex) {
		return;
	}

	GetLoggingMutex();
	{
		if (pszPrefix == NULL || pszPrefix[0] == '\0'
				|| strlen(pszPrefix) == 0) {
			ReleaseLoggingMutex();

			return;
		}

		if (pszBuffer == NULL || pszBuffer[0] == '\0'
				|| strlen(pszBuffer) == 0) {
			ReleaseLoggingMutex();

			return;
		}

		char* pszTimeString = GetCurrentTimeString();

		fprintf(fp, LOG_FORMAT, pszTimeString, pszPrefix, pszBuffer);
		fflush(fp);

		free(pszTimeString);
		pszTimeString = NULL;
	}
	ReleaseLoggingMutex();
}

void LogInfoToFileAndScreen(const char* pszMessage, ...) {
    if (IsNullOrWhiteSpace(pszMessage)) {
        return;
    }

    if (GetLogFileHandle() == NULL){
        SetLogFileHandle(stdout);
    }

    va_list args;
    va_start(args, pszMessage);

    LogInfo(pszMessage, args);

    if (GetLogFileHandle() != stdout) {
        fprintf(stdout, pszMessage, args);
    }

    va_end(args);
}

void LogInfo(const char* pszMessage, ...) {
	if (IsNullOrWhiteSpace(pszMessage)) {
		return;
	}

	if (g_bIsMute == TRUE)
		return;

	if (GetLogFileHandle() == NULL)
		SetLogFileHandle(stdout);

	va_list args;
	va_start(args, pszMessage);

	char szLogLine[LOG_BUFFER_SIZE + 1];
	memset(szLogLine, 0, LOG_BUFFER_SIZE + 1);

	vsprintf(szLogLine, pszMessage, args);

	char szTrimmedLogLine[LOG_BUFFER_SIZE + 1];
	memset(szTrimmedLogLine, 0, LOG_BUFFER_SIZE + 1);

	Trim(szTrimmedLogLine, LOG_BUFFER_SIZE + 1, szLogLine);

	WriteToLog(GetLogFileHandle(), INFO_MESSAGE_PREFIX, szTrimmedLogLine);

	va_end(args);
}

void LogWarning(const char* pszMessage, ...) {
	if (IsNullOrWhiteSpace(pszMessage)) {
		return;
	}

	if (g_bIsMute == TRUE)
		return;

	if (GetLogFileHandle() == NULL)
		SetLogFileHandle(stdout);

	va_list args;
	va_start(args, pszMessage);

	char szLogLine[LOG_BUFFER_SIZE + 1];
	memset(szLogLine, 0, LOG_BUFFER_SIZE + 1);

	vsprintf(szLogLine, pszMessage, args);

	char szTrimmedLogLine[LOG_BUFFER_SIZE + 1];
	memset(szTrimmedLogLine, 0, LOG_BUFFER_SIZE + 1);

	Trim(szTrimmedLogLine, LOG_BUFFER_SIZE + 1, szLogLine);

	WriteToLog(GetLogFileHandle(), WARN_MESSAGE_PREFIX, szTrimmedLogLine);

	va_end(args);
}

void LogError(const char* pszMessage, ...) {
	if (IsNullOrWhiteSpace(pszMessage)) {
		return;
	}

	if (g_bIsMute == TRUE)
		return;

	if (GetErrorLogFileHandle() == NULL)
		SetErrorLogFileHandle(stderr);

	va_list args;
	va_start(args, pszMessage);

	char szLogLine[LOG_BUFFER_SIZE + 1];
	memset(szLogLine, 0, LOG_BUFFER_SIZE + 1);

	vsprintf(szLogLine, pszMessage, args);

	char szTrimmedLogLine[LOG_BUFFER_SIZE + 1];
	memset(szTrimmedLogLine, 0, LOG_BUFFER_SIZE + 1);

	Trim(szTrimmedLogLine, LOG_BUFFER_SIZE + 1, szLogLine);

	WriteToLog(GetErrorLogFileHandle(), ERROR_MESSAGE_PREFIX, szTrimmedLogLine);

	va_end(args);
}

void LogDebug(const char* pszMessage, ...) {
	if (IsNullOrWhiteSpace(pszMessage)) {
		return;
	}

	if (g_bIsMute == TRUE)
		return;

	if (GetLogFileHandle() == NULL)
		SetLogFileHandle(stdout);

	va_list args;
	va_start(args, pszMessage);

	char szLogLine[LOG_BUFFER_SIZE + 1];
	memset(szLogLine, 0, LOG_BUFFER_SIZE + 1);

	vsprintf(szLogLine, pszMessage, args);

	char szTrimmedLogLine[LOG_BUFFER_SIZE + 1];
	memset(szTrimmedLogLine, 0, LOG_BUFFER_SIZE + 1);

	Trim(szTrimmedLogLine, LOG_BUFFER_SIZE + 1, szLogLine);

	WriteToLog(GetLogFileHandle(), DEBUG_MESSAGE_PREFIX, szTrimmedLogLine);

	va_end(args);
}
