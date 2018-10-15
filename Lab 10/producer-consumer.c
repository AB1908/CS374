#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PRODUCE 1
#define CONSUME 0

/**
 * struct critical_shared_data - Contains critical data shared among threads.
 * @buffer: Pointer to array-based queue implementation used as buffer
 * @buffer_size: Trivial
 * @read_index: Index for a read operation from buffer
 * @write_index: Index for write operation to buffer
 * 
 * In context of the implementation, critical data refers to data which has to 
 * be synchronised before access to prevent race conditions from arising.
 */
struct critical_shared_data {
	int *buffer;
	int buffer_size;
	int read_index;
	int write_index;
	sem_t free_buffer;
	sem_t loaded_buffer;
	sem_t buffer_sem;
};

/**
 * struct thread_shared_data - Contains non-critical data shared among threads.
 * @item_read_count: Number of items read from buffer
 * @item_write_count: Number of items written to buffer
 * @operation_count: Number of operations to be performed by each thread
 */
struct thread_shared_data {
	int item_read_count;
	int item_write_count;
	int operation_count;
};
/**
 * struct thread_data - Contains data local to a thread.
 * @pthread_no: Trivial
 * @thread_id: To generate a human readable thread number
 * @is_producer: Trivial
 * @*crt_sh_data: To allow every thread to access the shared critical data
 * @*shared_data: To allow every thread to access the shared non-critical data
 * 
 * All the data pertaining to a thread has been encapsulated into a thread_data
 * struct. This allow every thread to keep its own data as necessary and also 
 * reference any shared data it requires while logically separating shared data
 * from the (thread's) local data.
 */
struct thread_data {
	pthread_t pthread_no;
	int thread_id;
	int is_producer;
	struct critical_shared_data *crt_sh_data;
	struct thread_shared_data *shared_data;
};

/**
 * delete_buffer_item() - Deletes buffer and updates appropriate index
 * @*cd: Pointer to critical data
 * 
 * delete_buffer_item() resets the item value pointed to by &cd->read_index 
 * and wraps it around to the next item in the buffer. 
 */
int delete_buffer_item(struct critical_shared_data *cd)
{
	cd->buffer[cd->read_index] = 0;
	cd->read_index = ++(cd->read_index) % cd->buffer_size;
	return 0;
}

/**
 * write_buffer_item() - Writes item to buffer and updates appropriate index
 * @*cd: Pointer to critical data
 * @item: Item to be written to buffer
 * 
 * @item is added to the buffer at &cd->write_index which is then wrapped
 * around to the next location in the buffer. 
 */
int add_buffer_item(struct critical_shared_data *cd, int item)
{
	cd->buffer[cd->write_index] = item;
	cd->write_index = ++(cd->write_index) % cd->buffer_size;
	return 0;
}

/**
 * get_buffer_item() - Obtain and print an item from the buffer.
 * @td: Pointer to thread data
 */
int get_buffer_item(struct thread_data *td)
{
	printf("\nItem %d was read from the buffer by Thread %d.\n", 
			++(td->shared_data->item_read_count), td->thread_id);
	delete_buffer_item(td->crt_sh_data);
	return 0;
}

/**
 * put_buffer_item() - Put an item in the buffer with a message.
 * @td: Pointer to thread data
 */
int put_buffer_item(struct thread_data *td)
{
	printf("\nItem %d was put in the buffer by Thread %d.\n", 
			++(td->shared_data->item_write_count), td->thread_id);
	add_buffer_item(td->crt_sh_data, td->shared_data->item_write_count);
	return 0;
}

/**
 * buffer_operation() - Perform a get or put operation on the buffer.
 * @td: Pointer to thread data
 * @to_produce: Flag to check whether an item is to be produced or not
 * 
 * The buffer semaphore is first locked to ensure mutually exclusive access to
 * the buffer. The appropriate action is performed on the buffer and the lock 
 * is subsequently released.
 */
int buffer_operation(struct thread_data *td, int to_produce)
{
	sem_wait(&(td->crt_sh_data->buffer_sem));
	if (to_produce)
		put_buffer_item(td);
	else
		get_buffer_item(td);
	sem_post(&(td->crt_sh_data->buffer_sem));
	return 0;
}

