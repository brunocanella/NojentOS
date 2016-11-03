#include "task.h"

#include "dispatcher.h"

#include "memory.h"

void task_create( uint16_t id, uint16_t priority, function_ptr_t callback ) {
    task_t* handle = dispatcher_add_task();
    
    handle->id = id;
    handle->priority = priority;
    handle->callback = callback;
    
    // Quando criar a tarefa, inicia o contexto dela com o próprio endereço da tarefa.
    handle->context.work = 0;
    handle->context.bsr = 0;
    handle->context.status = 0;
    handle->context.stack.size = 1;
    handle->context.stack.values = (uint24_t*)malloc(sizeof(uint24_t));
    handle->context.stack.values[0] = (uint24_t)callback;    
    
    task_ready( handle );
}

void task_ready( task_t* handle ) {
    handle->state = task_state_ready;
}

void task_running( task_t* handle ) {
    handle->state = task_state_running;
}

/*asm("GLOBAL _task_block");

void task_block( task_t* handle ) {
    handle->state = task_state_blocked;    
}*/

//
//void task_delay( task_t* handle ) {
//    task_block(handle);
//    //TODO chamar delay
//}
