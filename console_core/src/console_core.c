/*
 * console_core.c
 *
 *  Created on: Jul 30, 2018
 *      Author: bhart
 */

#include "stdafx.h"
#include "console_core.h"

void print_strings(char** strings, int count) {
	// The value of count must be greater than or equal to zero
	if (count <= 0)
		return;

	for (int i = 0; i < count; i++) {
		printf("%s\r\n", strings[i]);
	}
}

int get_line(const char *prmpt, char *buff, int size){
	int ch, extra;

	flush_stdin();

	// Get line with buffer overrun protection.

	if (prmpt != NULL) {
		printf("%s", prmpt);
		fflush(stdout);
	}

	if (fgets(buff, size, stdin) == NULL)
		return NO_INPUT;

	// If it was too long, there'll be no newline. In that case, we flush
	// to end of line so that excess doesn't affect the next call.
	if (buff[strlen(buff) - 1] != '\n') {
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF) && extra != 1)
			extra = 1;
		return (extra == 1) ? TOO_LONG : OK;
	}

	// Otherwise remove newline and give string back to caller.
	buff[strlen(buff) - 1] = '\0';

	return (strlen(buff) == size) ? EXACTLY_CORRECT : OK;
}

int get_line_with_default(const char *prmpt, char *buff, const char *default_value, int size) {
	int ch, extra;

	flush_stdin();

	// Get line with buffer overrun protection.

	if (prmpt != NULL) {
		printf("%s", prmpt);
		fflush(stdout);
	}

	if (fgets(buff, size, stdin) == NULL) {
		/* If we are here and there is no input by the user, then
		 * give the default_value; make sure that buff is big enough
		 * to hold the default_value.   We assume that the size parameter
		 * is filled with the length of the array pointed to by buff. */

		if (size < strlen(default_value)) {
			return NO_INPUT;
		}
		strcpy(buff, default_value);
		return OK;
	}

	// If it was too long, there'll be no newline. In that case, we flush
	// to end of line so that excess doesn't affect the next call.
	if (buff[strlen(buff) - 1] != '\n') {
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF) && extra != 1)
			extra = 1;
		return (extra == 1) ? TOO_LONG : OK;
	}

	// Otherwise remove newline and give string back to caller.
	buff[strlen(buff) - 1] = '\0';

	return (strlen(buff) == size) ? EXACTLY_CORRECT : OK;
}

void flush_stdin() {
	// make stdin non-blocking
	int flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, O_NONBLOCK);
	char c;

	// remove all characters from stdin
	while ((c = getchar()) != '\n' && c != EOF) {
	}

	// restore original flags
	fcntl(0, F_SETFL, flags);
}

