#ifndef ARQUIVO_H
#define ARQUIVO_H
#define SIZE_MAX (size_t)-1

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct arquivo_t
{
    char nome[256];
    char pai[256];
    bool diretorio;
} arquivo_t;

typedef struct mem_arquivo_t
{
    arquivo_t *arquivo;
    struct mem_arquivo_t *pai;
    struct mem_arquivo_t **filhos;
    uint64_t filhos_count;
} mem_arquivo_t;

typedef struct comando_info_t
{
    char *nome;
    mem_arquivo_t *pai;
} comando_info_t;

extern mem_arquivo_t *arquivo_atual;

mem_arquivo_t *add_root();
mem_arquivo_t *add_arquivo(mem_arquivo_t *arquivo_pai, const bool diretorio, const char *nome);
mem_arquivo_t *buscar_filho(const mem_arquivo_t *pai, const char *nome);
mem_arquivo_t *buscar_arquivo(const char *caminho);
comando_info_t *obter_comando_info(const char *caminho);

void print_arquivos();
void free_arquivos();

#endif // ARQUIVO_H