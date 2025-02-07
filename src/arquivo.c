#include "arquivo.h"
#include "string.utils.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

mem_arquivo_t *root = NULL;
mem_arquivo_t *arquivo_atual = NULL;

void init_arquivo()
{
    arquivo_atual = root = calloc(1, sizeof(mem_arquivo_t));
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));

    strcpy(arquivo->nome, "/");
    arquivo->diretorio = TRUE;
    arquivo->pai = 0;
    arquivo->proximo = 0;

    arquivo_atual->endereco;
    arquivo_atual->filhos_count = 0;
    arquivo_atual->arquivo = arquivo;
    arquivo_atual->pai = NULL;
    arquivo_atual->filhos = calloc(arquivo_atual->filhos_count, sizeof(mem_arquivo_t *));
}

void free_arquivo(mem_arquivo_t *mem_arquivo)
{
    uint64_t i = 0;
    for (i = 0; i < mem_arquivo->filhos_count; i++)
    {
        free_arquivo(mem_arquivo->filhos[i]);
    }

    free(mem_arquivo->arquivo);
    free(mem_arquivo->filhos);
    free(mem_arquivo);
}

mem_arquivo_t *add_diretorio(mem_arquivo_t *pai, const char *nome)
{
    mem_arquivo_t *mem_arquivo = calloc(1, sizeof(mem_arquivo_t));
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));

    strncpy(arquivo->nome, nome, 255);
    arquivo->diretorio = TRUE;
    arquivo->pai = pai->endereco;
    arquivo->proximo = 0;

    mem_arquivo->endereco = escrever(arquivo);
    mem_arquivo->filhos_count = 0;
    mem_arquivo->arquivo = arquivo;
    mem_arquivo->pai = pai;
    mem_arquivo->filhos = calloc(mem_arquivo->filhos_count, sizeof(mem_arquivo_t *));

    pai->filhos = realloc(pai->filhos, (pai->filhos_count + 1) * sizeof(mem_arquivo_t *));
    pai->filhos[pai->filhos_count++] = mem_arquivo;

    return mem_arquivo;
}

mem_arquivo_t *buscar_filho(const mem_arquivo_t *pai, const char *nome)
{
    uint64_t i = 0;
    mem_arquivo_t *filho = NULL;
    for (i = 0; i < pai->filhos_count; i++)
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

    arquivo = trimmed[0] == '/' ? root : arquivo_atual;
    nome = strtok(trimmed, "/");

    while (nome)
    {
        if (!strcmp(nome, "."))
        {
            nome = strtok(NULL, "/");
            continue;
        }

        if (!strcmp(nome, ".."))
        {
            arquivo = arquivo->pai;
            nome = strtok(NULL, "/");
            continue;
        }

        break;
    }

    if (!nome)
    {
        return arquivo;
    }

    if (!(arquivo = buscar_filho(arquivo, nome)))
    {
        return NULL;
    }

    while (nome = strtok(NULL, "/"))
    {
        if (!(arquivo = buscar_filho(arquivo, nome)))
        {
            return NULL;
        }
    }

    return arquivo;
}

comando_info_t *obter_comando_info(const char *caminho)
{
    char *trimmed, *caminho_pai, *nome;
    comando_info_t *comando_info = calloc(1, sizeof(comando_info_t));
    int16_t last_index = -1;
    uint8_t trimmed_length;

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
            return NULL;
        }

        nome = calloc(trimmed_length - last_index, sizeof(char));
        strcpy(nome, trimmed + last_index + 1);
        comando_info->nome = nome;
    }

    return comando_info;
}

// void print_arquivos()
// {
//     int8_t i, j = 0;
//     for (i = 0; i < 50; i++)
//     {
//         printf("#");
//     }

//     printf("#\n");
//     printf("#");

//     for (i = 0; i < 49; i++)
//     {
//         printf(" ");
//     }

//     printf("#\n");

//     printf("# nome: %s", root->arquivo->nome);

//     for (j = strlen(root->arquivo->nome); j < 42; j++)
//     {
//         printf(" ");
//     }

//     printf("#\n");

//     for (i = 1; i < arquivos_count; i++)
//     {
//         printf("#");

//         for (j = 0; j < 49; j++)
//         {
//             printf("-");
//         }

//         printf("#\n");

//         printf("# pai: %s", arquivos[i]->pai->arquivo->nome);

//         for (j = strlen(arquivos[i]->pai->arquivo->nome); j < 43; j++)
//         {
//             printf(" ");
//         }

//         printf("#\n");

//         printf("# nome: %s", arquivos[i]->arquivo->nome);

//         for (j = strlen(arquivos[i]->arquivo->nome); j < 42; j++)
//         {
//             printf(" ");
//         }

//         printf("#\n");
//     }

//     for (i = 0; i < 50; i++)
//     {
//         printf("#");
//     }
//     printf("#\n");
// }