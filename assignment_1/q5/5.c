#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "%s\n", "Too few arguments...");
        exit(1);
    }

    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1)
    {
        perror("ERROR: File does not exist.");
        exit(1);
    }

    int fd_out = creat(argv[2], 0644);
    if (fd_out == -1)
    {
        perror("ERROR: File could not be created.");
        exit(1);
    }

    close(0);
    if (dup(fd_in) == -1)
    {
        perror("A problem has occurred.");
        exit(1);
    }

    close(1);
    if (dup(fd_out) == -1)
    {
        perror("A problem has occurred.");
        exit(1);
    }

    int a[2];
    if (pipe(a) == -1)
    {
        perror("Error creating pipe.");
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
        close(0);
        dup(a[0]);
        close(a[1]);
        //execl("C:/Users/Asus/GitHub/schrodingerszanycat/cs232/assignment_1/count", "count", (char *) NULL);
        execl("./count", "count", (char *) 0);
        perror("Error in execl");
        exit(1);    
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
            /* child, writing onto pipe */
            close(1);
            dup(a[1]);
            close(a[0]);
            //execl("C:/Users/Asus/GitHub/schrodingerszanycat/cs232/assignment_1/convert", "convert", (char *) NULL);
            execl("./convert", "convert", (char *) 0);
            perror("Error in execl");
            exit(1);
            close(a[1]);
        }
        else
        {
            /* parent */
            close(a[1]);
            close(a[0]);
            int status;
            for (int i = 2; i >= 1; i--) 
            {
                if (wait(&status) >= 0)
                {
                    if (WIFEXITED(status))
                    {
                        fprintf(stderr, "Child process %d exited with %d status\n", i, WEXITSTATUS(status));
                    }
                }
            }
        }
    }
    exit(0);
    return 0;
}
