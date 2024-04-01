#include <stdio.h>
#include <stdbool.h>

#define MAX_RESOURCES 10
#define MAX_PROCESSES 10

void find_deadlock(int resource_types, int instances[], int allocation[][MAX_RESOURCES], int request[][MAX_RESOURCES])
{
    int available[MAX_RESOURCES];
    bool finish[MAX_PROCESSES];
    int deadlock_processes[MAX_PROCESSES];
    int num_deadlock_processes = 0;

    // Initialize available resources
    for (int i = 0; i < resource_types; ++i)
    {
        available[i] = instances[i];
    }

    // Initialize finish array
    for (int i = 0; i < MAX_PROCESSES; ++i)
    {
        finish[i] = false;
    }

    while (true)
    {
        bool found = false;
        for (int i = 0; i < MAX_PROCESSES; ++i)
        {
            if (!finish[i])
            {
                bool can_finish = true;
                for (int j = 0; j < resource_types; ++j)
                {
                    if (request[i][j] > available[j])
                    {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish)
                {
                    // Process can finish, release resources
                    for (int j = 0; j < resource_types; ++j)
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
            // No more processes can finish, deadlock exists
            for (int i = 0; i < MAX_PROCESSES; ++i)
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
        for (int i = 0; i < num_deadlock_processes; ++i)
        {
            printf("%d ", deadlock_processes[i]);
        }
        printf("\n");
    }
}

int main()
{
    int resource_types = 3;
    int instances[] = {3, 5, 2};
    int allocation[][MAX_RESOURCES] = {{0, 1, 0}, {1, 0, 2}, {1, 3, 1}, {0, 0, 1}, {4, 2, 3}};
    int request[][MAX_RESOURCES] = {{1, 1, 0}, {1, 2, 3}, {3, 1, 1}, {0, 0, 1}, {3, 2, 2}};

    find_deadlock(resource_types, instances, allocation, request);

    return 0;
}