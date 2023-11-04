#include "matmultp.h"

int main(int argc, char* argv[]){
   
    FILE *in_file_a, *in_file_b; //struct
    in_file_a = fopen("a.txt",  "r"); /*change to handel new files "a+" */
    in_file_b = fopen("b.txt",  "r"); //fopen returns a pointer to file structure
    if (in_file_a == NULL || in_file_b == NULL){             
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

    get_matrix(in_file_a, 'a');
    // print_matrix('a');

    get_matrix(in_file_b, 'b');
    // print_matrix('b');

   // close the input file
    fclose(in_file_a);
    fclose(in_file_b);

    //                                      Threading
    pthread_t th[ (a_row * b_col) + a_row + 1];

    //part1
    if (pthread_create(&th[0], NULL, &thPerMat, NULL)){ 
        printf("Error: Could not create a thread");
        exit(1);
    }
    if(pthread_join(th[0], NULL)){
        printf("Error: Could not join a thread");
        exit(1);
    }

    //part 2
    for(int i = 0;i<a_row;i++){
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i+1], NULL, &thPerRow,a)){ 
            printf("Error: Could not create a thread");
            exit(1);
        }
        // free(a);

    }
    for(int i = 0;i<a_row;i++){
        if(pthread_join(th[i+1], NULL)){
            printf("Error: Could not join a thread");
            exit(1);
        }
    }

    //part 3
    struct counters index;

    for(int i = 0;i<a_row;i++){
        index.counter1 = malloc(sizeof(int*));
        *index.counter1 = i;
        for(int j = 0;j<b_col;j++){
            index.counter2 = malloc(sizeof(int*));
            *index.counter2 = j;
            if (pthread_create(&th[ ( i * b_col + j ) + a_row + 1 ], NULL, &thPerElement, (void*) &index)){ 
                printf("Error: Could not create a thread");
                exit(1);
            }
        }
    }
    for(int i = 0;i<a_row;i++){
        for(int j = 0;j<b_col;j++){
            if(pthread_join(th[( i * b_col + j ) + a_row + 1 ], NULL)){
                printf("Error: Could not join a thread");
                exit(1);
            }
        }
    }
    // free(index.counter1); free(index.counter2);

    fclose(out_file_1); fclose(out_file_2); fclose(out_file_3);

    // printf("\n==================Results==============");
    printf("\nMatrix :   Threads : 1   ");
    printf("Time(uS) : %lu \n", stop1.tv_usec - start1.tv_usec );
    printf("Row :      Threads : %d  ", a_row);
    printf("Time(uS) : %lu\n", stop2.tv_usec - start2.tv_usec );
    printf("Element :  Threads : %d ", a_row * b_col);
    printf("Time(uS) : %lu\n\n", stop3.tv_usec - start3.tv_usec );

    return 0;
}

