#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;

typedef struct node
{
	int data;
	struct node *next;
}node_t,*node_p,**node_pp;

node_p head = NULL;

static node_p alloc_node(int data)
{
	node_p temp = (node_p)malloc(sizeof(node_t));
	if(NULL == temp)
	{
		perror("malloc");
		return NULL;
	}
	temp->data = data;
	temp->next = NULL;
	return temp;
}

static void init(node_pp _list)
{
	node_p temp = alloc_node(0);
	if(NULL == temp)
	{
		return ;	
	}
	*_list = temp;
}

static void push_front(node_p _list,int data)
{
	node_p temp = alloc_node(data);

	node_p node = _list->next;

	_list->next = temp;
	temp->next = node;
}

static int is_empty_list(node_p _list)
{
//	if(_list->next == NULL)
//	{
//		return 1;
//	}
//	return 0;

	return (_list->next == NULL) ? 1 : 0;
}

static void pop_front(node_p _list, int *data)
{
	if(is_empty_list(_list))
	{
		return ;
	}

	node_p temp = _list->next;
	*data = temp->data;

	_list->next = temp->next;

	free(temp);
	temp = NULL;
}

void show_list(node_p _list)
{
	node_p start = _list->next;

	while(start)
	{
		printf("%d -> ",start->data);
		start = start->next;
	}
	printf("\n");
}

void *product(void *arg)
{
	int data = 0;
	while(1)
	{
		pthread_mutex_lock(&lock);

		data = rand()%11111;
		push_front(head,data);

		pthread_mutex_unlock(&lock);
		printf("producter done.... : %d \n",data);
		pthread_cond_signal(&cond);

		sleep(1);
	}
}

void *consumer(void *arg)
{
	int data  = 0;

	while(1)
	{
		pthread_mutex_lock(&lock);

		while(is_empty_list(head))
		{
			printf("consumer wait....  \n");
			pthread_cond_wait(&cond,&lock);
		}
		
		pop_front(head,&data);

		pthread_mutex_unlock(&lock);

		printf("consumer done.... : %d \n",data);
	}
}

int main()
{
	init(&head);
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cond,NULL);

	pthread_t th1 , th2;

	pthread_create(&th1,NULL,product,NULL);
	pthread_create(&th2,NULL,consumer,NULL);

	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);


//    int i = 0;
//
//	for (i = 0; i < 10; ++i)
//	{
//		push_front(&head,i);
//		show_list(head);
//		sleep(1);
//	}
//
//	int data = 0;
//
//	for (i = 0; i < 10; ++i)
//	{
//		pop_front(&head,&data);
//		show_list(head);
//		sleep(1);
//	}

	return 0;
}




