#include "../include/task.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <string.h>




int next_task_id = 1;
int waiting_tasks = 0;
Task* queue;


int schedule_get_next_task_id() {
    return next_task_id++;
}


void schedule_init(){
    queue = malloc(MAX_QUEUE_SIZE * sizeof(Task));
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        queue[i].occupation = false;
        queue[i].time = 0;
    }
}


bool schedule_empty_queue() { // verifica se a queue esta vazia
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (queue[i].occupation) {
            return false;
        }
    }
    return true;
}


int schedule_add_task(Task task_to_add) {


    // Se a fila estiver vazia ou a tarefa a ser adicionada tem tempo menor do que a primeira tarefa na fila
    if (schedule_empty_queue() || task_to_add.time < queue[0].time) {
        // Move todas as tarefas para frente para abrir espaço para a nova tarefa
        for (int i = MAX_QUEUE_SIZE - 1; i > 0; i--) {
            queue[i] = queue[i - 1];
        }
        queue[0] = task_to_add;
        queue[0].occupation = true;
    } else {
        // Encontra a posição correta para inserir a tarefa
        int insert_index = 0;
        while (insert_index < MAX_QUEUE_SIZE && queue[insert_index].occupation && queue[insert_index].time <= task_to_add.time) {
            insert_index++;
        }


        // Move as tarefas para frente para abrir espaço para a nova tarefa
        for (int i = MAX_QUEUE_SIZE - 1; i > insert_index; i--) {
            queue[i] = queue[i - 1];
        }
        queue[insert_index] = task_to_add;
        queue[insert_index].occupation = true;
    }
    waiting_tasks++;


    return task_to_add.id;
}


void schedule_remove_task(Task task_to_remove) {
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (queue[i].occupation && queue[i].id == task_to_remove.id) {
            // Remove fisicamente a tarefa da fila
            for (int j = i; j < MAX_QUEUE_SIZE - 1; j++) {
                queue[j] = queue[j + 1];
            }
            // Limpa o último elemento da fila
            queue[MAX_QUEUE_SIZE - 1].occupation = false;
            break;
        }
    }
    waiting_tasks--;
}


Task* schedule_get_scheduled_tasks() { // mudar pra var local
    Task* pendingTasks = malloc(waiting_tasks * sizeof(Task));
    for (int i = 0,j = 0; i < waiting_tasks; i++) {
        if (queue[i].occupation) {
            pendingTasks[j] = queue[i];
            j++;
        }
    }
    return pendingTasks;
}


int schedule_get_scheduled_tasks_num(){
    return waiting_tasks;
}


Task schedule_get_task(){
    return queue[0];
}


void schedule_delete_queue(){
    free(queue);
}
