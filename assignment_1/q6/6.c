#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>

sigjmp_buf env;

void alarm_handler(int signo) {
    fprintf(stderr, "Read timeout in second child, killing both children.\n");
    kill(0, SIGTERM);
    exit(1);
}

void child_termination_handler(int signo) {
    fprintf(stderr, "I've been killed\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    //signal(SIGALRM, alarm_handler);

    // Argument Parsing and Setup

    // Creating the pipe
    int a[2];
    if (pipe(a) == -1)
    {
        fprintf(stderr, "errno = %d\n", errno);
        perror("pipe error in main");
        exit(1);
    }

    // Save stack environment
    if (sigsetjmp(env, 1) != 0) {
        fprintf(stderr, "Abnormal child termination, killing the other child.\n");
        kill(0, SIGTERM);
        wait(NULL);
        exit(1);
    }

    // Set an alarm to go off in 15 seconds
    alarm(15);

    // Fork two children

    // Wait loop for both children

    // Handle exit conditions
    fprintf(stderr, "Normal children exit.\n");
    exit(0);
}

