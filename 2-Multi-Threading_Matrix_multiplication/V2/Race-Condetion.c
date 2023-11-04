//Content:
//-Race condetion -->> Happens only with multi-core processors**
//-Multi threading (mutex)
//-Threading in a loop


#include <stdio.h>
#include <pthread.h>

int mails = 0;
int lock = 0;
pthread_mutex_t mutex;
void* randfn(){ 

    //-Expected output -->> 2,000,000
    //-Real output example -->> 1179872
    //-This is what we call race condition 
    //-This happens due to the two threads are running in parallel and sharing the same 
    // memory space, so when the CPU stops one of them for any reason (i.e. executing 
    // another line) and return to execute this thread, then this thread overwrite the 
    // first in the race thread with older value 

    //                              //the problem
    // for(int i = 0; i<1000000; i++){

    //     mails++;
    //     //read..increase..write
    // }

    //                              //lock solution
    // //not very accurate
    // for(int i = 0; i<1000000; i++){
    //     // if (lock == 1){
    //     //     //wait until it 0
    //     // }
    //     while(lock==1) ; // ; -->> no operation
    //     lock =1;    //locked
    //     mails++;    //read..increase..write
    //     lock = 0;   //unlocked
    // }

                                    //Mutex Solution 
    //100% accurate
    //a bit less faster
    for(int i = 0; i<10000000; i++){
        pthread_mutex_lock(&mutex);
        mails++; //read..increase..write
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]){

    pthread_t th[4]; //number of needed threads
    int thLen = sizeof(th) / sizeof(th[0]);

    pthread_mutex_init(&mutex, NULL);

    //                       // sequential threading problem
    // for (int i = 0; i<thLen; i++){ //multi-threading
    //     if (pthread_create(&th[i], NULL, &randfn, NULL) ) return 1;
    //     printf("thread %d start\n", i);
    //     if (pthread_join(th[i],NULL) ) return 2;
    //     printf("thread %d end\n", i);
    // }

                         // sequential threading problem solution:
        // separate the creation loop from the join loop

    for (int i = 0; i<thLen; i++){ //multi-threading
        if (pthread_create(&th[i], NULL, &randfn, NULL) ) return 1;
        printf("thread %d start\n", i);
    }
    for (int i = 0; i<thLen; i++){ //multi-threading
        if (pthread_join(th[i],NULL) ) return 1;
        printf("thread %d end\n", i);
    }

    pthread_mutex_destroy(&mutex);

    printf("number of mails : %d\n", mails);  
    
    return 0;
}