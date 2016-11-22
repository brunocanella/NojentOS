#include "sa_baixo.h"
#include "sa.h"
#include "memory.h"
#include <string.h>
#include <stdint.h>

void MEU_format()
{
	format();
}

int MEU_fgetc(MEU_FILE *stream)
{
	uint16_t bloco_fisico, bloco_logico, offset;
	uint8_t buffer[32];

	if(stream->posicao == stream->arq.tamanho)
		return -1; //EOF

	bloco_logico = (stream->posicao) / 32;
	offset       = (stream->posicao) % 32;

	bloco_fisico = stream->arq.inicio;
	for (uint8_t x = 0; x < bloco_logico; x++)
		bloco_fisico = read_bloco_ponteiro(bloco_fisico);

	read_bloco_dados(bloco_fisico, buffer);

	stream->posicao++;
	return (int)buffer[offset];			
}

int MEU_fputc(char c, MEU_FILE *stream)
{

	if(strcmp(stream->modo, "r") == 0)
		return 0;

	uint16_t bloco_fisico, bloco_logico, offset, bloco, valor;
	uint8_t buffer[32];
    uint8_t id = stream->id;
  
	bloco_logico = (stream->posicao) / 32;
	offset       = (stream->posicao) % 32;

	if (bloco_logico == 0)
	{
		if (stream->arq.inicio == INVALIDO)//caso o primeiro bloco ainda não estiver alocado
		{
			stream->arq.inicio = aloca();
			write_entrada_arquivo(id, stream->arq);
		}
		bloco_fisico = stream->arq.inicio;
	}
	else
	{	
		//encontrar quem eh o bloco fisico X		
		bloco = stream->arq.inicio;
		for (uint8_t x = 0; x < bloco_logico; x++)
		{
			valor = read_bloco_ponteiro(bloco);
			/*Pode acontecer de o posição estar no 32, 
			ou seja próximo bloco e ainda não estar alocado o próximo bloco*/
			if (valor == INVALIDO) 	
			{
				valor = aloca();
				write_bloco_ponteiro(bloco, valor);
			}
			bloco = valor;
			bloco_fisico = valor;			
		}	
		//bloco_fisico ok
	}

	read_bloco_dados(bloco_fisico, buffer);
	buffer[offset] = c;
	write_bloco_dados(bloco_fisico, buffer);
	if (stream->posicao == stream->arq.tamanho) //Somente quando a posição e o tamanho forem os mesmos é que o arquivo aumenta
	{
		stream->arq.tamanho++;
		write_entrada_arquivo(stream->id, stream->arq);
	}
	stream->posicao++;
    //escreva_arquivo(id, a->posicao, dado);  
    return 1;  
}

MEU_FILE *MEU_fopen(char *name, char *mode)
{
    MEU_FILE *arquivo = NULL;
	entrada_arquivo_t tmp;
	uint8_t marcador;
    uint8_t id;
        // tenta abrir/criar,etc
        // se consegue, aloca com malloc e retorna
        // caso contrario retorna NULL

    if (strcmp(mode,"w") == 0)
    {
		marcador = existe(name);
	    if (marcador != 255) //EXISTE O ARQUIVO
	    	apagar_arquivo(marcador);
	  
	    // cria o arquivo
	    id = cria_arquivo(name);
	    if(id == 255) return NULL;
	    
	    read_entrada_arquivo(id, &tmp);
	                    
	    arquivo 		 	 = (MEU_FILE *)malloc (sizeof(MEU_FILE));
	    arquivo->id      	 = id;
	    arquivo->posicao 	 = 0;
	    arquivo->arq.status  = tmp.status;
	    arquivo->arq.inicio  = tmp.inicio;
	    arquivo->arq.tamanho = tmp.tamanho;
	    strcpy(arquivo->arq.nome, name);   
	    strcpy(arquivo->modo, "w");
    }
    else if (strcmp(mode,"r") == 0)
    {
    	marcador = existe(name);
    	if(marcador == 255)
    		return NULL;

	    read_entrada_arquivo(marcador, &tmp);
	                
	    arquivo 		 	 = (MEU_FILE *)malloc (sizeof(MEU_FILE));
	    arquivo->id      	 = marcador;
	    arquivo->posicao 	 = 0;
	    arquivo->arq.status  = tmp.status;
	    arquivo->arq.inicio  = tmp.inicio;
	    arquivo->arq.tamanho = tmp.tamanho;
	    strcpy(arquivo->arq.nome, name);   
	    strcpy(arquivo->modo, "r");    	
    }
    else if (strcmp(mode,"r+") == 0)
    {
    	marcador = existe(name);
    	if(marcador == 255)
    		return NULL;

	    read_entrada_arquivo(marcador, &tmp);
	                
	    arquivo 		 	 = (MEU_FILE *)malloc (sizeof(MEU_FILE));
	    arquivo->id      	 = marcador;
	    arquivo->posicao 	 = 0;
	    arquivo->arq.status  = tmp.status;
	    arquivo->arq.inicio  = tmp.inicio;
	    arquivo->arq.tamanho = tmp.tamanho;
	    strcpy(arquivo->arq.nome, name);   
	    strcpy(arquivo->modo, "r+"); 
    }

    return arquivo;
}

