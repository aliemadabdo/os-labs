#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define	MAX_SIZE_CMD	256			// max command size
#define	MAX_SIZE_ARG	16			//the maximum number of arguments that a command can have

char cmd[MAX_SIZE_CMD];				// string holder for the command
char *argv[MAX_SIZE_ARG];			// an array for 'command' and arguments
pid_t pid;							// global variable for the child process ID
char i;								// global for loop counter

void get_cmd();						// get command string from the user
void convert_cmd();					// convert the command string to the required format by execvp()
void c_shell();						// to start the shell
void log_handle(int sig);			// signal handler to add log statements

int main(){
	// tie the handler to the SGNCHLD signal
	signal(SIGCHLD, log_handle);

	// start the shell
	c_shell();

	return 0;
}

void c_shell(){
	while(1){
		// get the command from user
		get_cmd();

		// bypass empty commands
		if(!strcmp("", cmd)) continue;

		// check for "exit" command
        if(!strcmp("exit", cmd)) break;

		// fit the command into *argv[]
		convert_cmd();

		// fork and execute the command
		pid = fork();
		if(-1 == pid){
			printf("failed to create a child\n");
		}
		else if(0 == pid){
			// printf("hello from child\n");
			// execute a command
			execvp(argv[0], argv);
		}
		else{
			// printf("hello from parent\n");
			// wait for the command to finish if "&" is not present
			if(NULL == argv[i]) waitpid(pid, NULL, 0);
		}
	}
}

void get_cmd(){
	// get command from user
	printf("(*o*)>>>\t");
	fgets(cmd, MAX_SIZE_CMD, stdin);
	// remove trailing newline
	if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n'))
        	cmd[strlen (cmd) - 1] = '\0';
	//printf("%s\n", cmd);
}

void convert_cmd(){
	// split string into argv
	char *ptr; 
	i = 0; 	   

/* Explanation..

The strtok() function works by splitting a string into smaller "tokens" based on 
a delimiter character, in this case, a space character.

When strtok() is first called, it takes two arguments:
the string to be tokenized (cmd in this case) and the delimiter
character (" " in this case). It then returns a pointer to the 
first token found in the string, which is the first word of the command entered by the user.

If strtok() is called again with the same string (cmd) but with 
a NULL pointer as the first argument, it will start searching for
the next token in the string from where it left off last 
time (i.e., after the first token). This allows you to extract 
each subsequent token in the string by calling strtok() repeatedly 
with a NULL pointer as the first argument.

In the code example provided, the while loop uses strtok() to 
extract each token in the string (cmd) and store it in the argv array, 
which will eventually be passed as arguments to the execvp() function.

strtok returns NULL when there are no more tokens to be found in the input string.
*/
	ptr = strtok(cmd, " "); 
	while(ptr != NULL){
		//printf("%s\n", ptr);
		argv[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}

	// check for "&"
	if(!strcmp("&", argv[i-1])){
	argv[i-1] = NULL;
	argv[i] = "&";
	}else{
	argv[i] = NULL;
	}
	//printf("%d\n", i);
}

void log_handle(int sig){
	//printf("[LOG] child proccess terminated.\n");
	FILE *pFile;
        pFile = fopen("log.txt", "a");
        if(pFile==NULL) perror("Error opening file.");
        else fprintf(pFile, "[LOG] child proccess terminated.\n");
        fclose(pFile);
}

/*
getenv() : 
-takes a single argument, which is a string representing the name of the environment variable
-returns a pointer to the value of the variable as a null-terminated string. 
-If the specified environment variable does not exist, getenv() returns a null pointer.
-Note that getenv() returns a pointer to a string that is owned by the operating system, 
 not the program. Therefore, you should not modify the value of the string returned by getenv()

The environment : 
 a collection of key-value pairs that provide information about the operating system environment, 
 such as the current user, the current working directory, and various system settings. 

Environment variables :
 are a way to store and access this information in a program.

execpv(args[0], args):
-execute the command specified in args[0], passing the argument list args.
-If execvp() succeeds, it will not return, and the process will be replaced by the new program.
-If execvp() fails, it returns -1 and sets the errno variable to indicate the type of error that occurred.

perror("some string")
-print an error message to the console if execvp() failed. It prints some string
followed by a colon and a message describing the error indicated by errno.

fgets(cmd, MAX_CMD_LEN, stdin) :
-This reads a line of user input from the console using fgets() and stores it in the cmd array.

*/