#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

// https://i.imgur.com/pxDbqVa.jpg
// https://stackoverflow.com/questions/24793069/what-does-do-in-bash
// https://faculty.cs.niu.edu/~hutchins/csci480/signals.htm
// https://web.eecs.utk.edu/~huangj/cs360/360/notes/Setjmp/lecture.html
// https://stackoverflow.com/questions/61992736/what-is-exactly-wstatus-in-waitwstatus
// https://stackoverflow.com/questions/881388/what-is-the-reason-for-performing-a-double-fork-when-creating-a-daemon

jmp_buf env;

void handler()
{
    longjmp(env, 1);
    exit(1);
}

void child_handler()
{
    fprintf(stderr, "Process %d: I've been killed", getpid());
    exit(1);
}

int main(int argc, char *argv[])
{

    // Processing arguments
    int n_index = 1;
    for (; n_index < argc; n_index++)
    {
        if (argv[n_index][0] == '-')
        {
            break;
        }
    }

    if (argc >= 2)
    {
        int index = (n_index != 1) ? 1 : 2;
        if (index < argc)
        {
            int fd_1 = open(argv[index], O_RDONLY);
            if (fd_1 == -1)
            {
                fprintf(stderr, "errno=%d\n", errno);
                perror("open error in main");
                exit(1);
            }
            close(0);
            if (dup(fd_1) == -1)
            {
                fprintf(stderr, "errno=%d\n", errno);
                perror("dup error in main");
                exit(1);
            }
        }
    }

    if (argc >= 3)
    {
        int index = (n_index != 2) ? 2 : 3;
        if (index < argc)
        {
            int fd_2 = creat(argv[2], 0644);
            if (fd_2 == -1)
            {
                fprintf(stderr, "errno=%d\n", errno);
                perror("creat error in main");
                exit(1);
            }

            close(1);
            if (dup(fd_2) == -1)
            {
                fprintf(stderr, "errno=%d\n", errno);
                perror("dup error in main");
                exit(1);
            }
        }
    }

    // Creating the pipe
    int a[2];
    if (pipe(a) == -1)
    {
        fprintf(stderr, "errno = %d\n", errno);
        perror("pipe error in main");
        exit(1);
    }

    // Fork two processes
    pid_t p1 = fork();
    if (p1 > 0) // Parent
    {
        pid_t p2 = fork();
        if (p2 > 0) // Parent
        {
            close(a[1]);
            close(a[0]);

            int pid;
            int status;

            // SIGALRM handler
            signal(SIGALRM, handler);
            if (setjmp(env) != 0)
            {
                kill(p1, SIGTERM);
                kill(p2, SIGTERM);
                fprintf(stderr, "read timeout in second child, killing both children.");
                while ((pid = wait(&status)) > 0)
                {
                    fprintf(stderr, "child pid=%d reaped with exit status=%d\n",
                            pid, WEXITSTATUS(status));
                }
                exit(1);
            }

            alarm(15);
            while ((pid = wait(&status)) > 0)
            {
                fprintf(stderr, "child pid=%d reaped with exit status=%d\n",
                        pid, WEXITSTATUS(status));
                if (pid == p1 && WEXITSTATUS(status) == 2)
                {
                    fprintf(stderr,
                            "Process %d exited with status %d."
                            " Killing process %d.",
                            p1, WEXITSTATUS(status), p2);
                    if (kill(p2, SIGTERM) == -1)
                        fprintf(stderr, "Process %d is already dead.", p2);
                    wait(&status);
                    exit(1);
                }
            }
            alarm(0);
            fprintf(stderr, "Normal children exit\n");
            exit(0);
        }
        else if (p2 == 0) // Second child, writes to pipe
        {
            fprintf(stderr, "Second child's process id = %d\n", getpid());
            close(1);
            dup(a[1]);
            close(a[0]);
            signal(SIGTERM, child_handler);
            execl("./convert", "convert", (char *)NULL);
            close(a[1]);
        }
        else
        {
            fprintf(stderr, "errno = %d\n", errno);
            perror("fork fail in main");
            exit(1);
        }
    }
    else if (p1 == 0) // First child, reads from pipe
    {
        fprintf(stderr, "First child's process id = %d\n", getpid());
        close(0);
        dup(a[0]);
        close(a[1]);
        signal(SIGTERM, child_handler);
        if (n_index < argc)
            execl("./count", "count", argv[n_index], (char *)NULL);
        else
            execl("./count", "count", (char *)NULL);
        close(a[0]);
    }
    else
    {
        fprintf(stderr, "errno = %d\n", errno);
        perror("fork fail in main");
        exit(1);
    }

    return 0;
}