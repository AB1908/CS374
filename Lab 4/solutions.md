# Solutions

1. The first question required that we create a child process and a parent process. We then needed to create a pipe where one process would write "Hello <PID of process reading>" to the pipe which would be read by the other process. This had to be done by both processes.

    Since both the child and the parent process have the file descriptors to the read and write ends of the pipe, we can simply add `read()` and `write()` calls, one after the other in the required branches. This was followed up by closing the file descriptors. The implementation does not account for the string generation as I am unaware of how to concatenate strings in C. Care is also needed to not enter an arbitrary length for `read()` and `write()` calls as they will wait for further writing. The safest way to avoid this problem is to use a `sizeof()` call.

    Implementation: [`pipe-example.c`](pipe-example.c)

    Input:
    ```
    598 Child read
    Parent read 597
    ```

2. The question required that there be a skewed process tree which accepted an integer, incremented it and wrote to a pipe. It would be read by the child process, incremented and written to a pipe and this would continue up until the last child. The last child needed to read the value, increment it and write it to STDOUT.

    The implementation for this is incorrect as it only creates a single pipe which is read from and written to. It works because of the simple reason that pipes are fundamentally FIFO structures. Since the reads and writes occured sequentially, there were no bugs related to it. The implementation also produces an output that is one less than the expected value.

    Implementation: [`skewed-increment-tree.c`](skewed-increment-tree.c)

    Input:
    ```
    Enter a value for n: 4

    Enter a value to increment:5
    ```

    Output:
    ```
    731 has created a child process 734.

    The current incremented value is 6

    734 has created a child process 735.

    The current incremented value is 7

    735 has created a child process 736.

    The current incremented value is 8

    The final incremented value is 8
    ```

3. The final question required that there be two child processes, each of which would use an `exec()` call. The first child would execute `ls -l` and write it to the pipe, while the second child would read from the pipe, run `wc` on it, and display the results on screen.

    The implementation for this is inelegant for the simple reason that the core logic is flawed. Two children are first created which is entirely unnecessary as one child can be created, execute and die allowing for the next child to run, which cuts down on the space complexity. The implementation also uses a counter which is unnecessary and has three copies in existence, one for each child and another for the parent.
    
    In the original implementation, I tried passing the file descriptor array to a separate function which would be run as a subroutine by the child, but this produced erroneous output which I did not investigate further. Redesigning the implementation to execute in `main()` rectified the issue.

    Implementation: [`fork-and-pipe.c`](fork-and-pipe.c)
    
    Output:
    ```
    Process 811 was spawned by process 809

    Process 810 was spawned by process 809
    
    5      38     234
    ```