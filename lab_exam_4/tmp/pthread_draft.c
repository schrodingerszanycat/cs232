#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

int sentence_size = 0;
int sentence_index = 0;
char *input;
char *output;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *case_converter(void *arg)
{
    while (sentence_index < sentence_size)
    {
        pthread_mutex_lock(&mutex);
        int curr_index = sentence_index;
        sentence_index++;
        pthread_mutex_unlock(&mutex);

        int c = input[curr_index];
        if (isupper(c))
        {
            c = tolower(c);
        }
        else if (islower(c))
        {
            c = toupper(c);
        }

        output[curr_index] = c;
    }
}

int main()
{
    printf("Enter the size of the sentence\n");
    scanf("%d", &sentence_size);

    char input_arr[sentence_size + 1];
    printf("Enter the sentence\n");
    scanf("\n%[^\n]s", input_arr);
    input = input_arr;

    printf("Enter the number of threads\n");
    int n_threads;
    scanf("%d", &n_threads);

    char output_arr[sentence_size + 1];
    strcpy(output_arr, input_arr);
    output = output_arr;

    pthread_t thread_id[n_threads];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i < n_threads; i++)
        pthread_create(&thread_id[i], &attr, case_converter, NULL);

    for (int i = 0; i < n_threads; i++)
        pthread_join(thread_id[i], NULL);

    printf("Output: %s\n", output);

    return 0;
}