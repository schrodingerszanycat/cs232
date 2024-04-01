#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int N_RESOURCES = 0;
int N_PROCESSES = 0;

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
        for (int i = 0; i < num_deadlock_processes; i++)
        {
            printf("%d ", deadlock_processes[i]);
        }
        printf("\n");
    }
}

void run_menu()
{
    while (true)
    {
        printf("Enter the number of resource types or -1 to exit.\n");
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
