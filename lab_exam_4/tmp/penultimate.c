#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>   
#include <stdlib.h>
#include <string.h>

char *Input;
char *Output;
int str_idx = 0;
int length = 0;
int n_threads;
sem_t mutex;

char convert_case(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 32;  
    else if (c >= 'A' && c <= 'Z')
        return c + 32;  
    else
        return c;  
}

void *f(void *arg) 
{
    if (str_idx == length)
    {
        pthread_exit(0);
    }
    sem_wait(&mutex);
    int curr_idx = str_idx;
    str_idx++;
    sem_post(&mutex);
    Output[curr_idx] = convert_case(Input[curr_idx]);
}

int main(int argc, char **argv)
{
    printf("Enter string length: ");
    scanf("%d", &length);

    Input = (char *) malloc((length + 1) * sizeof(char));
    Output = (char *) malloc((length + 1) * sizeof(char));

    printf("Enter the string: ");
    scanf("\n%[^\n]s", Input);
    //length = strlen(Input);

    printf("Enter the number of threads: ");
    scanf("%d", &n_threads);

    pthread_t threads[n_threads];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < n_threads; i++)
    {
        pthread_create(&threads[i], &attr, f, NULL);
    }

    for (int i = 0; i < n_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Output: %s\n", Output);

    sem_destroy(&mutex);

    return 0;
}