#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 4
#define MAX_THREAD MAX *MAX

int matA[MAX][MAX] = {0};
int matB[MAX][MAX] = {0};
int matC[MAX][MAX] = {0};

void initialize_identity_matrix(int matrix[MAX][MAX], int size)
{
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            if (row == col)
            {
                matrix[row][col] = 1;
            }
            else
            {
                matrix[row][col] = 0;
            }
        }
    }
}

void *compute_element(void *arg)
{
    int ij = *((int *)arg);
    int i = ij / MAX;
    int j = ij % MAX;
    for (int k = 0; k < MAX; k++)
    {
        matC[i][j] += matA[i][k] * matB[k][j];
    }
    pthread_exit(NULL);
}

int main()
{
    initialize_identity_matrix(matA, MAX);
    initialize_identity_matrix(matB, MAX);

    pthread_t worker_threads[MAX_THREAD];
    int thread_ids[MAX_THREAD];

    // Create worker threads
    for (int i = 0; i < MAX_THREAD; i++)
    {
        thread_ids[i] = i;
        pthread_create(&worker_threads[i], NULL, compute_element, &thread_ids[i]);
    }

    // Wait for all worker threads to complete
    for (int i = 0; i < MAX_THREAD; i++)
    {
        pthread_join(worker_threads[i], NULL);
    }

    // Print the resulting matrix matC
    printf("Matrix C (Result of A * B):\n");
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            printf("%d ", matC[i][j]);
        }
        printf("\n");
    }

    return 0;
}