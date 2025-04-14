#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 16

int array[ARRAY_SIZE];
int total_sum = 0;
pthread_mutex_t sum_mutex;

void* partial_sum(void* arg) {
    long tid = (long)arg;
    int local_sum = 0;
    int start = tid * (ARRAY_SIZE / NUM_THREADS);
    int end = start + (ARRAY_SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        local_sum += array[i];
    }

    pthread_mutex_lock(&sum_mutex);
    total_sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    pthread_mutex_init(&sum_mutex, NULL);

    for (long t = 0; t < NUM_THREADS; t++) {
        pthread_create(&threads[t], NULL, partial_sum, (void*)t);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&sum_mutex);

    printf("Total sum of array elements: %d\n", total_sum);
    return 0;
}
