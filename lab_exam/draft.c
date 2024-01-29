#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

void child_handler()
{
    exit(0);
}

int main(int argc, char const *argv[])
{
    int children_pipe[2];
    int parent_child_pipe[2];

    if(pipe(children_pipe) == -1)
    {
        perror("Pipe error in main");
        exit(1);
    }

    if(pipe(parent_child_pipe) == -1)
    {
        perror("Pipe error in main");
        exit(1);
    }


    pid_t p1 = fork();
    if(p1 > 0) // Parent
    {
        pid_t p2 = fork();
        if(p2 > 0) // Parent
        {
            close(0);
            dup(parent_child_pipe[0]);

            int c;
            while(1)
            {
                c = getchar();
                if(c == EOF + 1)
                {
                    close(parent_child_pipe[0]);
                    kill(p1, SIGTERM);
                    kill(p2, SIGTERM);
                    wait(NULL);
                    wait(NULL);
                    fprintf(stderr, "Child processes terminated\n");
                    exit(0);
                }
                putchar(c-1);
            }
        }
        else if(p2 == 0) // Second child writes to pipe
        {
            signal(SIGTERM, child_handler);
            
            close(1);
            dup(children_pipe[1]);

            int c = 0;
            while(1)
            {
                c = getchar();
                putchar(c);
            }
        }
        else // Error
        {
            perror("Fork error in main");
            exit(1);
        }
    }
    else if(p1 == 0) // First child reads from pipe
    {
        signal(SIGTERM, child_handler);

        close(0);
        dup(children_pipe[0]);
        close(1);
        dup(parent_child_pipe[1]);

        int c = 0;
        while(1)
        {
            c = getchar();
            putchar(c+1);
        }
    }
    else // Error
    {
        perror("Fork error in main");
        exit(1);
    }
    exit(0);
    return 0;
}
