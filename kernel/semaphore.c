#include "semaphore.h"

void semaphore_init( semaphore_t* handle, uint8_t initial_value ) {
    handle->value = initial_value;
    queue_create( &handle->queue );
}

void semaphore_aquire( semaphore_t* handle ) {
    GLOBAL_INTERRUPTS_DISABLE();
    
    if( --handle->value < 0 ) {
        // Adiciona a tarefa a lista de tarefas em espera
        queue_add( &handle->queue, aptos.task_running );
        // Bloqueia a tarefa em execução
        // Escolhe a proxima tarefa
        // Troca a tarefa em execução
    }
    
    GLOBAL_INTERRUPTS_ENABLE();
}

void semaphore_release( semaphore_t* handle ) {
    GLOBAL_INTERRUPTS_DISABLE();
    
    if( ++handle->value <= 0 ) {
        uint_t removed = queue_remove( &handle->queue );
        
        aptos.fila_aptos[ removed ].task_state = READY;
    }
    
    GLOBAL_INTERRUPTS_ENABLE();
}
