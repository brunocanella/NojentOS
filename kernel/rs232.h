
#ifndef RS232_H
#define RS232_H

#include <stdint.h>

#define PIN_RX RC6
#define PIN_TX RC7
#define _XTAL_FREQ 32000000

bit inicializarUSART(const unsigned int baudrate);

void USARTwrite(char data);

void USARTwriteString(char* data);

char USARTread();

char USARTavailable();

void USARTenableRXInterrupt();

void USARTdisableRXInterrupt();

#endif


