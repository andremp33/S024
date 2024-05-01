
#include "../include/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FIFO_PATH "/tmp/task_fifo"

// Function to execute tasks using `execvp` instead of `system`
void executeTask(Task *task) {
    int status;
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return;
    } else if (pid == 0) {
        // Child process
        char output_filename[256];
        sprintf(output_filename, "task_output_%d.txt", task->id);
        int fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }

        // Redirect output to the file
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        // Prepare arguments for execvp
        char *args[64];
        char *context;
        int i = 0;
        args[i++] = strtok_r(task->progam, " ", &context);
        while (args[i - 1] != NULL && i < 63) {
            args[i++] = strtok_r(NULL, " ", &context);
        }
        args[i] = NULL;  // Terminate argument list

        // Execute the command
        execvp(args[0], args);
        perror("Execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process, wait for child to finish
        waitpid(pid, &status, 0);
        printf("Task ID %d completed with exit status %d\n", task->id, WEXITSTATUS(status));
    }
}

int main() {

    mkfifo(FIFO_PATH, 0666);
    int fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Failed to open FIFO");
        exit(EXIT_FAILURE);
    }

    Task task;
    while (read(fifo_fd, &task, sizeof(Task)) > 0) {
        close(fifo_fd);
        task.id = getpid(); // Assign a unique ID based on the PID
        printf("Received task ID %d for execution\n", task.id);
        executeTask(&task);
    }

    close(fifo_fd);
    unlink(FIFO_PATH);

    return 0;
}

