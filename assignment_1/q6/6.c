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

int main(int argc, char *argv[]) 
{
    int nid = -1;
    if (argc >= 2 && argv[1][0] == '-')
    {
        nid = 1;
    }
    
    int file1_id = -1;
    if (nid == 1) 
    {
        file1_id = 2;
    }
    else 
    {
        file1_id = 1;
    }

    int fd_1 = -1;
    if (file1_id != -1 && file1_id < argc)
    {
        fd_1 = open(argv[file1_id], O_RDONLY);
        if (fd_1 == -1)
        {
            perror("Error opening file.");
            exit(1);
        }
    }
    if(fd_1 != -1)
    {
        close(0);
        if (dup(fd_1) == -1)
        {
            perror("Error duping. 1");
            exit(1);
        }
    }

    int file2_id = -1;
    int fd_2 = -1;
    if (file1_id + 1 < argc && fd_1 != -1)
    {
        file2_id = file1_id + 1;
        fd_2 = creat(argv[file2_id], 0644);
        if (fd_2 == -1)
        {
            perror("Error creating file.");
            exit(1);
        }
    }
    if(fd_1 != -1)
    {
        close(1);
        if (dup(fd_2) == -1)
        {
            perror("Error duping.");
            exit(1);
        }
    }

    // Creating the pipe
    int a[2];
    if (pipe(a) == -1)
    {
        perror("Pipe error.");
        exit(1);
    }

    pid_t p1 = fork();
    if (p1 < 0)
    {
        perror("Fork failure.");
        exit(1);
    }
    else if (p1 == 0) 
    {
        /* child, reading from pipe */
        fprintf(stderr, "P1 child process id: %d.\n", getpid());
        close(0);
        dup(a[0]);
        close(a[1]);
        signal(SIGTERM, child_termination_handler);
        if (nid != -1)
            execl("./count", "count", argv[nid], (char *) 0);
        else
            execl("./count", "count", (char *) 0);
        close(a[0]);
    }
    else  
    {
        pid_t p2 = fork();
        if (p2 < 0)
        {
            perror("Fork failure.");
            exit(1);
        }
        else if (p2 == 0)
        {
            /* child, writing to pipe */
            fprintf(stderr, "P2 child process id: %d.\n", getpid());
            close(1);
            dup(a[1]);
            close(a[0]);
            signal(SIGTERM, child_termination_handler);
            execl("./convert", "convert", (char *) 0);
            close(a[1]);
        }
        else 
        {
            close(a[1]);
            close(a[0]);
            alarm(15);
            int pid;
            int status;
            signal(SIGALRM, handler);

            // returns 0 only the first time its been called 
            if (setjmp(env) != 0)
            {
                fprintf(stderr, "Read timeout in second child, killing both children.");
                child_termination_handler(p1);
                child_termination_handler(p2);
                while ((pid = wait(&status)) > 0)
                {
                    fprintf(stderr, "Child 2 reaped with exit status: %d.\n", WEXITSTATUS(status));
                }
                exit(1);
            }

            while ((pid = wait(&status)) > 0)
            {
                fprintf(stderr, "Child pid = %d reaped with exit status: %d.\n", pid, WEXITSTATUS(status));
                
                if (pid == p1 && WEXITSTATUS(status) == 2)
                {   // if process p1 is dead, killing p2
                    fprintf(stderr, "Process %d exited with status %d. Killing process %d.", p1, WEXITSTATUS(status), p2);

                    //if process p2 is already dead 
                    if (kill(p2, SIGTERM) == -1)
                    {
                        fprintf(stderr, "Process %d already dead.", p2);
                    }
                    wait(&status);
                    exit(1);
                }
            }
            fprintf(stderr, "Normal children exit.\n");
            exit(0);
        }
    }
}
