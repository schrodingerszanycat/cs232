#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>

#define BUFSIZE 10
#define PRODUCER_COUNT 5
#define CONSUMER_COUNT 5
#define MAX_OPS 50

char buf[BUFSIZE];

// https://stackoverflow.com/questions/70773/pthread-cond-wait-versus-semaphore

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

int buffer_top = -1;
int ops = 0;

void *producer(void *dummy_ptr)
{
    char item = 'i';
    do
    {
        pthread_mutex_lock(&mutex);
        while (buffer_top == BUFSIZE - 1)
            pthread_cond_wait(&empty, &mutex);
        buffer_top = buffer_top + 1;
        buf[buffer_top] = item;
        printf("Buffer filled to %d items\n", buffer_top + 1);
        ops++;
        if(ops > MAX_OPS)
            exit(0);
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
    } while (true);
}

void *consumer(void *dummy_ptr)
{
    char item;
    do
    {
        pthread_mutex_lock(&mutex);
        while (buffer_top == -1)
            pthread_cond_wait(&fill, &mutex);
        item = buf[buffer_top];
        buffer_top -= 1;
        printf("Buffer consumed to %d items\n", buffer_top + 1);
        ops++;
        if(ops > MAX_OPS)
            exit(0);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    } while (true);
}

int main()
{
    pthread_t producer_thread_id[PRODUCER_COUNT];
    pthread_t consumer_thread_id[CONSUMER_COUNT];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_create(&producer_thread_id[i], &attr, producer, NULL);

    for (int i = 0; i < CONSUMER_COUNT; i++)
        pthread_create(&consumer_thread_id[i], &attr, consumer, NULL);

    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_join(producer_thread_id[i], NULL);

    for (int i = 0; i < CONSUMER_COUNT; i++)
        pthread_join(consumer_thread_id[i], NULL);

    return 0;
}