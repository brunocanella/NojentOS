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

typedef struct semaphore_s {
    int8_t value;
    linked_list_t* queue;
} semaphore_t;

void semaphore_init( semaphore_t* handle, int8_t initial_value );
void semaphore_wait( semaphore_t* handle );
void semaphore_signal( semaphore_t* handle );

#endif	/* SEMAPHORE_H */

