# Lab 2

## Inter Process Communication

- In Linux, we can use `|` operator, also called a `pipe` to "pipe" the output of one process into another.
- The corresponding system call is `pipe()`. See `man pipe` for further details.
- It is a half duplex system, so it is crucial to establish communication between the parent and the child process. This typically involved closing file descriptors (read and write) of processes using the `close()` system call.

    Example: `$ ls /bin | grep bash`   
    This command lists the contents of `/bin` and provides that as input to the `grep` utility which then runs as a process to look for `bash`.

## `exec()` Family of System Calls

- The `exec()` call loads a new image in the currently executing process. By `exec()`, we mean the `exec` family of calls which contains `execvp()`, `execl()`, `execlp()` and so on. Read the man pages for more info.
- The address space is completely overwritten, thus saving the memory required to copy over the address space of the parent process during the `fork()` call.

## Assignment

1. Show that the address spaces of the child and parent process are **not** the same.

2. Create a process tree resembling a skewed binary tree, i.e. create the following process tree:
P<sub>1</sub> &rarr; P<sub>2</sub> &rarr; P<sub>3</sub> &rarr; P<sub>4</sub>..... &rarr; P<sub>n</sub> where &rarr; represents a child - parent relation. 

    Note: Be careful to ensure there are no orphaned processes.

See [solutions](solution.md) for answers.