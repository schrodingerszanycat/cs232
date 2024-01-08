#include <stdio.h>
#include <ctype.h>

int main()
{
    int ch = getchar();
    int notCharCnt = 0;

    while(ch != EOF)
    {
        if (!isalpha(ch))
            notCharCnt++;

        putchar(ch);
        ch = getchar();
    }

    fprintf(stderr, "Non-character count: %d\n", notCharCnt);
    return 0;
}
