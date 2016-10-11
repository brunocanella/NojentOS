#include "dispatcher.h"
#include "scheduler.h"

#include "defines.h"
#include "timers.h"


dispatcher_t dispatcher;

task_t idle;

void task_idle_callback() {
    while( TRUE ) {
        asm("nop");
    }
}

void dispatcher_init() {
    dispatcher.size = 0;
    dispatcher.callback = scheduler_round_robin;
    
    idle.id = 255;
    idle.priority = 0;
    idle.state = task_state_ready;
    idle.callback = task_idle_callback;     
//    idle.context.work = WREG;
//    idle.context.bsr = BSR;
//    idle.context.status = STATUS;
//    idle.context.stack.size = 1;
//    idle.context.stack.values[0] = (uint24_t)task_idle_callback;
    
    dispatcher.running = &idle;
}

void dispatcher_start() {        
    timer0_init( 10000, dispatcher_change_context_isr );
}

task_t* dispatcher_add_task() {
    task_t* task = &dispatcher.tasks[dispatcher.size];
    dispatcher.size++;
    return task;
}

/**
 * @remark O contexto deve ser salvo/carregado fora desta função.
 */
void dispatcher_change_context_isr() {
    // Recupera a tarefa que será a proxima a entrar em execução
    task_t* next = dispatcher.callback();
    // Suspende (passa para ready) a tarefa atual
    task_ready( dispatcher.running );
    // Troca a tarefa que está em execução
    dispatcher.running = next;
    // E resume o funcionamento dela ( passa para running )
    task_running( dispatcher.running );
}