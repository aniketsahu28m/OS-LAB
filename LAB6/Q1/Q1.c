#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 20

// Structure to hold process information
typedef struct {
    int pid;            // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Burst time
    int priority;       // Priority (for non-preemptive priority scheduling)
    int remainingTime;  // Remaining burst time (for preemptive scheduling)
    int waitingTime;    // Waiting time
    int turnaroundTime; // Turnaround time
    int finishTime;     // Finish time (used to calculate waiting and turnaround)
} Process;

// Function prototypes
void preemptiveSJF(Process processes[], int n);
void roundRobin(Process processes[], int n, int quantum);
void nonPreemptivePriority(Process processes[], int n);
void calculateWaitingTime(Process processes[], int n);
void calculateTurnaroundTime(Process processes[], int n);
void display(Process processes[], int n);
void resetProcesses(Process processes[], int n);

int main() {
    Process processes[MAX];
    int choice, n, quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Taking input for processes
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("\nEnter arrival time, burst time, and priority for process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrivalTime);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;  // Initialize remaining time
        printf("Priority (lower number indicates higher priority): ");
        scanf("%d", &processes[i].priority);
    }

    do {
        printf("\nMenu:\n");
        printf("1. Preemptive Shortest Job First (SJF)\n");
        printf("2. Round Robin (RR)\n");
        printf("3. Non-Preemptive Priority Scheduling\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                resetProcesses(processes, n);  // Reset burst times and remaining times
                preemptiveSJF(processes, n);
                break;
            case 2:
                resetProcesses(processes, n);  // Reset burst times and remaining times
                printf("Enter time quantum for Round Robin: ");
                scanf("%d", &quantum);
                roundRobin(processes, n, quantum);
                break;
            case 3:
                resetProcesses(processes, n);  // Reset burst times and remaining times
                nonPreemptivePriority(processes, n);
                break;
            case 4:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 4);

    return 0;
}

// Preemptive Shortest Job First (SJF) Scheduling
void preemptiveSJF(Process processes[], int n) {
    int time = 0, completed = 0;
    int minBurstIndex;
    int completedProcesses = 0;

    // Sort processes by arrival time first
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    while (completed < n) {
        minBurstIndex = -1;

        // Find the process with the shortest remaining burst time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                if (minBurstIndex == -1 || processes[i].remainingTime < processes[minBurstIndex].remainingTime) {
                    minBurstIndex = i;
                }
            }
        }

        if (minBurstIndex != -1) {
            // Execute the process
            processes[minBurstIndex].remainingTime--;
            time++;

            if (processes[minBurstIndex].remainingTime == 0) {
                processes[minBurstIndex].finishTime = time;
                completed++;
            }
        } else {
            time++;
        }
    }

    // Calculate waiting time and turnaround time
    calculateWaitingTime(processes, n);
    calculateTurnaroundTime(processes, n);
    display(processes, n);
}

// Round Robin (RR) Scheduling
void roundRobin(Process processes[], int n, int quantum) {
    int time = 0, completed = 0;
    int i = 0;

    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    while (completed < n) {
        if (processes[i].remainingTime > 0) {
            if (processes[i].remainingTime > quantum) {
                processes[i].remainingTime -= quantum;
                time += quantum;
            } else {
                time += processes[i].remainingTime;
                processes[i].remainingTime = 0;
                processes[i].finishTime = time;
                completed++;
            }
        }
        i = (i + 1) % n; // Round-robin mechanism
    }

    // Calculate waiting time and turnaround time
    calculateWaitingTime(processes, n);
    calculateTurnaroundTime(processes, n);
    display(processes, n);
}

// Non-Preemptive Priority Scheduling
void nonPreemptivePriority(Process processes[], int n) {
    int time = 0, completed = 0, minPriorityIndex;

    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    while (completed < n) {
        minPriorityIndex = -1;

        // Find the process with the highest priority (lowest priority number)
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                if (minPriorityIndex == -1 || processes[i].priority < processes[minPriorityIndex].priority) {
                    minPriorityIndex = i;
                }
            }
        }

        if (minPriorityIndex != -1) {
            // Execute the process
            time += processes[minPriorityIndex].remainingTime;
            processes[minPriorityIndex].finishTime = time;
            processes[minPriorityIndex].remainingTime = 0;
            completed++;
        } else {
            time++;
        }
    }

    // Calculate waiting time and turnaround time
    calculateWaitingTime(processes, n);
    calculateTurnaroundTime(processes, n);
    display(processes, n);
}

// Calculate waiting time for each process
void calculateWaitingTime(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].waitingTime = processes[i].finishTime - processes[i].arrivalTime - processes[i].burstTime;
    }
}

// Calculate turnaround time for each process
void calculateTurnaroundTime(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
    }
}

// Display results
void display(Process processes[], int n) {
    printf("\nProcess\tArrival\tBurst\tWaiting\tTurnaround\tFinish\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", processes[i].pid, processes[i].arrivalTime, processes[i].burstTime, processes[i].waitingTime, processes[i].turnaroundTime, processes[i].finishTime);
    }
}

// Reset processes for each new algorithm run
void resetProcesses(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].remainingTime = processes[i].burstTime;  // Reset remaining time
        processes[i].waitingTime = 0;  // Reset waiting time
        processes[i].turnaroundTime = 0;  // Reset turnaround time
        processes[i].finishTime = 0;  // Reset finish time
    }
}
