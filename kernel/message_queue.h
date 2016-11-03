#ifndef MESSAGE_QUEUE_H
#define	MESSAGE_QUEUE_H

#include "linked_list.h"

typedef linked_list_t* message_queue_t;
typedef struct
{
    uint8_t width;
    pointer_t data;
}message_queue_node_t;

int8_t message_queue_init(message_queue_t* descriptor);
uint8_t message_queue_read(message_queue_t descriptor, pointer_t buff, uint8_t size);
uint8_t message_queue_write(message_queue_t descriptor, pointer_t buff, uint8_t size);

#endif