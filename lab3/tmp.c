// Process *new_process = (Process *)malloc(sizeof(Process));

//         // Allocate memory for the name and copy the string
//         new_process->name = strdup(process_id);

//         new_process->arrival = atoi(arrival_time);
//         new_process->cpuburst = atoi(burst_time);
//         new_process->turnaround = 0;
//         new_process->wait = 0;

//         processtable[j] = *new_process;
//         free(new_process);
//         j++;

    // char p[MAX_PROCESS][MAX_PROCESS];
    // char a[MAX_PROCESS][MAX_PROCESS];
    // char b[MAX_PROCESS][MAX_PROCESS];


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
        print("[%d-%d]   %s running\n", start_time, current_time, processtable[i].name);
        start_time = current_time;
    }
}