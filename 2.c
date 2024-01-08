#include <stdio.h>
#include <ctype.h>

int main()
{
    int ch;
    int notCharCnt = 0;

    while(ch != EOF)
    {
        ch = getchar();

        if (!isalpha(ch))
            notCharCnt++;

        putchar(ch);
    }

    fprintf(stderr, "Non-character count: %d\n", notCharCnt);
    return 0;
}