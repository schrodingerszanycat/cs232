#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZ 1024

int main() {
    int fd;

    fd = open("./tmp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Error opening/creating file");
        return 1;
    }

    char buf[BUFSIZ];
    int n;

    while ((n = read(0, buf, BUFSIZ)) > 0) {
        write(fd, buf, n);
    }

    if (n < 0) {
        perror("Read error");
        return 1;
    }

    close(fd);
    return 0;
}

