

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int n;
int *array;

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void *sort1(void *dummyPtr) {
    for(int i = 0; i < n/2; i++) 
    {
        for(int j = i+1; j < n/2; j++) 
        {
            if (array[j] < array[i]) 
            {
                swap(&array[j], &array[i]);
            }
        }
    }
    pthread_exit(NULL);
}

void *sort2(void *dummyPtr) 
{
    for(int i = n/2; i < n; i++) 
    {
        for(int j = i+1; j < n; j++) 
        {
            if (array[j] < array[i]) 
            {
                swap(&array[j], &array[i]);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    printf("Enter no.of elements: ");
    scanf("%d", &n);

    array = (int *) calloc(n, sizeof(int));

    printf("Enter the elements of the array: ");
    for(int i = 0; i < n; i++)
        scanf("%d", &array[i]);
    
    pthread_t thread_ids[2];

    pthread_create(&thread_ids[0], NULL, &sort1, NULL);
    pthread_create(&thread_ids[1], NULL, &sort2, NULL);

    pthread_join(thread_ids[0], NULL);
    pthread_join(thread_ids[1], NULL);

    printf("The two sorted arrays:\n");
    for(int i = 0; i < n/2; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    for(int i = n/2; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    int p1 = 0;
    int p2 = n/2;
    int new_array_index = 0;
    int *new_array = (int *)malloc(n * sizeof(int));

    // merging
    while(p1 < n/2 && p2 < n) 
    {
        if(array[p1] < array[p2]) 
        {
            new_array[new_array_index++] = array[p1++];
        }
        else 
        {
            new_array[new_array_index++] = array[p2++];
        }
    }

    while(p1 < n/2) 
    {
        new_array[new_array_index++] = array[p1++];
    }

    while(p2 < n) 
    {
        new_array[new_array_index++] = array[p2++];
    }

    for(int i = 0; i < n; i++)
    {
        array[i] = new_array[i];
    }

    printf("The merged array: \n");
    for(int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
