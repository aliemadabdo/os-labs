#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

int a_row, a_col, b_row, b_col;

struct matrix_thread_args{
    int arrA[a_row][a_col];
    int arrB[b_row][b_col];
};
struct row_thread_args{
    int arrA[a_row];
    int arrB[b_row][b_col];
};
struct element_thread_args{
    int arrA[a_row];
    int arrB[b_row];
};

void *thPerMat(void *arg);
void *thPerRow(void *arg);
void *thPerElement(void *arg);

void get_matrix(FILE *in_file, int rows, int cols, int fileContent[][]);
void print_matrix(int matrix[20][20], int rows, int cols);
void write_to_file(int part, int matrix[20][20], int rows, int cols);

int main(int argc, char* argv[]){

    int a_elements, b_elements; 
    char mat1 = 'a', mat2 = 'b', outMat = 'c';
    int matrix_a[20][20],matrix_b[20][20];

    struct timeval start, stop;  //note that:
    gettimeofday(&start, NULL);  // this function with a non-null "TZ" parameter is now considered obsolete.

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
    printf("number of rows from matrix a   : %d\n", a_row);
    printf("number of columns from matrix a: %d\n", a_col);
    printf("number of rows from matrix a   : %d\n", b_row);
    printf("number of columns from matrix a: %d\n", b_col);

    //check if valid multiplication
    if (a_col != b_row){
        printf("Error: Not a valid dimentions for matrix multiplication");
        exit(1);
    }
    
    get_matrix(in_file_a, a_row, a_col, matrix_a);
    // print_matrix(matrix_a, a_row, a_col);

    get_matrix(in_file_b, b_row, b_col, matrix_b);
    // print_matrix(matrix_b, b_row, b_col);

    int size = sizeof(matrix_a) / sizeof(int);
    printf("\nmatrix size = %d \n", size);

   // close the input file
    fclose(in_file_a);
    fclose(in_file_b);

    // printf("size of int %lu \n", sizeof(int));
    gettimeofday(&stop, NULL);
    printf("execution time in seconds: %lu \n", stop.tv_sec - start.tv_sec ); 
    printf("execution time in micro seconds: %lu \n\n", stop.tv_usec - start.tv_usec );
                                        // ul --> unsigned long
    return 0;
}

void get_matrix(FILE *in_file, int rows, int cols, int fileContent[][]){
    /*
    this fuction get the matrix a and b from the file and check if 
    dimentions are valid

    Takes the file pointer , number of rows, number of columns and the matrix we want to set
    returns nothing
    */
    printf("number of rows passed to the fn    : %d\n", rows);
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
    printf("\ni = %d j = %d\n",i,j);

    if( (i != rows) || (j != cols)){
        printf("Error: File dimentions is not correct\n");
    }
    else {
        printf("Valid input file dimentions\n");
    }
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

// void write_to_file(int part, int output_matrex[][20], int rows, int cols){ 
//                                         // Output part
//     FILE *out_file;
//     out_file = fopen("as_we_input.txt", "w+");
//     if (out_file == NULL ){
//         printf("Error: Could not open the file\n");
//         return 2;
//     }
//     switch part{
//         case 1: fprintf(out_file, "Method: A thread per matrix"); break;
//         case 2: fprintf(out_file, "Method: A thread per row"); break;
//         case 3: fprintf(out_file, "Method: A thread per element"); break;
//     }

//     for (i = 0; i < rows; i++){
//         for(j = 0; j < cols; j++){
//              fprintf(out_file, "%d", output_matrex[i][j]);
//         }
//     }
//     fclose(out_file);
// }

// int matrix_size(int matrix[][]){
    
// }