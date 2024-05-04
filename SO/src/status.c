#include "../include/status.h"
#include "../include/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char status_buffer[1024];  // Buffer para armazenar o estado do servidor


void status_init() {
    memset(status_buffer, 0, sizeof(status_buffer));  // Limpa o buffer de status
}


char* status_get_server_state() {
    snprintf(status_buffer, sizeof(status_buffer), "Total Scheduled Tasks: %d", scheduler_get_scheduled_tasks_num());
    return status_buffer;
}


void status_add_task(Task task) {
    // Log de adição de tarefa
    printf("Task added: ID %d\n", task.id);
}


void status_remove_task(Task task) {
    // Log de remoção de tarefa
    printf("Task removed: ID %d\n", task.id);
}


void status_print_tasks() {
    Task* tasks = scheduler_get_scheduled_tasks();
    int num_tasks = scheduler_get_scheduled_tasks_num();
    printf("Current Scheduled Tasks:\n");
    for (int i = 0; i < num_tasks; i++) {
        printf("Task ID: %d\n", tasks[i].id);
    }
}














