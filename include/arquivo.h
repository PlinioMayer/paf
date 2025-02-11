#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdlib.h>
#include <stdint.h>
#include "io.h"
#include "boolean.h"

typedef struct mem_arquivo_t
{
    uint64_t endereco;
    uint64_t filhos_count;
    arquivo_t *arquivo;
    struct mem_arquivo_t *pai;
    struct mem_arquivo_t **filhos;
} mem_arquivo_t;

typedef struct comando_info_t
{
    char *nome;
    mem_arquivo_t *pai;
} comando_info_t;

extern mem_arquivo_t *arquivo_atual;
extern mem_arquivo_t *root;

void init_arquivo();
void free_arquivo();

mem_arquivo_t *add_diretorio(mem_arquivo_t *pai, const char *nome);
mem_arquivo_t *buscar_filho(const mem_arquivo_t *pai, const char *nome);
mem_arquivo_t *buscar_arquivo(const char *caminho);
comando_info_t *obter_comando_info(const char *caminho);

void print_arquivos();

#endif // ARQUIVO_H