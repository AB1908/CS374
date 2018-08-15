# Solutions

1. The first question required that we create a full process tree of height `h` and degree `n`. There are two ways of accomplishing it, one method is iterative and the other is recursive. The former has been implemented, thanks to @sonicaks. 

    - Iteratively, the solution required that we iterate over nodes and ensure that each node has `n` children. In the process, we must ensure that each _parent_ does not create more than `n` children, while the children do not execute the fork call and create further child processes. 
    
    - A `child_count` variable was used to keep track of the number of children created inside a `while` loop. A condition was added for the parent process to increment `child_count` and for the child process to break out of the `while` loop since we don't want extra `fork()` calls to be executed by the child process. (Hint: Child processes resume execution from after the `fork()` call.)
    
    - Now that a parent process has created the necessary number of children, we need the child process to call `fork()` until it reaches the required degree `n`. We simply add a condition for this after the `while` loop. Since we want a tree of height `h`, we must repeat this process `h` times using a looping method of choice.

    Implementation: [`full-process-tree.c`](full-process-tree.c)   
    Output: [`tree.out`](tree.out)

2. The second question required that we call the previously created `tree.out` using `exec()` calls (non-environment calls). The implementation had an interesting bug which @sonicaks was able to fix.

    - For some reason, it is **mandatory** to call `exec()` from the child process, **after** a `fork()`. This needs further investigation.

    - Initially, there was no `wait()` call in the parent. The `tree.out` process kept terminating before I could provide any input. I initially attributed it to not knowing the necessary parameters/functions required to call a binary output file. After not finding a satisfactory answer on the web, @sonicaks suggested adding a `wait()` call to the parent, which fixed the bug. A possible explanation has been described below.

    - Without a wait call, the parent process, `a.out`, died before its child received the necessary input. Since input was provided from the shell and the shell was directly interacting with the parent, once `tree.out` became an orphan, it was unable to interact with the shell as it was "fostered" by the `init` process (where it may be "starving" for input). Perhaps the connection to the shell could be re-established manually, but this has not been experimented upon. Once a `wait()` call was introduced, preventing `tree.out` from becoming an orphan, it was able to ask for input from the shell with no issues.

    Implementation: [`exec-process-tree.c`](exec-process-tree.c)