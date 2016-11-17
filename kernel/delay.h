/* 
 * File:   delay.h
 * Author: bruno
 *
 * Created on September 23, 2016, 6:35 PM
 */

#ifndef DELAY_H
#define	DELAY_H

#include "types.h"

void delay_init();

uint8_t timer1_isr();

void delay_cs( uint16_t a_time );

#endif	/* DELAY_H */

