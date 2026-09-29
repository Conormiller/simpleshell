What is this shell:
simpleshell is a basic command line shell that was made using the programming language C. It allows the users to interact with their operating system by typing in simple commands which will get executed by the shell. This shell can carry out many operations such as navigating directories, displaying environment variables, numerous output commands and even pausing and quitting the shell.

How to compile it:
To compile the shell, you should use the provided makefile by typing the command make inside the Stage2 directory, doing so will generate an executable file called simpleshell inside the directory called bin.

How to run it:
- Interactive mode: 
	o To start the shell using interactive mode, simply type in the command ./bin/simpleshell from the Stage2 directory.
	o You can now start entering commands. 
	o Example:
	o Stage2 ==> echo hello 
		hello

- Batch mode:
	o You can also read in commands from a batchfile. 
	o In the batchfile you can type out a list of commands that will be executed sequentially by the shell
	o For example, if you have a batchfile called commands.txt that contains the text echo hello from batch and run it using the command ./bin/simpleshell commands.txt, it will output hello from batch.


The internal commands:
- cd: changes the current working directory. Example: cd src will change the current working directory to src, if you only type cd and don't provide a directory name, it will print your current directory.
- clr: clears your screen.

- dir: lists the content of a directory. If you don't pass in a directory name it will list the contents of your current directory, but you can also give the name of a directory name to see its contents.

	o Stage2 ==> dir, will list everything in Stage2
	o Stage2 ==> dir src, will list everything in src

- environ: environ will list all the environment variables.
- echo: displays the text that was entered by the user
	o Stage2 ==> echo Hello World
		Hello World
- help: displays the user manual.
- pause: pauses the shell until the Enter key is pressed.
- quit: Exits the shell.


The External commands:
- Simpleshell can also execute standard UNIX programs such as ls, cat and pwd.
- These commands are run as a child process using fork and exec.
- Each of these external commands are executed with an environment variable parent set to the path of the Simpleshell executable.


I/O Redirection:
-	'<' redirects standard input from a file. Example: sort < input.txt
- '>' redirects standard output in to a file. Example: ls > output.txt (Overwrites the file)
- '>>' redirects standard output in to a file. Example: echo hello >> output.txt (appends to the file)
- I/O Redirection supports the use of both external and internal commands.


Background Execution:
- Adding an ampersand '&' at the end of a command will execute the command in the background.
- Example: sleep 10 &
	o The shell will return a prompt while the program continues running in the background.

	Foreground Execution (no &): The shell will wait for the command to finish executing before a new command can be run.
	Background Execution: The shell will not wait and will allow for more commands to be run while the command runs in the background.

