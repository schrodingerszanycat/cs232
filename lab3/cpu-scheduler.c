#include <stdio.h>
#define MAX_PROCESS 4

void ReadProcessTable(char * filename);

typedef struct {
    char * name;
    int arrival;
    int cpuburst;
    int turnaround;
    int wait;
} Process;

Process processtable[MAX_PROCESS];

void ReadProcessTable(char * filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("No such file.");
        return;
    }

    char process_id[MAX_PROCESS], arrival_time[MAX_PROCESS], burst_time[MAX_PROCESS]; 
    printf("I'm in ReadProcessTable!\n");
    while (fscanf(f, "%s %s %s ", process_id, arrival_time, burst_time) == 3)
        printf("%s %s %s\n", process_id, arrival_time, burst_time);

    for (int i = 0; i < MAX_PROCESS; i++)
        printf("%c", process_id[i]);
}

void runMenu()
{
    return;
}

int main(int)
{
    runMenu();
    ReadProcessTable("process.txt");
    return 0;
}