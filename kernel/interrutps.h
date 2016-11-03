/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef INTERRUTPS_H
#define	INTERRUTPS_H

#include <xc.h>
#include "types.h"

#include "dispatcher.h"

/**
 * Faz o tratamento das interrup��es e chama sub-ISRs.
 */
void interrupt global_isr() {
    // Timer 0
    if( INTCONbits.TMR0IE && INTCONbits.TMR0IF ) {
        // Primeira coisa que faz quando dispara uma interrup��o, � salvar os registradores
        CONTEXT_SAVE(dispatcher.running );
        
        timer0_isr();
        
        CONTEXT_LOAD(dispatcher.running );
    }
}
#endif	/* INTERRUTPS_H */

