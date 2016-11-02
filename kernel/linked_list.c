#include "linked_list.h"
#include "defines.h"
#include "memory.h"

#ifdef DEBUG_LINUX
#include <stdlib.h>
#include <stdio.h>
#endif

int8_t linked_list_create( linked_list_t** a_head ) {
    (*a_head) = (linked_list_t*) malloc( sizeof( linked_list_t ) );
    
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*) malloc( sizeof(linked_list_head_data_t) );
    
    l_head_data->size = 0;
    l_head_data->last = NULL;
    
    (*a_head)->data = l_head_data;
    (*a_head)->next = NULL;
    
    if( (*a_head) == NULL || l_head_data == NULL) {
        return ERROR_MALLOC_NULL;
    } else {
        return ERROR_NONE;
    }
}

int8_t linked_list_insert( linked_list_t* a_head, pointer_t a_data ) {
    linked_list_t* l_node = (linked_list_t*) malloc( sizeof(linked_list_t) );
    
    if( l_node == NULL ) {
        return ERROR_MALLOC_NULL;
    }
    
    l_node->next = NULL;
    l_node->data = a_data;
    
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    
    if( a_head->next == NULL ) {
        a_head->next = l_node;
        l_head_data->last = l_node;
    } else {
        l_head_data->last->next = l_node;
        l_head_data->last = l_node;
    }
    
    l_head_data->size++;
    
    return ERROR_NONE;
}

/*int8_t linked_list_remove_by_index( linked_list_t* a_head, uint16_t a_index ){
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    
    if (a_index >= l_head_data->size){      
        return ERROR_LIST_OUT_OF_RANGE;
    } else if( l_head_data->size == 0 ) {
        return ERROR_LIST_REMOVE_EMPTY;
    } else if( l_head_data->size == 1 ) {
        free(a_head->next);
        a_head->next = NULL;
        l_head_data->size--;
        l_head_data->last = NULL;
    } else {
        linked_list_t* l_iter_prev = a_head->next;
        linked_list_t* l_iter_curr = a_head->next->next;
        uint16_t iterator = 0;

        while( (l_iter_curr->next != NULL) && (iterator < a_index) ) {
            l_iter_prev = l_iter_curr;
            l_iter_curr = l_iter_curr->next;
            iterator++;
        }
        a_head->next = l_iter_curr;
        l_head_data->size--;
        free(l_iter_prev);
    }
    return ERROR_NONE;
}*/

int8_t linked_list_remove_last( linked_list_t* a_head ) {
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    
    if( l_head_data->size == 0 ) {
        return ERROR_LIST_REMOVE_EMPTY;
    } else if( l_head_data->size == 1 ) {
        free(a_head->next);
        a_head->next = NULL;
        l_head_data->size--;
        l_head_data->last = NULL;
    } else {
        linked_list_t* l_iter_prev = a_head->next;
        linked_list_t* l_iter_curr = a_head->next->next;
        
        while( l_iter_curr->next != NULL ) {
            l_iter_prev = l_iter_curr;
            l_iter_curr = l_iter_curr->next;
        }
        free(l_iter_curr);
        l_iter_prev->next = NULL;
        l_head_data->size--;
        l_head_data->last = l_iter_prev;
    }
    return ERROR_NONE;
}

int8_t linked_list_remove_first( linked_list_t* a_head ){
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    
    if( l_head_data->size == 0 ) {
        return ERROR_LIST_REMOVE_EMPTY;
    } else if( l_head_data->size == 1 ) {
        free(a_head->next);
        a_head->next = NULL;
        l_head_data->size--;
        l_head_data->last = NULL;
    } else {
        linked_list_t* l_iter_delet = a_head->next;
        linked_list_t* l_iter_new_first = a_head->next->next;       
        a_head->next = l_iter_new_first;
        l_head_data->size--;
        free(l_iter_delet);
    }
    return ERROR_NONE;  
}

int8_t linked_list_remove_all( linked_list_t* a_head ) {
    int8_t return_result = ERROR_NONE;
    while (return_result == ERROR_NONE)
    {
        return_result = linked_list_remove_last(a_head);
    }
    return return_result;
}

pointer_t linked_list_get_by_index( linked_list_t* a_head, uint16_t a_index ){
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    if (a_index >= l_head_data->size)      
        return NULL;

    uint16_t iterator_num = 0;
    linked_list_t* iterator_ptr = a_head->next;

    while(iterator_num < a_index){
        iterator_num++;
        iterator_ptr = iterator_ptr->next;
    }

    return  iterator_ptr->data;      
}

pointer_t linked_list_get_last( linked_list_t* a_head ){
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    return l_head_data->last->data;
}

pointer_t linked_list_get_first( linked_list_t* a_head ){
    return a_head->next->data;
}

uint8_t linked_list_get_size( linked_list_t* a_head ) {
    linked_list_head_data_t* l_head_data = (linked_list_head_data_t*)(a_head->data);
    return l_head_data->size;
}

#ifdef DEBUG_LINUX
void linked_list_print_all_uint8( linked_list_t* a_head ){
    linked_list_t* tmp = a_head->next;

    while( tmp != NULL ){
        printf("%d ", *(uint8_t*)tmp->data);
        tmp = tmp->next;
    }
}
#endif