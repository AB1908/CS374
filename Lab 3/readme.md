# Lab 3

## `dup()` and `dup2()` System Calls

- The `dup()` system call duplicates the file descriptor of the lowest unused value.

    Ex: 
    ```C
    close(0);
    dup(fd[0]);
    ```
- The above two statments can be replaced by a single `dup2()` call.

    Ex:
    ```C
    dup2(fd[0],0);
    ```

## IPC Using FIFO

- The `pipe` operator has limitations. A simple example would be that using a `pipe`, processes cannot dynamically communicate with each other. It is a **"one-time"** communication. Processes must also be from the same family tree.
- We therefore use a construct called FIFO for processes to dynamically communicate with each other.
- FIFOs are represented by files.

    Ex:
    ```C
    mkfifo("myfile.txt",0666);
    int fd = open("myfile.txt, O_WRONLY | O_APPEND);
    ```
- The `open()` call is, by default, a blocking operation. What this means is, the process calling `open()` (in this case with a _write-only_ flag) must wait (or is **blocked**) until a **reader** process exists.
- Many more flags can be passed to `open()`,for example `O_READONLY`.
- This is also an efficient way to pass messages between processes, a feature we can leverage to make an instant messaging app. We would need _four_ processes, two on each end to read and write from the FIFO. In such a case, we would need a non-blocking `open()` as we want to send and receive messages regardless of the operation of the other user. This can be done by passing the appropriate flag to `open()`.

## `read()` and `write()` System Calls

- Both calls return the number of bytes succesfully read/written.

## Assigment

1. Write a program to create a full process tree of height _h_ and degree _n_. Be careful to avoid orphaning any processes.

2. Use the `exec()` family of system calls to call the previously created program (`tree.out`). Use four variants of the `exec()` call: `execlp()`, `execl()`, `execv()`, `execvp()`.