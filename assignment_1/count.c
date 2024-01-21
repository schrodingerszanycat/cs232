#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    fprintf(stdout, "%s\n");
    int count = 0;
    int c = getchar();

    while(c != EOF)
    {
        if (!isalpha(c))
            count++;

        putchar(c);
        c = getchar();
    }

    fprintf(stderr, "Non-character count: %d\n", count);
    exit(0);
}
