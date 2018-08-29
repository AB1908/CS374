/**
 * Sieve of Eratosthenes program where filtering is done by a child process.
 * NOTE: Compilation must be done with the `-lm` flag as math library is not properly linked in gcc.
 */

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

/**
 * sieve() - Filters out the non-prime numbers and reallocs the array to a smaller size.
 * @*arr:           Pointer to the array to be filtered
 * @original_len:   The length of the array
 * @filter_val:     The number whose multiples are to be filtered out
 * 
 * The sieve() method takes the original array as input and then filters out the numbers
 * which are multiples of filter_no. final_length keeps track of the index where the
 * acceptable numbers are copied to. The condition essentially skips copying any number
 * which is a multiple of filter_no.
 * 
 * Return:          The new length of the array
 */
int sieve(int *arr, int original_len, int filter_val)
{
    int i, final_length = 0;

    for(i = 0; i < original_len; i++)
    {
        if((arr[i] % filter_val != 0) || (arr[i] == filter_val))
        {
            arr[final_length++] = arr[i];
        }
    }

    arr = (int*)realloc(arr, final_length*sizeof(int));
        
    return final_length;
}

/**
 * filter_print() - Print the array before and after applying sieve
 * @*arr:       Pointer to array
 * @n:          Length of array
 * @child_no:   Child process number, which is used for filtering
 * 
 * child_no is the parameter to sieve() as the first child filters out
 * multiples of two, second child filters out multiples of three and so on.
 * 
 * Return: New (smaller) array size
 */
int filter_print(int *arr, int n, int child_no)
{
    print_array(arr, n);
    n = sieve(arr, n, child_no);
    print_array(arr, n);
    return n;
}

int main()
{
    int fd[2];
    int n;

    printf("\nEnter a perfect square:");
    scanf("%d", &n);
    
    int arr_size = n - 1;
    int *arr = (int*)malloc(sizeof(int)*arr_size); // Arrays created on the stack cannot be reallocated
    int i, j;
    int children = (int)sqrt(n);
    int fork_status = 0;
    int pipe_status = pipe(fd);

    for(i = 1; i < children; i++)
    {
        if(fork_status > 0)
        {
            break;
        }
        /**
         * For the original process, generate the array and write it to a pipe
         */
        else if((fork_status == 0) && (i == 1))
        {
            for(j = 0; j < arr_size; j++)
            {
                arr[j] = j + 2;
            }
            
            fork_status = fork();
            if(fork_status > 0)
            {
                printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());
                print_array(arr, arr_size);
                close(fd[0]);                
                write(fd[1], &arr, sizeof(arr));
                close(fd[1]);
                wait(0);
            }
        }
        /**
        * For a child process, close the file descriptors of the existing pipe
        * after reading and apply the filter. Then, create a new pipe which is
        * accessible from the child which the (then) parent writes to for the
        * child to read in the next iteration.
        */
        else if(fork_status == 0)
        {
            close(fd[1]);            
            read(fd[0], &arr, sizeof(arr));
            close(fd[0]);
            
            printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());
            arr_size = filter_print(arr, arr_size, i);
            
            int pipe_status = pipe(fd);
            fork_status = fork();
            if(fork_status > 0)
            {
                close(fd[0]);
                write(fd[1], &arr, sizeof(arr));
                close(fd[1]);
                wait(0);
            }
        }
        else
        {
            printf("\nChild creation unsuccessful\n");
        }
    }
        
    /**
     *  The final child process needs to read from the pipe, filter and print the array.
     */
    if ((fork_status == 0) && (i == children))
    {
        close(fd[1]);
        int rd = read(fd[0], &arr, sizeof(arr));
        close(fd[0]);
        printf("\nThe process %d was spawned by parent process %d\n", (int)getpid(), (int)getppid());            
        n = filter_print(arr, arr_size, i);
        _exit(0);
    }   

    free(arr);
    
    return 0;
}