#include <stdio.h>
#include <ctype.h>

int main()
{
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
    return 0;
}