int MEU_fclose(MEU_FILE *fp)
{
	if(fp != NULL)
		free(fp);

	return 0;
}

size_t MEU_fread(void *ptr, size_t size, size_t nmemb, MEU_FILE *stream)
{
	uint16_t bloco_logico, bloco_fisico = 0;
	uint8_t buffer[32];
	size_t nmemb_read = 0;

	bloco_logico = (stream->posicao) / 32;
	
	bloco_fisico = stream->arq.inicio;
	for (uint8_t x = 0; x < bloco_logico; x++)
		bloco_fisico = read_bloco_ponteiro(bloco_fisico);

	read_bloco_dados(bloco_fisico, buffer);
	
	uint16_t i = 0;
	for(i = 0; (i < size * nmemb) && (stream->posicao < stream->arq.tamanho); i++)
	{
		((uint8_t*)ptr)[i] = buffer[(stream->posicao) % 32];
		stream->posicao++;

		if((i % size == 0) && (i != 0))
		{
			nmemb_read++;
		}

		if(bloco_logico < stream->posicao / 32)
		{
			bloco_fisico = read_bloco_ponteiro(bloco_fisico);
			read_bloco_dados(bloco_fisico, buffer);
			bloco_logico++;
		}
	}
	if(i != 0) //Só incrementa se tiver lido algo
		nmemb_read++;

	return nmemb_read;
}

size_t MEU_fwrite(void *ptr, size_t size, size_t nmemb, MEU_FILE *stream)
{

	if(strcmp(stream->modo, "r") == 0)
		return 0;

	/*FREAD*/
	uint16_t bloco_logico, bloco_fisico = 0;
	uint8_t buffer[32];
	size_t nmemb_read = 0;

	bloco_logico = (stream->posicao) / 32;

	if (bloco_logico == 0)
	{
		if (stream->arq.inicio == INVALIDO)//caso o primeiro bloco ainda não estiver alocado
		{
			stream->arq.inicio = aloca();
			write_entrada_arquivo(stream->id, stream->arq);
		}
		bloco_fisico = stream->arq.inicio;
		read_bloco_dados(bloco_fisico, buffer);
	}
	else
	{
		bloco_fisico = stream->arq.inicio;
		for (uint8_t x = 0; x < bloco_logico; x++)
			bloco_fisico = read_bloco_ponteiro(bloco_fisico);

		read_bloco_dados(bloco_fisico, buffer);	
	}	

	uint16_t i = 0;
	for(i = 0; i < size * nmemb; i++)
	{
		buffer[(stream->posicao) % 32] = ((uint8_t*)ptr)[i];
		stream->posicao++;

		if((i % size == 0) && (i != 0))
		{
			nmemb_read++;
		}

		if(bloco_logico < stream->posicao / 32)
		{
			write_bloco_dados(bloco_fisico, buffer);
			uint16_t bloco_fisico_tmp = read_bloco_ponteiro(bloco_fisico);
			if (bloco_fisico_tmp == INVALIDO)//caso o primeiro bloco ainda não estiver alocado
			{
				bloco_fisico_tmp = aloca();
				write_bloco_ponteiro(bloco_fisico, bloco_fisico_tmp);
			}
			else
			{
				read_bloco_dados(bloco_fisico, buffer);	
			}
			bloco_fisico = bloco_fisico_tmp;
			bloco_logico++;
		}
	}

	if (stream->posicao > stream->arq.tamanho) //Somente quando a posição for maior que o tamnho o arquivo aumentou
	{
		nmemb_read++;
		stream->arq.tamanho = stream->posicao;
		write_entrada_arquivo(stream->id, stream->arq);
		write_bloco_dados(bloco_fisico, buffer);
	}
	else
	{
		write_bloco_dados(bloco_fisico, buffer);
	}

	return nmemb_read;
}

int MEU_fseek(MEU_FILE *stream, int16_t offset, uint8_t whence)
{
	switch(whence)
	{
		case SEEK_SET:
			if(offset > stream->arq.tamanho) //Se o offset passar da última posição, a partir do início
				return -1;
			stream->posicao = offset;
			return 0;
		break;

		case SEEK_CUR:
			if(offset > (stream->arq.tamanho - stream->posicao)) //Se o offset passar da última posição apartir da corrente
				return -1;
			stream->posicao += offset;
			return 0;		
		break;

		case SEEK_END:
			if((offset > 0) || ((stream->arq.tamanho + offset) < 0)) //Se o offset for maior que zero, ou descontando da posição final menor que zero
				return -1;
			stream->posicao = stream->arq.tamanho + offset;
			return 0;
		break;
	}
	return -1;
}

uint16_t MEU_ftell(MEU_FILE *stream)
{
	return stream->posicao;
}

int MEU_feof(MEU_FILE *stream)
{
	if(stream->posicao == stream->arq.tamanho)
		return stream->posicao;

	return 0;
}