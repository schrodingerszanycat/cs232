#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFSIZ 1024

int main()
{
    int fd;
    fd = open("tmp.text", O_RDWR);
    if (fd < 0) 
        fd = creat("tmp.txt", O_RDWR);

    char buf[BUFSIZ];
    int n;
    while ((n = read(0, buf, BUFSIZ)) > 0)
    {
        
    }
    
    
}