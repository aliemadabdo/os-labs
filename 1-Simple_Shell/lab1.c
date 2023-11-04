#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

extern char **environ;

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

The parameters of the setenv() function are:
-name: A string representing the name of the environment variable to be set.
-value: A string representing the value to be assigned to the environment variable.
-overwrite: An integer value that specifies whether to overwrite an existing value of the environment variable. If overwrite is non-zero, the existing value will be replaced by the new value. If overwrite is zero, the existing value will be retained.
-The setenv() function returns zero on success and -1 on failure.
*/

void handle_export(char *cmd) {
  char *varname = strtok(cmd, "=");
  char *varvalue = strtok(NULL, "=");
  setenv(varname, varvalue, 1);
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

perror("some string")
-print an error message to the console if execvp() failed. It prints some string
followed by a colon and a message describing the error indicated by errno.

*/

void handle_cmd(char *cmd, int bg_exec) {
  char *args[MAX_ARGS];
  char *arg;
  int i = 0;
  
  arg = strtok(cmd, " ");
  while (arg != NULL) {
    if (getenv(arg) != NULL) {
      args[i] = getenv(arg);
    } else {
      args[i] = arg;
    }
    i++;
    arg = strtok(NULL, " ");
  }
  args[i] = NULL;
  
  pid_t pid = fork();
  
  if (pid < 0) {
    perror("Error forking");
    exit(1);
  } else if (pid == 0) { // Child process
    execvp(args[0], args);
    perror("Error executing command");
    exit(1);
  } else { // Parent process
    if (bg_exec) {
      printf("Running process in background with PID: %d\n\n", pid);
    } else {
      int status;
      waitpid(pid, &status, 0);
      if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        printf("Command exited with abnormal return code: %d\n", WEXITSTATUS(status));
      }
    }
  }
}

// To debug...
void sigchld_handler(int signum) {
    int pid, status;

    // Wait for any child processes that have terminated
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            // Log the terminated process in a file
            FILE *log_file = fopen("process_log.txt", "a");
            if (log_file == NULL) {
                perror("Failed to open process log file");
                exit(EXIT_FAILURE);
            }
            fprintf(log_file, "Process %d terminated with status %d\n", pid, WEXITSTATUS(status));
            fclose(log_file);
        }
    }
}

int main(int argc, char *argv[]) {
  char cmd[MAX_CMD_LEN];
  
  // struct sigaction sa;
  //     sa.sa_handler = sigchld_handler;
  //     sigemptyset(&sa.sa_mask);
  //     sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  //     if (sigaction(SIGCHLD, &sa, NULL) == -1) {
  //         perror("Failed to set up SIGCHLD handler");
  //     }
      

// fgets(cmd, MAX_CMD_LEN, stdin) :
// -This reads a line of user input from the console using fgets() and stores it in the cmd array.
  while (1) {
    printf("rex>>> ");
    fgets(cmd, MAX_CMD_LEN, stdin);
    
    if (strcmp(cmd, "exit\n") == 0) {
      break;
    } else if (strncmp(cmd, "export", 6) == 0) {
      handle_export(cmd + 7);
    } else {
      char *newline = strchr(cmd, '\n');
      if (newline != NULL) {
        *newline = '\0';
      }
      
      int bg_exec = 0;  // background execution
      char *ampersand = strchr(cmd, '&'); //returns a pointer to the location of the character
                                          // in the string. Otherwise, it returns a null pointer.
      if (ampersand != NULL) {
        bg_exec = 1;
        *ampersand = '\0';
      }
      
      handle_cmd(cmd, bg_exec);
      
    }
    
  }
  
  return 0;
}
