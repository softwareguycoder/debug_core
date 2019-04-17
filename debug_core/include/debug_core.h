// debug.h - Functions to provide logging support

#ifndef __DEBUG_CORE_H__
#define __DEBUG_CORE_H__

#include <../../common_core/common_core/include/common_core.h>

/**
 * @name DEBUG_MESSAGE_PREFIX
 * @brief Constant that defines the string prefix for a message at the DEBUG level.
 */
#define DEBUG_MESSAGE_PREFIX	"[   DEBUG   ]"

/**
 * @name ERROR_MESSAGE_PREFIX
 * @brief Constant that defines the string prefix for a message at the ERROR level.
 */
#define ERROR_MESSAGE_PREFIX	"[   ERROR   ]"

/**
 * @name WARN_MESSAGE_PREFIX
 * @brief Constant that defines the string prefix for a message at the WARN level.
 */
#define WARN_MESSAGE_PREFIX		"[   WARN    ]"

/**
 * @name INFO_MESSAGE_PREFIX
 * @brief Constant that defines the string prefix for a message at the INFO level.
 */
#define INFO_MESSAGE_PREFIX     "[   INFO    ]"

/**
 * @name OK
 * @brief Exit code to be returned from processes upon successful completion; operating system/standard
 * library functions also (generally, as a rule) return this to indicate success.
 */
#ifndef OK
#define OK		0		// Code to return to the operating system to indicate successful program termination
#endif //OK

/**
 * @name ERROR
 * @brief Exit code to be returned from processes when an error condition has arisen; some operating system/standard
 * library functions also return this upon failure, although generally, as a rule, these functions return some
 * other nonzero value.
 */
#ifndef ERROR
#define ERROR	-1		// Code to return to the operating system to indicate an error condition
#endif //ERROR

/**
 * @name close_log_file_handles
 * @brief Closes the log file handles.
 * @remarks Call this function any time you are logging to disk instead of STDOUT/STDERR (the default).
 */
void CloseLogFileHandles();

/**
 * @name get_error_log_file_handle
 * @brief Gets a reference to the FILE pointer that is in use for error logging.
 */
FILE* GetErrorLogFileHandle();

/**
 * @name get_log_file_handle
 * @brief Gets a reference to the FILE pointer that is in use for logging.
 * @returns FILE pointer to the file open for logging.  Can be set to stdin, stdout, or stderr.
 */
FILE* GetLogFileHandle();

/**
 * @name set_error_log_file
 * @brief Sets the FILE pointer to be used for error logging.  Can be set to stdin, stdout, or stderr or a file.
 * @param fpErrorLogFile FILE pointer to be used for error logging.
 * @remarks If this function is not called, then stderr is used for error logging by default.
 */
void SetErrorLogFileHandle(FILE* fpErrorLogFile);

/**
 * @name set_log_file
 * @brief Sets the FILE pointer to be used for logging.  Can be set to stdin, stdout, or stderr.
 * @remarks If this function is not called, then stdout is used for logging by default.
 */
void SetLogFileHandle(FILE* fpLogFile);

/**
 * @name toggle_debug
 * @brief Turns this library's messages on or off.
 * @param enabled TRUE to enable this library; FALSE to turn all messages off.
 * @remarks This library is enabled by default.
 */
void ToggleDebug(BOOL bEnabled);

void LogInfoToFileAndScreen(const char* pszMessage, ...);

/**
 * @name log_info
 * @brief Logs a formatted [INFO] message to standard output.
 * @param message The message to be displayed, with any printf-compatible format specifiers.
 * @param ... Format arguments.
 */
void LogInfo(const char* pszMessage, ...);

/**
 * @name log_warning
 * @brief Logs a formatted [WARN] message to standard output.
 * @param message The message to be displayed, with any printf-compatible format specifiers.
 * @param ... Format arguments.
 */
void LogWarning(const char* pszMessage, ...);

/**
 * @name log_error
 * @brief Logs a formatted [ERROR] message to standard output.
 * @param message The message to be displayed, with any printf-compatible format specifiers.
 * @param ... Format arguments.
 */
void LogError(const char* pszMessage, ...);

/**
 * @name log_debug
 * @brief Logs a formatted [DEBUG] message to standard output.
 * @param message The message to be displayed, with any printf-compatible format specifiers.
 * @param ... Format arguments.
 */
void LogDebug(const char* pszMessage, ...);

#endif //__DEBUG_CORE_H__
