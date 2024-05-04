#ifndef TASK_H
#define TASK_H


#include <sys/types.h>
#include <stdbool.h>


#define MAX_QUEUE_SIZE 100


typedef struct {
 int id;
 int time;
 char command[10];
 char program[100];
 char flag[3];
 pid_t pid;
 bool occupation;
} Task;


void scheduler_init();
int scheduler_add_task(Task task);
void scheduler_remove_task(Task task);
Task scheduler_get_task();
void scheduler_delete_queue();
bool scheduler_empty_queue();
int scheduler_get_next_task_id();
Task* scheduler_get_scheduled_tasks();
int scheduler_get_scheduled_tasks_num();


#endif


