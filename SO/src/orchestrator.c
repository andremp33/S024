#include "../include/orchestrator.h"
#include "../include/task.h"
#include "../include/processor.h"
#include "../include/status.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>



#define SERVER_CLIENT_FIFO "/tmp/server_client_fifo"
#define ADMIN_PASSWORD "password"


int main(int argc, char** argv) {
    if(argc < 3){
        char error_message[100];
        snprintf(error_message, sizeof(error_message), "Use: %s <output-path> <parallel_tasks>\n", argv[0]);
        write(STDERR_FILENO, error_message, strlen(error_message));
        return 1;
    }
    int max_parallel_tasks = atoi(argv[2]);
    if(max_parallel_tasks == 0){
        char error_message[] = "Min parallel tasks of zero\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    char outputPath[100];
    strcpy(outputPath, argv[1]);


    char output_log_file_path[256];
    snprintf(output_log_file_path, sizeof(output_log_file_path), "%s/Tasks.log", outputPath);

    struct stat st = {0};
    if (stat(outputPath, &st) == -1) {
        mkdir(outputPath, 0700);
    }

    int logFile_fd = open(output_log_file_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (logFile_fd == -1) {
        char errorMsg[] = "Erro ao abrir/criar o arquivo de log";
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        return 1;
    }
    char header[] = "ID, Program, Arguments, Runtime\n";
    if (write(logFile_fd, header, strlen(header)) <= 0) {
        printf("Erro ao escrever no ficheiro log\n");
        return 1;
    }


    if (mkfifo(SERVER_CLIENT_FIFO, 0666) == -1) {
        if(errno != EEXIST){
            char errorMsg[] = "Erro na criação do fifo";
            write(STDERR_FILENO, errorMsg, strlen(errorMsg));
            return 1;
        }
    }


    printf("Server running...\n");
    schedule_init();
    status_init(max_parallel_tasks);
    int num_tasks_executing = 0;
    int server_status = 1;
    while(server_status){
        Task task_read;
        Task task_executing;
        int server_client_fifo = open(SERVER_CLIENT_FIFO, O_RDONLY);


        while((read(server_client_fifo, &task_read, sizeof(Task))) > 0){
            if(strcmp(task_read.flag, "C") == 0) {
                num_tasks_executing--;
                status_remove_task(task_read);
                close(server_client_fifo);
                continue;
            }
            if(strcmp(task_read.command, "status") == 0){
                char buffer[4096] = {'\0'};
                strcat(buffer, status_server_state());
                server_client_fifo = open(SERVER_CLIENT_FIFO, O_WRONLY);
                write(server_client_fifo, buffer, strlen(buffer));
                close(server_client_fifo);
                continue;
            }
            
            close(server_client_fifo);
            int id = schedule_get_next_task_id();
            task_read.id = id;


            schedule_add_task(task_read);
            server_client_fifo = open(SERVER_CLIENT_FIFO, O_WRONLY);
            write(server_client_fifo, &id, sizeof(int));
            close(server_client_fifo);


            if((num_tasks_executing < max_parallel_tasks) && !schedule_empty_queue()){
                task_executing = schedule_get_task();
                schedule_remove_task(task_executing);
                status_add_task(task_executing);


                pid_t pid = fork();
                if(pid == 0){
                    if(strcmp(task_executing.flag, "-u") == 0) task_executing = processortaskexecuter(task_executing, logFile_fd);
                    if(strcmp(task_executing.flag, "-p") == 0) task_executing = processorpipelinerexecuter(task_executing, logFile_fd);


                    strcpy(task_executing.flag, "C");
                    server_client_fifo = open(SERVER_CLIENT_FIFO, O_WRONLY);
                    write(server_client_fifo, &task_executing, sizeof(Task));
                    close(server_client_fifo);


                    _exit(1);
                }
                num_tasks_executing++;
                continue;
            }
        }
        if((num_tasks_executing < max_parallel_tasks) && !schedule_empty_queue()){
            task_executing = schedule_get_task();
            schedule_remove_task(task_executing);
            status_add_task(task_executing);


            pid_t pid = fork();
            if(pid == 0){
                if(strcmp(task_executing.flag, "-u") == 0) task_executing = processortaskexecuter(task_executing, logFile_fd);
                if(strcmp(task_executing.flag, "-p") == 0) task_executing = processorpipelinerexecuter(task_executing, logFile_fd);


                strcpy(task_executing.flag, "C");
                server_client_fifo = open(SERVER_CLIENT_FIFO, O_WRONLY);
                write(server_client_fifo, &task_executing, sizeof(Task));
                close(server_client_fifo);


                _exit(1);
            }
            num_tasks_executing++;
        }


    }
    unlink(SERVER_CLIENT_FIFO);
    return 0;
}






