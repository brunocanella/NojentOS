#include "semaphore.h"
#include "linked_list.h"

void semaphore_init( semaphore_t* handle, int8_t initial_value ) {
    handle->value = initial_value;
    handle->queue_size = 0;
    handle->queue_first = 0;
}

void semaphore_wait( semaphore_t* handle ) {
    // Verifica se o recurso está exausto. Se tiver, bloqueia a tarefa.
    if( --handle->value < 0 ) {
        // Adiciona a tarefa a lista de tarefas em espera
        uint8_t l_index = ( handle->queue_first + handle->queue_size ) % TASKS_MAX;
        handle->queue[l_index] = dispatcher.running;
        handle->queue_size++;
        // Faz a troca de contexto para a proxima tarefa ativa.
        dispatcher_change_context( task_state_blocked );
    }
}

void semaphore_signal( semaphore_t* handle ) {
    // Aumenta o recurso e desbloqueia se tiver itens na fila.
    if( ++handle->value <= 0 && handle->queue_size > 0 ) {
        // Recupera a primeira tarefa da lista de bloqueados
        handle->queue_size--;
        uint8_t l_index = ( handle->queue_first + handle->queue_size ) % TASKS_MAX;
        task_t* l_task = handle->queue[l_index];
        handle->queue_first++; // Avança em 1 a posição do primeiro item da lista.
        // Passa a tarefa que estava bloqueada para pronta.
        task_ready( l_task );
        // E faz um chamado na troca de contexto.
        dispatcher_change_context( task_state_ready );        
    }
}
