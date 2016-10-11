/* 
 * File:   scheduler.h
 * Author: bruno
 *
 * Created on September 23, 2016, 4:41 PM
 */

#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include "task.h"

typedef task_t* (*scheduler_function_ptr_t)(void);

//task_t* scheduler_priority();

task_t* scheduler_round_robin();

//task_t* scheduler_priority_rr();

#endif	/* SCHEDULER_H */

