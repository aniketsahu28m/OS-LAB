#include <stdio.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int processes, resources;
int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

int safeSequence[MAX_PROCESSES];

void initialize() {
    printf("Enter number of processes: ");
    scanf("%d", &processes);

    printf("Enter number of resources: ");
    scanf("%d", &resources);

    printf("\nEnter available resources:\n");
    for(int i=0; i<resources; i++) {
        scanf("%d", &available[i]);
    }

    printf("\nEnter maximum matrix:\n");
    for(int i=0; i<processes; i++) {
        for(int j=0; j<resources; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    printf("\nEnter allocation matrix:\n");
    for(int i=0; i<processes; i++) {
        for(int j=0; j<resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

int isSafe() {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int count = 0;

    for(int i=0; i<resources; i++)
        work[i] = available[i];

    while(count < processes) {
        int found = 0;
        for(int p=0; p<processes; p++) {
            if(!finish[p]) {
                int j;
                for(j=0; j<resources; j++) {
                    if(need[p][j] > work[j])
                        break;
                }
                if(j == resources) {
                    for(int k=0; k<resources; k++)
                        work[k] += allocation[p][k];
                    safeSequence[count] = p;
                    finish[p] = 1;
                    count++;
                    found = 1;
                }
            }
        }
        if(!found) return 0;
    }
    return 1;
}

void resourceRequest(int process) {
    int request[MAX_RESOURCES];

    printf("\nEnter resource request for process %d:\n", process);
    for(int i=0; i<resources; i++) {
        scanf("%d", &request[i]);
    }

    // Check if request is valid
    for(int i=0; i<resources; i++) {
        if(request[i] > need[process][i]) {
            printf("Error: Request exceeds maximum claim\n");
            return;
        }
        if(request[i] > available[i]) {
            printf("Error: Resources not available\n");
            return;
        }
    }

    // Try to allocate
    for(int i=0; i<resources; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    if(isSafe()) {
        printf("Request granted\n");
    } else {
        printf("Request denied - unsafe state\n");
        // Rollback
        for(int i=0; i<resources; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
    }
}

int main() {
    initialize();

    if(isSafe()) {
        printf("\nSystem is in safe state.\nSafe sequence: ");
        for(int i=0; i<processes; i++)
            printf("P%d ", safeSequence[i]);
        printf("\n");

        int process;
        printf("\nEnter process number (0-%d) to request resources: ", processes-1);
        scanf("%d", &process);
        resourceRequest(process);
    } else {
        printf("\nSystem is not in safe state!\n");
    }

    return 0;
}
