#include "../include/processor.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h> // Para mkdir



void engine_execute_task(Task task, const char *outputPath, int logFile_fd) {
    char outputFilename[256];
    sprintf(outputFilename, "%s/task_output_%d.txt", outputPath, task.id);
    printf("Attempting to create output file at: %s\n", outputFilename);


    struct stat st;
    if (stat(outputPath, &st) == -1) {
        if (mkdir(outputPath, 0777) == -1) {
            perror("Failed to create output directory");
            return;  // Use return instead of exit to avoid stopping the server
        }
    }


    int outputFd = open(outputFilename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (outputFd == -1) {
        perror("Failed to open output file");
        return;  // Use return instead of exit to avoid stopping the server
    }


    pid_t pid = fork();
    if (pid == 0) {  // Child process
        dup2(outputFd, STDOUT_FILENO);
        dup2(outputFd, STDERR_FILENO);
        char *args[] = {task.program, NULL};
        execvp(task.program, args);
        perror("Execvp failed");
        exit(EXIT_FAILURE);
    } else {  // Parent process
        int status;
        waitpid(pid, &status, 0);
        close(outputFd);


        char logEntry[256];
        sprintf(logEntry, "Task %d completed with status %d\n", task.id, WEXITSTATUS(status));
        write(logFile_fd, logEntry, strlen(logEntry));
    }
}


