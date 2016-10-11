/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef TIMERS_H
#define	TIMERS_H

#include "types.h"

void timer0_init( uint16_t delay_us, function_ptr_t callback );

void timer0_isr();

#endif	/* TIMERS_H */

