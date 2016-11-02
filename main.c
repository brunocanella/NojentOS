/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 * Versão: 0.000000001
 */
#include <xc.h>
#include <pic18f4520.h>

// Config tem que vir antes de NojentOS, pois os defines dele sobrescrevem valores do SO.
#include "config.h"

#include "nojentOS.h"

/*static void task_1() {
    TRISDbits.RD0 = OUTPUT;
    PORTDbits.RD0 = LOW;
    while(TRUE) {
        PORTDbits.RD0 = !LATDbits.LATD0;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
    }
}

static void task_2() {
    TRISDbits.RD1 = OUTPUT;
    PORTDbits.RD1 = LOW;
    while(TRUE) {
        PORTDbits.RD1 = !LATDbits.LATD1;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
    }
}

static void task_3() {
    TRISDbits.RD2 = OUTPUT;
    PORTDbits.RD2 = LOW;
    while(TRUE) {
        PORTDbits.RD2 = !LATDbits.LATD2;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
    }
}*/

semaphore_t g_sem_1;
semaphore_t g_sem_2;
//semaphore_t g_sem_3;

static void task_1_priority() {
    TRISDbits.RD0 = OUTPUT;
    PORTDbits.RD0 = LOW;
    
    semaphore_init( &g_sem_1, 0 );
    semaphore_init( &g_sem_2, 0 );
    
    while(TRUE) {
        PORTDbits.RD0 = !LATDbits.LATD0;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
        semaphore_wait( &g_sem_1 );
    }
}

static void task_2_priority() {
    TRISDbits.RD1 = OUTPUT;
    PORTDbits.RD1 = LOW;
    while(TRUE) {
        PORTDbits.RD1 = !LATDbits.LATD1;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
        semaphore_wait( &g_sem_2 );
        semaphore_signal( &g_sem_1 );
    }
}

static void task_3_priority() {
    TRISDbits.RD2 = OUTPUT;
    PORTDbits.RD2 = LOW;
    while(TRUE) {
        PORTDbits.RD2 = !LATDbits.LATD2;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }        
        semaphore_signal( &g_sem_2 );
    }
}

void main(void) {
    
    nojo_init();
    
    //asm("GLOBAL _task_1, _task_2, _task_3, _task_idle_callback");
    asm("GLOBAL _task_1_priority, _task_2_priority, _task_3_priority, _task_idle_callback");
    
    task_create( 1, 200, task_1_priority );
    task_create( 2, 100, task_2_priority );
    task_create( 3,  50, task_3_priority );
    
    nojo_start();
    
    while(1) {
        asm("nop");
    }
}