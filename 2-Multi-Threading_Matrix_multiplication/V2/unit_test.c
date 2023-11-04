/*
-------OBJECTIVES------
we have to implement 3 functions 
1- A thread per matrix
2- A thread per row of the output C
3- A thread per element for output C  

                                        Inputs 

a log file as input default names a & b 
ex:
  row=3 col=5
  1 2 3 4 5
  6 7 8 9 10
  11 12 13 14 15

**check if its a valid matrix maltiplication
**Assume I will not test with matrix size larger than 20 x 20.

                                        OutputS

A log file default name is z 
output example:

 Method: A thread per row
 row=2 col=2
 1 2
 3 4

also print to consol:
-The number of threads created
-The time taken

**check that 3 ways output are equal
*/


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

int rows_row = 0; //threads counter
int elements_row = 0; //threads counter
int elements_col = 0; //threads counter
pthread_mutex_t mutex;
FILE *out_file_1;
FILE *out_file_2;
FILE *out_file_3;


typedef struct {
    int **arrA;
    int **arrB;
    int *a_rows, *a_cols, *b_rows, *b_cols;
} matrix_thread_args;

typedef struct {
    int *arrA;
    int **arrB;
    int *a_rows, *a_cols, *b_rows, *b_cols;

} row_thread_args;

typedef struct {
    int *arrA;
    int *arrB;
    int *a_rows, *a_cols, *b_rows, *b_cols;
} element_thread_args; 
//s for struct


/*for each function we need 
  the execution time [done]
  the calculations
  write output to a log file or return it*/
void *thPerMat(void *arg);
void *thPerRow(void *arg);
void *thPerElement(void *arg);

void get_matrix(FILE *in_file, int rows, int cols, int fileContent[20][20]);
void print_matrix(int matrix[20][20], int rows, int cols);
void write_to_file(int part,int *output_matrex, int *rows, int *cols);
void print_ptr_matrix(int *matrix, int *rows, int *cols);



/*for main function we need
  read inputs row-a col-a row-b col-b [done]
  check if multplication is valid [done]
  get the output dimentions [done]
  get thr number of thread for each approach [done]
  2 loops to creat threads and call there functions in addition to the thPerMat function [done]
  check if outputs (elements and dimentions) are the same 
  calc the total execution time [done]
  loop on the elements to verify these dimentions [done]*/


