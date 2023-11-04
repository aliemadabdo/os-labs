// important note..
// child anfd parent share the same memory at forking but
// each has his own space in memory
// not like threads, threads share the same space in memory

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int pid1 = fork(); // 2 processes
    int pid2 = fork(); // 4 processes
    // number of processes = 2^n 
    // where n is the number of forks
    if (pid1==0){
        if (pid2==0){
            printf("iam child 3 \n");
        }
        else{
            printf("iam child 1 \n");
        }
    }
    else if (pid2==0){
        printf("iam child 2 \n");
    }
    else{
        printf("iam the cook \n");
    }
    // wait returns the pid you waited for (useful for maltiprocesses)
    // wait also returns -1 at errors
    //"ECHILD": This is an error code that indicates 
    //that there are no child processes to wait for.

    while(wait(NULL) != -1 || errno != ECHILD ){
        printf("i tried, i waited, and i successed;) \n");
    }
    printf("\n\n");
    //notice the different printing configrations
    return 0;
}