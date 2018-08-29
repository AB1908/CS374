# Lab 4

## Assignment

1. Write a program to illustrate the use of pipe for inter-process communication. Consider a parent process **P** and its only child process **C**. **P** should write a message `Hello C_PID`. C should read and display the message. C then writes `Hello P_PID` which P should read and display. `P_PID` and `C_PID` are the PIDs of P and C respectively.

2. Extend the Skewed Process Tree program such that P<sub>1</sub> accepts an integer, increments it by one and writes it to a pipe. Process P<sub>2</sub> then reads the value written by its parent, increments it by one and writes it to a pipe. Subsequent children perform the same operation except for the last process. The last process reads from the pipe, increments it and then displays it on screen.

3. Implement a process that will fork two children: Child-1 will run `ls -l` command and send this as input to Child-2, which will run `wc`. Parent will wait for both child processes to terminate and exit. The output of `ls -l` should not be displayed on screen.

    Note: This is equivalent to running `ls -l | wc` in the shell.