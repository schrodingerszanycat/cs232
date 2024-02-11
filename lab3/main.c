#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESS 10
#define NAME_SIZE 5

typedef struct
{
    char *name;
    int arrival;
    int cpuburst;
    int turnaround;
    int wait;
} Process;

Process processtable[MAX_PROCESS];

void ReadProcessTable(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("fopen error with errno %d", errno);
        exit(1);
    }

    char name[NAME_SIZE];
    int arrival;
    int burst;
    int table_index = 0;
    while (table_index < MAX_PROCESS)
    {
        if (fscanf(fp, "%s %d %d", name, &arrival, &burst) != 3)
        {
            fclose(fp);
            return;
        }

        processtable[table_index].name = (char *)malloc(NAME_SIZE * sizeof(char));
        strcpy(processtable[table_index].name, name);

        processtable[table_index].arrival = arrival;
        processtable[table_index].cpuburst = burst;
        table_index++;
    }
}

void PrintProcessTable()
{
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (!processtable[i].name)
            break;
        printf("%s %d %d\n", processtable[i].name,
               processtable[i].arrival, processtable[i].cpuburst);
    }
}

void FCFS()
{
    printf("--------------------------------------------------\n");
    printf("\tFirst Come First Served Scheduling\n");
    printf("--------------------------------------------------\n");
    int current_time = 0;
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (!processtable[i].name)
            break;

        printf("[%d-", current_time);
        processtable[i].wait = current_time - processtable[i].arrival;
        current_time += processtable[i].cpuburst;
        processtable[i].turnaround = current_time - processtable[i].arrival;
        printf("%d] %s running\n", current_time, processtable[i].name);
    }
}

void RR(int quantum)
{
    printf("--------------------------------------\n");
    printf("\tRound Robin Scheduling\n");
    printf("--------------------------------------\n");
    int process_count = 0;
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (!processtable[i].name)
            break;
        process_count++;
    }
    int cpuburstcopy[process_count];
    for (int i = 0; i < process_count; i++)
    {
        cpuburstcopy[i] = 0;
    }
    int processes_reaped = 0;
    int current_time = 0;
    int processtable_index = 0;
    int active_process_count = 0;
    int executing_process_index = 0;
    while (processes_reaped != process_count)
    {
        while (processtable_index < process_count &&
               processtable[processtable_index].arrival <= current_time)
        {
            cpuburstcopy[processtable_index] = processtable[processtable_index].cpuburst;
            processtable_index++;
            active_process_count++;
        }
        if (active_process_count > 0)
        {
            while (cpuburstcopy[executing_process_index] <= 0)
            {
                if(processtable_index < process_count)
                    executing_process_index = (executing_process_index + 1) % (processtable_index + 1);
                else
                    executing_process_index = (executing_process_index + 1) % (process_count);
            }
            printf("[%d-", current_time);
            if (cpuburstcopy[executing_process_index] < quantum)
            {
                current_time += cpuburstcopy[executing_process_index];
                printf("%d] %s running\n", current_time, processtable[executing_process_index].name);
                processtable[executing_process_index].wait = current_time - processtable[executing_process_index].arrival - processtable[executing_process_index].cpuburst;
                processtable[executing_process_index].turnaround = current_time - processtable[executing_process_index].arrival;
                cpuburstcopy[executing_process_index] = 0;
                active_process_count--;
                executing_process_index = (executing_process_index + 1) % (process_count);
                processes_reaped++;
                continue;
            }
            else
            {
                printf("%d] %s running\n", current_time + quantum, processtable[executing_process_index].name);
                cpuburstcopy[executing_process_index] -= quantum;
                if (cpuburstcopy[executing_process_index] == 0)
                {
                    processtable[executing_process_index].wait = current_time + quantum - processtable[executing_process_index].arrival - processtable[executing_process_index].cpuburst;
                    processtable[executing_process_index].turnaround = current_time + quantum - processtable[executing_process_index].arrival;
                    active_process_count--;
                    processes_reaped++;
                }
                executing_process_index = (executing_process_index + 1) % (process_count);
            }
        }
        else
        {
            printf("%d] No process running\n", current_time + quantum);
        }
        current_time += quantum;
    }
}

