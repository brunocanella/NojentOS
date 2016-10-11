/* 
 * File:   semaphore.h
 * Author: bruno
 *
 * Created on October 6, 2016, 2:45 PM
 */

#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "utils/queue.h"

typedef struct semaphore_s {
    uint8_t value;
    queue_t queue;
} semaphore_t;

void semaphore_init( semaphore_t* handle, uint8_t initial_value );
void semaphore_aquire( semaphore_t* handle );
void semaphore_release( semaphore_t* handle );

#endif	/* SEMAPHORE_H */

