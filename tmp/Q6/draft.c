#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX_PROCESS 4

int current_time;

void ReadProcessTable(char * filename);
void PrintProcessTable();
void PrintStatistics();
void runMenu();
void FCFS();
void RR(int quantum);

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

bool AllCompleted(int cpuburstcopy[MAX_PROCESS])
{
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (cpuburstcopy[i] != -1)
            return false;
    }
    return true;
}

void RR(int quantum)
{
    printf("-------------------------------------------------\n");
    printf("             Round Robin Scheduling              \n");
    printf("-------------------------------------------------\n");
    
    current_time = 0;
    int cpuburstcopy[MAX_PROCESS];
    int finishing_time[MAX_PROCESS];
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        cpuburstcopy[i] = 0;
        finishing_time[i] = 0;
    }

    int first = 0;
    int start_time;
    while (!AllCompleted(cpuburstcopy)) {

        if (cpuburstcopy[first] == -1)
        {
            first = (first + 1) % MAX_PROCESS;
            continue;
        }

        if (cpuburstcopy[first] == 0 && processtable[first].arrival <= current_time) 
        {
            cpuburstcopy[first] = processtable[first].cpuburst;
        }
        else if (cpuburstcopy[first] == 0 && processtable[first].arrival > current_time)
        {
            first = (first + 1) % MAX_PROCESS;
            continue;
        }
        
        start_time = current_time;
        if (cpuburstcopy[first] > 0) 
        {
            printf("[%d-", start_time);
            int tmp = 0;
            if ((cpuburstcopy[first] - quantum) <= 0)
            {
                tmp = cpuburstcopy[first];
                cpuburstcopy[first] = -1;
                current_time += tmp;
                finishing_time[first] = current_time;
                printf("%d]    %s running\n", current_time, processtable[first].name);
            } 
            else 
            {
                cpuburstcopy[first] -= quantum;
                current_time += quantum;
                printf("%d]    %s running\n", current_time, processtable[first].name);
            }
            first = (first + 1) % MAX_PROCESS;
            continue;
        }
    }

    for (int i = 0; i < MAX_PROCESS; i++)
    {
        processtable[i].turnaround = finishing_time[i]-processtable[i].arrival;
        processtable[i].wait = processtable[i].turnaround-processtable[i].cpuburst;
    }

    printf("\n");
    PrintStatistics();
    printf("\n");
    // printf("Hit any key to continue...\n");
}

int findIndex(int min_value, int cpuburstcopy[MAX_PROCESS])
{
    for(int i = 0; i < MAX_PROCESS; i++)
    {
        if (cpuburstcopy[i] == min_value)
            return i;
    }
    return -1;
}

int findBest(int cpuburstcopy[MAX_PROCESS])
{
    int min_value = INT_MAX;
    int min_index = -1;
    for(int i = 0; i < MAX_PROCESS; i++)
    {
        if (cpuburstcopy[i] == -1 || cpuburstcopy[i] == 0)
            continue;
        min_value = MIN(min_value, cpuburstcopy[i]);
        min_index = findIndex(min_value, cpuburstcopy);
    }
    return min_index;
}

void refurbish(int cpuburstcopy[MAX_PROCESS])
{
    for(int i = 0; i < MAX_PROCESS; i++)
    {
        if (cpuburstcopy[i] == 0 && processtable[i].arrival <= current_time) 
        {
            cpuburstcopy[i] = processtable[i].cpuburst;
        }
    }
}

void SRBF()
{
    printf("-------------------------------------------------\n");
    printf("         Shortest Remaining Burst First          \n");
    printf("-------------------------------------------------\n");
    current_time = 0;
    int finishing_time[MAX_PROCESS];
    int cpuburstcopy[MAX_PROCESS];
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        cpuburstcopy[i] = 0;
        finishing_time[i] = 0;
    }

    int best;
    int start_time;
    while (!AllCompleted(cpuburstcopy))
    {
        start_time = current_time;
        refurbish(cpuburstcopy);
        // for(int i = 0; i < MAX_PROCESS; i++)
        // {
        //     printf("%d ", cpuburstcopy[i]);
        // }
        //printf("\n");
        best = findBest(cpuburstcopy);
        cpuburstcopy[best] -= 1;
        current_time++;
        printf("")
        
        if (cpuburstcopy[best] == 0)
        {
            finishing_time[best] = current_time;
            cpuburstcopy[best] = -1;
        }      
    }

    // printf("\n\nFinishing times: ");
    // for (int i = 0; i < MAX_PROCESS; i++)
    // {
    //     printf("%d ", finishing_time[i]);
    // }
    // printf("\n");
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
                int quantum;
                printf("Enter the time quantum: ");
                scanf("%d", &quantum);
                printf("\n");
                RR(quantum);
                break;
            case 3:
                SRBF();
                break;
            case 4:
                break;                
        }
    }
    return;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: ./a.out <filename.txt>\n");
        return 0;
    }
    ReadProcessTable(argv[1]);
    PrintProcessTable();
    runMenu();
    return 0;
}