/* 
 * File:   queue.h
 * Author: bruno
 *
 * Created on October 6, 2016, 2:54 PM
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include "../types.h"

#include "../task.h"

typedef struct queue_s {
    task_t* values[TASKS_MAX];
    uint8_t size;
    uint8_t first;
} queue_t;

void queue_create( queue_t* handle );
void queue_add( queue_t* handle, task_t* ptr_task );
task_t* queue_remove( queue_t* handle );

#endif	/* QUEUE_H */

