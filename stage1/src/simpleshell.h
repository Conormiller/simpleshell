#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#include <stdio.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t"
#define NEWLINE "\r\n"

extern FILE *input;

void display_prompt();
void parse_and_execute(char *buf);

#endif

