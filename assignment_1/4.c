#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int fd;
    fd = open("tmp.txt", O_RDWR);
    if (fd < 0) 
        fd = creat("tmp.txt", O_RDWR);

    char buf[BUFSIZ];
    int n;

    while ((n = read(0, buf, BUFSIZ)) > 0)
    {
        c = 
    }

    if (n < 0)
    {
        perror("Read error");
        return 1;
    }

    close(fd);
    return 0;
}
