#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

int array[ARRAY_SIZE];
int total_sum = 0;
pthread_mutex_t sum_mutex;

double get_time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
}

void* partial_sum(void* arg) {
    long tid = (long)arg;
    int chunk = ARRAY_SIZE / NUM_THREADS;
    int start = tid * chunk;
    int end = (tid == NUM_THREADS - 1) ? ARRAY_SIZE : start + chunk;

    int local_sum = 0;
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
    struct timeval start, end;

    for (int i = 0; i < ARRAY_SIZE; i++) array[i] = 1;

    int serial_sum = 0;
    gettimeofday(&start, NULL);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        serial_sum += array[i];
    }
    gettimeofday(&end, NULL);
    printf("Serial Sum: %d\n", serial_sum);
    printf("Serial Time: %.0f microseconds\n", get_time_diff(start, end));

    total_sum = 0;
    pthread_mutex_init(&sum_mutex, NULL);
    gettimeofday(&start, NULL);
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, partial_sum, (void*)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);
    pthread_mutex_destroy(&sum_mutex);

    printf("Parallel Sum: %d\n", total_sum);
    printf("Parallel Time: %.0f microseconds\n", get_time_diff(start, end));

    return 0;
}

}
