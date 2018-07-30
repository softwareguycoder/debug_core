/*
 * console_core.h
 *
 *  Created on: Jul 30, 2018
 *      Author: bhart
 */

#ifndef __CONSOLE_CORE_H__
#define __CONSOLE_CORE_H__

#include "symbols.h"

int char_to_long(const char* input, long* output);
void flush_stdin();
int get_line(const char *prmpt, char *buff, int size);
int get_line_with_default(const char *prmpt, char *buff, const char *default_value, int size);
void print_strings(char** strings, int count);

#endif /* __CONSOLE_CORE_H__ */