/**
 * producer() - Produce items to be read.
 * @td: Pointer to thread data
 * 
 * The &td->crt_sh_data->free_buffer is checked if there are any free buffers 
 * available for writing to. The lock is not released to ensure that other
 * threads do not assume that the position in the buffer is free. Instead, the 
 * &td->crt_sh_data->loaded_buffer is incremented to indicate that there is a 
 * buffer position occupied which can be read from.
 */
int producer(struct thread_data *td)
{
	int i;
	for (i = 0; i < td->shared_data->operation_count; i++) {
		sem_wait(&(td->crt_sh_data->free_buffer));
		buffer_operation(td, PRODUCE);
		sem_post(&(td->crt_sh_data->loaded_buffer));
	}
	return 0;
}

/**
 * consumer() - Consume items from the buffer
 * @td: Pointer to thread data
 * 
 * The &td->crt_sh_data->loaded_buffer is checked if there are any buffers 
 * available for reading from. The lock is not released to ensure that other
 * threads do not try reading the same item again. Instead, the 
 * &td->crt_sh_data->free_buffer is incremented to indicate that there is a 
 * buffer position available to be written to.
 */
int consumer(struct thread_data *td)
{
	int i;
	for (i = 0; i < td->shared_data->operation_count; i++)	{
		sem_wait(&(td->crt_sh_data->loaded_buffer));
		buffer_operation(td, CONSUME);
		sem_post(&(td->crt_sh_data->free_buffer));
	}
	return 0;
}

/**
 * thread_start() - To call the appropriate start routine for a thread
 * @arg: Pointer to a thread_data struct
 */
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

/**
 * main() initialises all necessary variables along with asking for the 
 * requrired input. These inputs are then assigned to the necessary structs
 * which are then used as arguments to for pthread_create(). After execution, 
 * cleanup is performed by waiting for the threads to join and then freeing the
 * previously allocated memory.
 */
int main()
{
	int i;
	int producer_number, consumer_number;
	int opno;
	int bufsize;
	struct critical_shared_data *cdata;
	struct thread_shared_data *tshared;
	struct thread_data **thread_count;
	
	printf("\nEnter number of producer threads to be created:");
	scanf("%d", &producer_number);
	printf("\nEnter number of consumer threads to be created:");
	scanf("%d", &consumer_number);
	printf("\nEnter size of buffer:");
	scanf("%d", &bufsize);
	printf("\nEnter number of operations to be performed per thread:");
	scanf("%d", &opno);

	tshared = malloc(sizeof(struct thread_data));
	thread_count = malloc((producer_number + consumer_number)*
					sizeof(struct thread_data *));
	cdata = malloc(sizeof(struct critical_shared_data));
	cdata->buffer = malloc(bufsize * sizeof(int));
	cdata->buffer = memset(cdata->buffer, 0, bufsize);
	cdata->read_index = 0;
	cdata->write_index = 0;
	cdata->buffer_size = bufsize;
	tshared->item_read_count = 0;
	tshared->item_write_count = 0;
	tshared->operation_count = opno;
	sem_init(&cdata->buffer_sem, 0, 1);
	sem_init(&cdata->loaded_buffer, 0, 0);
	sem_init(&cdata->free_buffer, 0, bufsize);

	for (i = 0; i < producer_number + consumer_number; i++) {
		thread_count[i] = malloc(sizeof(struct thread_data));
		thread_count[i]->crt_sh_data = cdata;
		thread_count[i]->shared_data = tshared;
		thread_count[i]->thread_id = i;
		thread_count[i]->is_producer = i < producer_number ? 1 : 0;
		pthread_create(&thread_count[i]->pthread_no, NULL, 
				&thread_start, (void *) thread_count[i]);
	}
	
	for (i = 0; i< producer_number + consumer_number; i++) {
		pthread_join(thread_count[i]->pthread_no, NULL);
	}
	for (i = 0; i< producer_number + consumer_number; i++) {
		free(thread_count[i]);
	}
	free(cdata);
	free(tshared);
	free(thread_count);
	return 0;
}