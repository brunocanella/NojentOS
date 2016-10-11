/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef INTERRUTPS_H
#define	INTERRUTPS_H

#include <xc.h>
#include "types.h"

#include "dispatcher.h"

byte _WREG;
byte _BSR;
byte _STATUS;

/**
 * Faz o tratamento das interrupções e chama sub-ISRs.
 */
void interrupt global_isr() {
    _WREG = WREG;
    _BSR = BSR;
    _STATUS = STATUS;
    // Timer 0
    if( INTCONbits.TMR0IE && INTCONbits.TMR0IF ) {
        // Primeira coisa que faz quando dispara uma interrupção, é salvar os registradores
        do {
            task_t* p_task = (dispatcher.running);                                                      
            if( p_task == NULL ) { break; }                                               
            task_context_t* p_context = &(p_task->context);                                 
            p_context->work = _WREG;                                                       
            p_context->bsr = _BSR;                                                         
            p_context->status = _STATUS;                                                   
            task_context_stack_t* p_stack = &p_context->stack;                              
            while( (STKPTR & 0b11111) > 0 ) {                                                       
                uint8_t i = p_stack->size++;                                                              
                p_stack->values[i] = (TOSU << 16) | (TOSH << 8) | TOSL;                                                 
                POP();                                                                                  
            }
        }while(0);
        
        timer0_isr();
        
        do {
            task_t* n_task = (dispatcher.running);                                                      
            if( n_task == NULL ) { break; }                                               
            task_context_t* n_context = &(n_task->context);                                 
            _WREG = n_context->work;                                                       
            _BSR = n_context->bsr;                                                         
            _STATUS = n_context->status;                                                   
            task_context_stack_t* n_stack = &n_context->stack;                              
            while( n_stack->size > 0 ) {                                                  
                uint8_t i = --n_stack->size;   
                PUSH();                                                                 
                TOSU = n_stack->values[i] >> 16;
                TOSH = n_stack->values[i] >>  8;
                TOSL = n_stack->values[i] >>  0;
                //TOS = n_stack->values[i];
                                                                             
            }
        } while(0);        
    }    
    BSR = _BSR;
    STATUS = _STATUS;
    WREG = _WREG;
}
#endif	/* INTERRUTPS_H */