void SRBF()
{
    printf("-------------------------------------------\n");
    printf(" Shortest Remaining Burst First Scheduling\n");
    printf("-------------------------------------------\n");

    int current_time = 0;
    int cpuburstcopy[MAX_PROCESS];
    int reaped[MAX_PROCESS] = {0};
    int process_count = 0;
    int processes_reaped = 0;

    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (!processtable[i].name)
            break;

        cpuburstcopy[i] = processtable[i].cpuburst;
        process_count++;
    }

    int executing_process_index = -1;
    int prev_executing_process_index = -1;

    while (1)
    {
        if(processes_reaped == process_count)
            break;

        int min_burst = INT_MAX;
        prev_executing_process_index = executing_process_index;
        executing_process_index = -1;

        for (int i = 0; i < MAX_PROCESS; i++)
        {
            if (!processtable[i].name)
                break;

            if (processtable[i].arrival <= current_time && cpuburstcopy[i] < min_burst && cpuburstcopy[i] > 0)
            {
                min_burst = cpuburstcopy[i];
                executing_process_index = i;
            }
        }

        if (executing_process_index == -1)
        {
            current_time++;
            continue;
        }

        if (executing_process_index != prev_executing_process_index)
            printf("[%d-", current_time);

        cpuburstcopy[executing_process_index]--;
        current_time++;

        if (cpuburstcopy[executing_process_index] == 0)
        {
            processtable[executing_process_index].turnaround = current_time - processtable[executing_process_index].arrival;
            processtable[executing_process_index].wait = processtable[executing_process_index].turnaround - processtable[executing_process_index].cpuburst;

            printf("%d] %s running\n", current_time, processtable[executing_process_index].name);
            reaped[executing_process_index] = 1;
            processes_reaped++;
        }

        for (int i = 0; i < MAX_PROCESS; i++)
        {
            if (!processtable[i].name)
                break;

            if (processtable[i].arrival == current_time && cpuburstcopy[i] < cpuburstcopy[executing_process_index] && !reaped[i])
            {
                printf("%d] %s running\n", current_time, processtable[executing_process_index].name);
            }
        }
    }
}

void PrintStatistics()
{
    printf("\n");
    int process_count = 0;
    int total_turnaround_time = 0;
    double average_turnaround_time;
    int total_wait_time = 0;
    double average_wait_time;
    printf("Turnaround times: ");
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (!processtable[i].name)
            break;
        if (!i)
            printf("%s[%d]", processtable[i].name, processtable[i].turnaround);
        else
            printf(", %s[%d]", processtable[i].name, processtable[i].turnaround);
        process_count++;
        total_turnaround_time += processtable[i].turnaround;
    }
    printf("\n");
    printf("Wait times: ");
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (!processtable[i].name)
            break;
        if (!i)
            printf("%s[%d]", processtable[i].name, processtable[i].wait);
        else
            printf(", %s[%d]", processtable[i].name, processtable[i].wait);
        total_wait_time += processtable[i].wait;
    }
    printf("\n\n");
    average_turnaround_time = ((double)total_turnaround_time) / process_count;
    average_wait_time = ((double)total_wait_time) / process_count;
    printf("Average turnaround time: %lf\n", average_turnaround_time);
    printf("Average wait time: %lf\n", average_wait_time);
    printf("\n");
}

void resetStatistics()
{
    for(int i = 0; i < MAX_PROCESS; i++)
    {
        processtable[i].turnaround = 0;
        processtable[i].wait = 0;
    }
}

void run_menu()
{
    int choice;
    do
    {
        printf("-------------------------------------------\n");
        printf("\t CPU Scheduling Simulation\n");
        printf("-------------------------------------------\n");
        printf("Select the scheduling algorithm [1,2,3 or 4]:\n");
        printf("1. First Come First Served (FCFS)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. SRBF\n");
        printf("4. Exit\n");
        printf("\n");

        scanf("%d", &choice);

        int quantum;
        switch (choice)
        {
        case 1:
            FCFS();
            PrintStatistics();
            break;
        case 2:
            printf("Enter the time quantum:\n");
            scanf("%d", &quantum);
            RR(quantum);
            PrintStatistics();
            break;
        case 3:
            SRBF();
            PrintStatistics();
            break;
        case 4:
            break;
        default:
            printf("Invalid input\n");
            break;
        }
        resetStatistics();
        // printf("Hit any key to continue...\n\n");
        // char c;
        // scanf("\n%c", &c);
    } while (choice != 4);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("File name must be passed.\n");
        exit(1);
    }

    ReadProcessTable(argv[1]);
    PrintProcessTable();
    run_menu();
    return 0;
}