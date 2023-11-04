#include <stdio.h>
#include <unistd.h>

int main() {

/* getpid()
a system call in Unix-like operating systems.
When a user program calls getpid(), it triggers an interrupt to
the operating system, which then handles the request and 
returns the PID of the calling process to the user program.
*/
    __pid_t pid;    // data type for the pid
    pid = getpid(); // system call to get the current pid
    printf("the current pid is : %u", getpid());
    printf("\n \n \n");

     __pid_t ppid; // data type for the ppid
    ppid = getppid(); // system call to get the current ppid
    printf("the current ppid is : %u", getppid());
    printf("\n \n \n");
    // Note:
    // the ppid is constant at the terminal as it is the parent process for these runs 
    // but when you run from vscode directly the ppid changes as every time vscode 
    // creates a new parent process. 

    // every thing is running a shell program is a child from it

    
    __pid_t forkPid;
    printf("one \n");    
    forkPid = fork(); // chile created 
    
    if (forkPid == -1){
        printf("unsuccessful forking");
        return 1;
    }
    printf("two \n");
    return 0; //return back to the parent process (shell)
              // but the new child returns to the parent of this program 
              
}
