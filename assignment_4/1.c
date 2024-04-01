#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int N_RESOURCES = 0;
int N_PROCESSES = 0;

bool safe_state(int available[], int allocation[][N_RESOURCES],
                int request[][N_RESOURCES], bool finish[])
{

    int new_available[N_RESOURCES];
    int new_allocation[N_PROCESSES][N_RESOURCES];
    int new_request[N_PROCESSES][N_RESOURCES];
    bool new_finish[N_PROCESSES];

    for (int i = 0; i < N_PROCESSES; i++)
    {
        new_finish[i] = finish[i];
        for (int j = 0; j < N_RESOURCES; j++)
        {
            new_allocation[i][j] = allocation[i][j];
            new_request[i][j] = request[i][j];
        }
    }

    for (int j = 0; j < N_RESOURCES; j++)
    {
        new_available[j] = available[j];
    }

    while (true)
    {
        bool found = false;
        for (int i = 0; i < N_PROCESSES; ++i)
        {
            if (!new_finish[i])
            {
                bool can_finish = true;
                for (int j = 0; j < N_RESOURCES; ++j)
                {
                    if (new_request[i][j] > new_available[j])
                    {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish)
                {
                    for (int j = 0; j < N_RESOURCES; ++j)
                    {
                        new_available[j] += new_allocation[i][j];
                    }
                    new_finish[i] = true;
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            for (int i = 0; i < N_PROCESSES; ++i)
            {
                if (!new_finish[i])
                {
                    return false;
                }
            }
            return true;
        }
    }
}

void resolve_deadlock(int available[], int allocation[][N_RESOURCES],
                      int request[][N_RESOURCES], bool finish[],
                      int deadlock_processes[], int num_deadlock_processes)
{
    int max_requested[N_RESOURCES];
    memset(max_requested, 0, N_RESOURCES * sizeof(int));
    for (int i = 0; i < num_deadlock_processes; i++)
    {
        for (int j = 0; j < N_RESOURCES; j++)
        {
            if (request[deadlock_processes[i]][j] > max_requested[j])
                max_requested[j] = request[deadlock_processes[i]][j];
        }
    }

    // Calculate surplus
    int surplus[num_deadlock_processes];
    memset(surplus, 0, sizeof(int) * num_deadlock_processes);
    for (int i = 0; i < num_deadlock_processes; i++)
    {
        for (int j = 0; j < N_RESOURCES; j++)
        {
            if (max_requested[j] > allocation[deadlock_processes[i]][j])
                surplus[i] += allocation[deadlock_processes[i]][j];
            else
                surplus[i] = max_requested[j];
        }
    }

    // Sort on the basis of surplus
    for (int i = 0; i < num_deadlock_processes; i++)
    {
        int mini = i;
        for (int j = i + 1; j < num_deadlock_processes; j++)
        {
            if (surplus[j] > surplus[mini])
                mini = j;
        }
        if (mini != i)
        {
            int temp_surplus = surplus[i];
            int temp_i = deadlock_processes[i];
            surplus[i] = surplus[mini];
            deadlock_processes[i] = deadlock_processes[mini];
            surplus[mini] = temp_surplus;
            deadlock_processes[mini] = temp_i;
        }
    }

    // Kill processes in the decreasing order of surplus
    for (int i = 0; i < num_deadlock_processes; i++)
    {
        for (int j = 0; j < N_RESOURCES; j++)
        {
            available[j] += allocation[deadlock_processes[i]][j];
        }
        finish[deadlock_processes[i]] = true;
        printf("%d ", deadlock_processes[i] + 1);
        if (safe_state(available, allocation, request, finish))
        {
            return;
        }
    }
}

void find_deadlock(int instances[], int allocation[][N_RESOURCES], int request[][N_RESOURCES])
{
    int available[N_RESOURCES];
    bool finish[N_PROCESSES];
    int deadlock_processes[N_PROCESSES];
    int num_deadlock_processes = 0;

    for (int i = 0; i < N_RESOURCES; ++i)
    {
        available[i] = instances[i];
    }

    for (int i = 0; i < N_PROCESSES; i++)
    {
        for (int j = 0; j < N_RESOURCES; j++)
        {
            available[j] -= allocation[i][j];
            if (available[j] < 0)
            {
                printf("Error: Invalid input. More values allocated than possible with given resource instances\n");
                exit(1);
            }
        }
    }

    for (int i = 0; i < N_PROCESSES; ++i)
    {
        finish[i] = false;
    }

    while (true)
    {
        bool found = false;
        for (int i = 0; i < N_PROCESSES; ++i)
        {
            if (!finish[i])
            {
                bool can_finish = true;
                for (int j = 0; j < N_RESOURCES; ++j)
                {
                    if (request[i][j] > available[j])
                    {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish)
                {
                    for (int j = 0; j < N_RESOURCES; ++j)
                    {
                        available[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            for (int i = 0; i < N_PROCESSES; ++i)
            {
                if (!finish[i])
                {
                    deadlock_processes[num_deadlock_processes++] = i;
                }
            }
            break;
        }
    }

    if (num_deadlock_processes == 0)
    {
        printf("No deadlock.\n");
    }
    else
    {
        printf("Deadlock exists. Terminate process(es): ");
        resolve_deadlock(available, allocation, request, finish,
                         deadlock_processes, num_deadlock_processes);
        printf("\n");
    }
}

void run_menu()
{
    while (true)
    {
        printf("Enter the number of resource types (-1 to exit):\n");
        scanf("%d", &N_RESOURCES);
        if (N_RESOURCES == -1)
            break;

        int instances[N_RESOURCES];
        printf("Enter the number of instances of each resource type\n");
        for (int i = 0; i < N_RESOURCES; i++)
        {
            scanf("%d", &instances[i]);
        }

        printf("Enter the number of processes\n");
        scanf("%d", &N_PROCESSES);

        int allocation[N_PROCESSES][N_RESOURCES];
        printf("Enter the allocation matrix\n");
        for (int i = 0; i < N_PROCESSES; i++)
        {
            for (int j = 0; j < N_RESOURCES; j++)
            {
                scanf("%d", &allocation[i][j]);
            }
        }

        int request[N_PROCESSES][N_RESOURCES];
        printf("Enter the request matrix\n");
        for (int i = 0; i < N_PROCESSES; i++)
        {
            for (int j = 0; j < N_RESOURCES; j++)
            {
                scanf("%d", &request[i][j]);
            }
        }

        find_deadlock(instances, allocation, request);
    }
}

int main()
{
    run_menu();
    return 0;
}