int main(int argc, char* argv[]){

    int a_row, a_col, b_row, b_col, a_elements, b_elements; 
    // char mat1 = 'a', mat2 = 'b', outMat = 'c';
    int matrix_a[20][20],matrix_b[20][20]; //or you can use the first struct

    pthread_mutex_init(&mutex, NULL);


    struct timeval start, stop;  //note that:
    gettimeofday(&start, NULL);  // this function with a non-null "TZ" parameter is now considered obsolete.
                                 // Instead, the function "localtime" or similar functions should be used instead.
                                 // Additionally, the function itself is semi-obsolete, and most callers should 
                                 // use "time" or "clock_gettime" instead.

    // take matrices names from the user
    // scanf("%c %c %c", &mat1, &mat2, &outMat);

    FILE *in_file_a, *in_file_b; //struct
    in_file_a = fopen("a.txt",  "r"); /*change to handel new files "a+" */
    in_file_b = fopen("b.txt",  "r"); //fopen returns a pointer to file structure
    if (in_file_a == NULL || in_file_b == NULL){             // "r" indecates reading state
        printf("Error: Could not open the file\n");
        exit(1);
    }

    // read input dimentions
    fscanf(in_file_a, "row=%d col=%d",&a_row,&a_col);
    fscanf(in_file_b, "row=%d col=%d",&b_row,&b_col);

    //Don't worry pointer is handling the indexing of the file 
    printf("number of rows    for matrix a   : %d\n", a_row);
    printf("number of columns for matrix a   : %d\n", a_col);
    printf("number of rows    for matrix b   : %d\n", b_row);
    printf("number of columns for matrix b   : %d\n", b_col);

    //check if valid multiplication
    if (a_col != b_row){
        printf("Error: Not a valid dimentions for matrix multiplication");
        exit(1);
    }

    get_matrix(in_file_a, a_row, a_col, matrix_a);
    print_matrix(matrix_a, a_row, a_col);

    get_matrix(in_file_b, b_row, b_col, matrix_b);
    print_matrix(matrix_b, b_row, b_col);

    // int size = sizeof(matrix_a) / sizeof(int);
    // printf("\nmatrix size = %d \n", size);

   // close the input file
    fclose(in_file_a);
    fclose(in_file_b);

    //                                      Threading
    pthread_t th[ (a_row * b_col) + a_row + 1];

    printf("\nNumber of threads per Matrix = 1\n");
    printf("Number of threads per row = %d\n", a_row);
    printf("Number of threads per element = %d\n", a_row * b_col);

        // printf("gooooooooooooooo\nrows = %d , cols = %d\n", a_row,a_col);

    //part1
    matrix_thread_args  matrix_arg;
    matrix_arg.a_rows = (int *) malloc(sizeof(int));
    matrix_arg.a_cols = (int *) malloc(sizeof(int));
    matrix_arg.b_rows = (int *) malloc(sizeof(int));
    matrix_arg.b_cols = (int *) malloc(sizeof(int));
    matrix_arg.arrA = (int **) malloc(sizeof(int) * a_row * a_col * 2);
    matrix_arg.arrB = (int **) malloc(sizeof(int) * b_row * b_col * 2);
    // The (int *) cast is used to cast the returned pointer from void * to int *,
    // since malloc returns a pointer of type void *. 
    // The result of this line is that: matrix_arg.a_rows now points to 
    // a newly allocated block of memory that can hold a single integer.

    *matrix_arg.a_rows = a_row; //data of a_rows = a_row
    *matrix_arg.a_cols = a_col;
    *matrix_arg.b_rows = b_row;
    *matrix_arg.b_cols = b_col;

    int **temp_a = matrix_arg.arrA;
    for (int i = 0; i<a_row; i++){
        for(int j=0; j<a_col; j++){
            *matrix_arg.arrA = matrix_a[i][j];
            printf(" %d ", *matrix_arg.arrA);
            matrix_arg.arrA++;
        }
        printf("\n");
    }
    printf("\n");
    matrix_arg.arrA = temp_a;

    int **temp_b = matrix_arg.arrB;
    for (int i = 0; i<b_row; i++){
        for(int j=0; j<b_col; j++){
            *matrix_arg.arrB = matrix_b[i][j];
            printf(" %d ", *matrix_arg.arrB);
            matrix_arg.arrB++;
        }
        printf("\n");
    }
    printf("\n");
    matrix_arg.arrB = temp_b;
    
    // for (int i = 0; i<a_row; i++){
    //     for(int j=0; j<a_col; j++){
    //         printf(" %d ", *matrix_arg.arrA);
    //         matrix_arg.arrA++;
    //     }
    //     printf("\n");
    // }

    print_ptr_matrix(matrix_arg.arrA, matrix_arg.a_rows, matrix_arg.a_cols);
    print_ptr_matrix(matrix_arg.arrB, matrix_arg.b_rows, matrix_arg.b_cols);
    // printf("matrix_a element 1: %d\n", matrix_arg.arrA[2][0]);
    // printf("matrix_b element 1: %d\n", matrix_arg.arrB[1][0]);

    // printf("a_cols pointer: %d\n", *(matrix_arg.a_rows));
    printf("a_rows data: %d\n", *matrix_arg.a_rows);
    printf("a_cols data: %d\n", *matrix_arg.a_cols);
    printf("b_rows data: %d\n", *matrix_arg.b_rows);
    printf("b_cols data: %d\n", *matrix_arg.b_cols);

    printf("a_rows pointer: %p\n", matrix_arg.a_rows);
    printf("a_cols pointer: %p\n", matrix_arg.a_rows);
    printf("b_rows pointer: %p\n", (void *)matrix_arg.a_rows);
    printf("b_cols pointer: %p\n", (void *)matrix_arg.a_rows);
    // the four will have the same memory locations as they whthin the same struct

    if (pthread_create(&th[0], NULL, &thPerMat,(void *) &matrix_arg)){ 
        printf("Error: Could not create a thread");
        exit(1);
    }
    if(pthread_join(th[0], NULL)){
        printf("Error: Could not join a thread");
        exit(1);
    }

    //part 2
    for(int i = 0;i<a_row;i++){
        // struct row_thread_args row_arg = {matrix_a[i],matrix_b, a_row, a_col, b_row, b_col};
                // try to initialize it befor for loop and set only the rows and cols then 
                // the matrix itself inside the loop

        if (pthread_create(&th[i+1], NULL, &thPerRow, &matrix_arg)){ 
            printf("Error: Could not create a thread");
            exit(1);
        }
        if(pthread_join(th[i+1], NULL)){
            printf("Error: Could not join a thread");
            exit(1);
        }
    }

    //part 3
    for(int i = 0;i<a_row;i++){
        for(int j = 0;j<b_col;j++){
            // struct element_thread_args element_arg = {matrix_a[i],matrix_b[j], a_row, a_col, b_row, b_col};
                // try to initialize it befor for loop and set only the rows and cols then 
                // the matrix itself inside the loop

            if (pthread_create(&th[ ( i * b_col + j ) + a_row + 1 ], NULL, &thPerElement, &matrix_arg)){ 
                printf("Error: Could not create a thread");
                exit(1);
            }
            if(pthread_join(th[( i * b_col + j ) + a_row + 1 ], NULL)){
                printf("Error: Could not join a thread");
                exit(1);
            }
        }
    }

    fclose(out_file_1);
    fclose(out_file_2);
    fclose(out_file_3);

    pthread_mutex_destroy(&mutex);

    printf("\nNumber of threads per Matrix = 1\n");
    printf("Number of threads per row = %d\n", a_row);
    printf("Number of threads per element = %d\n", a_row * b_col);

    // printf("size of int %lu \n", sizeof(int));
    gettimeofday(&stop, NULL);
    printf("main execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("main execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );
                                        // ul --> unsigned long
    return 0;
}



