# Solutions

1. The easiest way to accomplish this is to prove by contradiction. 

    - Initialise a variable before forking the process, which will then exist in the address spaces of both the child and the parent.
    - Change the contents of the variable in the child and observe the value of the same variable in the parent.
    - Clearly, the variable in the parent does not change, implying that both do **NOT** share the same address space.

    Note: Interestingly though, if one tries to print the addresses of both variables (the one inside the parent and the one inside the child), they print the same address! This is because we are only able to view the logical address, not the physical address. I suspect this may be due to the way the TLB handles addressing, though further analysis is necessary.

    Implementation: [address-space-verify.c](address-space-verify.c)

    Output:
    ```
    3984 is the PID of the current process.
    Inside the child process, the variable i has been incremented.

    3984 is the PID of the current process.
    Child created successfully.
    Child terminated successfully.
    After termination of the child process, the value of i is 0.
    ```

2. The objective of this question is to create a process tree with each process having only one child. This is accomplished by a simple check on the return value of the `fork()` call. To make sure the processes are not orphaned and belong to a single tree, we simply add a `wait()` call to the respective parent process.

    Note: The looping necessary to achieve the n-sized process tree introduces an unnnecessary overhead. I tried mitigating this by introducing a break, but I am unable to accurately test the performance.

    Implementation: [skewed-process-tree.c](skewed-process-tree.c)

    Output:
    ```
    Enter a value for n: 4

    1969 has created a child process 5139.

    5139 has created a child process 5140.

    5140 has created a child process 5141.

    5141 has created a child process 5142.
    ```