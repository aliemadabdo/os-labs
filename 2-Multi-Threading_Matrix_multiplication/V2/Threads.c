#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>


void* randfn(){
    printf("hLOOOO from thread\n");
    sleep(2);
    printf("after sleep\n");
}

int main(int argc, char *argv[]){
    printf("hi\n");
    
    pthread_t t1,t2; // a struct to store information about the thread

    pthread_create(&t1, NULL, &randfn, NULL); //returns 0 if no error
    if (pthread_create(&t2, NULL, &randfn, NULL) ) return 1; //safer
    //1-a refrence to thread varaiable
    //2- the result but we have no return 
    //3-a refrence to the subroutine
    //4-sub rountine parameters

    pthread_join(t1,NULL);// the wait_like process /\ returns 0 if no error
    if (pthread_join(t2,NULL) ) return 2; //safer 
    //1-Thread varaiable
    //2-The result but we have no return 



    printf("\n\nDa4ml\n\n");        
    return 0;
}