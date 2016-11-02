/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef TASK_H
#define	TASK_H

#include "types.h"
#include "task_context.h"

typedef enum task_state_e {
    task_state_ready,
    task_state_running,
    task_state_blocked
} task_state_t;

typedef struct task_s {
    uint16_t id;
    uint16_t priority;
    task_state_t state;
    function_ptr_t callback;
    task_context_t context;
} task_t;

void task_create( uint16_t id, uint16_t priority, function_ptr_t callback );

void task_ready( task_t* handle );

void task_running( task_t* handle );

void task_block( task_t* handle );

// void task_delay( task_t* handle );

#endif	/* TASK_H */

