// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// #define MAX 100

// typedef struct {
//     int* arr;
//     int size;
// } ArrayInfo;

// void* merge_sort(void* arg) 
// {
//     ArrayInfo* info = (ArrayInfo*)arg;
//     int* arr = info->arr;
//     int size = info->size;
//     if (size <= 1)
//         return NULL;

//     int mid = size / 2;

//     ArrayInfo left_info = { arr, mid };
//     ArrayInfo right_info = { arr + mid, size - mid };

//     pthread_t tid;
//     pthread_create(&tid, NULL, merge_sort, &left_info);
//     merge_sort(&right_info);

//     pthread_join(tid, NULL);

//     int* merged = (int*)malloc(size * sizeof(int));
//     int i = 0, j = mid, k = 0;
//     while (i < mid && j < size) {
//         if (arr[i] <= arr[j])
//             merged[k++] = arr[i++];
//         else
//             merged[k++] = arr[j++];
//     }
//     while (i < mid)
//         merged[k++] = arr[i++];
//     while (j < size)
//         merged[k++] = arr[j++];

//     for (i = 0; i < size; i++)
//         arr[i] = merged[i];

//     free(merged);
//     return NULL;
// }

// void merge(int* arr, int left_size, int right_size) 
// {
//     int* merged = (int*)malloc((left_size + right_size) * sizeof(int));
//     int i = 0, j = left_size, k = 0;
//     while (i < left_size && j < left_size + right_size) {
//         if (arr[i] <= arr[j])
//             merged[k++] = arr[i++];
//         else
//             merged[k++] = arr[j++];
//     }
//     while (i < left_size)
//         merged[k++] = arr[i++];
//     while (j < left_size + right_size)
//         merged[k++] = arr[j++];

//     for (i = 0; i < left_size + right_size; i++)
//         arr[i] = merged[i];

//     free(merged);
// }

// int main() {
//     int n;
//     printf("Enter the number of elements: ");
//     scanf("%d", &n);
//     if (n <= 0 || n > MAX) {
//         printf("Input should be between 0 and %d.\n", MAX);
//         return 1;
//     }

//     int arr[MAX];
//     printf("Enter %d integers:\n", n);
//     for (int i = 0; i < n; i++)
//     {
//         scanf("%d", &arr[i]);
//     }

//     pthread_t tid;
//     ArrayInfo info = { arr, n };
//     pthread_create(&tid, NULL, merge_sort, &info);
//     pthread_join(tid, NULL);

//     merge(arr, n / 2, n - n / 2);


//     printf("Sorted array: ");
//     for (int i = 0; i < n; i++)
//         printf("%d ", arr[i]);
//     printf("\n");

//     return 0;
// }

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <pthread.h>
// // #include<time.h>

// // #define MAX 100

// // void swap(int *x, int *y) {
// //     int temp = *x;
// //     *x = *y;
// //     *y = temp;
// // }

// // void* sort(int n, int arr[n])
// // {
// //     for(int i = 0; i < n; i++)
// //     {
// //         for(int j = i+1; j < n; j++)
// //         {
// //             if (arr[j] < arr[j-1])
// //             {
// //                 swap(arr[j], arr[j+1]);
// //             }
// //         }
// //     }
// // 	pthread_exit(0);
// // }

// // int main(int argc, char **argv)
// // {
// // 	int n;
// //     printf("Enter the number of elements: ");
// //     scanf("%d", &n);
// //     if (n <= 0 || n > MAX) {
// //         printf("Input should be between 0 and %d.\n", MAX);
// //         return 1;
// //     }

// //     int arr[MAX];
// //     printf("Enter %d integers:\n", n);
// //     for (int i = 0; i < n; i++)
// //     {
// //         scanf("%d", &arr[i]);
// //     }

// //     int s1 = n/2;
// //     int s2 = n - (n/2);

