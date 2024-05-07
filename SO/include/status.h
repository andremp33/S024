#ifndef STATUS_H
#define STATUS_H

#include "../include/task.h"

char* status_schedule_tasks();
char* status_executed_tasks();
char* status_executing_tasks();
char* status_server_state();
void status_init(int max_parallel_tasks);
void status_add_task(Task task_executing);
void status_remove_task(Task task_executed);




#endif
