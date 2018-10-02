#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_info {
        pthread_t thread_id;
        int *first_arr;
        int *second_arr;
        int *sum_array;
};

int array_input(int m, int n, int arr[][n], int no)
{
        int i, j;
        printf("\nArray %d\n", no);
        for (i = 0; i < m; i++) {
                for (j = 0; j < n; j++) {
                        printf("\nEnter an element:");
                        scanf("%d", &arr[i][j]);
                }
        }
        // printf("\n%d\n", sizeof(arr));
        return 0;
}

void *thread_start(void *arg)
{
        struct thread_info *t;
        t = (struct thread_info *) arg;
        *t->sum_array= *t->first_arr + *t->second_arr;
}

int main()
{
        int i, j;
        int m, n;
        printf("\nEnter the number of rows for the matrix:");
        scanf("%d", &m);
        printf("\nEnter the number of columns for the matrix:");
        scanf("%d", &n);

        int arr1[m][n];
        int arr2[m][n];
        int sum[m][n];

        array_input(m, n, arr1, 1);
        array_input(m, n, arr2, 2);
        
        struct thread_info *t1;
        t1 = (struct thread_info *) malloc(sizeof(struct thread_info));
        t1->first_arr   = *arr1;
        t1->second_arr  = *arr2;
        t1->sum_array   = *sum;
        t1->thread_id   = 1;
        printf("\n%d\n", t1->sum_array[0][0]);
        // printf("\n%d\n", sizeof(int));
        pthread_create(t1->thread_id, NULL, thread_start, (void *) t1);
        return 0;
}