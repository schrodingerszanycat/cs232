#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int N_RESOURCES = 0;
int N_PROCESSES = 0;

bool safe_state(int *available, int **allocation, int **request, bool *finish);

void resolve_deadlock(int *available, int **allocation, int **request, bool *finish, int *deadlock_processes, int num_deadlock_processes);

void find_deadlock(int *instances, int **allocation, int **request);

void free_2d_array(int **array, int rows);

void run_menu();

int main() {
    run_menu();
    return 0;
}

bool safe_state(int *available, int **allocation, int **request, bool *finish) {
    int *new_available = malloc(N_RESOURCES * sizeof(int));
    int **new_allocation = malloc(N_PROCESSES * sizeof(int *));
    int **new_request = malloc(N_PROCESSES * sizeof(int *));
    bool *new_finish = malloc(N_PROCESSES * sizeof(bool));

    if (!new_available || !new_allocation || !new_request || !new_finish) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < N_PROCESSES; i++) {
        new_allocation[i] = malloc(N_RESOURCES * sizeof(int));
        new_request[i] = malloc(N_RESOURCES * sizeof(int));
        if (!new_allocation[i] || !new_request[i]) {
            printf("Memory allocation failed.\n");
            exit(1);
        }

        new_finish[i] = finish[i];
        for (int j = 0; j < N_RESOURCES; j++) {
            new_allocation[i][j] = allocation[i][j];
            new_request[i][j] = request[i][j];
        }
    }

    memcpy(new_available, available, N_RESOURCES * sizeof(int));

    while (true) {
        bool found = false;
        for (int i = 0; i < N_PROCESSES; ++i) {
            if (!new_finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < N_RESOURCES; ++j) {
                    if (new_request[i][j] > new_available[j]) {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {
                    for (int j = 0; j < N_RESOURCES; ++j) {
                        new_available[j] += new_allocation[i][j];
                    }
                    new_finish[i] = true;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            for (int i = 0; i < N_PROCESSES; ++i) {
                if (!new_finish[i]) {
                    free_2d_array(new_allocation, N_PROCESSES);
                    free_2d_array(new_request, N_PROCESSES);
                    free(new_available);
                    free(new_finish);
                    return false;
                }
            }
            free_2d_array(new_allocation, N_PROCESSES);
            free_2d_array(new_request, N_PROCESSES);
            free(new_available);
            free(new_finish);
            return true;
        }
    }
}

void resolve_deadlock(int *available, int **allocation, int **request, bool *finish,
                      int *deadlock_processes, int num_deadlock_processes) {
    int *max_requested = malloc(N_RESOURCES * sizeof(int));
    int *surplus = malloc(num_deadlock_processes * sizeof(int));

    if (!max_requested || !surplus) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    memset(max_requested, 0, N_RESOURCES * sizeof(int));

    for (int i = 0; i < num_deadlock_processes; i++) {
        for (int j = 0; j < N_RESOURCES; j++) {
            if (request[deadlock_processes[i]][j] > max_requested[j])
                max_requested[j] = request[deadlock_processes[i]][j];
        }
    }

    for (int i = 0; i < num_deadlock_processes; i++) {
        surplus[i] = 0;
        for (int j = 0; j < N_RESOURCES; j++) {
            if (max_requested[j] > allocation[deadlock_processes[i]][j])
                surplus[i] += allocation[deadlock_processes[i]][j];
            else
                surplus[i] = max_requested[j];
        }
    }

    for (int i = 0; i < num_deadlock_processes; i++) {
        int mini = i;
        for (int j = i + 1; j < num_deadlock_processes; j++) {
            if (surplus[j] > surplus[mini])
                mini = j;
        }
        if (mini != i) {
            int temp_surplus = surplus[i];
            int temp_i = deadlock_processes[i];
            surplus[i] = surplus[mini];
            deadlock_processes[i] = deadlock_processes[mini];
            surplus[mini] = temp_surplus;
            deadlock_processes[mini] = temp_i;
        }
    }

    for (int i = 0; i < num_deadlock_processes; i++) {
        for (int j = 0; j < N_RESOURCES; j++) {
            available[j] += allocation[deadlock_processes[i]][j];
        }
        finish[deadlock_processes[i]] = true;
        printf("%d ", deadlock_processes[i] + 1);
        if (safe_state(available, allocation, request, finish)) {
            free(max_requested);
            free(surplus);
            return;
        }
    }
    free(max_requested);
    free(surplus);
}

void find_deadlock(int *instances, int **allocation, int **request) {
    int *available = malloc(N_RESOURCES * sizeof(int));
    bool *finish = malloc(N_PROCESSES * sizeof(bool));
    int *deadlock_processes = malloc(N_PROCESSES * sizeof(int));

    if (!available || !finish || !deadlock_processes) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < N_RESOURCES; ++i) {
        available[i] = instances[i];
    }

    for (int i = 0; i < N_PROCESSES; i++) {
        for (int j = 0; j < N_RESOURCES; j++) {
            available[j] -= allocation[i][j];
            if (available[j] < 0) {
                printf("Error: Invalid input. More values allocated than possible with given resource instances\n");
                exit(1);
            }
        }
    }

    for (int i = 0; i < N_PROCESSES; ++i) {
        finish[i] = false;
    }

    while (true) {
        bool found = false;
        for (int i = 0; i < N_PROCESSES; ++i) {
            if (!finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < N_RESOURCES; ++j) {
                    if (request[i][j] > available[j]) {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {
                    for (int j = 0; j < N_RESOURCES; ++j) {
                        available[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            for (int i = 0; i < N_PROCESSES; ++i) {
                if (!finish[i]) {
                    deadlock_processes[i] = i;
                }
            }
            break;
        }
    }

    if (num_deadlock_processes == 0) {
        printf("No deadlock.\n");
    } else {
        printf("Deadlock exists. Terminate process(es): ");
        resolve_deadlock(available, allocation, request, finish, deadlock_processes, N_PROCESSES);
        printf("\n");
    }

    free(available);
    free(finish);
    free(deadlock_processes);
}

void run_menu() {
    while (true) {
        printf("Enter the number of resource types (-1 to exit):\n");
        scanf("%d", &N_RESOURCES);
        if (N_RESOURCES == -1)
            break;

        int *instances = malloc(N_RESOURCES * sizeof(int));
        if (!instances) {
            printf("Memory allocation failed.\n");
            exit(1);
        }

        printf("Enter the number of instances of each resource type\n");
        for (int i = 0; i < N_RESOURCES; i++) {
            scanf("%d", &instances[i]);
        }

        printf("Enter the number of processes\n");
        scanf("%d", &N_PROCESSES);

        int **allocation = malloc(N_PROCESSES * sizeof(int *));
        int **request = malloc(N_PROCESSES * sizeof(int *));
        if (!allocation || !request) {
            printf("Memory allocation failed.\n");
            exit(1);
        }

        printf("Enter the allocation matrix\n");
        for (int i = 0; i < N_PROCESSES; i++) {
            allocation[i] = malloc(N_RESOURCES * sizeof(int));
            if (!allocation[i]) {
                printf("Memory allocation failed.\n");
                exit(1);
            }
            for (int j = 0; j < N_RESOURCES; j++) {
                scanf("%d", &allocation[i][j]);
            }
        }

        printf("Enter the request matrix\n");
        for (int i = 0; i < N_PROCESSES; i++) {
            request[i] = malloc(N_RESOURCES * sizeof(int));
            if (!request[i]) {
                printf("Memory allocation failed.\n");
                exit(1);
            }
            for (int j = 0; j < N_RESOURCES; j++) {
                scanf("%d", &request[i][j]);
            }
        }

        find_deadlock(instances, allocation, request);

        free(instances);
        free_2d_array(allocation, N_PROCESSES);
        free_2d_array(request, N_PROCESSES);
    }
}

void free_2d_array(int **array, int rows) {
    if (array) {
        for (int i = 0; i < rows; i++) {
            free(array[i]);
        }
        free(array);
    }
}
