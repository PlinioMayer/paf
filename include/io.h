#ifndef IO_H
#define IO_H
#define FLAG_SIZE 1
#define ARQUIVO_SIZE 273
#define DATA_SIZE 31726

#include <stdint.h>

typedef struct flag_t
{
    uint8_t lixo : 7;
    uint8_t arquivo : 1;
} flag_t;

typedef struct arquivo_t
{
    char nome[256];    // 256 bytes
    uint8_t diretorio; // 1 byte
    uint64_t pai;      // 8 bytes
    uint64_t conteudo; // 8 bytes
} arquivo_t;

extern uint64_t ultimo_endereco;

void init_io(char *file_name);
void free_io();

flag_t *ler_flag();
arquivo_t *ler_prox_arquivo();
void escrever(const arquivo_t *arquivo);

#endif // IO_H