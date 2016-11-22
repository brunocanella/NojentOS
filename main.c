/*
 * NojentOS - Nojo Embedded Task Operating System
 * by Alunos 2016-2 - Sistemas Operacionais Embarcados
 * Versão: 0.000000001
 */
#include <xc.h>
#include <pic18f4520.h>
#include <time.h>
#include <string.h>
// Config tem que vir antes de NojentOS, pois os defines dele sobrescrevem valores do SO.
#include "config.h"

#include "kernel/message_queue.h"
#include "kernel/defines.h"
#include "nojentOS.h"
#include "kernel/delay.h"
#include "kernel/rs232.h"

#if SCHEDULER == SCHEDULER_ROUND_ROBIN

static void task_1() {
    TRISDbits.RD0 = OUTPUT;
    PORTDbits.RD0 = LOW;
    while(TRUE) {
        PORTDbits.RD0 = !LATDbits.LATD0;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
        delay_cs( 100 );
    }
}

static void task_2() {
    TRISDbits.RD1 = OUTPUT;
    PORTDbits.RD1 = LOW;
    while(TRUE) {
        PORTDbits.RD1 = !LATDbits.LATD1;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
    }
}

static void task_3() {
    TRISDbits.RD2 = OUTPUT;
    PORTDbits.RD2 = LOW;
    while(TRUE) {
        PORTDbits.RD2 = !LATDbits.LATD2;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
    }
}

#else

semaphore_t g_sem_1;
semaphore_t g_sem_2;
message_queue_t descriptor;

static void task_1_priority() {
    
    TRISA = 0x00;
    PORTA = 0x00;
    
    semaphore_init( &g_sem_1, 0 );
    semaphore_init( &g_sem_2, 0 );
    message_queue_init( &descriptor );
    uint8_t buff_write = 0;
    uint8_t buff;
    message_queue_write(descriptor, &buff_write, 1);
    while(TRUE) {
//        semaphore_wait( &g_sem_1 );
//        message_queue_read(descriptor, &buff, 1);
//        PORTA = buff;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
        delay_cs( 100 );
//        buff_write++;
//        message_queue_write(descriptor, &buff_write, 1);
    }
}

static void task_2_priority() {

    
    TRISB = 0x00;
    PORTB = 0x00;
    
    uint8_t buff_write = 1;
    uint8_t buff;
    message_queue_write(descriptor, &buff_write, 1);
    while(TRUE) {
//        semaphore_wait( &g_sem_2 );
//        message_queue_read(descriptor, &buff, 1);
//        PORTB = buff;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }
//        buff_write++;
//        message_queue_write(descriptor, &buff_write, 1);        
//        semaphore_signal( &g_sem_1 );
    }
}

static void task_3_priority() {
    
    TRISC = 0x00;
    PORTC = 0x00;
    
    uint8_t buff_write = 2;
    uint8_t buff;
    message_queue_write(descriptor, &buff_write, 1);
    while(TRUE) {
//        message_queue_read(descriptor, &buff, 1);
//        PORTC = buff;
        for( uint16_t i = 0; i < 60000; i++ ) { NOP(); }        
//        buff_write++;
//        message_queue_write(descriptor, &buff_write, 1);
//        semaphore_signal( &g_sem_2 );
    }
}

#endif

typedef struct
{
	uint8_t valor_8bits;
	uint16_t valor_16bits;
	char nome[15];
}teste_t;

int main(void) {    
    nojo_init();
#if SCHEDULER == SCHEDULER_ROUND_ROBIN
    asm("GLOBAL _task_1, _task_2, _task_3, _task_idle_callback");
    task_create( 1, 200, task_1 );
    task_create( 2, 100, task_2 );
    task_create( 3,  50, task_3 );
#else
    asm("GLOBAL _task_1_priority, _task_2_priority, _task_3_priority, _task_idle_callback");
    task_create( 1, 200, task_1_priority );
    task_create( 2, 100, task_2_priority );
    task_create( 3,  50, task_3_priority );
#endif

	//printf("Iniciando\n");
	MEU_format();
	//printf("Finalizado\n\n");
	MEU_FILE *A;
	teste_t teste_write[2], teste_read[2];

    A = MEU_fopen("UFSC0","w");


	if (A == NULL)
    {
        //printf("Nao consegue criar arquivo novo\n");
        return(1);
    }
    else
    {
    	//printf("ok\n\n");
    }  

    //Arquivo A
    //printf("Arquivo A\n");
	teste_write[0].valor_8bits = 100;
	teste_write[0].valor_16bits = 3578;
	strcpy(teste_write[0].nome, "Novo Nome");

	teste_write[1].valor_8bits = 246;
	teste_write[1].valor_16bits = 4578;
	strcpy(teste_write[1].nome, "Vou Textar Aqui");

    int a = MEU_fwrite(teste_write, sizeof(teste_t), 2, A);
    MEU_fseek(A, 0, SEEK_SET);
    MEU_fread(teste_read, sizeof(teste_t), 5, A);   	
	

	MEU_fclose(A);

    
    nojo_start();
    
    while(1) {
        asm("nop");
    }
    
    return 0;
}
