#include <pic18f4520.h>

#include "nojentOS.h"

#include "kernel/interrutps.h"

#ifndef OSC_INTOSC
#define OSC_INTOSC OSC_INTOSC_8MHZ
#endif//OSC_INTOSC

#ifndef OSC_PLLENABLE
#define OSC_PLLENABLE OFF
#endif//OSC_PLLENABLE

void nojo_init() {
    // Configurando o Clock
    OSCCONbits.IRCF = OSC_INTOSC;
    OSCTUNEbits.PLLEN = OSC_PLLENABLE;
    
    dispatcher_init();
    delay_init();
}

void nojo_start() {
    dispatcher_start();
    
    GLOBAL_INTERRUPTS_ENABLE();
}