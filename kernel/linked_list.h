#ifndef LINKED_LIST_H
#define	LINKED_LIST_H

#include "types.h"

struct linked_list_s;

typedef struct linked_list_s linked_list_t;

#pragma pack(1)
struct linked_list_s {
    linked_list_t* next;
    pointer_t data;
};

#pragma pack(1)
typedef struct linked_list_head_data_s {
    uint8_t size;
    linked_list_t* last;
} linked_list_head_data_t;

int8_t linked_list_create( linked_list_t** a_head ); //ok

int8_t linked_list_insert( linked_list_t* a_head, pointer_t a_data ); //ok

/*int8_t linked_list_remove_by_index( linked_list_t* a_head, uint16_t a_index );*/

int8_t linked_list_remove_last( linked_list_t* a_head ); //ok

int8_t linked_list_remove_first( linked_list_t* a_head ); //ok

int8_t linked_list_remove_all( linked_list_t* a_head ); //ok

pointer_t linked_list_get_by_index( linked_list_t* a_head, uint16_t a_index ); //ok

pointer_t linked_list_get_last( linked_list_t* a_head ); //ok

pointer_t linked_list_get_first( linked_list_t* a_head ); //ok

#ifdef DEBUG_LINUX
void linked_list_print_all_uint8( linked_list_t* a_head ); //ok
#endif

#endif	/* LINKED_LIST_H */

