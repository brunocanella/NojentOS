
#include "memoria_EEPROM.h"
#include<xc.h>

uint8_t read_byte(uint8_t address)
{
    // Escrita do endere�o no registrador certo
    EEADR = (uint8_t) address;

    EECON1bits.EEPGD = 0; // Selecioa a EEPROM para manipular
    EECON1bits.CFGS = 0; // Acesso a EEPROM e n�o os registradores
    EECON1bits.RD = 1; // Taca lhe pau marco veio!

    // Esperar at� concluir a leitura
    while (EECON1bits.RD == 1);

    return EEDATA; // Retorno do dado lido
}

void write_byte(uint8_t address, uint8_t data)
{
    // Configura��o do registrador de endere�o para manipula��o
    EEADR = address;

    EEDATA = data; // Configura��o do dado que ser� gravado
    EECON1bits.EEPGD = 0; // Seleciona a EEPROM data memory
    EECON1bits.CFGS = 0; // Acesso flash/EEPROM e n�o aos registradores de configura��o
    EECON1bits.WREN = 1; // Habilita escrita na EEPROM 


    // Inicializa��o da escrita
    EECON2 = 0x55; // 
    EECON2 = 0xAA; // 
    EECON1bits.WR = 1; // Vamo dale!

    // Espera at� concluir
    while (PIR2bits.EEIF == 0);

    // Configura��es de finaliza��o
    PIR2bits.EEIF = 0; //Limpa a flag do bit EEIF
    EECON1bits.WREN = 0; // Desabilita a escrita
}

void config_to_write()
{

}

void config_to_read()
{

}

void write_bytes(uint8_t addr, uint8_t *buffer, uint8_t qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        write_byte(addr + i, buffer[i]);
    }
}

void read_bytes(uint8_t addr, uint8_t *buffer, uint8_t qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        buffer[i] = read_byte(addr + i);
    }
}