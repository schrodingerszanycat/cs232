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

int main(int argc, char *argv[])
{
    int cc_pipe[2];
    if (pipe(cc_pipe) == -1)
    {
        perror("Error creating children pipe.");
        exit(1);
    }

    int cp_pipe[2];
    if (pipe(cp_pipe) == -1)
    {
        perror("Error creating child-parent pipe.");
        exit(1);
    }

    pid_t p1 = fork();
    if (p1 < 0)
    {
        perror("Fork failure.");
        exit(1);
    }
    else if (p1 == 0) // child
    {
        dup2(cc_pipe[0], 0);
        dup2(cp_pipe[1], 1);
        int ch = 0;
        while(1)
        {
            ch = getchar();
            putchar(ch + 1);
        }
    }
    else // parent
    {
        pid_t p2 = fork();
        if (p2 < 0)
        {
            perror("Fork failure.");
            exit(1);
        }
        else if (p2 == 0) // child
        {   
            dup2(cc_pipe[1], 1);
            int ch = 0;
            fprintf(stderr, "%s\n", "Press Ctrl-D to enter EOF after entering characters.");
            while(1)
            {
                ch = getchar();
                putchar(ch);
            }
        }
        else
        {
            dup2(cp_pipe[0], 0);
            int ch = 0;
            while(1) 
            { 
                ch = getchar();
                if(ch == EOF + 1)  // because parent receives incremented
                {
                    fprintf(stderr, "Child processes terminated.\n");
                    exit(0);
                }
                putchar(ch - 1);
            } 
        }
    }
    return 0;
}

