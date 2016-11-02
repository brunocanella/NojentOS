#include "semaphore.h"
#include "dispatcher.h"
#include "linked_list.h"

void semaphore_init( semaphore_t* handle, int8_t initial_value ) {
    handle->value = initial_value;
    linked_list_create( &handle->queue );
}

void semaphore_wait( semaphore_t* handle ) {
    // Verifica se o recurso está exausto. Se tiver, bloqueia a tarefa.
    if( --handle->value < 0 ) {
        // Adiciona a tarefa a lista de tarefas em espera
        linked_list_insert( handle->queue, (pointer_t)dispatcher.running );
        // Faz a troca de contexto para a proxima tarefa ativa.
        dispatcher_change_context( task_state_blocked );
    }
}

void semaphore_signal( semaphore_t* handle ) {
    // Aumenta o recurso e desbloqueia se tiver itens na fila.
    if( ++handle->value <= 0 && linked_list_get_size( handle->queue ) > 0 ) {
        // Recupera a primeira tarefa da lista de bloqueados
        task_t* l_task = (task_t*)linked_list_get_first( handle->queue );
        // TODO: tratar exceção.
        linked_list_remove_first( handle->queue );
        // Passa a tarefa que estava bloqueada para pronta.
        task_ready( l_task );
        // E faz um chamado na troca de contexto.
        dispatcher_change_context( task_state_ready );        
    }
}
