#ifndef STATUS_H
#define STATUS_H


char* status_schedule_tasks();
char* status_executed_tasks();
char* status_executing_tasks();
char* status_server_state();
void status_init();
void status_add_task();
void status_remove_task();




#endif