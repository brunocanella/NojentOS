/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#include "queue.h"

void queue_create( queue_t* handle ) {    
    // Inicializa a fila    
    handle->size = 0;
    handle->first = 0;
}

void queue_add( queue_t* handle, task_t* ptr_task ) {
    uint8_t pos = ( handle->first + handle->size ) % TASKS_MAX;
    handle->values[pos] = ptr_task;
    handle->size++;    
}

uint8_t queue_remove( queue_t* handle ) {
    task_t* removed = handle->values[handle->first];
    
    handle->first = (handle->first + 1) % TASKS_MAX;
    handle->size--;
    
    return removed;
}