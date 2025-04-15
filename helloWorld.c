#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_THREADS 5

void* print_hello(void* thread_id) {
    long tid = (long)thread_id;
    printf("Hello World from thread ID: %ld\n", tid);
    pthread_exit(NULL);
}

double get_time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct timeval start, end;

    gettimeofday(&start, NULL);
    for (long i = 0; i < NUM_THREADS; i++) {
        printf("Hello World from thread ID: %ld\n", i);
    }
    gettimeofday(&end, NULL);
    printf("Serial Time: %.0f microseconds\n", get_time_diff(start, end));

    gettimeofday(&start, NULL);
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, print_hello, (void*)i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);
    printf("Parallel Time: %.0f microseconds\n", get_time_diff(start, end));

    return 0;
}
