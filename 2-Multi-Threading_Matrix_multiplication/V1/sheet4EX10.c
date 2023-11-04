//the problem is to overwrite myglobal in parallel to get 20 not 40
// solution: we can make another local variable in main like j in thread function
// and apply the lock solution using these variables

#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 

int myglobal; 
/*lobal variables in C are automatically initialized to a default value depending on
 their data type. The default initial value for uninitialized global variables is:

-For numeric types (int, float, double, etc.), the initial value is 0.
-For pointers, the initial value is a null pointer (i.e., a pointer that points to nothing).
-For char arrays and strings, the initial value is an empty string
 (i.e., a string with a single null character '\0').
*/
pthread_mutex_t mutex;

void *thread_function(void *arg) {
     int i,j; 

    // printf("fn Thread ID: %lu\n", pthread_self());
    // printf("fn Process ID: %d\n", getpid());

    // pthread_mutex_lock(&mutex);
    //  sleep(1);
     for ( i=0; i<20; i++ ) { 

        j=myglobal; 
        j=j+1; 
        printf(" F%d",i); 
        printf(" %d  \n",myglobal);  

        fflush(stdout); //comment fflush(stdout) and notice how the outputs are printed 
                        //together not one by one
        sleep(1); 
        // if ( j ==  myglobal+1 ){
            myglobal=j; 
        // }
        } 

    // pthread_mutex_unlock(&mutex);

        return NULL; 
    } 
int main(void) { 
    pthread_t mythread; 
    int i; 
    pthread_mutex_init(&mutex, NULL);
    
    if ( pthread_create( &mythread, NULL, thread_function, NULL) ) { 
        printf("error creating thread."); 
        abort(); 
    } 
    // printf("Thread ID: %lu\n", pthread_self()); //different thread ids
    // printf("Process ID: %d\n", getpid());        // same pid

    // printf("  hi from main thread  ");
    for ( i=0; i<20; i++) { 
        myglobal=myglobal+1; 
        printf(" M%d",i); 
        printf(" %d ",myglobal); 

        fflush(stdout); //comment fflush(stdout) and notice how the outputs are printed 
                        //together not one by one
        sleep(1); 
    }

 if ( pthread_join ( mythread, NULL ) ){
     printf("error joining thread."); 
     abort(); 
    } 
    pthread_mutex_destroy(&mutex);
    
    printf("\nmyglobal equals %d\n",myglobal); 
    exit(0);
 }
