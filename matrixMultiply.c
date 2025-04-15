#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
int C_serial[ROWS_A][COLS_B];  

double get_time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
}

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

void print_matrix(const char* label, int mat[ROWS_A][COLS_B]) {
    printf("%s\n", label);
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            printf("%4d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct timeval start, end;

    gettimeofday(&start, NULL);
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            C_serial[i][j] = 0;
            for (int k = 0; k < COLS_A; k++) {
                C_serial[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    gettimeofday(&end, NULL);
    double serial_time = get_time_diff(start, end);

    gettimeofday(&start, NULL);
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, multiply_row, (void*)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);
    double parallel_time = get_time_diff(start, end);

    print_matrix("Serial Result Matrix C_serial (A x B):", C_serial);
    print_matrix("Parallel Result Matrix C (A x B):", C);

    printf("Serial Time:   %.0f microseconds\n", serial_time);
    printf("Parallel Time: %.0f microseconds\n", parallel_time);

    return 0;
}


    return 0;
}
