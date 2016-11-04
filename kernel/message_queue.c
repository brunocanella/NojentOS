#include "message_queue.h"
#include "defines.h"
#include "memory.h"

int8_t message_queue_init(message_queue_t* descriptor) {
    return linked_list_create((linked_list_t**)descriptor);    
}

uint8_t message_queue_read(message_queue_t descriptor, pointer_t buff, uint8_t size) {
    GLOBAL_INTERRUPTS_DISABLE(); 
     
    if(buff == NULL) {
        GLOBAL_INTERRUPTS_ENABLE();
        return 0;
    }
    
    //Instanciando um nó de mensagem
    message_queue_node_t* message_queue_node = (message_queue_node_t*)linked_list_get_first((linked_list_t*)descriptor); 
    
    //Caso o que for ser lido for de tamanho diferente da p?oxima mensagem gravada não lê
    if((message_queue_node == NULL) || (message_queue_node->width != size)) {
        GLOBAL_INTERRUPTS_ENABLE();
        return 0;
    }
    
    //Atribuindo o conteudo ao buffer para dar free no dado que estava salvo na lista
    uint8_t i;
    for(i = 0; i < size; i++)
        ((uint8_t*)buff)[i] = ((uint8_t*)(message_queue_node->data))[i];
    
    //buffer já está salvo, retirando dado da lista
    linked_list_remove_first((linked_list_t*)descriptor);
    //dando free no dado que estava na lista encadeada
    free(message_queue_node);
    GLOBAL_INTERRUPTS_ENABLE();
    return size;
}

uint8_t message_queue_write(message_queue_t descriptor, pointer_t buff, uint8_t size) {
    GLOBAL_INTERRUPTS_DISABLE();
    
//Instanciando um nó de mensagem
    message_queue_node_t* message_queue_node = (message_queue_node_t*)malloc(sizeof(message_queue_node_t));
    if(message_queue_node == NULL) {
        GLOBAL_INTERRUPTS_ENABLE();
        return 0;
    }
    message_queue_node->width = size;
    message_queue_node->data = (pointer_t*)malloc(size);
    if(message_queue_node->data == NULL) {
        GLOBAL_INTERRUPTS_ENABLE();
        return 0;
    }
    
        //Atribuindo o conteudo ao buffer para dar free no dado que estava salvo na lista
    uint8_t i;
    for(i = 0; i < size; i++)
        ((uint8_t*)(message_queue_node->data))[i] = ((uint8_t*)buff)[i];
    
    //Tenta inserir a mensagem na lista, caso der certo retorna o número de bytes gravados
    if(linked_list_insert((linked_list_t*)descriptor, message_queue_node) == ERROR_NONE) {
        GLOBAL_INTERRUPTS_ENABLE();
        return size;
    }
    //caso tenha dada falha no malloc    
    GLOBAL_INTERRUPTS_ENABLE();
    return 0;
}