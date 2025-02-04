#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t arquivos_count;
static mem_arquivo_t **arquivos;
mem_arquivo_t *arquivo_atual;

mem_arquivo_t *add_root()
{
    arquivos_count = 0;
    mem_arquivo_t *mem_arquivo = calloc(1, sizeof(mem_arquivo_t));
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));

    strncpy(arquivo->nome, "/", 256);
    arquivo->ultimo = true;

    mem_arquivo->arquivo = arquivo;
    mem_arquivo->pai = NULL;
    mem_arquivo->filhos_count = 0;
    mem_arquivo->filhos = calloc(mem_arquivo->filhos_count, sizeof(mem_arquivo_t *));

    arquivos = calloc(1, sizeof(mem_arquivo_t *));
    arquivos[arquivos_count++] = mem_arquivo;

    return mem_arquivo;
}

mem_arquivo_t *add_arquivo(mem_arquivo_t *arquivo_pai, char *nome)
{
    mem_arquivo_t *mem_arquivo = calloc(1, sizeof(mem_arquivo_t));
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));

    strncpy(arquivo->nome, nome, 256);
    strncpy(arquivo->pai, arquivo_pai->arquivo->nome, 256);

    mem_arquivo->arquivo = arquivo;
    mem_arquivo->pai = arquivo_pai;
    mem_arquivo->filhos_count = 0;
    mem_arquivo->filhos = calloc(mem_arquivo->filhos_count, sizeof(mem_arquivo_t *));

    arquivo_pai->filhos = realloc(arquivo_pai->filhos, (arquivo_pai->filhos_count + 1) * sizeof(mem_arquivo_t *));
    arquivo_pai->filhos[arquivo_pai->filhos_count++] = mem_arquivo;

    arquivos = realloc(arquivos, (arquivos_count + 1) * sizeof(mem_arquivo_t *));
    arquivos[arquivos_count++] = mem_arquivo;

    return mem_arquivo;
}

void print_arquivos()
{
    for (size_t i = 0; i < 50; i++)
    {
        printf("#");
    }

    printf("#\n");
    printf("#");

    for (size_t i = 0; i < 49; i++)
    {
        printf(" ");
    }

    printf("#\n");

    printf("# nome: %s", arquivos[0]->arquivo->nome);

    for (size_t j = strlen(arquivos[0]->arquivo->nome); j < 42; j++)
    {
        printf(" ");
    }

    printf("#\n");

    for (size_t i = 1; i < arquivos_count; i++)
    {
        printf("#");

        for (size_t j = 0; j < 49; j++)
        {
            printf("-");
        }

        printf("#\n");

        printf("# pai: %s", arquivos[i]->pai->arquivo->nome);

        for (size_t j = strlen(arquivos[i]->pai->arquivo->nome); j < 43; j++)
        {
            printf(" ");
        }

        printf("#\n");

        printf("# nome: %s", arquivos[i]->arquivo->nome);

        for (size_t j = strlen(arquivos[i]->arquivo->nome); j < 42; j++)
        {
            printf(" ");
        }

        printf("#\n");
    }

    for (size_t i = 0; i < 50; i++)
    {
        printf("#");
    }
    printf("#\n");
}

void free_arquivos()
{
    for (int i = 0; i < arquivos_count; i++)
    {
        free(arquivos[i]->arquivo);
        free(arquivos[i]->filhos);
    }
}