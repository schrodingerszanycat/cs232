// #include <stdio.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <stdlib.h>

// #define MAX_READERS 5
// #define MAX_WRITERS 2

// // https://stackoverflow.com/questions/42861913/why-use-usleep-and-not-sleep

// int shared_resource = 0;

// sem_t mutex;
// sem_t read_count_mutex;
// int read_count = 0;

// void *reader(void *arg)
// {
//     int reader_id = *(int *)arg;
//     while (1)
//     {
//         sem_wait(&read_count_mutex);
//         read_count++;
//         if (read_count == 1)
//         {
//             sem_wait(&mutex);
//         }
//         sem_post(&read_count_mutex);

//         if(shared_resource >= 10)
//             exit(0);

//         printf("Reader %d reads: %d\n", reader_id, shared_resource);

//         sem_wait(&read_count_mutex);
//         read_count--;
//         if (read_count == 0)
//         {
//             sem_post(&mutex);
//         }
//         sem_post(&read_count_mutex);

//         usleep(100000);
//     }
//     return NULL;
// }

// void *writer(void *arg)
// {
//     int writer_id = *(int *)arg;
//     while (1)
//     {
//         sem_wait(&mutex);

//         shared_resource++;
//         printf("Writer %d writes: %d\n", writer_id, shared_resource);

//         sem_post(&mutex);

//         usleep(200000);
//     }
//     return NULL;
// }

// int main()
// {
//     pthread_t readers[MAX_READERS];
//     pthread_t writers[MAX_WRITERS];
//     int reader_ids[MAX_READERS];
//     int writer_ids[MAX_WRITERS];

//     sem_init(&mutex, 0, 1);
//     sem_init(&read_count_mutex, 0, 1);

//     for (int i = 0; i < MAX_READERS; ++i)
//     {
//         reader_ids[i] = i + 1;
//         pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
//     }

//     for (int i = 0; i < MAX_WRITERS; ++i)
//     {
//         writer_ids[i] = i + 1;
//         pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
//     }

//     // Wait for threads to finish (not shown here)
//     for (int i = 0; i < MAX_READERS; ++i)
//     {
//         pthread_join(readers[i], NULL);
//     }

//     for (int i = 0; i < MAX_WRITERS; ++i)
//     {
//         pthread_join(writers[i], NULL);
//     }

//     // Clean up
//     sem_destroy(&mutex);
//     sem_destroy(&read_count_mutex);

//     return 0;
// }

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_READERS 5
#define MAX_WRITERS 2

// https://stackoverflow.com/questions/42861913/why-use-usleep-and-not-sleep

int shared_resource = 0;

sem_t mutex;
sem_t read_count_mutex;
int read_count = 0;

void *reader(void *arg)
{
    int reader_id = *(int *)arg;
    while (1)
    {
        sem_wait(&read_count_mutex);
        read_count++;
        if (read_count == 1)
        {
            sem_wait(&mutex);
        }
        sem_post(&read_count_mutex);

        if(shared_resource >= 10)
            exit(0);

        printf("Reader %d reads: %d\n", reader_id, shared_resource);

        sem_wait(&read_count_mutex);
        read_count--;
        if (read_count == 0)
        {
            sem_post(&mutex);
        }
        sem_post(&read_count_mutex);

        usleep(100000);
    }
    return NULL;
}

void *writer(void *arg)
{
    int writer_id = *(int *)arg;
    while (1)
    {
        sem_wait(&mutex);

        shared_resource++;
        printf("Writer %d writes: %d\n", writer_id, shared_resource);

        sem_post(&mutex);

        usleep(200000);
    }
    return NULL;
}

int main()
{
    pthread_t readers[MAX_READERS];
    pthread_t writers[MAX_WRITERS];
    int reader_ids[MAX_READERS];
    int writer_ids[MAX_WRITERS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    sem_init(&mutex, 0, 1);
    sem_init(&read_count_mutex, 0, 1);

    for (int i = 0; i < MAX_READERS; ++i)
    {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], &attr, reader, &reader_ids[i]);
    }

    for (int i = 0; i < MAX_WRITERS; ++i)
    {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], &attr, writer, &writer_ids[i]);
    }

    // Wait for threads to finish (not shown here)
    for (int i = 0; i < MAX_READERS; ++i)
    {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < MAX_WRITERS; ++i)
    {
        pthread_join(writers[i], NULL);
    }

    // Clean up
    sem_destroy(&mutex);
    sem_destroy(&read_count_mutex);

    return 0;
}