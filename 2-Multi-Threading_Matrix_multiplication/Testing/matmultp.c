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

struct matrix_thread_args{
    int arrA[20][20];
    int arrB[20][20];
    int a_rows, a_cols, b_rows, b_cols;
};
struct row_thread_args{
    int arrA[20];
    int arrB[20][20];
    int a_rows, a_cols, b_rows, b_cols;

};
struct element_thread_args{
    int arrA[20];
    int arrB[20];
    int a_rows, a_cols, b_rows, b_cols;
}; //s for struct


/*for each function we need 
  the execution time [done]
  the calculations
  write output to a log file or return it*/
void *thPerMat(void *arg);
void *thPerRow(void *arg);
void *thPerElement(void *arg);

void get_matrix(FILE *in_file, int rows, int cols, int fileContent[20][20]);
void print_matrix(int matrix[20][20], int rows, int cols);
void write_to_file(int part, int matrix[20][20], int rows, int cols);


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
    printf("START\n");
    int a_row, a_col, b_row, b_col, a_elements, b_elements; 
    char mat1 = 'a', mat2 = 'b', outMat = 'c';
    int matrix_a[20][20],matrix_b[20][20]; //or you can use the first struct

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
    // print_matrix(matrix_a, a_row, a_col);

    get_matrix(in_file_b, b_row, b_col, matrix_b);
    // print_matrix(matrix_b, b_row, b_col);

    // int size = sizeof(matrix_a) / sizeof(int);
    // printf("\nmatrix size = %d \n", size);

   // close the input file
    fclose(in_file_a);
    fclose(in_file_b);

    //                                      Threading
    pthread_t th[ (a_row * b_col) + a_row + 1];

    printf("\nNumber of threads per Matrix = 1\n");
    printf("Number of threads per Matrix = %d\n", a_row);
    printf("Number of threads per Matrix = %d\n", a_row * b_col);

        // printf("gooooooooooooooo\nrows = %d , cols = %d\n", a_row,a_col);

    //part1
    struct matrix_thread_args  matrix_arg  = { **matrix_a, **matrix_b, a_row, a_col, b_row, b_col};

    if (pthread_create(&th[0], NULL, &thPerMat, &matrix_arg)){ 
        printf("Error: Could not create a thread");
        exit(1);
    }
    if(pthread_join(th[0], NULL)){
        printf("Error: Could not join a thread");
        exit(1);
    }

    // //part 2
    // for(int i = 0;i<a_row;i++){
    //     struct row_thread_args row_arg = {matrix_a[i],matrix_b, a_row, a_col, b_row, b_col};
                //try to initialize it befor for loop and set only the rows and cols then 
                //the matrix itself inside the loop

    //     if (pthread_create(&th[i+1], NULL, &thPerRow, &row_arg)){ 
    //         printf("Error: Could not create a thread");
    //         exit(1);
    //     }
    //     if(pthread_join(th[i+1], NULL)){
    //         printf("Error: Could not join a thread");
    //         exit(1);
    //     }
    // }

    // //part 3
    // for(int i = 0;i<a_row;i++){
    //     for(int j = 0;j<b_col;j++)
    //         struct element_thread_args element_arg = {matrix_a[i],matrix_b[j], a_row, a_col, b_row, b_col};
                //try to initialize it befor for loop and set only the rows and cols then 
                //the matrix itself inside the loop

    //         if (pthread_create(&th[ ( i * b_col + j ) + a_row + 1 ], NULL, &thPerElement, &element_arg)){ 
    //             printf("Error: Could not create a thread");
    //             exit(1);
    //         }
    //         if(pthread_join(th[( i * b_col + j ) + a_row + 1 ], NULL)){
    //             printf("Error: Could not join a thread");
    //             exit(1);
    //         }
    // }

    // printf("size of int %lu \n", sizeof(int));
    gettimeofday(&stop, NULL);
    printf("execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );
                                        // ul --> unsigned long
    return 0;
}










void *thPerMat(void *arg){

    
    struct timeval start, stop;
    gettimeofday(&start, NULL);

    printf("\n\nhii iam thread part 1\n");

    struct matrix_thread_args *my_args = (struct matrix_thread_args *) arg; 
    //create a pointer to struct and type cast the struct that we point to
    int matrix_c[ my_args->a_rows ][ my_args->b_cols ];
    // printf("rows = %d , cols = %d\n", my_args->a_rows,my_args->a_cols);
    // print_matrix(my_args->arrA,my_args->a_rows, my_args->b_cols);
    // int size = sizeof(my_args->arrA) / sizeof(int);
    // printf("\nmatrix size = %d \n", size);

    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 5; j++) {
            printf("%d ", my_args->arrA[i][j]);
        }
        printf("\n");
    }

    // printf("this should be 2 : %d\n", my_args->arrA[0][1]);

    int *ptr;
    *ptr = matrix_c[0][0];
    for(int i = 0; i < (my_args->a_rows * my_args->b_cols); i++){
        for(int i=0; i<my_args->a_rows; i++){
            for(int j=0; j<my_args->b_cols; j++){
                *ptr = my_args->arrA[i][j] * my_args->arrB[j][i];
                ptr++;
            }
        }
    }  

    write_to_file(1,matrix_c,my_args->a_rows,my_args->b_cols);

    gettimeofday(&stop, NULL);
    printf("execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );
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
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void write_to_file(int part, int output_matrex[][20], int rows, int cols){ 
                                        // Output part
    FILE *out_file;

    switch (part){
        case 1:
            out_file = fopen("c_per_matrix.txt", "w+");
            fprintf(out_file, "Method: A thread per matrix");
            break;
        case 2:
            out_file = fopen("c_per_row.txt", "w+");
            fprintf(out_file, "Method: A thread per matrix");
            break;
        case 3:             
            out_file = fopen("c_per_element.txt", "w+");
            fprintf(out_file, "Method: A thread per matrix");
            break;
    }

    if (out_file == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
             fprintf(out_file, "%d", output_matrex[i][j]);
        }
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