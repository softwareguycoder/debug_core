/*

 * debug.c
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 *      Contains routines for debugging, such as logging etc.
 */

#include "stdafx.h"
#include "debug_core.h"

void log_info(const char* message, ...) {
	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	printf("%s %s\n", INFO_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_warning(const char* message, ...) {
	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	printf("%s %s\n", WARN_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_error(const char* message, ...) {
	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	printf("%s %s\n", ERROR_MESSAGE_PREFIX, buf);

	va_end(args);
}

void log_debug(const char* message, ...) {

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	printf("%s %s\n", DEBUG_MESSAGE_PREFIX, buf);

	va_end(args);
}
