#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS_A 4
#define COLS_A 3
#define COLS_B 4
#define NUM_THREADS ROWS_A 

int A[ROWS_A][COLS_A] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {10, 11, 12}
};

int B[COLS_A][COLS_B] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
};

int C[ROWS_A][COLS_B]; 

void* multiply_row(void* arg) {
    int row = (int)(long)arg;

    for (int j = 0; j < COLS_B; j++) {
        C[row][j] = 0;
        for (int k = 0; k < COLS_A; k++) {
            C[row][j] += A[row][k] * B[k][j];
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, multiply_row, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Result matrix C (A x B):\n");
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            printf("%4d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
