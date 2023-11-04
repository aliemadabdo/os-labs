// content:
// create 10 threads each taking a uniqu prime from the primes array and print it on the screen 
//
//
// Motevation:
// At certain point you might want to do some thing based on the threads oreder of creation
// how can we do that with pthread API

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

//                        Way 1 (my way) -->> 2 warnings [Done] 
// but why? i think that it is because of i send the value not the index 
//  so no interference happened between indexes

void* print_prime(void* prime){
    // int index = *((int*) arg); // cast from void pointer to int pointer then derefrence it
    printf(" %d ", *(int*) prime );
}

int main (int argc, char* argv[]){

    pthread_t th[10];
    int thLen = sizeof(th) / sizeof(th[0]);

    for (int i = 0; i<thLen; i++){
        if( pthread_create(&th[i], NULL, &print_prime,(void**) &primes[i]) ){
            perror("faild to create thread");
        }
    }
    for (int i = 0; i<thLen; i++){
        if( pthread_join(th[i], NULL) ){         //NULL or the return value from a routine
            perror("faild to join thread");
        } 
    }

    return 0;
}

// //                            way 2 from video (sometimes give me random value)
// void* print_prime(void* arg){
//     // sleep(1);
//     int index = *((int*) arg); // cast from void pointer to int pointer then derefrence it
//     printf(" %d ", primes[index] );
//     // free(arg);
// }

// int main (int argc, char* argv[]){

//     pthread_t th[10];
//     int thLen = sizeof(th) / sizeof(th[0]);

//     int* a = malloc(sizeof(int));

//     for (int i = 0; i<thLen; i++){
//         *a = i;
//         if( pthread_create(&th[i], NULL, &print_prime, a) ){
//             perror("faild to create thread");
//         }
//     }
//     for (int i = 0; i<thLen; i++){
//         if( pthread_join(th[i], NULL) ){         //NULL or the return value from a routine
//             perror("faild to join thread");
//         } 
//     }

//     // free(*a);
//     return 0;
// }