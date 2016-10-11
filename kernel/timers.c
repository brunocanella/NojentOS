#include "timers.h"

#include "pic18f4520.h"

uint16_t _timer0_delay_us = 0;
function_ptr_t _timer0_callback = NULL;

void timer0_reset() {
    // Limpa a flag
    INTCONbits.TMR0IF = OFF;
    // Reinicia o tempo
    TMR0 = _timer0_delay_us;
}

void timer0_init( uint16_t delay_us, function_ptr_t callback  ) {
    // Stoping the Timer
    T0CONbits.TMR0ON = OFF;
    // And also the interrupts
    INTCONbits.TMR0IE = OFF;
    
    // Setting the custom timer0 data
    // Resetando o timer
    _timer0_delay_us = delay_us;
    // Gravando a função de callback para interrupções
    _timer0_callback = callback;    
    
    // 8/16 bit control (1 = 8, 0 = 16) bits
    T0CONbits.T08BIT = OFF;
    // Clock Source Select Bit ( 0 = Internal Instruction Cycle Clock (CLK0), 1 = Transition on T0CKI pin )
    T0CONbits.T0CS = LOW;
    // Source Edge Select Bit ( 0 = low-high, 1 = high-low )
    T0CONbits.T0SE = 0;
    // Prescaler Assigment Bit (1 = Use Scalar, 0 = Bypass Scalar)! TA INVERTIDO NO MANUAL.
    T0CONbits.PSA = ON;
    // Prescaler select bits
    T0CONbits.T0PS = 0b010; // Timer_Fosc = Fosc / ( 4 * 8 )
    // Timer 0 Overflow Interrupt bit
    INTCONbits.TMR0IF = OFF;
    // Timer 0 interrupt Priority HIGH/LOW
    INTCON2bits.TMR0IP = HIGH;
    
    // Reseta o Timer 0
    timer0_reset();    
    
    // Timer 0 Interrupt Enable
    INTCONbits.TMR0IE = ON;
    // Enables/Stops Timer 0
    T0CONbits.TMR0ON = ON;
}

void timer0_isr() {
    timer0_reset();
    
    if( _timer0_callback ) {
        _timer0_callback();
    }
}