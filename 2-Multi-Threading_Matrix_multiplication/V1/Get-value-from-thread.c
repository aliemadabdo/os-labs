//Content:
//Get values from threads using malloc() (double pointer in the pthread_join)

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

void* roll_dice(){           //this is a pointer to function as we call it by address later
    int* res = malloc(sizeof(int)); //Dynamic allocation 
    *res = (rand() % 7);
    printf("result within function: %d\n",*res);
    printf("function address: %p\n", res);

    return (void*) res;
  
              //-the following implementation gives a segmentaion error as we pass a local 
            //variable of a function  which is deallocated after return (function life time)
   
    // int step = (rand() % 7);
    //return (void*) &result; 
    //returning only value is noy enough so we return a reference, so it will give as
    //an intger pointer which can be casted to void pointer
}

int main(int argc, char *argv[]){
    int* result;
    pthread_t th[2]; //number of needed threads
    int thLen = sizeof(th) / sizeof(th[0]);

    srand(time(NULL));

    for (int i = 0; i<thLen; i++){ //multi-threading creation
        if (pthread_create(&th[i], NULL, &roll_dice, NULL) ) return 1;
    }

    for (int i = 0; i<thLen; i++){ //multi-threading wait
        if (pthread_join(th[i],(void**) &result) ) return 1; 
            printf("result within the loop : %d\n", *result);

        // pthread_join takes a double pointer as a seconed parameter to return the result 
        // from the fuction it waits for
        // respectively, we need to retern a void pointer from the called function
    }
    //notice that outputs isn't ordered

    printf("main address: %p\n", result);
    printf("result in main: %d\n", *result);
    
    free(result);  //to deallocate after using malloc()
    return 0;
}