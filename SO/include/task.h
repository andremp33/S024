#ifndef TASK_H
#define TASK_H


#include <sys/types.h>
#include <stdbool.h>


#define MAX_QUEUE_SIZE 50


typedef struct {
 char command[10];
 char program[100];
 char flag[3];
 bool occupation;
 int id;
 int time;
} Task;


void schedule_init();
int schedule_add_task(Task task);
void schedule_remove_task(Task task);
bool schedule_empty_queue();
int schedule_get_next_task_id();
Task* schedule_get_scheduled_tasks();
int schedule_get_scheduled_tasks_num();
Task schedule_get_task();
void schedule_delete_queue();



#endif // TASK_H


