/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include "kernel/defines.h"

#pragma config WDT = OFF                                                        // Desabilita watchdog timer
#pragma config OSC = INTIO7                                                     // Oscilador Interno
#pragma config PBADEN = OFF                                                     // Necess�rio para funcionar os pull-ups da porta B

#define OSC_INTOSC OSC_INTOSC_8MHZ                                              // Oscilador Interno de 8MHz
#define OSC_PLLENABLE ON                                                        // Oscilador Interno x4 >> 8MHz x4 >> 32 MHz

#define TASKS_MAX 3                                                             // Numero m�ximo de tarefas
#define STACK_LEVEL_MAX 31                                                      // N�veis da pilha de hardware

#endif	/* CONFIG_H */

