/* Sieve of Eratosthenes*/

#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void print_array(int *arr, int length)
{
    int j;
    
    printf("\n");
    for(j = 0; j < length; j++)
        printf("%d ", arr[j]);
    printf("\n");
}

// This method filters out the non-prime numbers and reallocs the array to the smaller size required.
int sieve(int *arr, int original_len, int child_no)
{
    int i, final_length = 0;

    for(i = 0; i < original_len; i++)
    {
        if((arr[i] % child_no != 0) || (arr[i] == child_no))
        {
            arr[final_length++] = arr[i];
        }
    }

    arr = (int*)realloc(arr, (final_length)*sizeof(int));
        
    return final_length;
}

// This method prints the array before and after filtering
int filter_print(int *arr, int n, int child_no)
{
    print_array(arr, n);
    n = sieve(arr, n, child_no+1);
    print_array(arr, n);
    return n;
}

int main()
{
    int fd[2];
    int n;

    printf("\nEnter a perfect square:");
    scanf("%d", &n);

    int *arr = (int*)malloc(sizeof(int)*n); // Arrays created on the stack cannot be reallocated
    int i, j;
    int children = (int)sqrt(n);
    int fork_status = 0;

    // Initialise the original array
    for(i = 1; i <= n; i++)
    {
        arr[i-1] = i;
    }

    // Since the implementation starts with a read as below, the initial array is written to a pipe. A better implementation could be possible.
    int pipe_status = pipe(fd);
    int wr = write(fd[1], &arr, sizeof(int)*n);
    close(fd[1]);

    for(i = 1; i < children; i++)
    {
        if(fork_status > 0)
        {
            break;
        }
        else if(fork_status == -1)
        {
            printf("\nChild creation unsuccessful\n");
        }
        else // Child block
        {
            int rd = read(fd[0], &arr, sizeof(arr));
            
            printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());
            n = filter_print(arr, n, i);
            
            // Create a new pipe which is accessible from the child which the (then) parent writes to for the child to read in the next iteration
            close(fd[0]);
            pipe_status = pipe(fd);          
            fork_status = fork();
            if(fork_status > 0)
            {
                write(fd[1], &arr, sizeof(arr));
                close(fd[1]);
                wait(0);
            }
        }
    }
        
    // The final child process needs to read from the pipe, filter and print the array
    if ((fork_status == 0) && (i == children))
    {
        int rd = read(fd[0], &arr, sizeof(arr));
        close(fd[0]);
        printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());            
        n = filter_print(arr, n, i);
    }   

    free(arr);
    
    return 0;
}