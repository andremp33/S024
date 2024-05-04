#include "../include/task.h"
#include "../include/processor.h"
#include "../src/task.c"
#include "../src/processor.c"
#include "../src/status.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define SERVER_CLIENT_FIFO "/tmp/task_fifo"


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <output-path> <parallel_tasks>\n", argv[0]);
        return 1;
    }


    int max_parallel_tasks = atoi(argv[2]);
    if (max_parallel_tasks <= 0) {
        fprintf(stderr, "Invalid number of parallel tasks: %d\n", max_parallel_tasks);
        return 1;
    }


    char outputPath[256];
    strcpy(outputPath, argv[1]);
    char output_log_file_path[256];
    sprintf(output_log_file_path, "%s/Tasks.log", outputPath);
    int logFile_fd = open(output_log_file_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (logFile_fd == -1) {
        perror("Error opening/creating log file");
        return 1;
    }


    char header[] = "ID, Program, Arguments, Runtime\n";
    if (write(logFile_fd, header, strlen(header)) <= 0) {
        perror("Error writing to log file");
        return 1;
    }


    if (mkfifo(SERVER_CLIENT_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO");
        return 1;
    }


    printf("Server running...\n");
    scheduler_init(max_parallel_tasks);
    status_init(max_parallel_tasks); // Uncomment if status.h is available


    int num_tasks_executing = 0;
    Task task_read;
    int server_client_fifo_fd = open(SERVER_CLIENT_FIFO, O_RDONLY);
    if (server_client_fifo_fd == -1) {
        perror("Failed to open FIFO for reading");
        return 1;
    }


    while (read(server_client_fifo_fd, &task_read, sizeof(Task)) > 0) {
        if (strcmp(task_read.command, "status") == 0) {
                   printf("sss\n");
            // Handle status command
            char buffer[4096] = {'\0'};
                   printf("sss2\n");
            strcat(buffer, status_get_server_state());
                   printf("sss3\n"); // Implement status handling
            close(server_client_fifo_fd);
            server_client_fifo_fd = open(SERVER_CLIENT_FIFO, O_WRONLY);
            write(server_client_fifo_fd, buffer, strlen(buffer));
            close(server_client_fifo_fd);
            continue;
            
        }
        


        int id = get_next_task_id();  // This function needs to be implemented
        task_read.id = id;
        scheduler_add_task(task_read);  // This function needs to be implemented


        if (num_tasks_executing < max_parallel_tasks && !queue_empty()) {  // queue_empty needs to be defined
            Task task_executing = scheduler_get_task();  // This function needs to be implemented
            scheduler_remove_task(task_executing);  // This function needs to be implemented
            status_add_task(task_executing);  // This function needs to be implemented


            pid_t pid = fork();
            if (pid == 0) {
                // Child process to execute the task
                if (strcmp(task_executing.flag, "-u") == 0) {
                    engine_execute_task(task_executing, outputPath, logFile_fd);  // This function needs to be implemented
                }
                strcpy(task_executing.flag, "C");
                exit(0);
            }
            num_tasks_executing++;
        }
    }
    close(server_client_fifo_fd);
    return 0;
}
