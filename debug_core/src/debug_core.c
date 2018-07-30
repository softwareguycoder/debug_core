/*

 * debug.c
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 *      Contains routines for debugging, such as logging etc.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "debug_core.h"

void log_info(const char* message, ...) {
	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	printf("[INFO] %s\n", buf);

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

	printf("[WARN] %s\n", buf);

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

	printf("ERROR: %s\n", buf);

	va_end(args);
}

void log_debug(const char* message, ...) {
#ifndef DEBUG
	return;
#endif

	va_list args;
	va_start(args, message);

	if (message == NULL || message[0] == '\0') {
		return;
	}

	char buf[strlen(message + 1)];

	vsprintf(buf, message, args);

	printf("[DEBUG] %s\n", buf);

	va_end(args);
}
