#include "scheduler.h"

#include "dispatcher.h"

task_t* scheduler_priority() {
    // De inicio, a A principio, a proxima tarefa a executar ser� a idle (mas em tese, n�o deve ser ela nunca.)
    task_t* highest = &idle;
    // Procura agora em todas as tarefas, qual � a tarefa de maior prioridade.
    for( int i = 0; i < dispatcher.size; i++ ) {
        // current == ready && current > highest
        if( ( dispatcher.tasks[i].state == task_state_ready ) && ( highest->priority < dispatcher.tasks[i].priority ) ) {
            highest = &(dispatcher.tasks[i]);
        }
    }
    // Retorna a tarefa de maior prioridade que est� pronta para rodar.
    return highest;
}

task_t* scheduler_round_robin() {
    int offset = -1;
    for( int i = 0; i < dispatcher.size; i++ ) {
        if( (&dispatcher.tasks[i]) == dispatcher.running ) {
            offset = i;
            break;
        }
    }
    
    // Caso offset seja -1 ainda, � porque a tarefa que estava rodando era a IDLE.
    
    task_t* next = &idle;
    for( int i = 0; i < dispatcher.size; i++ ) {
        // o +1 � para come�ar no registro seguinte ao offset. No caso de IDLE (-1), offset vai dar 0 (-1+1).
        // Se n�o for IDLE (tasks[0], por exemplo), ele vai come�ar buscando de task[1].
        int j = (offset+1+i) % dispatcher.size;
        if( dispatcher.tasks[j].state == task_state_ready ) {
            next = &dispatcher.tasks[j];
            break;
        }
    }
    
    return next;
}
