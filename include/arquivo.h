#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    char nome[256];
    char pai[256];
    bool ultimo;
} arquivo_t;

typedef struct mem_arquivo_t
{
    arquivo_t *arquivo;
    struct mem_arquivo_t *pai;
    struct mem_arquivo_t **filhos;
    size_t filhos_count;
} mem_arquivo_t;

extern mem_arquivo_t *arquivo_atual;

mem_arquivo_t *add_root();
mem_arquivo_t *add_arquivo(mem_arquivo_t *arquivo_pai, char *nome);
void print_arquivos();

#endif // ARQUIVO_H