#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    fd = open("tmp.text", "w+");
    if (fd < 0) 
        fd = creat("tmp.txt", "w+");
    int c = getchar();
    
}