#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

typedef unsigned char byte;

#ifndef NULL
#define NULL ((void*)0)
#endif

#define HEAP_SIZE 512

extern byte _heap[HEAP_SIZE];

void* malloc( size_t size );

typedef struct block_s block_t;

//------------------------------------------------------------------------------
void print_blocks();
void print_heap();
//------------------------------------------------------------------------------

#endif//MEMORY_H
