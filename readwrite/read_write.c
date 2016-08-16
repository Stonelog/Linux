#include <stdio.h>
#include <pthread.h>

pthread_rwlock_t lock;
int count = 0;

// int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);

void* read(void *arg)
{
	while(1)
	{
		if(pthread_rwlock_tryrdlock(&lock) != 0)
		{
			printf("writer is writing , reader wait....\n");
		}
		else
		{
			printf("reader is reading,count = %d ....done\n",count);
			pthread_rwlock_unlock(&lock);
		}
		sleep(2);
	}
}

void* write(void *arg)
{
	while(1)
	{
		if(pthread_rwlock_trywrlock(&lock) != 0)
		{
			printf("reader is reading , writer wait....\n");
		}
		else
		{
			++count;
			printf("write is reading....done\n");
			pthread_rwlock_unlock(&lock);
			sleep(1);
		}
	}
}

int main()
{
	pthread_t th1 , th2;

	pthread_rwlock_init(&lock,NULL);

	pthread_create(&th1,NULL,read,NULL);
	pthread_create(&th2,NULL,write,NULL);

	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	pthread_rwlock_destroy(&lock);

	return 0;
}


