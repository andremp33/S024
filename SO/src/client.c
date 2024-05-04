#include "../include/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#define SERVER_CLIENT_FIFO "/tmp/task_fifo"
#define BUFFER_SIZE 4096


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s execute <time_ms> -u|-p 'program [args...]' | status\n", argv[0]);
        return 1;
    }


    Task tarefa;
    memset(&tarefa, 0, sizeof(Task));  // Initialize the task structure to zero


    int server_client_fifo_fd;
    ssize_t bytes_written;


    if (strcmp(argv[1], "execute") == 0) {
        if (argc < 5 || (strcmp(argv[3], "-u") != 0 && strcmp(argv[3], "-p") != 0)) {
            printf("Usage: %s execute <time_ms> -u|-p 'program [args...]' \n", argv[0]);
            return 1;
        }


        // Parse the time and command
        tarefa.time = atol(argv[2]);
        strncpy(tarefa.program, argv[4], sizeof(tarefa.program) - 1);
        tarefa.program[sizeof(tarefa.program) - 1] = '\0';


        // Concatenate additional arguments if any
        for (int i = 5; i < argc; i++) {
            strncat(tarefa.program, " ", sizeof(tarefa.program) - strlen(tarefa.program) - 1);
            strncat(tarefa.program, argv[i], sizeof(tarefa.program) - strlen(tarefa.program) - 1);
        }


        // Flag for execution mode
        strncpy(tarefa.flag, argv[3], sizeof(tarefa.flag) - 1);
        tarefa.flag[sizeof(tarefa.flag) - 1] = '\0';


        // Open FIFO for writing
        server_client_fifo_fd = open(SERVER_CLIENT_FIFO, O_WRONLY);
        if (server_client_fifo_fd == -1) {
            perror("Failed to open FIFO for writing");
            return 1;
        }


        // Send the task structure
        bytes_written = write(server_client_fifo_fd, &tarefa, sizeof(Task));
        if (bytes_written <= 0) {
            perror("Failed to write to FIFO");
            close(server_client_fifo_fd);
            return 1;
        }


        close(server_client_fifo_fd);


    } else if (strcmp(argv[1], "status") == 0) {
        // Send status command
        strncpy(tarefa.command, "status", sizeof(tarefa.command) - 1);
        tarefa.command[sizeof(tarefa.command) - 1] = '\0'; // Ensure null-terminated


        server_client_fifo_fd = open(SERVER_CLIENT_FIFO, O_WRONLY);
        if (server_client_fifo_fd == -1) {
            perror("Failed to open FIFO for writing");
            return 1;
        }


        bytes_written = write(server_client_fifo_fd, &tarefa, sizeof(Task));
        if (bytes_written <= 0) {
            perror("Failed to write to FIFO");
            close(server_client_fifo_fd);
            return 1;
        }


        close(server_client_fifo_fd);


    } else if (strcmp(argv[1], "quit") == 0) {
        printf("Client quitting.\n");
        return 0;
    } else {
        printf("Unknown command. Use 'execute' or 'status'.\n");
        return 1;
    }


    return 0;
}
