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

//// STARTS HERE


void SRBF()
{
    printf("-------------------------------------------------\n");
    printf("         Shortest Remaining Burst First          \n");
    printf("-------------------------------------------------\n");
    current_time = 0;
    int finishing_time[MAX_PROCESS];
    // Readying the cpuburstcopy
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (cpuburstcopy[first] == 0 && processtable[first].arrival <= current_time) 
        {
            cpuburstcopy[first] = processtable[first].cpuburst;
        }
        finishing_time[i] = 0;
    }

    int best;
    int start_time;
    while (!AllCompleted(cpuburstcopy))
    {
        best = findBest(cpuburstcopy);
        cpuburstcopy[first] -= 1;
        current_time++;
        if (cpuburstcopy[first] == 0)
        {
            finishing_time[first] = current_time;
            cpuburstcopy[first] = -1;
        }
    }

    printf("\n\nFinishing times: ")
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        printf("%d ", finishing_time[i]);
    }
    printf("\n");
}

int findBest(int cpuburstcopy[MAX_PROCESS])
{
    int mini = INT_MAX;
    for(int i = 0; i < MAX_PROCESS; i++)
    {
        if (cpuburstcopy[i] == -1 || cpuburstcopy[i] == 0)
            continue;
        mini = MIN(mini, cpuburstcopy[i]);
    }
    return mini;
}