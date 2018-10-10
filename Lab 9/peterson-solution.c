#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct critical_data {
	int turn;
	int flag[2];
	int balance;
};

struct thread_data {
	pthread_t tid;
	int thread_num;
	int deduct;
	struct critical_data *cdata;
};

int peterson_entry(struct thread_data *tdata)
{
	int next_thread_id = (tdata->thread_num + 1) % 2;
	tdata->cdata->flag[tdata->thread_num] = 1;
	tdata->cdata->turn = next_thread_id;
	while(tdata->cdata->flag[next_thread_id] && tdata->cdata->turn == next_thread_id);
	return 0;
}

int peterson_exit(struct thread_data *tdata)
{
	tdata->cdata->flag[tdata->thread_num] = 0;
	return 0;
}

void *deduct(void *arg)
{
	struct thread_data *tdata;
	tdata = arg;
	peterson_entry(tdata);
	tdata->cdata->balance -= tdata->deduct;
	sleep(1);
	peterson_exit(tdata);
}

int main()
{
	int i;
	int balance;
	struct critical_data *cd;
	cd = malloc(sizeof(struct critical_data));
	cd->flag[0] = cd->flag[1] = 0;
	printf("\nEnter the initial balance (>= 300):");
	scanf("%d", &(cd->balance));
	struct thread_data *td[2];
	
	for(i = 0; i < 2; i++)
	{
		td[i] = malloc(sizeof(struct thread_data));
		td[i]->thread_num = i;
		td[i]->cdata = cd;
		printf("\nEnter the amount to deduct:");
		scanf("%d", &(td[i]->deduct));
		pthread_create(&(td[i]->tid), NULL, &deduct, (void *) td[i]);
	}
	for(i = 0; i < 2; i++)
	{
		pthread_join(td[i]->tid, NULL);
		printf("\nCurrent balance is: %d\n", td[i]->cdata->balance);
		free(td[i]);
	}	
	
	free(cd);
	return 0;
}