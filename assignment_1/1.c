#include <stdio.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) 
    {
        if (argv[i][0] == '-') continue;
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}