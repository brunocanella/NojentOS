/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 */

#ifndef DISPATCHER_H
#define	DISPATCHER_H

#ifndef TASKS_MAX
#define TASKS_MAX 4
#endif//TASKS_MAX

#include <pic18f4520.h>
#include "task.h"
#include "scheduler.h"
#include "memory.h"

typedef struct dispatcher_s {
    task_t tasks[TASKS_MAX];
    uint8_t size;
    task_t* running;
    scheduler_function_ptr_t callback;
} dispatcher_t;

void dispatcher_init();

void dispatcher_start();

task_t* dispatcher_add_task();

void dispatcher_change_context_isr();

extern dispatcher_t dispatcher;

extern task_t idle;

#define STACK_VALUE() (STKPTR & 0b11111)

#define CONTEXT_SAVE(TASK)                                                      \
do {                                                                            \
    task_t* task = (TASK);                                                      \
    if( task == NULL ) { break; }                                               \
    task_context_t* context = &(task->context);                                 \
    context->work = WREG;                                                       \
    context->bsr = BSR;                                                         \
    context->status = STATUS;                                                   \
    free(context->stack.values);                                                \
    context->stack.values = (uint24_t*)malloc(sizeof(uint24_t) * STACK_VALUE());\
    task_context_stack_t* stack = &(context->stack);                            \
    while( STACK_VALUE() > 0 ) {                                                \
        uint8_t i = stack->size++;                                              \
        stack->values[i] = TOS;                                                 \
        POP();                                                                  \
    }                                                                           \
} while(0)

#define CONTEXT_LOAD(TASK)                                                      \
do {                                                                            \
    task_t* task = (TASK);                                                      \
    if( task == NULL ) { break; }                                               \
    task_context_t* context = &(task->context);                                 \
    WREG = context->work;                                                       \
    BSR = context->bsr;                                                         \
    STATUS = context->status;                                                   \
    task_context_stack_t* stack = &(context->stack);                            \
    while( stack->size > 0 ) {                                                  \
        uint8_t i = --stack->size;                                              \
        PUSH();                                                                 \
        TOS = stack->values[i];                                                 \
    }                                                                           \
    free(context->stack.values);                                                \
} while(0)


#endif	/* DISPATCHER_H */

