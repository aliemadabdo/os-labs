// objective..
// 1-get familiar with wait system call 
// 2-handel the processes order between parent and child

// notes:
//
// 1-wait fn only wait for one child to finish so if we has 
// matiable fork() fns and we used only one wait(NULL) fn
// at the end there will be a problem when the second child
// arrives (orphan process)
//
//2-
//

#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main (int argc,char* argv[]){
    int pid = fork();
    int n;

    if (pid==0){
        n = 1;
    } else {  
        n=6;
    }

    if (pid != 0 ){
        wait();
    }
    // wait(); --> waiting here will stop the main function
    // int i;
    for (int i=n; i<n+5; i++){
        printf("%d",i);
        fflush(stdout); //it causes any buffered output to be immediately written 
                         //to the standard output device (usually the terminal).
    }
    if (pid != 0){
        printf("\n");

    }

    return 0;
}