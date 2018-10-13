#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define pft printf("\nNo errors till now\n")
// #define pf() printf("")

struct thread_data {
	pthread_t pthread_no;
	int *buffer;
	int thread_id;
	int *buffer_end;
	int *buffer_size;
	int is_producer;
	sem_t *free_buffer;
	sem_t *loaded_buffer;
	sem_t *buffer_sem;
};

int get_buffer_item(struct thread_data *td)
{
	// pft;
	printf("\nItem %d was read from the buffer by Thread %d.\n", *td->buffer_end + 1, td->thread_id);
	// printf("\nbufend:%d\n", *td->buffer_end);
	td->buffer[*td->buffer_end]--;
	(*td->buffer_end)--;
	return 0;
}

int put_buffer_item(struct thread_data *td)
{
	// pft;
	printf("\nItem %d was put in the buffer by Thread %d.\n", *td->buffer_end + 1, td->thread_id);
	// printf("\nbufend:%d\n", *td->buffer_end);
	td->buffer[*td->buffer_end]++;
	(*td->buffer_end)++;
	return 0;
}

int producer(struct thread_data *td)
{
	int i;
	for (i = 0; i < 4; i++)
	{
	// pft;
		sem_wait(td->free_buffer);
		sem_wait(td->buffer_sem);
		put_buffer_item(td);
		sem_post(td->buffer_sem);
		sem_post(td->loaded_buffer);
	}
}

int consumer(struct thread_data *td)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		sem_wait(td->loaded_buffer);
		sem_wait(td->buffer_sem);
		get_buffer_item(td);
		sem_post(td->buffer_sem);
		sem_post(td->free_buffer);
	}
}

void *thread_start(void *arg)
{
	struct thread_data *td;
	td = (struct thread_data *) arg;
	if (td->is_producer)
		producer(td);
	else
		consumer(td);
}

int main()
{
	int producer_number, consumer_number;
	int bufsize;
	printf("\nEnter number of producer threads to be created:");
	scanf("%d", &producer_number);
	printf("\nEnter number of consumer threads to be created:");
	scanf("%d", &consumer_number);
	printf("\nEnter size of buffer:");
	scanf("%d", &bufsize);
	int i;

	int buf[bufsize];
	int bufend = 0;
	struct thread_data *thread_count[producer_number + consumer_number];
	sem_t fillbuf, emptybuf, buflock;
	sem_init(&buflock, 0, 1);
	sem_init(&fillbuf, 0, 0);
	sem_init(&emptybuf, 0, bufsize);
	for (i = 0; i < producer_number + consumer_number; i++) {
		thread_count[i] = malloc(sizeof(struct thread_data));
		thread_count[i]->buffer = buf; 
		thread_count[i]->buffer_end = &bufend;
		thread_count[i]->buffer_size = &bufsize;
		thread_count[i]->thread_id = i;
		thread_count[i]->free_buffer = &emptybuf;
		thread_count[i]->loaded_buffer = &fillbuf;
		thread_count[i]->buffer_sem = &buflock;
		thread_count[i]->is_producer = i < producer_number ? 1 : 0;
		pthread_create(&thread_count[i]->pthread_no, NULL, &thread_start, (void *) thread_count[i]);
	}
	for (i = 0; i< producer_number + consumer_number; i++) {
		pthread_join(thread_count[i]->pthread_no, NULL);
	}
}