/* 
 * File:   memoria_EEPROM.h
 * Author: EveraldoJr
 *
 * Created on October 27, 2016, 2:12 PM
 */

#ifndef MEMORIA_EEPROM_H
#define	MEMORIA_EEPROM_H
#include<stdint.h>
void write_bytes(uint8_t addr, uint8_t *buffer, uint8_t qtd);
void read_bytes(uint8_t addr, uint8_t *buffer, uint8_t qtd);
void config_EEPROM();


#endif	/* MEMORIA_EEPROM_H */

