#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];
sem_t qmutex;

int queue[N];
int front = -1, rear = -1;

void enQueue(int phnum)
{
    sem_wait(&qmutex);
    if (front == -1)
        front = 0;
    rear = (rear + 1) % N;
    queue[rear] = phnum;
    sem_post(&qmutex);
}

int top()
{
    sem_wait(&qmutex);
    if (front == -1)
        return -1;
    int phnum;
    phnum = queue[front];
    sem_post(&qmutex);
    return phnum;
}

int deQueue()
{
    int phnum;
    sem_wait(&qmutex);
    phnum = queue[front];
    if (front == rear)
        front = rear = -1;
    else
        front = (front + 1) % N;
    sem_post(&qmutex);
    return phnum;
}

void test(int phnum)
{
    int q_front_ele = top();
    if ((q_front_ele == -1 || q_front_ele == phnum) && state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        deQueue();
        state[phnum] = EATING;
        sleep(2);
        // pri  ntf("Philosopher %d takes fork %d and %d\n",
        //        phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
        sem_post(&S[phnum]);
    }
    // else
    // {
    //     printf("%d waiting for %d\n", phnum + 1, q_front_ele + 1);
    //     if(phnum == q_front_ele)
    //     {
    //         printf("%d %d %d\n", state[phnum], state[LEFT], state[RIGHT]);
    //     }
    // }
}

void take_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = HUNGRY;
    // printf("Philosopher %d is Hungry\n", phnum + 1);
    test(phnum);
    sem_post(&mutex);
    sem_wait(&S[phnum]);
    sleep(1);
}

void put_fork(int phnum)
{
    sem_wait(&mutex);
    state[phnum] = THINKING;
    // printf("Philosopher %d putting fork %d and %d down\n",
    //        phnum + 1, LEFT + 1, phnum + 1);
    // printf("Philosopher %d is thinking\n", phnum + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void *philosopher(void *num)
{
    int *j = num;
    enQueue(*j);
    while (1)
    {
        int *i = num;
        sleep(1);
        take_fork(*i);
        sleep(1);
        put_fork(*i);
        enQueue(*i);
    }
}

int main()
{
    int i;
    pthread_t thread_id[N];

    sem_init(&mutex, 0, 1);
    sem_init(&qmutex, 0, 1);

    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++)
    {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        state[i] = THINKING;
        // printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}