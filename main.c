/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 * Vers�o: 0.000000001
 */
#include <xc.h>
#include <pic18f4520.h>

// Config tem que vir antes de NojentOS, pois os defines dele sobrescrevem valores do SO.
#include "config.h"

#include "nojentOS.h"

static void task_1() {
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
}

void main(void) {
    
    nojo_init();
    
    asm("GLOBAL _task_1, _task_2, _task_3, _task_idle_callback");
    
    task_create( 1, 200, task_1 );
    task_create( 2, 100, task_2 );
    task_create( 3,  50, task_3 );
    
    nojo_start();
    
    while(1) {
        asm("nop");
    }
}