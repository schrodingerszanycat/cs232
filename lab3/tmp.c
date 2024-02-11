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


// void FCFS()
// {
//     printf("-------------------------------------------------\n");
//     printf("       First Come First Served Scheduling        \n");
//     printf("-------------------------------------------------\n");
//     current_time = 0;
//     int start_time = 0;
//     for(int i = 0; i < MAX_PROCESS; i++)
//     {
//         int tmp = 0;
//         while (tmp != processtable[i].cpuburst)
//         {
//             tmp++;
//             current_time++;
//         }
//         print("[%d-%d]   %s running\n", start_time, current_time, processtable[i].name);
//         start_time = current_time;
//     }
// }

void RR(int quantum) {
    printf("-------------------------------------------------\n");
    printf("             Round Robin Scheduling              \n");
    printf("-------------------------------------------------\n");

    int current_time = 0;
    int cpuburstcopy[MAX_PROCESS];
    int finishing_time[MAX_PROCESS];
    bool allFinished = false;

    for (int i = 0; i < MAX_PROCESS; i++) {
        if (processtable[i].arrival > current_time) {
            cpuburstcopy[i] = 0;
            continue;
        }
        cpuburstcopy[i] = processtable[i].cpuburst;
    }

    int first = 0;

    while (!allFinished) {
        for (int i = 0; i < MAX_PROCESS; i++) {
            printf("%d ", cpuburstcopy[i]);

            if (cpuburstcopy[i] > 0) {
                cpuburstcopy[i] -= quantum;
                if (cpuburstcopy[i] <= 0) {
                    finishing_time[i] = current_time;
                }
            }

            current_time++;
            first = (first + 1) % MAX_PROCESS;
        }

        // Check if all processes are finished
        allFinished = true;
        for (int i = 0; i < MAX_PROCESS; i++) {
            if (cpuburstcopy[i] > 0) {
                allFinished = false;
                break;
            }
        }
    }
    printf("\n");
}

 // while(!allFinished)
    // {
    //     for(int i = 0; i < MAX_PROCESS; i++)
    //     {
    //         for(int i = 0; i < MAX_PROCESS; i++)
    //         {
    //             printf("%d ", cpuburstcopy[i]);
    //         }
    //         cpuburstcopy[i] -= 2;
    //         if (cpuburstcopy[i] == 0)
    //             finishing_time[i] = current_time; 
    //         first = (first + 1) % MAX_PROCESS;
    //         current_time++;
    //         for (int i = 0; i < MAX_PROCESS; i++)
    //         {
    //             if (cpuburstcopy[i] != 0)
    //                 break;
    //             if (i == MAX_PROCESS)
    //             {
    //                 allFinished = true;
    //                 break;
    //             }                               
    //         }
    //     }
    // }
    // printf("\n");

    // for (int i = 0; i < MAX_PROCESS; i++)
    // {
    //     if (processtable[i].arrival > current_time)
    //     {
    //         cpuburstcopy[i] = 0;
    //         continue;
    //     }
    //     cpuburstcopy[i] = processtable[i].cpuburst;
    // }

    // void RR(int quantum)
// {
//     printf("-------------------------------------------------\n");
//     printf("             Round Robin Scheduling              \n");
//     printf("-------------------------------------------------\n");
    
//     int current_time = 0;
//     int cpuburstcopy[MAX_PROCESS];
//     int finishing_time[MAX_PROCESS];
//     for (int i = 0; i < MAX_PROCESS; i++)
//     {
//         cpuburstcopy[i] = 0;
//         finishing_time[i] = 0;
//     }

//     int first = 0;
//     bool allFinished = false;
//     while (!allFinished) {
//         for (int i = 0; i < MAX_PROCESS; i++) 
//         {
//             printf("%d ", cpuburstcopy[i]);

//             if (cpuburstcopy[i] == 0 && processtable[i].arrival <= current_time) 
//             {
//                 cpuburstcopy[i] = processtable[i].cpuburst;
//             }
            
//             if (cpuburstcopy[i] > 0) 
//             {
//                 int tmp = 0;
//                 if (cpuburstcopy[i] - quantum < 0)
//                 {
//                     tmp = cpuburstcopy[i];
//                     cpuburstcopy[i] = -1;
//                     current_time += tmp;
//                     finishing_time[i] = current_time;
//                     break;
//                 } 
//                 cpuburstcopy[i] -= quantum;
//                 current_time += quantum;
//                 first = (first + 1) % MAX_PROCESS;
//             }
//         }

//         allFinished = true;
//         for (int i = 0; i < MAX_PROCESS; i++) {
//             if (cpuburstcopy[i] > 0) {
//                 allFinished = false;
//                 break;
//             }
//         }
//     }
//     printf("\n\nFinishing times: ");

//     for (int i = 0; i < MAX_PROCESS; i++)
//     {
//         printf("%d ", finishing_time[i]);
//     }
//     printf("\n");
// }