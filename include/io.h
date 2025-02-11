#ifndef IO_H
#define IO_H
#define FLAG_SIZE 1
#define ARQUIVO_SIZE 273
#define ENDERECO_SIZE 8
#define DATA_SIZE 32000

#include <stdint.h>
#include <stdio.h>

typedef enum tipo_informacao
{
    DATA = 0,
    ARQUIVO = 1
} tipo_informacao;

typedef struct flag_t
{
    uint8_t lixo : 6;
    uint8_t usado : 1;
    tipo_informacao tipo : 1;
} flag_t;

typedef struct arquivo_t
{
    char nome[256];    // 256 bytes
    uint8_t diretorio; // 1 byte
    uint64_t pai;      // 8 bytes
    uint64_t conteudo; // 8 bytes
} arquivo_t;

extern uint64_t file_pointer;

void init_io(char *file_name);
void free_io();

arquivo_t *ler_prox_arquivo();
void escrever(const arquivo_t *arquivo);
void escrever_conteudo(arquivo_t *arquivo, const uint64_t tamanho, const uint8_t *conteudo);

#endif // IO_H