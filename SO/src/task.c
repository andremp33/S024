#include "../include/task.h"
#include <string.h>


static Task task_queue[MAX_QUEUE_SIZE];
static int task_count = 0;
static int next_task_id = 1;
static int current_task_id = 0;


void scheduler_init() {
 memset(task_queue, 0, sizeof(task_queue));
 task_count = 0;
 next_task_id = 1;
}


int scheduler_add_task(Task task) {
 if (task_count < MAX_QUEUE_SIZE) {
 task.id = next_task_id++;
 task_queue[task_count++] = task;
 return task.id;
 }
 return -1;
}


void scheduler_remove_task(Task task) {
 for (int i = 0; i < task_count; i++) {
 if (task_queue[i].id == task.id) {
 memmove(&task_queue[i], &task_queue[i + 1], (task_count - i - 1) * sizeof(Task));
 task_count--;
 break;
 }
 }
}


Task scheduler_get_task() {
 if (task_count == 0) {
 return (Task){0};
 }
 Task task = task_queue[0];
 scheduler_remove_task(task);
 return task;
}


void scheduler_delete_queue() {
 memset(task_queue, 0, sizeof(task_queue));
 task_count = 0;
}


bool scheduler_empty_queue() {
 return task_count == 0;
}

bool queue_empty() {
    return scheduler_get_scheduled_tasks_num() == 0;  // Assume que essa função retorna o número de tarefas agendadas
}

int get_next_task_id() {
    return ++current_task_id;  // Incrementa e retorna um novo ID de tarefa
}

Task* scheduler_get_scheduled_tasks() {
 return task_queue;
}


int scheduler_get_scheduled_tasks_num() {
 return task_count;
}