void *thPerMat(){

    gettimeofday(&start1, NULL);
    // printf("\n\nhii iam thread part 1\n");

    out_file_1 = fopen("c_per_matrix.txt", "w+");
    fprintf(out_file_1, "Method: A thread per matrix\n");
    if (out_file_1 == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    int sum = 0; 
    // for each row in the output matrix
    for(int r = 0; r < a_row; r++){
        //for each column in the output matrix
        for(int c = 0; c < b_col; c++){ 
            //for each output element   
            for(int j=0; j < b_row; j++){
                sum = sum + matrix_a[r][j] * matrix_b[j][c];
                // printf("%d.%d.%d sum = %d\n",r,c,j,sum);
            }
            fprintf(out_file_1, "%d ", sum);
            // printf("\n%d.%d sum = %d\n",r,c,sum);
            sum = 0;
        }
        fprintf(out_file_1, " \n");
    }  

    gettimeofday(&stop1, NULL);
    return NULL;
}
void *thPerRow(void *arg){

    gettimeofday(&start2, NULL);
    // int *r = (int*)malloc(sizeof(int));
    int r = *((int*) arg);

    // printf("\n\nhii iam thread part 2\n");

    if (out_file_2 == NULL){
        out_file_2 = fopen("c_per_row.txt", "w+");
        fprintf(out_file_2, "Method: A thread per row\n");
    } 

    if (out_file_2 == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }
    
    int sum = 0; 
    //for each column in the output matrix
    for(int c = 0; c < b_col; c++){ 
            //for each output element   
        for(int j=0; j < b_row; j++){
            sum = sum + matrix_a[r][j] * matrix_b[j][c];
            // printf("%d.%d.%d sum = %d\n",*r,c,j,sum);
        }
        fprintf(out_file_2, "%d ", sum);
        fflush(out_file_2);
        // printf("\n%d.%d sum = %d\n",*r,c,sum);
        sum = 0;
    }
    fprintf(out_file_2, " \n");
    fflush(out_file_2);
    // free(arg);
    gettimeofday(&stop2, NULL);
    return NULL;
}
void *thPerElement(void *arg){

    gettimeofday(&start3, NULL);

    struct counters* index = (struct counters*) arg;

    // int *r = malloc(sizeof(int*));
    // int *c = malloc(sizeof(int*));

    int r = *index->counter1;
    int c = *index->counter2;

    // printf("\n\nhii iam thread part 3\n");

    if (out_file_3 == NULL){
        out_file_3 = fopen("c_per_element.txt", "w+");
        fprintf(out_file_3, "Method: A thread per element\n");
    }          
    if (out_file_3 == NULL ){
        printf("Error: Could not open the file\n");
        exit(1);
    }

    int sum = 0; 
    //for each column in the output matrix  
    for(int j=0; j < b_row; j++){
        sum = sum + matrix_a[r][j] * matrix_b[j][c];
        // printf("%d.%d.%d sum = %d\n",*r,*c,j,sum);
    }

    fprintf(out_file_3, "%d ", sum);
    fflush(out_file_3);
    if(c == b_col-1){
        fprintf(out_file_3, "\n");
    }
    fflush(out_file_3);
    // printf("\n%d.%d sum = %d\n",*r,*c,sum);
    // free(arg);
    gettimeofday(&stop3, NULL);
    return NULL;
}
void get_matrix(FILE *in_file, char ab){

    int flag = 0, i, j;
    switch (ab){
        case 'a':
            for (i = 0; i < a_row; i++){
                for(j = 0; j < a_col; j++){
                    if( ( fscanf(in_file, "%d ", &matrix_a[i][j]) ) - 1){
                        flag = 1;
                        break;
                    }
                    // printf("%d ", fileContent[i][j]);
                }
                if(flag == 1){ break;}
            }
            break;
        case 'b':
            for (i = 0; i < b_row; i++){
                for(j = 0; j < b_col; j++){
                    if( ( fscanf(in_file, "%d", &matrix_b[i][j]) ) - 1){
                        flag = 1;
                        break;
                    }
                }
                if(flag == 1){ break;}
            }
            break;
    }
    // check_dimention_matching(i,j, ab);
}
void print_matrix(char ab) {
        switch (ab){
        case 'a':
            for (int i = 0; i < a_row; i++) {
                for (int j = 0; j < a_col; j++) {
                    printf("%d ", matrix_a[i][j]);
                }
                printf("\n");
            }
            break;
        case 'b':
            for (int i = 0; i < b_row; i++) {
                for (int j = 0; j < b_col; j++) {
                    printf("%d ", matrix_b[i][j]);
                }
                printf("\n");
            }
            break;
    }
}
void check_dimention_matching(int i, int j,char ab){
   
    printf("i = %d j = %d\n",i,j);

    if( (i != a_row) || (j != a_col)){
        printf("Error: File dimentions is not correct\n");
    }
    else {
        printf("Valid input file dimentions\n");
    }
}
