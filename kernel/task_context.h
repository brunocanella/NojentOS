/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef TASK_CONTEXT_H
#define	TASK_CONTEXT_H

#include "defines.h"
#include "types.h"

#ifndef STACK_LEVEL_MAX
#define STACK_LEVEL_MAX 31
#endif//STACK_LEVEL_MAX

typedef struct task_context_stack_s {
    uint24_t* values;
    uint8_t size;
} task_context_stack_t;

typedef struct task_context_s {
    byte work;
    byte bsr;
    byte status;
    task_context_stack_t stack;
} task_context_t;

#endif	/* TASK_CONTEXT_H */

