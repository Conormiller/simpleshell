What is this shell:
simpleshell is a basic command line shell that was made using the programming language C. It allows the users to interact with their operating system by typing in simple command which will get executed by the shell. This shell can carry out many operations such as navigating directories, displaying environment variables, numerous output commands and even pausing and quitting the shell.

How to compile it:
To compile the shell, you should you the provided makefile by typing the command make inside the Stage1 directory, doing so will generate a executable file called simpleshell inside the directory called bin.

How to run it:
- Interactive mode: 
	o To start the shell using interactive mode, simply type in the command ./bin/simpleshell from the Stage1 directory.
	o You can now start entering commands. 
	o Example:
	o Stage1 ==> echo hello 
		hello

- Batch mode:
	o You can also read in commands from a batchfile. 
	o In the batchfile you can type out a list of commands that will be execute in the shell
	o For example, if you have a batchfile called commands.txt that contains the text echo hello from batch and run it using the command ./bin/simpleshell commands.txt, it will output hello from batch.


The internal commands:
- cd: changes the current working directory. Example: cd src will change the current working directory to src, if you only type cd and don't provide a directory name, it will print your current directory.
- clr: clears your screen.

- dir: lists the content of a directory. If you don't pass in a directory name it will lists the contents of your current directory, but you can also give the name of a directory name to see its contents.

	o Stage1 ==> dir, will list everything in Stage1
	o Stage1 ==> dir src, will list everything in src

- environ: environ will list all the environment variables.
- echo: displays the text that was entered by the user
	o Stage1 ==> echo Hello Word
		Hello World
- help: displays the user manual.
- pause: pauses the shell until the Enter key is pressed.
- quit: Exits the shell.
