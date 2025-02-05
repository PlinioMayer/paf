#include "arquivo.h"
#include "string.utils.h"
#include "log.h"
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

mem_arquivo_t *add_arquivo(mem_arquivo_t *arquivo_pai, const char *nome)
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

mem_arquivo_t *buscar_filho(const mem_arquivo_t *pai, const char *nome)
{
    mem_arquivo_t *filho = NULL;
    for (int i = 0; i < pai->filhos_count; i++)
    {
        if (!strcmp(pai->filhos[i]->arquivo->nome, nome))
        {
            filho = pai->filhos[i];
            break;
        }
    }
    return filho;
}

mem_arquivo_t *buscar_arquivo(const char *caminho)
{
    mem_arquivo_t *arquivo = NULL;
    char *trimmed, *nome;

    trimmed = trim(caminho);

    if (!trimmed || !strlen(trimmed))
    {
        return NULL;
    }

    arquivo = trimmed[0] == '/' ? arquivos[0] : arquivo_atual;
    nome = strtok(trimmed, "/");

    while (nome)
    {
        if (!strcmp(nome, "."))
            continue;

        if (!strcmp(nome, ".."))
        {
            arquivo = arquivo->pai;
            continue;
        }

        break;
    }

    if (!nome)
    {
        free(trimmed);
        return arquivo;
    }

    if (!(arquivo = buscar_filho(arquivo, nome)))
    {
        free(trimmed);
        return NULL;
    }

    while (nome = strtok(NULL, "/"))
    {
        if (!(arquivo = buscar_filho(arquivo, nome)))
        {
            free(trimmed);
            return NULL;
        }
    }

    free(trimmed);
    return arquivo;
}

comando_info_t *obter_comando_info(const char *caminho)
{
    char *trimmed, *caminho_pai, *nome;
    comando_info_t *comando_info = calloc(1, sizeof(comando_info_t));
    int last_index = -1;
    size_t trimmed_length;

    trimmed = trim(caminho);

    if (!trimmed || !(trimmed_length = strlen(trimmed)))
    {
        return NULL;
    }

    last_index = last_index_of(trimmed, '/');

    if (last_index < 0)
    {
        comando_info->pai = arquivo_atual;

        nome = calloc(trimmed_length + 1, sizeof(char));
        strcpy(nome, trimmed);
        comando_info->nome = nome;
    }
    else
    {
        caminho_pai = calloc(last_index + 2, sizeof(char));
        strncpy(caminho_pai, trimmed, last_index + 1);
        comando_info->pai = buscar_arquivo(caminho_pai);

        if (!comando_info->pai)
        {
            free(trimmed);
            return NULL;
        }

        nome = calloc(trimmed_length - last_index, sizeof(char));
        strcpy(nome, trimmed + last_index + 1);
        comando_info->nome = nome;
    }

    free(trimmed);
    return comando_info;
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

    free(arquivos);
}