void *thPerMat(void *arg){

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    printf("\n\nhii iam thread part 1\n");

    matrix_thread_args *my_args = (matrix_thread_args *) arg; 
    //create a pointer to struct and type cast the struct that we point to
    // int *hamo1 = *(my_args->arrA) ; // i think we can not cast pointer to integer but 
    // int *hamo2 = *(my_args->arrB) ;       // but the reverse is valid
    // int hamo3 = hamo1 * hamo2;

    // my_args->arrA = (int **) malloc(sizeof(int) * 3 * 4 );
    // my_args->arrB = (int **) malloc(sizeof(int) * 4 * 3 );

    // printf("test rows: %d\n", *(my_args->a_rows));
    // printf("test cols: %d\n", *(my_args->b_cols));
    // printf("test array a: %d\n", hamo1);
    // printf("test array b: %d\n", hamo2);
    // printf("hamo3 = %d\n", hamo3);
        
    // for (int i = 0; i<a_row; i++){
    //     for(int j=0; j<a_col; j++){
    //         printf(" %d ", *matrix_arg.arrA);
    //         matrix_arg.arrA++;
    //     }
    //     printf("\n");
    // }

    // print_ptr_matrix(my_args->arrA, my_args->a_rows, my_args->a_cols);
    // print_ptr_matrix(my_args->arrB, my_args->b_rows, my_args->b_cols); (edit function to +2)

    int matrix_c[ *(my_args->a_rows) ][ *(my_args->b_cols) ];
    // printf("rows = %d , cols = %d\n", my_args->a_rows,my_args->a_cols);
    // print_matrix(my_args->arrA,my_args->a_rows, my_args->b_cols);
    // int size = sizeof(my_args->arrA) / sizeof(int);
    // printf("\nmatrix size = %d \n", size);



   
    // printf("this should be 2 : %d\n", my_args->arrA[0][1]);
    int hamo1, hamo2, hamo3;
   
    // calculating the result
    // int *ptr = malloc(sizeof(int));
    // ptr = matrix_c;
    int *tempA = my_args->arrA;
    int *tempB = my_args->arrB;
    // printf("temp = %d\n", temp);
    int sum = 0; 
    for(int r = 0; r < *my_args->a_rows; r++){
        for(int c = 0; c < *my_args->b_cols; c++){    
            for(int i=0; i < *my_args->a_cols; i++){
                    my_args->arrA = tempA + (r * *my_args->a_cols + i)*2 ;
                    //calculate the next location for matrix a
                    my_args->arrB = tempB + (i * (*my_args->b_cols) + c)*2 ;
                    //calculate the next location for matrix b
                    hamo1 = *my_args->arrA ;
                    hamo2 = *my_args->arrB ;
                    hamo3 = hamo1 * hamo2;
                    sum = sum + hamo3;
                    // printf("%d.%d.%d = %d --> %d : (%d) %d : (%d)\n",r,c,i,hamo3,hamo1,my_args->arrA,hamo2,my_args->arrB);
            }
            // printf("\n");
            // printf(" sum = %d \n", sum);
            matrix_c[r][c] = sum;
            // printf("r : %d , c : %d \n", r,c);
            // printf("adrress matrix_c[%d][%d] = %d \n", r,c,&matrix_c[r][c]);
            sum = 0;
        }
        // printf("\n");
    }  
    // printf("\nmatrix c :\n");
    print_ptr_matrix(matrix_c, my_args->a_rows , my_args->b_cols);

    // write_to_file(1, matrix_c, my_args->a_rows, my_args->b_cols);

    out_file_1 = fopen("c_per_matrix.txt", "w+");
    fprintf(out_file_1, "Method: A thread per matrix\n");
    if (out_file_1 == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    for (int i = 0; i < *my_args->a_rows; i++){
        for(int j = 0; j < *my_args->b_cols; j++){
             fprintf(out_file_1, "%d ", matrix_c[i][j]);
        }
        fprintf(out_file_1, " \n");
    }

    my_args->arrA = tempA;
    my_args->arrB = tempB;

    gettimeofday(&stop, NULL);
    // printf("execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("thread per matrix execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );
}


void *thPerRow(void *arg){

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    printf("\n\nhii iam thread part 2\n");

    matrix_thread_args *my_args = (matrix_thread_args *) arg; 

    // printf("test rows: %d\n", *(my_args->a_rows));
    // printf("test cols: %d\n", *(my_args->b_cols));

    int matrix_c[1][ *(my_args->b_cols) ];

    pthread_mutex_lock(&mutex);
    // printf("this should be 2 : %d\n", my_args->arrA[0][1]);
    int hamo1, hamo2, hamo3;
   
    int *tempA = my_args->arrA;
    int *tempB = my_args->arrB;
    // printf("temp = %d\n", temp);
    int sum = 0; 
    for(int c = 0; c < *my_args->b_cols; c++){    
        for(int i=0; i < *my_args->a_cols; i++){
                my_args->arrA = tempA +(rows_row * *my_args->a_cols + i)*2 ;
                //calculate the next location for matrix a
                my_args->arrB = tempB + (i * (*my_args->b_cols) + c)*2 ;
                //calculate the next location for matrix b
                hamo1 = *my_args->arrA ;
                hamo2 = *my_args->arrB ;
                hamo3 = hamo1 * hamo2;
                sum = sum + hamo3;
                printf("%d.%d = %d --> %d : (%d) %d : (%d)\n",c,i,hamo3,hamo1,my_args->arrA,hamo2,my_args->arrB);
        }
        // printf("\n");
        // printf(" sum = %d \n", sum);
        matrix_c[0][c] = sum;
        // printf(" c : %d \n",c);
        // printf("adrress matrix_c[%d] = %d \n", c,&matrix_c[c]);
        sum = 0;
    }
    // printf("\n");
    // printf("\nmatrix c :\n");
    int one = 1;
    // print_ptr_matrix(matrix_c, &one , my_args->b_cols);

    // write_to_file(2, matrix_c, &one , my_args->b_cols);
    if (out_file_2 == NULL){
        out_file_2 = fopen("c_per_row.txt", "w+");
        fprintf(out_file_2, "Method: A thread per row\n");
    } 

    if (out_file_2 == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    for (int i = 0; i < *my_args->b_cols; i++){
        fprintf(out_file_2, "%d ", matrix_c[0][i]);
    }
    fprintf(out_file_2, " \n");

    gettimeofday(&stop, NULL);
    // printf("execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("thread per row execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );

    rows_row++; //for the next thread location

    my_args->arrA = tempA;
    my_args->arrB = tempB;
    pthread_mutex_unlock(&mutex);

}


void *thPerElement(void *arg){

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    printf("\n\nhii iam thread part 3\n");

    matrix_thread_args *my_args = (matrix_thread_args *) arg; 

    // printf("test rows: %d\n", *(my_args->a_rows));
    // printf("test cols: %d\n", *(my_args->b_cols));

    int matrix_c[1][1];

    int r = 0; 
    int c = 0; //threads counters
    pthread_mutex_lock(&mutex);
    // printf("this should be 2 : %d\n", my_args->arrA[0][1]);
    int hamo1, hamo2, hamo3;
   
    int *tempA = my_args->arrA;
    int *tempB = my_args->arrB;
    // printf("temp = %d\n", temp);
    int sum = 0; 
    for(int i=0; i < *my_args->a_cols; i++){
            my_args->arrA = tempA +(elements_row * *my_args->a_cols + i)*2 ;
            //calculate the next location for matrix a
            my_args->arrB = tempB + (i * (*my_args->b_cols) + elements_col)*2 ;
            //calculate the next location for matrix b
            hamo1 = *my_args->arrA ;
            hamo2 = *my_args->arrB ;
            hamo3 = hamo1 * hamo2;
            sum = sum + hamo3;
            printf("%d = %d --> %d : (%d) %d : (%d)\n",i,hamo3,hamo1,my_args->arrA,hamo2,my_args->arrB);
    }
    // elements_col++;
    // printf("\n");
    printf("part 3 sum = %d \n", sum);
    matrix_c[0][0] = sum;
    // printf(" c : %d \n",c);
    // printf("adrress matrix_c[%d] = %d \n", c,&matrix_c[c]);
    // sum = 0;
    // printf("\n");
    // printf("\nmatrix c :\n");
    int one = 1;
    // print_ptr_matrix(matrix_c[0][0], &one , &one);

    // write_to_file(3, matrix_c, &one, &one);

    gettimeofday(&stop, NULL);
    // printf("thread per element execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("thread per element execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );
    if (elements_col % *my_args->b_cols == 0 && elements_col != 0){
        elements_row++; 
    }
    else{
        elements_row = 0; //for the next thread location
    }

    if (elements_col % *my_args->b_cols == 0 && elements_col != 0){
        elements_col = 0; 
    }
    else{
        elements_col++; //for the next thread location
    }


    if (out_file_3 == NULL){
        out_file_3 = fopen("c_per_element.txt", "w+");
        fprintf(out_file_3, "Method: A thread per element\n");
    }          
    

    if (out_file_3 == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    fprintf(out_file_3, "%d ", matrix_c[0][0]);

    if (elements_col % *my_args->b_cols == 0){
        fprintf(out_file_3, "\n");
    }

    my_args->arrA = tempA;
    my_args->arrB = tempB;
    pthread_mutex_unlock(&mutex);

}


void get_matrix(FILE *in_file, int rows, int cols, int fileContent[][20]){
    /*
    this fuction get the matrix a and b from the file and check if 
    dimentions are valid

    Takes the file pointer , number of rows, number of columns and the matrix we want to set
    returns nothing
    */
    printf("\nnumber of rows passed to the fn    : %d\n", rows);
    printf("number of columns passed to the fn : %d\n", cols);
    // read matrix elements
    int flag = 0, i, j;
    for (i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            // printf("%d ",( fscanf(in_file, "%d", &fileContent[i][j]) ));
            if( ( fscanf(in_file, "%d", &fileContent[i][j]) ) - 1){
                flag = 1;
                break;
            }
            // printf("%d ", fileContent[i][j]);
        }
        if(flag == 1){ break;}
    }

    // check dimention matching
    printf("i = %d j = %d\n",i,j);

    if( (i != rows) || (j != cols)){
        printf("Error: File dimentions is not correct\n");
    }
    else {
        printf("Valid input file dimentions\n");
    }
    
        /*Reading files:
    fgetc()– This function is used to read a single character from the file.
    fgets()– This function is used to read strings from files.
    fscanf()– This function is used to read formatted input from a file.
    fread()– This function is used to read the block of raw bytes from files. 
    This is used to read binary files.
    */
}

void print_matrix(int matrix[][20], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_ptr_matrix(int *matrix, int *rows, int *cols){
    // int **temp = matrix;
    for (int i = 0; i<*rows; i++){
        for(int j=0; j<*cols; j++){
            printf(" %d ", *matrix);
            matrix = matrix + 1;
        }
        printf("\n");
    }
    printf("\n");
    // matrix = temp;
}


void write_to_file(int part,int *output_matrex, int *rows, int *cols){ 
                                        // Output part
    FILE *out_file;

    switch (part){
        case 1:
            out_file = fopen("c_per_matrix.txt", "w+");
            fprintf(out_file, "Method: A thread per matrix\n");
            break;
        case 2:
            out_file = fopen("c_per_row.txt", "w+");
            fprintf(out_file, "Method: A thread per row\n");
            break;
        case 3:             
            out_file = fopen("c_per_element.txt", "w+");
            fprintf(out_file, "Method: A thread per element\n");
            break;
    }

    if (out_file == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    printf("test rows: %d\n", *rows);
    printf("test cols: %d\n", *cols);

    for (int i = 0; i < *rows; i++){
        for(int j = 0; j < *cols; j++){
             fprintf(out_file, "%d ", *output_matrex);
             output_matrex++;
        }
        fprintf(out_file, " \n");
    }
    fclose(out_file);
}


/*
notes:

1- the statement int *ptr = malloc(sizeof(int));, 
-we are using malloc() to allocate a block of memory that is the size of one integer, 
-and then assigning the pointer to the first byte of that memory block to the variable ptr. 
-This creates a dynamic memory allocation for one integer value.

*/