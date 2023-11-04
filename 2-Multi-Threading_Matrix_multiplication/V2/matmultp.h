#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>
#include <string.h>

int a_row, a_col, b_row, b_col;
int matrix_a[20][20],matrix_b[20][20];
struct timeval start1, stop1, start2, stop2, start3, stop3;
pthread_mutex_t mutex;
FILE *out_file_1, *out_file_2, *out_file_3;
struct counters {
    int *counter1;
    int *counter2;
};

void *thPerMat();
void *thPerRow(void *arg);
void *thPerElement(void *arg);
void check_dimention_matching(int i, int j, char ab);
void get_matrix(FILE *in_file, char ab);
void print_matrix(char ab);
void write_to_file(int part,int *output_matrex, int *rows, int *cols);
void print_ptr_matrix(int *matrix, int *rows, int *cols);