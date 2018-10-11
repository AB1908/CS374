# Lab 10

## Using Semaphores in C

- Operations on shared data require synchronised access to the shared variable(s), so we use semaphores.
- `sem_t` is the semaphore type of variable in C. Remember to include appropriate header.
- Initialise the semaphore object using `sem_init()`. The function has a flag `pshared` to specify whether the semaphore is shared among the child processes.
- Ex: `sem_init(&mutex, 0, 10)` assigns an initial value of 10 to a semaphore object named mutex.
- Wait on a semaphore using `sem_wait()`. Increment using `sem_post()`.
- Find and destroy functions are also useful. For more info, look up the man pages of the semaphore header.

## Assignments

1. Bounded buffer problem. Main thread creates 3 producer threads and 3 consumer threads. Fix the buffer size as 5. Producer thread produces 4 items into the shared buffer. Each consumer thread consumes 4 items from the shared buffer. After any operation by a thread, it sleeps for 1s.

2. Dining philosophers problem with 5 philosophers and 5 chopsticks. An odd philosopher picks up their left chopstick first and then right chopstick. An even philosopher picks up the right chopstick first and then the left chopstick.