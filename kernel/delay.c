#include "delay.h";

#include "timers.h"
#include "task.h"
#include "dispatcher.h"

void delay_callback() {
    for( int i; i < delays.size; i++ ) {
        if( delays.queue[i].delay > 100 ) {
            delays.queue[i].delay -= 100;
            continue;
        }
        
        delays.queue[i].delay = 0;
        // Devolve a tarefa para o estado de READY
        task_halt( delays.queue[i].task );
        
        
#if SCHEDULER_PRIORITY
        dispatcher_change_context_isr();
#endif      
    }
}

void delay_init() {
    timer0_init( 100, delay_callback );
}

void delay_task( task_t* handle );