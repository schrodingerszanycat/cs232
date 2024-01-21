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
                fprintf(stderr, "Process %d: Invalid value of n provided. Exiting.", getpid());
                exit(2);
            }
        }
    }
    int c = 007;
    int count = 0;
    int temp = 0;
    // fprintf(stdout, "%s", "Use CTRL + D to terminate program and print result\n");
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
        temp++;
        if(n != 0 && temp == n)
            break;
    }
    fprintf(stderr, "Count : %d\n", count); // includes newlines and eof
    exit(0);
}