/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef DEFINES_H
#define	DEFINES_H

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

#endif	/* DEFINES_H */

