#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include<sys/wait.h>

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
        fprintf(stderr, "%s\n", "ERROR: File does not exist.");
        exit(1);
    }

    int fd_out = creat(argv[2], 0644);
    if (fd_out == -1)
    {
        fprintf(stderr, "%s\n", "ERROR: Fiole could not be created.");
        exit(1);
    }
    
    int n;
    close(0);
    if ((n = dup(fd_in)) == -1)
    {
        perror("A problem has occurred.\n");
    }
    
    close(1);
    if ((n = dup(fd_out)) == -1)
    {
        perror("A problem has occurred.\n");
    }

    /*
       pipe_[0] will be the fd(file descriptor) for the 
       read end of pipe.
       pipe_[1] will be the fd for the write end of pipe.
       Returns : 0 on Success.
       -1 on error. 
    */

    int pipe_[2];
    if (pipe(pipe_) == -1)
    {
        perror("Error creating pipe.");
    }

    pid_t p1 = fork();
    if (p1 < 0) {
        perror("Fork failure.");
        exit(1);
    }
    else if (p1 == 0) {
        /* child */
        close(0);
        dup(pipe_[0]);
        close(pipe_[1]); // close the write end to pipe before read or else deadlock...
        execl("Users/Asus/GitHub/schrodingerszanycat/cs232/assignment_1/count", "count", (char *) 0);
        close(pipe_[0]); // close read end of pipe
    }    
    else {
        /* parent */
        pid_t p2 = fork(); // forking another child process
        if (p2 < 0) {
            perror("Fork failure.");
            exit(1);
        }
        else if (p2 == 0) {
            /* child */
            close(1);
            dup(pipe_[1]);
            close(pipe_[0]);
            execl("Users/Asus/GitHub/schrodingerszanycat/cs232/assignment_1/convert", "convert", (char *) 0);
            close(pipe_[1]); // close write end of pipe
        }
        else {
            /* parent */
            close(pipe_[1]);
            close(pipe_[0]);
            int status;
            if (wait(&status) >= 0)
            {
                if (WIFEXITED(status))
                    {
                    /* Child process exited normally, through `return` or `exit` */
                    printf("Child process 2 exited with %d status\n", WEXITSTATUS(status));
                }
            }
            if (wait(&status) >= 0)
            {
                if (WIFEXITED(status))
                    {
                    /* Child process exited normally, through `return` or `exit` */
                    printf("Child process 1 exited with %d status\n", WEXITSTATUS(status));
                }
            }
        }
    }
    return 0;
}