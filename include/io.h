#ifndef IO_H
#define IO_H
#define FLAGS_SIZE (uint8_t)1
#define ENDERECO_SIZE (uint8_t)8
#define ATRIBUTOS_SIZE (uint8_t)1
#define NOME_SIZE (uint16_t)256
#define ARQUIVO_SIZE (uint16_t)(FLAGS_SIZE + ENDERECO_SIZE + ATRIBUTOS_SIZE + NOME_SIZE)

#include "boolean.h"
#include <stdint.h>
#include <stdio.h>

typedef enum tipo_arquivo
{
    DOCUMENTO = 0,
    DIRETORIO = 1
} tipo_arquivo;

typedef struct flags_t
{
    uint8_t lixo : 7;
    boolean usado : 1;
} flags_t;

typedef struct atributos_t
{
    uint8_t lixo : 7;
    tipo_arquivo tipo : 1;
} atributos_t;

typedef struct arquivo_t
{
    uint64_t tamanho; // 8 bytes
    uint64_t pai;           // 8 bytes
    atributos_t *atributos; // 1 byte
    char nome[NOME_SIZE];   // 256 bytes
} arquivo_t;                // 265 bytes

void init_io(char *file_name);
void free_io();

uint64_t get_file_pointer();
arquivo_t *ler_prox_arquivo();
void escrever(const arquivo_t *arquivo);
void remover(const uint64_t endereco);

#endif // IO_H