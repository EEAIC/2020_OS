#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int max;
int loops;
int *buffer;

int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

pthread_cond_t empty, fill;
pthread_mutex_t mutex;

int psum = 0;
int csum = 0;

void put(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % max;
    count++;
    psum += (value - fill_ptr);
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % max;
    count--;
    csum += (tmp - use_ptr);
    return tmp;
}

void *producer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex);
        while (count == max)
        {
            pthread_cond_wait(&empty, &mutex);
        }
        put(i);
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *consumer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex);
        while(count == 0)
            pthread_cond_wait(&fill, &mutex);
        int tmp = get();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <buffersize> <loops>\n", argv[0]);
        exit(1);
    }
    max = atoi(argv[1]);
    loops = atoi(argv[2]);

    buffer = (int*)malloc(max * sizeof(int));
    assert(buffer != NULL);

    int i;
    for (i = 0; i < max; i++) {
        buffer[i] = 0;
    }

    pthread_t p1, p2, c1, c2;

    pthread_create(&p1, NULL, producer, NULL);
    pthread_create(&p2, NULL, producer, NULL);
    pthread_create(&c1, NULL, consumer, NULL);
    pthread_create(&c2, NULL, consumer, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    
    if (psum == csum)
        printf("Test OK\n");
    else
        printf("Wrong\n");

    return 0;
}
