#include "simpleshell.h"

FILE *input = NULL;

int main(int argc, char **argv) {
	char buf[MAX_BUFFER];
	
	input = stdin; // default input as stdin

	// setting the shell environment
	char fullpath[MAX_BUFFER];
	if (realpath(argv[0], fullpath) != NULL) {
		setenv("shell", fullpath, 1);
	}

	// Batch mode
	
	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			perror("Error opening batch file");
			return 1;
		} else {
			printf("Batch mode: reading %s\n", argv[1]);
		}
	}

	while (1) {
		// if the input isnt a file
		if (input == stdin) {
			display_prompt(); //only displays the prompt in interactive mode
		}

		if (!fgets(buf, MAX_BUFFER, input)) //reads in the input and stores inside buf
			break;

		buf[strcspn(buf, NEWLINE)] = '\0';

		// skips command if its an empty line
		if (buf[0] == '\0')
			continue;
		
		// calls the function from utility.c
		parse_and_execute(buf);
	}

	//close the batch file
	if (input != stdin) {
		fclose(input);
	}

	return 0;
}
