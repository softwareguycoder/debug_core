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

int char_to_long(const char* input, long* output) {
	char *endptr = NULL; /* pointer to additional chars */
	int base = 10; /* default to reading digits and interpreting as base 10 */
	long number = 0; /* variable holding return */

	/* reset errno to zero before call */
	errno = 0;

	/* call to strtol assigning return to number */
	number = strtol(input, &endptr, base);

	/* test return to number and errno values */
	if (input == endptr)
		return INVALID_CHARS;
	else if (errno == ERANGE && number == LONG_MIN)
		return BUFFER_UNDERFLOW;
	else if (errno == ERANGE && number == LONG_MAX)
		return BUFFER_OVERFLOW;
	else if (errno == EINVAL) /* not all c99 implementations; gcc ok */
		return INVALID_BASE;
	else if (errno != 0 && number == 0)
		return UNSPECIFIED_ERROR;
	else if (errno == 0 && input && !*endptr) {
		/* result is valid and represents all characters read */
		*output = number;
		return EXACTLY_CORRECT;
	} else if (errno == 0 && input && *endptr != 0) {
		/* result is valid but the string contained non-digit characters too */
		*output = number;
		return OK;
	}

	return 0;
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

