#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler()
{
    fprintf(stderr, "Process %d: I've been killed because my pipe reader died!", getpid());
    exit(2);
}

int main()
{
    signal(SIGPIPE, handler);
    char buf[BUFSIZ];
    int size_read;
    while (1)
    {
        size_read = read(0, buf, BUFSIZ);
        for (int i = 0; i < size_read; i++)
        {
            if (islower(buf[i]))
            {
                buf[i] = toupper(buf[i]);
            }
            else if (isupper(buf[i]))
            {
                buf[i] = tolower(buf[i]);
            }
        }
        if (size_read == 0)
            exit(0);
        else if (size_read > 0)
            write(1, buf, size_read);
    }
    return 0;
}