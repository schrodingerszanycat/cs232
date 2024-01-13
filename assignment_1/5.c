#include <sys/types.h>
#include <fcntl.h>

int main()
{
    if (argc < 3)
    {
        fprintf(stderr, "%s\n", "Too few arguments...");
        exit(1);
    }

    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1)
    {
        fprintf(stderr, "%s\n", "ERROR: File does not exist.");
        exit(1);
    }

    fd_out = creat(argv[2], 0644);
    if (fd_out == -1)
    {
        fprintf(stderr, "%s\n", "ERROR: Fiole could not be created.");
        exit(1);
    }
    
    close(0);
    if ((n = dup(fd_in)) == -1)
    {
        perror("A problem has occurred.\n");
    }
    
    close(1);
    if ((n = dup(fd_out)) == -1)
    {
        perror("A problem has occurred.\n");
    }

    int p[2];
    if (pipe(p) == -1)
    {
        perror("Error creating pipe.");
    }

    pid_t 

    return 0;
}