#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFSIZ 1024
#define MSGSIZE 100

char *msg = "This is a program demonstrating use of:";
char *func_msg = "1. open()\n2. creat()\n3. dup()\n4. dup2()\n5. pipe()\n6. read()\n7. write()";

int main() {
    
    char inbuf[MSGSIZE]; 
    int p[2]; 
  
    if (pipe(p) < 0) 
        exit(1); 
  
    write(p[1], msg, MSGSIZE); 
    write(p[1], func_msg, MSGSIZE); 
  
    for (i = 0; i < 2; i++) { 
        read(p[0], inbuf, MSGSIZE); 
        printf("%s\n", inbuf); 
    } 

    int fd;
    fd = open("./tmp.txt", O_RDWR | O_CREAT | O_APPEND);
    if (fd < 0) {
        perror("Error opening/creating file.");
        return 1;
    }

    int n;
    int buf[BUFSIZ];

    printf("Type something to write onto the file...\n");
    while ((n = read(0, buf, BUFSIZ)) > 0) {
        write(fd, buf, n);
    }

    if (n < 0) {
        perror("Read error.");
        return 1;
    }

    int copy_desc = dup(fd); 
    write(copy_desc,"This will be output to the file named tmp.txt\n", 46);       
    write(fd,"This will also be output to the file named tmp.txt\n", 51); 

    dup2(fd, 1) ;  
    printf("I am a print statement AND I will be printed in the
     file tmp.txt!\n"); 
    
    close(fd);
    return 0;
}



