#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    fork();

    fork() && fork() || fork();

    printf("hi\n");

    return 0;
}
