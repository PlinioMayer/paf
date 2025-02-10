#include "arquivo.h"
#include "string.utils.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

mem_arquivo_t *root = NULL;
mem_arquivo_t *arquivo_atual = NULL;

static void add_filho(mem_arquivo_t *pai, mem_arquivo_t *filho)
{
    pai->filhos = realloc(pai->filhos, (pai->filhos_count + 1) * sizeof(mem_arquivo_t *));
    pai->filhos[pai->filhos_count++] = filho;
}

void init_arquivo()
{
    mem_arquivo_t **mem_arquivos = calloc(0, sizeof(mem_arquivo_t *));
    uint64_t i = 0, j = 0, min = 0, max = 0, mem_arquivos_count = 0;
    flag_t *flag = NULL;

    while (flag = ler_flag())
    {
        mem_arquivos = realloc(mem_arquivos, sizeof(mem_arquivo_t *) * (mem_arquivos_count + 1));
        mem_arquivos[mem_arquivos_count] = calloc(1, sizeof(mem_arquivo_t));
        mem_arquivos[mem_arquivos_count]->diretorio = flag->diretorio;
        mem_arquivos[mem_arquivos_count]->endereco = ultimo_endereco - 1;
        mem_arquivos[mem_arquivos_count]->filhos_count = 0;
        mem_arquivos[mem_arquivos_count]->arquivo = ler_arquivo();
        mem_arquivos[mem_arquivos_count]->filhos = calloc(mem_arquivos[mem_arquivos_count]->filhos_count, sizeof(mem_arquivo_t *));
        mem_arquivos_count++;
    }

    if (!mem_arquivos_count)
    {
        arquivo_atual = root = add_diretorio(NULL, "/");
        return;
    }

    arquivo_atual = root = mem_arquivos[0];

    for (i = 1; i < mem_arquivos_count; i++)
    {
        min = 0;
        max = mem_arquivos_count;

        while (TRUE)
        {
            j = (min + max) / 2;

            if (mem_arquivos[j]->endereco == mem_arquivos[i]->arquivo->pai)
            {
                mem_arquivos[i]->pai = mem_arquivos[j];
                add_filho(mem_arquivos[j], mem_arquivos[i]);
                break;
            }

            if (mem_arquivos[i]->arquivo->pai < mem_arquivos[j]->endereco)
                max = j;
            else
                min = j;
        }
    }
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

    strncpy(arquivo->nome, nome, 256);
    arquivo->pai = pai ? pai->endereco : 0;

    mem_arquivo->endereco = escrever(TRUE, arquivo);
    mem_arquivo->diretorio = TRUE;
    mem_arquivo->filhos_count = 0;
    mem_arquivo->arquivo = arquivo;
    mem_arquivo->pai = pai;
    mem_arquivo->filhos = calloc(mem_arquivo->filhos_count, sizeof(mem_arquivo_t *));

    if (pai)
        add_filho(pai, mem_arquivo);

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
    char *nome = NULL, *temp = calloc(strlen(caminho) + 1, sizeof(char));

    strcpy(temp, caminho);

    arquivo = caminho[0] == '/' ? root : arquivo_atual;
    nome = strtok(temp, "/");

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
    char *caminho_pai = NULL, *nome = NULL;
    comando_info_t *comando_info = calloc(1, sizeof(comando_info_t));
    int16_t last_index = -1;
    uint8_t caminho_length = strlen(caminho);

    last_index = last_index_of(caminho, '/');

    if (last_index < 0)
    {
        comando_info->pai = arquivo_atual;

        nome = calloc(caminho_length + 1, sizeof(char));
        strcpy(nome, caminho);
        comando_info->nome = nome;
    }
    else
    {
        caminho_pai = calloc(last_index + 2, sizeof(char));
        strncpy(caminho_pai, caminho, last_index + 1);
        comando_info->pai = buscar_arquivo(caminho_pai);

        if (!comando_info->pai)
        {
            return NULL;
        }

        nome = calloc(caminho_length - last_index, sizeof(char));
        strcpy(nome, caminho + last_index + 1);
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