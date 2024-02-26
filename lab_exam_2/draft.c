#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>

jmp_buf env;

void handler()
{
    longjmp(env, 2);
}

bool is_prime(int n)
{
    if(n == 1)
        return false;
    for(int i = 2; i <= sqrt(n); i++)
    {
        if(n%i == 0)
            return false;
    }
    return true;
}

int main()
{
    int x, y, z;
    scanf("%d %d %d", &x, &y, &z);

    pid_t p1 = fork();
    if(p1 > 0)
    {
        int sj = setjmp(env);
        if(sj == 0)
        {
            signal(SIGALRM, handler);
            alarm(z);
            while(wait(NULL) > 0)
            {
                printf("Child process reaped in time.\n");
            }
            alarm(0);
            longjmp(env, 1);
        }
        else if(sj == 1)
        {
            printf("All the prime numbers printed.\n");
            exit(0);
        }
        else if(sj == 2)
        {
            printf("Calculation is taking too much time.\n");
            printf("Killing child process.\n");
            kill(p1, SIGKILL);
            wait(NULL);
            printf("Child killed.\n");
            exit(1);
        }
    }
    else if(p1 == 0)
    {
        for(int i = x; i <= y; i++)
        {
            if(is_prime(i))
                printf("%d\n", i);
        }
        exit(0);
    }
    else
    {
        printf("Fork error.\n");
    }
}
