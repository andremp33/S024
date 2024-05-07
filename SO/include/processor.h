#ifndef PROCESSOR_H
#define PROCESSOR_H


#include "../include/task.h"


Task processortaskexecuter (Task task, int logFile_fd);
Task processorpipelinerexecuter (Task task, int logFile_fd);

#endif
