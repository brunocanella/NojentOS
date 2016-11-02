#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#include "types.h"

#define HEAP_SIZE 512

extern byte _heap[HEAP_SIZE];


void* malloc( size_t size );

void free( void* a_pointer );

typedef struct block_s block_t;


#ifdef DEBUG_LINUX
//------------------------------------------------------------------------------
void print_blocks();
void print_heap();
//------------------------------------------------------------------------------
#endif//DEBUG_LINUX

#endif//MEMORY_H
