#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // This block is executed by the child process
        printf("Child Process:\n");
        printf("PID: %d\n", getpid());        // Process ID of the child
        printf("PPID: %d\n", getppid());      // Parent Process ID of the child
        printf("This is the child process!\n");
    } else {
        // This block is executed by the parent process
        printf("Parent Process:\n");
        printf("PID: %d\n", getpid());        // Process ID of the parent
        printf("PPID: %d\n", getppid());      // Parent Process ID of the parent (usually the init process if it's the root)
        printf("This is the parent process!\n");
    }

    return 0;
}
