# Solutions

1. The first question required that we create a child process and a parent process. We then needed to create a pipe where one process would write "Hello <PID of process reading>" to the pipe which would be read by the other process. This had to be done by both processes.

Initially, as 