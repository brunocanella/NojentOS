#include "memoria.h"

#ifdef _LINUX_

memoria_i2c::memoria_i2c( uint8_t a_id_dispositivo, uint8_t a_pino_dados, uint8_t a_pino_clock ) {
    // Abre o arquivo da partição para leitura+escrita
    m_file = fopen( "PARTICAO.bin","r+" );
    // Se ele não existir...
    if( m_file == NULL ) {
        // ... cria um arquivo vazio e abre ele para escrita.
        m_file = fopen( "PARTICAO.bin","w+" );
    }
}


memoria_i2c::~memoria_i2c()
{
    // Verifica se ha um arquivo para ser fechado
    if( m_file != NULL ) {
        // Fecha o arquivo e seta a variavel para nulo.
        fclose( m_file );
        m_file = NULL;
    }    
}

void memoria_i2c::read( uint16_t a_offset, uint16_t a_count, uint8_t* a_buffer ) {
    // Posiciona o cursor na posição a_offset, a partir do inicio do arquivo s_file.
    fseek( m_file, a_offset, SEEK_SET );
    // Le um array de 'a_count' elementos (de tamanho igual a 1 byte) do arquivo s_file, e grava eles no a_buffer.
    fread( a_buffer, sizeof(uint8_t), a_count, m_file );
}

void memoria_i2c::write( uint16_t a_offset, uint16_t a_count, uint8_t* a_buffer ) {
    // Posiciona o cursor na posição a_offset, a partir do inicio do arquivo s_file.
    fseek( m_file, a_offset, SEEK_SET );
    // Escreve um array de bytes no arquivo.
    fwrite( a_buffer, sizeof(uint8_t), a_count, m_file );
    // Garante o envio dos dados.
    fflush( m_file );
}

#else

#include "i2c_software.h"

#define TAM_PAGINA 32

#define MIN(a,b) (((a)<(b))?(a):(b))

#include <stdio.h>

void memoria_i2c::init( uint8_t a_id_dispositivo, uint8_t a_pino_dados, uint8_t a_pino_clock ) {
    dispositivo = a_id_dispositivo;
    i2c_configura( a_pino_dados, a_pino_clock );
    i2c_start();
	i2c_stop();
}

memoria_i2c::~memoria_i2c() {}

void memoria_i2c::read(uint16_t end, uint16_t tam, uint8_t *b)
{
	uint16_t R, qtd, contador=0;

	while (tam>0)
	{
		R = (end / TAM_PAGINA) * TAM_PAGINA +TAM_PAGINA;
		qtd = i2c_sequencial_read (dispositivo, end, b+contador, MIN(R - end, tam));
		contador+=qtd;
		end+=qtd;  tam-=qtd;
	}
}

void memoria_i2c::write(uint16_t end, uint16_t tam, uint8_t *b)
{
	uint16_t R, qtd,  contador=0;

	while (tam>0)
	{
		R = (end / TAM_PAGINA) * TAM_PAGINA +TAM_PAGINA;
		qtd = i2c_sequencial_write( dispositivo, end, b+contador, MIN(R - end, tam) );
		end+=qtd;  tam-=qtd;
		contador+=qtd;
	}
}

#endif

