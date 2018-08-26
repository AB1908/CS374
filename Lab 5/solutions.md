# Solutions

1. The assignment required a process to write an array of input size `n` elements to a pipe. This would then be read by the child process which would filter out all the multiples of two excluding two itself and write the remaining data to another pipe. The process would continue until all non-prime numbers have been filtered out by subsequent child processes which would then be written to STDOUT by the last child process instead of a pipe.

    The core logic of the implementation revolves around the idea that a child process always starts by reading from a pipe. After reading, it would filter out the unnecessary elements, close the original pipe, create a new pipe and then fork a child so that its child has the file descriptors to access this new pipe. The current process (now a parent) can write to the pipe and then wait for its child to execute.

    There are two edge cases for the implementation, namely the original process and the last child process. The original process needs to generate the necessary data and write it to the pipe, while the last child need not execute a `fork()` and simply read and display the data after applying the corresponding filter.

    There are also many ways of handling the data. One implementation is to create an array of the required size and keep track of the size as it decrements. This, however, would mean that the complete array exists and can be accessed, which may be an unnecessary usage of space. Therefore, I set out to reallocate the array as its size decreased. I made a messier implementation at first, creating a copy of the original array which was entirely unnecessary, which can be seen by perusing the commit history. Since the array size dynamically changed, the original memory allocation for it must be done using `malloc()` as data on the stack cannot be modified.

    **Implementation**: `prime-number-gen.c`

    **Input**:
    ```
    Enter a perfect square:9
    ```

    **Output**:
    ```
    The process 15265 was spawned by parent process 11384

    2 3 4 5 6 7 8 9

    The process 15265 was spawned by parent process 15264

    2 3 4 5 6 7 8 9 

    2 3 5 7 9 

    The process 15266 was spawned by parent process 15265

    2 3 5 7 9

    2 3 5 7
    ```