// //     int a1[s1];
// //     int a2[s2];
// //     for (int i = 0; i < s1; i++) {
// //         a1[i] = arr[i];
// //     }
// //     for (int i = 0; i < s2; i++) {
// //         a2[i] = arr[s1 + i];
// //     }

// //     // for (int i = 0; i < s1; i++) {
// //     //     printf("%d", a1[i]);
// //     // }
// //     // for (int i = 0; i < s2; i++) {
// //     //     printf("%d", a2[i]);
// //     // }

// // 	pthread_t tid[2];

// // 	pthread_attr_t attr;
// // 	pthread_attr_init(&attr);

// // 	pthread_create(&tid[0], &attr, &sort, &s1, &a1);
// //     pthread_create(&tid[1], &attr, &sort, &s2, &a2);

// // 	pthread_join(tid[0], NULL);
// //     pthread_join(tid[1], NULL);

// //     for (int i = 0; i < s1; i++) {
// //         printf("%d", a1[i]);
// //     }
// //     for (int i = 0; i < s2; i++) {
// //         printf("%d", a2[i]);
// //     }

// // }


// #include <stdio.h>
// #include <pthread.h>
// #include <stdlib.h>

// int n;
// int *arr;

// void swap(int *x, int *y) {
//     int temp = *x;
//     *x = *y;
//     *y = temp;
// }

// void *sort1(void *dummyPtr)
// {
//     int start = 0;
//     int end = n/2 - 1;

//     for(int i = start; i <= end; i++)
//     {
//         for(int j = i+1; j < n; j++)
//         {
//             if (arr[j] < arr[j-1])
//             {
//                 swap(&arr[j], &arr[j+1]);
//             }
//         }
//     }
//     pthread_exit(NULL);
// }

// void *sort2(void *dummyPtr)
// {
//     int start = n/2;
//     int end = n - 1;

//     for(int i = start; i <= end; i++)
//     {
//         for(int j = i+1; j < n; j++)
//         {
//             if (arr[j] < arr[j-1])
//             {
//                 swap(&arr[j], &arr[j+1]);
//             }
//         }
//     }

//     pthread_exit(NULL);
// }

// int main()
// {
//     printf("Enter the array size\n");
//     scanf("%d", &n);

//     arr = (int *) calloc(n, sizeof(int));

//     printf("Enter the elements of the array\n");
//     for(int i = 0; i < n; i++)
//         scanf("%d", &arr[i]);
    
//     pthread_t thread_ids[2];

//     pthread_create(&thread_ids[0], NULL, &sort1, NULL);
//     pthread_create(&thread_ids[1], NULL, &sort2, NULL);

//     pthread_join(thread_ids[0], NULL);
//     pthread_join(thread_ids[1], NULL);

//     printf("The two sorted halves that need to be merged:\n");
//     for(int i = 0; i < n/2; i++)
//         printf("%d ", arr[i]);
//     printf("\n");
//     for(int i = n/2; i < n; i++)
//         printf("%d ", arr[i]);
//     printf("\n");


//     int ptr1 = 0;
//     int ptr2 = n/2;

//     int narr[n];
//     int narr_index = 0;

//     while(ptr1 < n/2 && ptr2 < n)
//     {
//         if(arr[ptr1] < arr[ptr2])
//         {
//             narr[narr_index] = arr[ptr1];
//             ptr1++;
//         }
//         else
//         {
//             narr[narr_index] = arr[ptr2];
//             ptr2++;
//         }
//         narr_index++;
//     }

//     while(ptr1 < n/2)
//     {
//         narr[narr_index] = arr[ptr1];
//         ptr1++;
//         narr_index++;
//     }

//     while(ptr2 < n)
//     {
//         narr[narr_index] = arr[ptr2];
//         narr_index++;
//         ptr2++;
//     }

//     for(int i = 0; i < n; i++)
//         arr[i] = narr[i];

//     printf("The merged array: \n");
//     for(int i = 0; i < n; i++)
//         printf("%d ", arr[i]);
//     printf("\n");


//     free(arr);

//     return 0;
// }
