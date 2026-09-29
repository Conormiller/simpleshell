#include "simpleshell.h"

void display_prompt() {
	char cwd[MAX_BUFFER];
	if (getcwd(cwd,sizeof(cwd)) != NULL) {
		printf("%s ==> ", cwd); //displays the prompt and the current working directory
	}
}

// function to check if the command should be run in the background (ends with &)
int check_background(char **args) {
	int i;
	for (i = 0; args[i] != NULL; i++);

	if (i > 0 && strcmp(args[i - 1], "&") == 0) {
		args[i - 1] = NULL;
		return 1;
	}
	return 0;
}

// checks to see if the command that was passed through is one of the internal commands 
int is_internal(char *cmd) {
	const char *commands[] = {
		"cd", "clr", "dir", "environ", "echo", "help", "pause", "quit", NULL // list of all the internal commands and if no command is passed
	};

	for (int i = 0; commands[i] != NULL; i++) {
		if (strcmp(cmd, commands[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void apply_redirection(char *input_file, char *output_file, int append) {
	if (input_file) { // if input redirection is true (<)
		if (!freopen(input_file, "r", stdin)) { //reopen stdin to read from input_file
			perror("Input redirection failed");
			exit(1); // terminate the child process if it fails
		}
	}
	if (output_file) {
		if (!freopen(output_file, append ? "a" : "w", stdout)) { // open the file in either append mode (>>) or write (>) mode
			perror("Output redirection failed");
			exit(1); // terminate the child process if it fails
		}
	}
}

void io_redirection(char **args, char **input_file, char **output_file, int *append) {
	for (int i = 0; args[i] != NULL; i++) { // scans through the arguments to find one of the redirection operators
		if (strcmp(args[i], "<") == 0 && args[i + 1]) { // input redirection
			*input_file = args[i + 1]; // stores the input filenames
			args[i] = NULL; // remove < from the arguments
			args[i + 1] = NULL; // remove the file name from the arguments
			i++; // skips the inputfile
		} else if (strcmp(args[i], ">") == 0 && args[i + 1]) { 
			*output_file = args[i + 1];
			*append = 0; // append is set to false making it overwrite mode
			args[i] = NULL;
			args[i + 1] = NULL;
			i++; 
		} else if (strcmp(args[i], ">>") == 0 && args[i + 1]) {
			*output_file = args[i + 1];
			*append = 1; // append mode is true
			args[i] = NULL;
			args[i + 1] = NULL;
			i++;
		}
	}
	
	// compacts the array of arguments to remove NULL gaps left by the redirection tokens
	int j = 0;
	for (int i = 0; i < MAX_ARGS && args[i] != NULL; i++) {
		if (args[i] != NULL) {
			args[j++] = args[i];
		}
	}
	args[j] = NULL;
}

void run_internal(char **args) {

	// cd
	if (!strcmp(args[0], "cd")) {
		char cwd[MAX_BUFFER];
		if (args[1] == NULL) {  // If no argument is passed after cd
			if (getcwd(cwd, sizeof(cwd)) != NULL) { // gets the current working directory
				printf("%s\n", cwd); // prints the current working directory 
			} else {
				perror("cd getcwd failed"); // if getcwd fails print error message
			}
		} else {
			if (chdir(args[1]) != 0) { // checks response code of chdir, 0 means it was a success, -1 means an error
				perror("cd failed"); // if chdir fails print error message
			} else {
				if (getcwd(cwd, sizeof(cwd)) != NULL) {
					setenv("PWD", cwd, 1); //updates the environment variable after changing the directory
				}
			}
		}
	}

	// clr
	else if (!strcmp(args[0], "clr")) {
		system("clear");
	}

	// dir
	else if (!strcmp(args[0], "dir")) {
		char command[MAX_BUFFER];

		if (args[1] == NULL) {
			snprintf(command, sizeof(command), "ls -al ."); // if a directory isn't given run ls -al on the current directory
		} else {
			snprintf(command, sizeof(command), "ls -al %s", args[1]); // if a directory is given run ls -al on the given directory
		}

		if (system(command) == -1) {
			perror("dir failed"); // if system call fails
		}
	}

	// environ
	else if (!strcmp(args[0], "environ")) {
		char **env = environ;
		while (*env) { // loop to iterate through the environment variables 
			printf("%s\n", *env++); // print the current variable 
		}
	}

	//echo
	else if (!strcmp(args[0], "echo")) {
		char **p = args + 1; // skips the word echo in the command
		int first = 1; // first on to be true

		while (*p) {
			if (!first) printf(" "); // if it isn't the first word print a space between the arguments, this ensures that the output doesn't start with a space
			printf("%s", *p); // 
			first = 0; // sets first to False to make sure a space is printed between the arguments
			p++;
		}
		printf("\n");
	}

	//help
	else if (!strcmp(args[0], "help")) {

		// Use the environment variable to find the full file path of the simpleshell executable
		char *shellPath = getenv("shell");
		if (!shellPath) {
			perror("shell variable not set");
			return;
		}

		char helpPath[MAX_BUFFER];
		strcpy(helpPath, shellPath);
		
		// remove the executable name from the file path by removing everything after the last slash
		char *lastSlash = strrchr(helpPath, '/');
		if (lastSlash) {
			*lastSlash = '\0';
			strcat(helpPath, "/../manual/help.txt"); //as the executable is in Stage2/bin/simpleshell by removing the simpleshell and then appending ../manual/help.txt we have the location of help.txt
		}

		// fork a child process to run "more", doing this prevents replacing the shell process a problem I encountered while trying different implementations
		pid_t pid = fork();

		if (pid == 0) {
			execlp("more", "more", helpPath, NULL); // child process executes "more" to display help.txt
			perror("help failed"); // if exec fails
			exit(1);
		} else if (pid > 0) {
			wait(NULL); // wait for child process to finish
		} else {
			perror("fork failed");
		}
	}

	//pause
	else if (!strcmp(args[0], "pause")) {
		printf("Paused: Press Enter to continue...");
		fflush(stdout); //makes sure the message is printed immediately
		while (getchar() != '\n'); // waits for the user to press the Enter key
	}

	//quit
	else if (!strcmp(args[0], "quit")) {
		exit(0);
	}
}

// executes the internal commands, uses fork if redirection or background execution is needed
void exec_internal(char **args, char *input_file, char *output_file, int append, int background) {
	if (input_file || output_file || background) { //if redirection or background use fork
		pid_t pid = fork();

		if (pid == 0) {
			apply_redirection(input_file, output_file, append); // uses child process to apply any I/O redirection
			run_internal(args); // execute the internal command
			exit(0); // terminates the child processes after it executes
		} else if (pid > 0) { // parent process, waits unless it is running in the background
			if (!background) {
				wait(NULL);
			}
		} else {
			perror("fork failed");
		}
	} else {
		run_internal(args); // if no fork call the internal arguments
	}
}

void exec_external(char **args, char *input_file, char *output_file, int append, int background) {
	pid_t pid = fork();

	if (pid == 0) {
		char *shell = getenv("shell");
		if (shell) {
			setenv("parent", shell, 1); // sets the parent environment variable
		}

		apply_redirection(input_file, output_file, append); // applies input/output redirection

		execvp(args[0], args);
		
		// user friendly error if a command is not found, it originally displayed an exec error where no such file or directory was found, I thought that I could make it more user friendly by making it more shell like
		if (errno == ENOENT) {
			fprintf(stderr, "%s: command not found\n", args[0]);
		} else {
			perror("exec failed");
		}
		exit(1); // terminates the child process on failure

	} else if (pid > 0) {
		if (!background) {
			wait(NULL); // waits unless a background process is requested
		}
	} else {
		perror("fork failed");
	}
}

// parses the input, converts it into arguments and then executes the commands
void parse_and_execute(char *buf) {
	char *args[MAX_ARGS];
	int i = 0;

	// splits the input string into tokens using a whitespace separator
	args[i] = strtok(buf, SEPARATORS);
	while (args[i] != NULL && i < MAX_ARGS - 1) {
		i++;
		args[i] = strtok(NULL, SEPARATORS);
	}
	args[i] = NULL;

	// ignores the input if its empty
	if (args[0] == NULL) {
		return;
	}

	char *input_file = NULL;
	char *output_file = NULL;
	int append = 0;

	int background = check_background(args); // checks for background execution
	io_redirection(args, &input_file, &output_file, &append); // detect and processes any I/O redirection operation symbol

	// I encounted an error while testing where if I ran < file.txt without passing in a command before hand I ran into a segmentation fault, this fixes that
	if (args[0] == NULL) {
		fprintf(stderr, "Error: No command provided\n");
		return;
	}

	// determines if a command is internal or external and then executes them accordingly
	if (is_internal(args[0])) {
		exec_internal(args, input_file, output_file, append, background);
	} else {
		exec_external(args, input_file, output_file, append, background);
	}
}
