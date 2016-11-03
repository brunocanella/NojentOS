/* 
 * File:   semaphore.h
 * Author: bruno
 *
 * Created on October 6, 2016, 2:45 PM
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "types.h"
#include "linked_list.h"
#include "dispatcher.h"

typedef struct semaphore_s {
    int8_t value;
    task_t* queue[TASKS_MAX];
    uint8_t queue_size;
    uint8_t queue_first;
} semaphore_t;

void semaphore_init( semaphore_t* handle, int8_t initial_value );
void semaphore_wait( semaphore_t* handle );
void semaphore_signal( semaphore_t* handle );

#endif	/* SEMAPHORE_H */

