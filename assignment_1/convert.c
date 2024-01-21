#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    fprintf(stdout, "\n");
    int ch = getchar();

    while(ch != EOF)
    {
        if (islower(ch))
        {
            putchar(toupper(ch));
        }
        else if (isupper(ch))
        {
            putchar(tolower(ch));
        }
        else
        {
            putchar(ch); 
        }      
        ch = getchar(); 
    }
    exit(0);
}

