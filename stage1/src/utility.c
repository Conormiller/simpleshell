#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "simpleshell.h"

extern char **environ;
extern FILE *input;

// get the current wordking directory
void display_prompt() {
	char cwd[MAX_BUFFER];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("%s ==> ", cwd);
	}
}

void parse_and_execute(char *buf) {
	char *args[MAX_ARGS];
	int i = 0;

	args[i] = strtok(buf, SEPARATORS);
	while (args[i] != NULL && i < MAX_ARGS - 1) {
		i++;
		args[i] = strtok(NULL, SEPARATORS);
	}
	
	// check if the command line is empty 
	args[i] = NULL;
	if (args[0] == NULL)
		return;

	// clr
	
	if (!strcmp(args[0], "clr")) {
		system("clear");
		return;
	}

	// dir
	
	if (!strcmp(args[0], "dir")) {
		char command[MAX_BUFFER];
		
		if (args[1] == NULL) {
			snprintf(command, sizeof(command), "ls -al ."); // lists the contents of the current directory if no directory is given
		} else {
			snprintf(command, sizeof(command), "ls -al %s", args[1]); //snprintf rewrites the command variable to include the given directory so that it can later be pushed as system(command).
		}
		if (system(command) == -1) {
			perror("dir failed");
		}
		return;
	}

	// cd
	
	if (!strcmp(args[0], "cd")) {
		char cwd[MAX_BUFFER];
		// if no argument is given, report the current directory
		if (args[1] == NULL) {
			if (getcwd(cwd, sizeof(cwd)) != NULL) {
				printf("%s\n", cwd);
			} else {
				perror("cd: getcwd failed");
			} 
		} else {
			if (chdir(args[1]) != 0) {
			perror("cd failed");
			} else {
			// update the environment after the directory is changed
			if (getcwd(cwd, sizeof(cwd)) != NULL) {
				setenv("PWD", cwd, 1);
			} else {
				perror("cd: getcwd failed");
				}
			}
		}
		return;
	}

	// environ

	if (!strcmp(args[0], "environ")) {
		char **env = environ;

		while (*env)
			printf("%s\n", *env++);
		return;
	}

	// echo
	
	if (!strcmp(args[0], "echo")) {
		char **pArg = args + 1; // skips "echo"
		int first = 1;

		while (*pArg) {
			if (!first) printf(" "); //only prints one space between arguments 
			printf("%s", *pArg);
			first = 0;
			pArg++;
		}
		printf("\n");
		return;
	}

	//help
	
	if (!strcmp(args[0], "help")) {
		char *shellPath = getenv("shell");
		if (shellPath == NULL) {
			perror("shell variable not set");
			return;
		}

		char helpPath[MAX_BUFFER];

		strcpy(helpPath, shellPath);
		char *lastSlash = strrchr(helpPath, '/'); //remove the last, used to remove the name of the executable from the path
		if (lastSlash) {
			*lastSlash = '\0';
			strcat(helpPath, "/../manual/help.txt"); // find the location of the help.txt by added /../manual/help.txt to the end of the path of the binary shell file.
		}

		pid_t pid = fork(); // create a child process

		if (pid == 0) {
			execlp("more", "more", helpPath, NULL); // child process runs the program
			perror("help failed");
			exit(1);
		} else if (pid > 0) {
			wait(NULL);
		} else {
			perror("fork failed");
		}
		return;
	}

	// pause
	
	if (!strcmp(args[0], "pause")) {
		printf("Paused: Press Enter to continue...");
		fflush(stdout);
		while (getchar() != '\n');
		return;
	}

	// quit
	
	if (!strcmp(args[0], "quit")) {
		exit(0);
	}
	// return error if the provided command isnt found.
	printf("%s: Command not found\n", args[0]);
}

