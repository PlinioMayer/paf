#ifndef IO_H
#define IO_H

#include <stdint.h>

typedef struct arquivo_t
{
    char nome[256];   // 256 bytes
    int8_t diretorio; // 1 byte
    uint64_t pai;     // 8 bytes
    uint64_t proximo; // 8 bytes
} arquivo_t;

extern uint64_t ultimo_endereco;

void init_io(char *file_name);
void free_io();

uint64_t escrever(const arquivo_t *arquivo);

#endif // IO_H