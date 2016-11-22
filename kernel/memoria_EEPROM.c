
#include "memoria_EEPROM.h"
#include<xc.h>

uint8_t read_byte(uint8_t address)
{
    // Escrita do endereço no registrador certo
    EEADR = (uint8_t) address;

    EECON1bits.EEPGD = 0; // Selecioa a EEPROM para manipular
    EECON1bits.CFGS = 0; // Acesso a EEPROM e não os registradores
    EECON1bits.RD = 1; // Taca lhe pau marco veio!

    // Esperar até concluir a leitura
    while (EECON1bits.RD == 1);

    return EEDATA; // Retorno do dado lido
}

void write_byte(uint8_t address, uint8_t data)
{
    // Configuração do registrador de endereço para manipulação
    EEADR = address;

    EEDATA = data; // Configuração do dado que será gravado
    EECON1bits.EEPGD = 0; // Seleciona a EEPROM data memory
    EECON1bits.CFGS = 0; // Acesso flash/EEPROM e não aos registradores de configuração
    EECON1bits.WREN = 1; // Habilita escrita na EEPROM 


    // Inicialização da escrita
    EECON2 = 0x55; // 
    EECON2 = 0xAA; // 
    EECON1bits.WR = 1; // Vamo dale!

    // Espera até concluir
    while (PIR2bits.EEIF == 0);

    // Configurações de finalização
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