
#ifndef _SA_
#define _SA_
#include <stdint.h>
#include <stddef.h>
#include "sa_baixo.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct {
    uint8_t  id;
    uint16_t posicao;
    char modo[3];
    entrada_arquivo_t arq;    
}MEU_FILE;

void MEU_init();
void MEU_format();

MEU_FILE *MEU_fopen(char *name, char *mode);
int MEU_fclose(MEU_FILE *fp); //Feito

int MEU_fgetc(MEU_FILE *stream); //Feito
int MEU_fputc(char c, MEU_FILE *stream); //Feito

size_t MEU_fread(void *ptr, size_t size, size_t nmemb, MEU_FILE *stream); //Feito
size_t MEU_fwrite(void *ptr, size_t size, size_t nmemb, MEU_FILE *stream); //Feito

int MEU_fseek(MEU_FILE *stream, int16_t offset, uint8_t whence); //Feito
uint16_t MEU_ftell(MEU_FILE *stream); //Feito
int MEU_feof(MEU_FILE *stream); //Feito

#endif