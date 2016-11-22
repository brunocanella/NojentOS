
#ifndef _MEMORIA_I2C_
#define _MEMORIA_I2C_
#include <stdint.h>

#ifdef _LINUX_
#include <stdio.h>
#endif

/**
 * @brief The memoria_i2c Classe que abstrai a manipulação de dados na memória permanente
 */
class memoria_i2c {
public:
    /**
     * @brief memoria_i2c Inicializa e formata um espaço de memoria.
     * @param a_id_dispositivo Identificador do dispositivo.
     * @param a_pino_dados Pino que fara a transmissão dos dados.
     * @param a_pino_clock Pino que fará o controle do dispositivo.
     */
    void init( uint8_t a_id_dispositivo, uint8_t a_pino_dados, uint8_t a_pino_clock );
    /**
     * Finaliza a escrita na memória.
     */
    ~memoria_i2c();

    /**
     * @brief read Faz uma leitura de um tamanho especificado da partição, a partir de uma posição inicial.
     * @param a_offset Posição inicial de onde será feita a leitura na partição.
     * @param a_count Quantidade de bytes a serem lidos.
     * @param a_buffer Local onde os dados serão recuperados.
     */
    void read(uint16_t a_offset, uint16_t a_count, uint8_t* a_buffer );
    /**
     * @brief write Faz a escrita de um buffer de tamanho a_count na partição, a partir de uma posição inicial.
     * @param a_offset Posição inicial onde será feita a escrita na partição.
     * @param a_count Quantidade de bytes a serem escritos.
     * @param a_buffer Local de onde originam os dados a serem escritos.
     */
    void write(uint16_t a_offset, uint16_t a_count, uint8_t* a_buffer );
private:
#ifdef _LINUX_
    /**Partição onde serão criados os arquivos*/
    FILE* m_file;
#else
    uint8_t dispositivo;
#endif
};

#endif
