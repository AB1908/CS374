#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define produce 1
#define consume 0

struct critical_data {
	int *buffer;
	int buffer_size;
	int read_index;
	int write_index;
	sem_t *free_buffer;
	sem_t *loaded_buffer;
	sem_t *buffer_sem;
};

struct thread_data {
	pthread_t pthread_no;
	int thread_id;
	int is_producer;
	int *item_read_count;
	int *item_write_count;
	struct critical_data *crt_data;
};

int delete_buffer_item(struct critical_data *cd)
{
	cd->buffer[cd->read_index] = 0;
	cd->read_index = --(cd->read_index) % cd->buffer_size;
	return 0;
}

int write_buffer_item(struct critical_data *cd, int item)
{
	cd->buffer[cd->write_index] = item;
	cd->write_index = ++(cd->write_index) % cd->buffer_size;
	return 0;
}

int get_buffer_item(struct thread_data *td)
{
	printf("\nItem %d was read from the buffer by Thread %d.\n"
		, ++(*td->item_read_count), td->thread_id);
	delete_buffer_item(td->crt_data);
	return 0;
}

int put_buffer_item(struct thread_data *td)
{
	printf("\nItem %d was put in the buffer by Thread %d.\n"
		, ++(*td->item_write_count), td->thread_id);
	write_buffer_item(td->crt_data, *td->item_write_count);
	return 0;
}

int buffer_operation(struct thread_data *td, int prod)
{
	sem_wait(td->crt_data->buffer_sem);
	if (prod)
		put_buffer_item(td);
	else
		get_buffer_item(td);
	sem_post(td->crt_data->buffer_sem);
	return 0;
}

int producer(struct thread_data *td)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		sem_wait(td->crt_data->free_buffer);
		buffer_operation(td, produce);
		sem_post(td->crt_data->loaded_buffer);
	}
	return 0;
}

int consumer(struct thread_data *td)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		sem_wait(td->crt_data->loaded_buffer);
		buffer_operation(td, consume);
		sem_post(td->crt_data->free_buffer);
	}
	return 0;
}

void *thread_start(void *arg)
{
	struct thread_data *td;
	td = (struct thread_data *) arg;
	if (td->is_producer)
		producer(td);
	else
		consumer(td);
	return NULL;
}

int main()
{
	int producer_number, consumer_number;
	int bufsize;
	int i;
	int *buf; 
	int rdcnt = 0;
	int wrtcnt = 0;
	struct critical_data *cdata;
	sem_t fillbuf, emptybuf, buflock;
	
	printf("\nEnter number of producer threads to be created:");
	scanf("%d", &producer_number);
	printf("\nEnter number of consumer threads to be created:");
	scanf("%d", &consumer_number);
	printf("\nEnter size of buffer:");
	scanf("%d", &bufsize);

	struct thread_data *thread_count[producer_number + consumer_number];
	buf = malloc(bufsize * sizeof(int));
	buf = memset(buf, 0, bufsize);
	sem_init(&buflock, 0, 1);
	sem_init(&fillbuf, 0, 0);
	sem_init(&emptybuf, 0, bufsize);
	cdata = malloc(sizeof(struct critical_data));
	cdata->buffer = buf; 
	cdata->read_index = 0;
	cdata->write_index = 0;
	cdata->buffer_size = bufsize;
	cdata->free_buffer = &emptybuf;
	cdata->loaded_buffer = &fillbuf;
	cdata->buffer_sem = &buflock;

	for (i = 0; i < producer_number + consumer_number; i++) {
		thread_count[i] = malloc(sizeof(struct thread_data));
		thread_count[i]->crt_data = cdata;
		thread_count[i]->thread_id = i;
		thread_count[i]->is_producer = i < producer_number ? 1 : 0;
		thread_count[i]->item_read_count = &rdcnt;
		thread_count[i]->item_write_count = &wrtcnt;
		pthread_create(&thread_count[i]->pthread_no, NULL, &thread_start, (void *) thread_count[i]);
	}
	for (i = 0; i< producer_number + consumer_number; i++) {
		pthread_join(thread_count[i]->pthread_no, NULL);
	}
	return 0;
}