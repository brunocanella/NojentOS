
#ifndef _SA_BAIXO_
#define _SA_BAIXO_
#include <stdint.h>
#define TAM_MEMORIA 256
#define INVALIDO 0xFFFF

#define tipo_numero_bloco uint16_t
#define tipo_tamanho      uint16_t

// typefdef struct 
//
//}Tipo tststst;


#pragma pack(1)
typedef struct entrada_arquivo{
	uint8_t status;
	char nome[8];
	uint16_t inicio;
	uint16_t tamanho;
}entrada_arquivo_t;



#define QTD_ARQUIVOS 2
#define TAM_ENTRADAS (QTD_ARQUIVOS*(sizeof(struct entrada_arquivo)))

#define TAM_CABECALHO (sizeof(tipo_numero_bloco))
#define TAM_BL_DADOS 32
#define TAM_BL_PTR    2
#define TAM_BLOCOS (TAM_BL_DADOS+TAM_BL_PTR)


#define QTD_BLOCOS  ((TAM_MEMORIA-(TAM_ENTRADAS+TAM_CABECALHO))/TAM_BLOCOS)

#define INICIO_PONTEIROS (TAM_ENTRADAS+TAM_CABECALHO)
#define INICIO_DADOS      (INICIO_PONTEIROS + QTD_BLOCOS*TAM_BL_PTR)

tipo_numero_bloco aloca (void);
void format (void);
uint16_t read_bloco_ponteiro (tipo_numero_bloco numero);
void write_bloco_ponteiro (tipo_numero_bloco numero, uint16_t x);
uint16_t read_bloco_livre (void);
void write_bloco_livre (tipo_numero_bloco numero);
void read_entrada_arquivo (uint8_t numero,  struct entrada_arquivo *A);
void write_entrada_arquivo (uint8_t numero,  struct entrada_arquivo A);
uint8_t existe(char nome[]);
uint8_t cria_arquivo(char nome[]);
void apagar_arquivo(uint8_t numero_id);
void read_bloco_dados (tipo_numero_bloco numero, uint8_t buff[]);
void write_bloco_dados (tipo_numero_bloco numero, uint8_t buff[]);

#endif
