// Sieve of Eratosthenes

#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_array(int *arr, int length)
{
    int j;
    
    printf("\n");
    for(j = 0; j < length; j++)
        printf("%d ", arr[j]);
    printf("\n");
}

// Since realloc requires a size to reallocate to before the call, the size is calculated beforehand in this function. This could easily have been implemented more elegantly inside the sieve method itself.
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

// This method filters out the non-prime numbers and reallocs the array to the smaller size required.
int sieve(int *arr, int original_len, int child_no)
{
    int i, final_length = 0;

    int size = size_reduce(arr, original_len, child_no);
    
    for(i = 0; i < original_len; i++)
    {
        if((arr[i] % child_no != 0) || (arr[i] == child_no))
        {
            arr[final_length++] = arr[i];
        }
    }

    arr = (int*)realloc(arr, size*sizeof(int));
        
    return size;
}

int filter_print(int *arr, int n, int child_no)
{
    print_array(arr, n);            // Print the array
    n = sieve(arr, n, child_no+1);  // before and after
    print_array(arr, n);            // filtering
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
    int children = usr_sqrt(n);
    int fork_status = 0;

    for(i = 1; i <= n; i++) // Initialise the original array
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
            
            close(fd[0]);                               // Close the existing pipe and then
            pipe_status = pipe(fd);                     // Create a new pipe
            fork_status = fork();                       // which is now shared with a child
            if((fork_status > 0) && (i != children))    // and write to the pipe if parent and not the final child
            {
                write(fd[1], &arr, sizeof(arr));
                close(fd[1]);
                wait(0);
            }
        }
    }
        
    if ((fork_status == 0) && (i == children)) // The final child process needs to read from the pipe, filter and printthe array
    {
        int rd = read(fd[0], &arr, sizeof(arr));
        close(fd[0]);
        printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());            
        n = filter_print(arr, n, i);
    }   

    free(arr);
    
    return 0;
}