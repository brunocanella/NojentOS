/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef DEFINES_H
#define	DEFINES_H

#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif//NULL

#define TRUE 1
#define FALSE 0

#define HIGH TRUE
#define LOW FALSE

#define ON HIGH
#define OFF LOW

#define INPUT HIGH
#define OUTPUT LOW

#define GLOBAL_INTERRUPTS_ENABLE() do{ INTCONbits.GIE = ON; }while(0)
#define GLOBAL_INTERRUPTS_DISABLE() do{ INTCONbits.GIE = OFF; }while(0)

#define PUSH() asm("PUSH")
#define POP() asm("POP")

// Valores para o oscilador
#define OSC_INTOSC_8MHZ 0b111                                                   // Oscilador Interno de 8MHz

/**Deu boa*/
#define ERROR_NONE 0
/**Faltou espaço na memória. Ferrou.*/
#define ERROR_MALLOC_NULL -1
/**Tentou remover com a lista vazia*/
#define ERROR_LIST_REMOVE_EMPTY -2
/**Tentou remover item foram do tamanho da lista*/
#define ERROR_LIST_OUT_OF_RANGE -3
 
#define SCHEDULER_ROUND_ROBIN 1
#define SCHEDULER_PRIORITY    2

#define SCHEDULER SCHEDULER_PRIORITY

#endif	/* DEFINES_H */

