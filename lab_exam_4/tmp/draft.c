#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

char *Input;
char *Output;
int length;
int n;
sem_t *semaphores;

char convert_case(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 32;  
    else if (c >= 'A' && c <= 'Z')
        return c + 32;  
    else
        return c;  
}

void *thread_function(void *arg) {
    int index = *((int *)arg);
    for (int i = index; i < length; i += n) {
        sem_wait(&semaphores[i]); 
        Output[i] = convert_case(Input[i]);
        sem_post(&semaphores[(i + n) % length]); 
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    printf("Enter string length: ");
    scanf("%d", &length);

    Input = (char *)malloc((length + 1) * sizeof(char));
    Output = (char *)malloc((length + 1) * sizeof(char));
    semaphores = (sem_t *)malloc(length * sizeof(sem_t));

    printf("Enter the string: ");
    scanf("%s", Input);

    printf("Enter the value of n: ");
    scanf("%d", &n);

    pthread_t threads[n];
    int indices[n];

    for (int i = 0; i < length; i++) {
        sem_init(&semaphores[i], 0, 0);
    }
    for (int i = 0; i < n; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &indices[i]);
    }

    sem_post(&semaphores[0]); 

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    Output[length] = '\0';

    printf("Converted string: %s\n", Output);

    free(Input);
    free(Output);
    for (int i = 0; i < length; i++) {
        sem_destroy(&semaphores[i]);
    }
    free(semaphores);

    return 0;
}
