#include "sa_baixo.h"
#include <stdint.h>
#include <string.h>
#include "memoria_EEPROM.h"
#include <pic18.h> 

void mem_write(uint16_t end, uint16_t tam, uint8_t *b)
{
    write_bytes(end, b, tam);
}

void mem_read(uint16_t end, uint16_t tam, uint8_t *b)
{
    read_bytes(end, b, tam);   
}

void format (void)
{
	entrada_arquivo_t tmp; //Arquivo temporário
	uint8_t buffer[32];
	
	tmp.status = 0;
	strcpy(tmp.nome,"NADA");
	tmp.tamanho = 0;
	tmp.inicio = INVALIDO;

	write_bloco_livre(0); //Primeiro bloco livre é o zero

	for (uint8_t x = 0; x < QTD_ARQUIVOS; x++) //Preenchendo todas as entradas como vazia 
		write_entrada_arquivo(x, tmp);

	for (uint16_t x = 0; x < QTD_BLOCOS - 1; x++) //Fazendo com que cada bloco de ponteiro aponte para o próximo
		write_bloco_ponteiro(x, x + 1);

    write_bloco_ponteiro((QTD_BLOCOS - 1), INVALIDO);//Fazendo com que o último bloco de ponteiro aponte para um valor inválido
	
	/*printf("Preenchendo buffer\n");
	for (uint16_t x = 0; x < 32; x++) //Fazendo com que o buffer seja inicializado com zeros
		buffer[x] = 0;

	printf("Preenchendo bloco de dados\n");
	for (uint16_t x = 0; x < QTD_BLOCOS; x++) //Gravando todos os blocos de dados com zeros 
		write_bloco_dados(x, buffer);*/
	
}

tipo_numero_bloco aloca (void) //aloca um bloco de dados
{
	uint16_t b, conteudo;

	b = read_bloco_livre(); //lê o próximo bloco livre
	conteudo = read_bloco_ponteiro(b); //lê o conteúdo do bloco de ponteiro, que é o próximo bloco livre

	write_bloco_livre(conteudo); // escreve o próximo bloco livre
	write_bloco_ponteiro(b, INVALIDO); //grava no bloco de ponteiro INVALIDO, indicando que ele é o ultimo da sua lista

	return b;
}

uint8_t existe(char nome[]) //Lê cada entrada de arquivo e verifica se é o nome é igual ao procurado e o status é válido
{
    entrada_arquivo_t tmp;

    for (uint8_t x=0; x <  QTD_ARQUIVOS;x++)
    {
        read_entrada_arquivo (x, &tmp);
        if (strcmp(tmp.nome,nome) == 0 && tmp.status == 1)
        {
            return x;
        }
    }
    return 255; //Caso não existe
}

uint8_t cria_arquivo(char nome[])
{
    uint8_t x;
    entrada_arquivo_t tmp;

    for (x = 0; x < QTD_ARQUIVOS; x++) //Procura uma entrada livre
    {
        read_entrada_arquivo (x, &tmp);
        if (tmp.status == 0) break;
    }

    if (x == QTD_ARQUIVOS) return 255; //Caso não existirem mais enstradas retorna erro
    
    tmp.status = 1;
    strcpy(tmp.nome,nome);
    tmp.inicio = INVALIDO;
    tmp.tamanho = 0;
    write_entrada_arquivo(x, tmp);

    return x; //retorna a entrada a qual o arquivo foi associado
}

void apagar_arquivo(uint8_t numero_id)
{
/*
typedef struct entrada_arquivo{
	uint8_t status;
	char nome[8];
	uint16_t inicio;
	uint16_t tamanho;
}entrada_arquivo_t;
*/
	entrada_arquivo_t entrada_arquivo;
	tipo_numero_bloco bloco_livre = read_bloco_livre();

	read_entrada_arquivo(numero_id, &entrada_arquivo);
	
	//Buscando o último bloco para fazer com que este aponte para o primeiro bloco livre
	tipo_numero_bloco anterior, atual;
	anterior = atual = entrada_arquivo.inicio;
	for(uint16_t i = 0; i < (entrada_arquivo.tamanho / 32); i++)
	{
		anterior = atual;
		atual = read_bloco_ponteiro (anterior);
	}

	write_bloco_ponteiro(atual, bloco_livre);
	write_bloco_livre (entrada_arquivo.inicio);
	entrada_arquivo.status = 0;
	entrada_arquivo.tamanho = 0;
	entrada_arquivo.inicio = INVALIDO;
	write_entrada_arquivo(numero_id, entrada_arquivo);
}

//   ************* ROTINAS QUE OPERAM SOBRE ENTRADAS ARQUIVOS **************** 
void read_entrada_arquivo (uint8_t numero,  entrada_arquivo_t *A)
{
	mem_read( (2 + numero*(sizeof(entrada_arquivo_t))), (sizeof(entrada_arquivo_t)), (uint8_t *)A);
}

void write_entrada_arquivo (uint8_t numero,  entrada_arquivo_t A)
{
	mem_write( (2 + numero*(sizeof(entrada_arquivo_t))), (sizeof(entrada_arquivo_t)), (uint8_t *)&A);
}
// ************** ROTINAS QUE OPERAM SOBRE  O BLOCO LIVRE
uint16_t read_bloco_livre (void)
{
	uint16_t x; 
	mem_read( 0, 2, (uint8_t *)&x);
	return x;
}

void write_bloco_livre (tipo_numero_bloco numero)
{
	mem_write(0, 2, (uint8_t *)&numero);
}
//   ************* ROTINAS QUE OPERAM SOBRE BLOCOS PONTEIROS **************** 
uint16_t read_bloco_ponteiro (tipo_numero_bloco numero)
{
	uint16_t x; 
	mem_read( (INICIO_PONTEIROS + numero*TAM_BL_PTR), 2, (uint8_t *)&x);
	return x;
}

void write_bloco_ponteiro (tipo_numero_bloco numero, uint16_t x)
{
	mem_write( (uint16_t)(INICIO_PONTEIROS + numero*TAM_BL_PTR), 2, (uint8_t *)&x);
}
//   ************* ROTINAS QUE OPERAM SOBRE BLOCOS DADOS **************** 
void read_bloco_dados (tipo_numero_bloco numero, uint8_t buff[])
{
	mem_read( INICIO_DADOS + numero*TAM_BL_DADOS, TAM_BL_DADOS, buff);
}

void write_bloco_dados (tipo_numero_bloco numero, uint8_t buff[])
{
	mem_write( INICIO_DADOS + numero*TAM_BL_DADOS, TAM_BL_DADOS, buff);
}