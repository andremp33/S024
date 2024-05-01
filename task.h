#ifndef TASK.H
#define TASK.H

#include <sys/types.h>
#include <stdbool.h>
#define MAX_QUEUE_SIZE 100

typedef struct {

    int id;
    int time;
    char command[10];
    char progam[100];
    char flag[3];
    pid_t pid;
    bool ocupation;
    
}Task;


void schedulerinit();
int scheduleaddtask(Task tasktoadd);
void scheduleremovetask(Task tastoremove);
Task schedulegettask();
void scheduledeletequeue();
bool emptyqueue();
int getnextatksid();
Task* schedulegetscheduletasks();
int schedulegetscheduletasksnum();

#endif

