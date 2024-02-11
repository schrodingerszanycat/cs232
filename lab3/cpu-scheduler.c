#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_PROCESS 4

int current_time;

void ReadProcessTable(char * filename);
void PrintProcessTable();
void PrintStatistics();

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

    char process_id[MAX_PROCESS];
    char arrival_time[MAX_PROCESS];
    char burst_time[MAX_PROCESS];

    printf("I'm in ReadProcessTable!\n\n");

    int j = 0;
    while (fscanf(f, "%s %s %s ", process_id, arrival_time, burst_time) == 3)
    {
        Process *new_process = (Process *)malloc(sizeof(Process));
        new_process->name = strdup(process_id);
        new_process->arrival = atoi(arrival_time);
        new_process->cpuburst = atoi(burst_time);
        new_process->turnaround = 0;
        new_process->wait = 0;
        processtable[j] = *new_process;
        free(new_process);
        j++;
    }
    printf("Reading completed!\n");
    fclose(f);
}

void PrintProcessTable()
{
    printf("I'm in PrintProcessTable!\n\n");
    for(int i = 0; i < MAX_PROCESS; i++)
    {
        printf("%s %d %d\n", processtable[i].name, processtable[i].arrival, processtable[i].cpuburst);
    }
}

void PrintStatistics()
{
    int sum_turnaround = 0;
    int sum_wait = 0;
    printf("Turnaround times: ");
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (i == MAX_PROCESS-1)
        {
            printf("%s[%d] ", processtable[i].name, processtable[i].turnaround);
            sum_turnaround += processtable[i].turnaround;
            break;
        }
        printf("%s[%d], ", processtable[i].name, processtable[i].turnaround);
        sum_turnaround += processtable[i].turnaround;
    }
    printf("\n");
    printf("Wait times:       ");
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (i == MAX_PROCESS-1)
        {
            printf("%s[%d] ", processtable[i].name, processtable[i].wait);
            sum_wait += processtable[i].wait;
            break;
        }
        printf("%s[%d], ", processtable[i].name, processtable[i].wait);
        sum_wait += processtable[i].wait;
    }
    printf("\n\n");
    printf("Average turnaround time: %0.2f\n", (double) sum_turnaround/MAX_PROCESS);
    printf("Average wait time: %0.2f\n", (double) sum_wait/MAX_PROCESS);
}

void FCFS()
{
    printf("-------------------------------------------------\n");
    printf("       First Come First Served Scheduling        \n");
    printf("-------------------------------------------------\n");
    current_time = 0;
    int start_time = 0;

    for(int i = 0; i < MAX_PROCESS; i++)
    {
        int tmp = 0;
        while (tmp != processtable[i].cpuburst)
        {
            tmp++;
            current_time++;
        }
        printf("[%d-%d]    %s running\n", start_time, current_time, processtable[i].name);
        processtable[i].turnaround = current_time - processtable[i].arrival;
        processtable[i].wait = start_time - processtable[i].arrival;
        start_time = current_time;
    }
    printf("\n");
    PrintStatistics();
    printf("\n");
    // printf("Hit any key to continue...\n");
}

void runMenu()
{
    int choice = -1;
    while(choice != 4)
    {
        printf("-------------------------------------------------\n");
        printf("            CPU Scheduling Simulation            \n");
        printf("-------------------------------------------------\n");
        printf("Select the scheduling algorithm [1, 2, 3, or 4]:\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. SRBF\n");
        printf("4. Exit\n\n");

        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                FCFS();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;                
        }
    }
    return;
}

int main(int)
{
    ReadProcessTable("process.txt");
    PrintProcessTable();
    runMenu();
    return 0;
}