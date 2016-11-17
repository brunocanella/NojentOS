#include "delay.h";

#include "timers.h"
#include "task.h"
#include "dispatcher.h"

typedef struct {
    task_t* task;
    uint16_t time;
} delay_task_t;

static delay_task_t delay_queue[TASKS_MAX];

void timer1_reset() {
    PIR1bits.TMR1IF = OFF;
    TMR1 = 45536;
}

uint8_t timer1_isr() {
    timer1_reset();
    
    uint8_t count = 0;
    
    for( uint8_t i = 0; i < TASKS_MAX; i++ ) {
        delay_task_t* l_item = &delay_queue[i];
        if( l_item->task == NULL ) {
            continue;
        }
        l_item->time--;
        if( l_item->time == 0 ) {
            task_ready( l_item->task );
            l_item->task = NULL;
            count++;
        }
    }
    
    return count;
}

void delay_init() {
    for( int i = 0; i < TASKS_MAX; i++ ) {
        delay_queue[i].time = 0;
        delay_queue[i].task = NULL;
    }
    // ----- //
    // 16 bits
    T1CONbits.RD16 = ON;
    // Oscilador
    T1CONbits.T1RUN = ON;
    // Prescalar
    T1CONbits.T1CKPS = 0b00;
    // Liga oscilador do timer1
    T1CONbits.T1OSCEN = ON;
    // 
    T1CONbits.TMR1CS = 0;
    
    timer1_reset();
    
    PIE1bits.TMR1IE = ON;
       
    INTCONbits.PEIE = ON;            
    
    T1CONbits.TMR1ON = ON;
    
    // ----- //    
}

void delay_cs( uint16_t a_time ) {
    for( uint8_t i = 0; i < TASKS_MAX; i++ ) {
        delay_task_t* l_item = &delay_queue[i];
        if( l_item->task != NULL ) {
            continue;
        }
        l_item->task = dispatcher.running;
        l_item->time = a_time;
        break;
    }
    
    dispatcher_change_context( task_state_blocked );
}
