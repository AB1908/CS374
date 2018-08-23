// Sieve of Eratosthenes

#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

// User defined square root function
int usr_sqrt(int n)
{
    int i;
    for(i = 1; i < n; i++)
    {
        if(i*i == n)
        {
            break;
        }
    }
    return i;
}

// Function to print the array
void print_array(int *arr, int n)
{
    printf("\n");
    int j;
    for(j = 0; j < n; j++)
    printf("%d ", arr[j]);
    printf("\n");
}

int size_reduce(int *arr, int n, int child_no)
{
    int i;
    int size = n;
    for(i = 0; i < n; i++)
    {
        if((arr[i] % child_no == 0) && (arr[i] != child_no))
        {
            size--;
        }
    }
    return size;
}

int* sieve(int *arr, int n, int child_no)
{
    // reallo
    int i, j = 0;
    int size = size_reduce(arr,n, child_no);
    int divisors;
    // int newarr[size];
    int *newarr = malloc(sizeof(int)*size);
    // int newarr = arr;
    // *arr = reallocarray(arr, size, sizeof(int));
    for(i = 0; i < n; i++)
    {
        if((arr[i] % child_no == 0) && (arr[i] != child_no))
        {
            arr[j++] = newarr[i];
        }
    }
    
    return newarr;
}

int main()
{
    int fd[2];
    int n;

    printf("\nEnter a perfect square:");
    scanf("%d", &n);

    int arr[n];
    int i, j;
    int children = usr_sqrt(n);
    int fork_status = 0;

    for(i = 1; i <= n; i++)
    {
        arr[i-1] = i;
    }
    int pipe_status = pipe(fd);
    int wr = write(fd[1], &arr, sizeof(arr));
    close(fd[1]);

    for(i = 1; i <= children; i++)
    {
        if(fork_status > 0)
        {
            // wait(0);
            break;
        }
        else if(fork_status == -1)
        {
            printf("\nChild creation unsuccessful\n");
        }
        else if ((fork_status == 0) && (i == children))
        {
            int rd = read(fd[0], &arr, sizeof(arr));
            print_array(arr, n);
        }
        else //if(i != children)
        {
            int rd = read(fd[0], &arr, sizeof(arr));
            
            print_array(arr, n);

            int *arr = sieve(arr, n, i); 

            close(fd[0]);
            pipe_status = pipe(fd);
            fork_status = fork();
            if(fork_status > 0)
            {
                printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());
                if(i != children)
                {
                write(fd[1], &arr, sizeof(arr));
                close(fd[1]);
                wait(0);
                }
            }
        }   
    }
}