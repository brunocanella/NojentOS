#include "rs232.h"
#include <xc.h>

/*
 * Limitações: 1200 até 57600 de baudrate
 */
bit inicializarUSART(const unsigned int baudrate)
{
    unsigned long int spbrg = ((unsigned long int) _XTAL_FREQ - (unsigned long int) 16 * baudrate) / ((unsigned long int) 16 * baudrate);

    if (spbrg > 255)
        return 0;

    BRG16 = 0b0;
    SPBRG = spbrg;
    TXSTAbits.TX9 = 0b0; //Setando transmiss?o em 8 bits
    TXSTAbits.TXEN = 0b1; //Habilitando transmiss?o
    TXSTAbits.SYNC = 0b0; //Setando ass?ncrono
    TXSTAbits.SENDB = 0b0; //Sync Break no fim da transmiss?o
    TXSTAbits.BRGH = 0b1; //Setando como alta velocidade

    RCSTAbits.SPEN = 0b1; //Habilitando porta serial
    RCSTAbits.RX9 = 0b0;
    RCSTAbits.CREN = 0b1; //Habilitando recep??o cont?nua 

    TRISCbits.TRISC7 = 0b1; //Setando como descrito no datashet
    TRISCbits.TRISC6 = 0b0; //Setando como descrito no datashet

    return 1;
}

void USARTwrite(char data)
{
    while (!TXSTAbits.TRMT);
    TXREG = data;
}

void USARTwriteString(char* data)
{
    unsigned int i;
    for (i = 0; data[i] != '\0'; i++)
        USARTwrite(data[i]);
}

char USARTread()
{
    while (!PIR1bits.RCIF)
    {
        asm("clrwdt");
    }

    return RCREG;
}

/*void USARTreadString(char* stringOut, unsigned int length)
{
    unsigned int i;
    for(i = 0; i < length; i++)
        stringOut[i] = UARTread();
}*/

char USARTavailable()
{
    return PIR1bits.RCIF;
}

void USARTenableRXInterrupt()
{
    INTCONbits.GIE = 0b1;
    INTCONbits.PEIE = 0b1;
    PIE1bits.RCIE = 0b1; //Habilitando interrup??o de recep??o USART
}

void USARTdisableRXInterrupt()
{
    PIE1bits.RCIE = 0b0; //Desabilitando interrup??o de recep??o USART
}
/*Funcs para o printf*/
