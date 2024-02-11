#include <stdio.h>

int main()
{
    int a = 5;
    while(1)
    {
        if (a == 5)
        {
            if (a % 5 == 0)
            {
                break;
            }
            printf("with inner if");
        }
        printf("still in while loop");
    }
    printf("OUT OF WHILE LOOP!");
    return 0;
}