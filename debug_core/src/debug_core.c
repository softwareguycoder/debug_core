/*

 * debug.c
 *
 *  Created on: Jul 13, 2018
 *      Author: bhart
 *      Contains routines for debugging, such as logging etc.
 */

#include "stdafx.h"
#include "debug_core.h"

// Flag that turns this library on and off
BOOL mute = FALSE;

/**
 * Turns this library's messages on or off.
 * @param enabled TRUE to enable this library; FALSE to turn all messages off.
 * @remarks This library is enabled by default.
 */
void toggle_debug(BOOL enabled)
{
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

	printf("%s %s\n", INFO_MESSAGE_PREFIX, buf);

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

	printf("%s %s\n", WARN_MESSAGE_PREFIX, buf);

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

	printf("%s %s\n", ERROR_MESSAGE_PREFIX, buf);

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

	printf("%s %s\n", DEBUG_MESSAGE_PREFIX, buf);

	va_end(args);
}
