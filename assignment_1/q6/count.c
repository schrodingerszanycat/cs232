#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int n = 0;
    if (argc > 1)
    {
        if (argv[1][0] == '-')
        {
            n = atoi(argv[1] + 1);
            if (n <= 0)
            {
                fprintf(stderr, "Process %d: Invalid n. Exiting...", getpid());
                exit(2);
            }
        }
    }
    int c = 0;
    int count = 0;
    int tmp = 0;
    while ((c = getchar()) != EOF)
    {
        if (!isalpha(c))
        {    
            count++;
            putchar(c);
        }
        else
        {
            putchar(c);
        }
        tmp++;
        if(n != 0 && tmp == n)
            break;
    }
    fprintf(stderr, "Non-character count: %d\n", count);
    exit(0);
}