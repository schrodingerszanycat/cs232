#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

jmp_buf env;

void handler() {
    longjmp(env, 1);
    exit(1);
}

void child_termination_handler(int pid) {
    kill(pid, SIGTERM);
    fprintf(stderr, "Process %d: I've been killed", getpid());
    exit(1);
}

bool is_prime(int n)
{
    if (n == 1)
        return false;
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    int x, y, z;
    printf("Enter x: \n");
    scanf("%d", &x);
    printf("Enter y: \n");
    scanf("%d", &y);
    printf("Enter z: \n");
    scanf("%d", &z);
    if (x > y)
    {
        printf("x should be less than y\n");
        exit(1);
    }

    int a[2];
    if (pipe(a) == -1)
    {
        perror("pipe error");
        exit(1);
    }

    pid_t p = fork();
    if (p < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if (p == 0)
    {
        for(int j = x; j <= y; j++)
        {
            if (is_prime(j))
                printf("%d ", j);
        }
        exit(0);
    }
    else 
    {
        alarm(z);
        signal(SIGALRM, handler);
        int s = setjmp(env);
        if(s == 0)
        {
            wait(NULL);
            longjmp(env, 1);
        }
        else if(s == 1)
        {
            printf("\nAll the prime numbers printed\n");
            exit(0);
        }
        else
        {
            fprintf(stderr, "\nCalculation is taking too much time");
            child_termination_handler(p);
        }
    }
    return 0;
}



























































// int main(int argc, char *argv[])
// {
//     int cc_pipe[2];
//     if (pipe(cc_pipe) == -1)
//     {
//         perror("Error creating children pipe.");
//         exit(1);
//     }

//     int cp_pipe[2];
//     if (pipe(cp_pipe) == -1)
//     {
//         perror("Error creating child-parent pipe.");
//         exit(1);
//     }

//     pid_t p1 = fork();
//     if (p1 < 0)
//     {
//         perror("Fork failure.");
//         exit(1);
//     }
//     else if (p1 == 0) // child
//     {
//         dup2(cc_pipe[0], 0);
//         dup2(cp_pipe[1], 1);
//         int ch = 0;
//         while(1)
//         {
//             ch = getchar();
//             putchar(ch + 1);
//         }
//     }
//     else // parent
//     {
//         pid_t p2 = fork();
//         if (p2 < 0)
//         {
//             perror("Fork failure.");
//             exit(1);
//         }
//         else if (p2 == 0) // child
//         {   
//             dup2(cc_pipe[1], 1);
//             int ch = 0;
//             fprintf(stderr, "%s\n", "Press Ctrl-D to enter EOF after entering characters.");
//             while(1)
//             {
//                 ch = getchar();
//                 putchar(ch);
//             }
//         }
//         else
//         {
//             dup2(cp_pipe[0], 0);
//             int ch = 0;
//             while(1) 
//             { 
//                 ch = getchar();
//                 if(ch == EOF + 1)  // because parent receives incremented
//                 {
//                     fprintf(stderr, "Child processes terminated.\n");
//                     exit(0);
//                 }
//                 putchar(ch - 1);
//             } 
//         }
//     }
//     return 0;
// }

