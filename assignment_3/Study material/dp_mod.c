#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> // for rand()

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N
#define MAX_WAIT_TIME 5 // Maximum waiting time in seconds

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t S[N];

void test(int phnum)
{
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        // state that eating
        state[phnum] = EATING;

        sleep(2);

        printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);

        // sem_post(&S[phnum]) has no effect
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}

// take up chopsticks
void take_fork(int phnum)
{
    sem_wait(&mutex);

    // state that hungry
    state[phnum] = HUNGRY;

    printf("Philosopher %d is Hungry\n", phnum + 1);

    // eat if neighbors are not eating
    test(phnum);

    sem_post(&mutex);

    // if unable to eat wait to be signaled with timeout
    int wait_time = 0;
    while (state[phnum] == HUNGRY && wait_time < MAX_WAIT_TIME) {
        sem_wait(&S[phnum]);
        wait_time++;
    }

    if (state[phnum] == HUNGRY) {
        // Timeout reached, release the acquired chopsticks and enter thinking state again
        printf("Philosopher %d timed out and is putting fork %d and %d down\n",
               phnum + 1, LEFT + 1, phnum + 1);
        sem_post(&mutex);
        state[phnum] = THINKING;
    }

    sleep(1);
}

// put down chopsticks
void put_fork(int phnum)
{
    sem_wait(&mutex);

    // state that thinking
    state[phnum] = THINKING;

    printf("Philosopher %d putting fork %d and %d down\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT);
    test(RIGHT);

    sem_post(&mutex);
}

void* philosopher(void* num)
{
    while (1) {
        int* i = num;

        sleep(1);

        take_fork(*i);

        sleep(0); // EAT

        put_fork(*i);
    }
}

void initialization_code()
{
    // initialize the semaphores
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);
}

int main()
{
    int i;
    pthread_t thread_id[N];

    initialization_code();

    for (i = 0; i < N; i++) {
        // create philosopher processes
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    return 0;
}
