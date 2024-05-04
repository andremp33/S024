#ifndef STATUS_H
#define STATUS_H


#include "task.h"  // Presumindo que Task está definido em task.h


void status_init();
char* status_get_server_state();
void status_add_task(Task task);
void status_remove_task(Task task);
void status_print_tasks();


#endif
