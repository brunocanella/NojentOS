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
#if SCHEDULER == SCHEDULER_ROUND_ROBIN
    dispatcher.callback = scheduler_round_robin;
#else
    dispatcher.callback = scheduler_priority;
#endif
    
    idle.id = 255;
    idle.priority = 0;
    idle.state = task_state_ready;
    idle.callback = task_idle_callback;     
    //idle.context.work = 0;
    //idle.context.bsr = 0;
    //idle.context.status = 0;
    idle.context.stack.size = 1;
    idle.context.stack.values = (uint24_t*)malloc(sizeof(uint24_t)); 
    idle.context.stack.values[0] = (uint24_t)task_idle_callback;
    
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

void dispatcher_change_context( task_state_t task_state ) {
    // Desabilita as interrupções globais antes de mexer nas tarefas.
    GLOBAL_INTERRUPTS_DISABLE();    
    // Salva o contexto da tarefa que estava rodando.
    CONTEXT_SAVE( dispatcher.running );
    // Passa a tarefa atual para o estado informado escolhido.
    dispatcher.running->state = task_state;
    // Descobre quem será a tarefa que será a proxima a entrar em execução
    task_t* next = dispatcher.callback();
    // Coloca a tarefa como a que está em execução
    dispatcher.running = next;
    // E atualiza o status dela como rodando.
    task_running( dispatcher.running );
    // Finalmente, carrega a pilha desta tarefa
    CONTEXT_LOAD( dispatcher.running );    
    // e por fim, reabilita as interrupções globais antes de retornar.
    GLOBAL_INTERRUPTS_ENABLE();
}