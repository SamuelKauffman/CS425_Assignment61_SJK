#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 5

void* print_hello(void* thread_id) {
    long tid = (long)thread_id;
    printf("Hello World from thread ID: %ld\n", tid);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    for (long t = 0; t < NUM_THREADS; t++) {
        pthread_create(&threads[t], NULL, print_hello, (void*)t);
    }
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    return 0;
}
