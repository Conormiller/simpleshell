#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t"
#define NEWLINE "\r\n"

extern char **environ;
extern FILE *input;

// Function prototypes for Stage1
void display_prompt();
void parse_and_execute(char *buf);

// Function prototypes for Stage2
void io_redirection(char **args, char **input_file, char **output_file, int *append);
int check_background(char **args);


#endif
