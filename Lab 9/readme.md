# Lab 9

## Assignments

1. `main()` creates two threads. Thread 1 deducts 100 from balance, while thread 2 deducts 200 from balance. Make sure there is a sleep of 1s before the printing. Apply Peterson's solution to solve this critical section problem. Create two functions `peterson_entry()` and `peterson_exit()` which should be called by both threads, which must contain the critical section code.

2. Perform merge sort using threading.