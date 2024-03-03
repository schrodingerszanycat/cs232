#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#define N 5
#define LEFT (p + 4) % N
#define RIGHT (p + 1) % N

int philosophers[N];
char* state[N];

sem_t mutex;
sem_t S[N];

void test(int p)
{
	if (state[p] == "HUNGRY" && state[LEFT] != "EATING" && state[RIGHT] != "EATING") 
	{
		state[p] = "EATING";
		sleep(2);
		printf("Philosopher %d takes fork %d and %d\n", p + 1, LEFT + 1, p + 1);
		printf("Philosopher %d is Eating\n", p + 1);
		// sem_post(&S[p]) has no effect
		// during takefork
		// used to wake up hungry philosophers
		// during putfork
		sem_post(&S[p]);
	}
}

void take_fork(int p)
{
	sem_wait(&mutex);
	state[p] = "HUNGRY";
	printf("Philosopher %d is Hungry\n", p + 1);
	test(p);
	sem_post(&mutex);

	// if unable to eat wait to be signalled
	sem_wait(&S[p]);
	sleep(1);
}

void put_fork(int p)
{
	sem_wait(&mutex);
	state[p] = "THINKING";
	printf("Philosopher %d putting fork %d and %d down\n", p + 1, LEFT + 1, p + 1);
	printf("Philosopher %d is thinking\n", p + 1);
	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void* philosopher(void* num)
{
	while (1) 
	{
		int* i = num;
		sleep(1);
		take_fork(*i);
		sleep(0); // EAT
		put_fork(*i);
	}
}

void initialisation_code()
{
	sem_init(&mutex, 0, 1);
	for (int i = 0; i < N; i++)
	{
        philosophers[i] = i;
		sem_init(&S[i], 0, 0);
	}
}

int main()
{
    int i;
	pthread_t thread_id[N];
	initialisation_code();
	for (i = 0; i < N; i++) 
	{
		pthread_create(&thread_id[i], NULL, philosopher, &philosophers[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}
	for (i = 0; i < N; i++)
    {
		pthread_join(thread_id[i], NULL);
	}
}
