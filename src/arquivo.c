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

static mem_arquivo_t *add_arquivo(mem_arquivo_t *pai, atributos_t *atributos, const char *nome)
{
    mem_arquivo_t *mem_arquivo = calloc(1, sizeof(mem_arquivo_t));
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));

    strncpy(arquivo->nome, nome, 256);
    arquivo->pai = pai ? pai->endereco : 0;
    arquivo->atributos = atributos;

    escrever(arquivo);
    mem_arquivo->endereco = get_file_pointer() - ARQUIVO_SIZE;
    mem_arquivo->filhos_count = 0;
    mem_arquivo->arquivo = arquivo;
    mem_arquivo->pai = pai;
    mem_arquivo->filhos = NULL;

    if (pai)
        add_filho(pai, mem_arquivo);

    return mem_arquivo;
}

void init_arquivo()
{
    arquivo_t *arquivo = NULL;
    mem_arquivo_t **mem_arquivos = calloc(0, sizeof(mem_arquivo_t *));
    uint64_t i = 0, j = 0, min = 0, max = 0, mem_arquivos_count = 0;

    while (arquivo = ler_prox_arquivo())
    {
        mem_arquivos = realloc(mem_arquivos, sizeof(mem_arquivo_t *) * (mem_arquivos_count + 1));
        mem_arquivos[mem_arquivos_count] = calloc(1, sizeof(mem_arquivo_t));
        mem_arquivos[mem_arquivos_count]->endereco = get_file_pointer() - ARQUIVO_SIZE;
        mem_arquivos[mem_arquivos_count]->arquivo = arquivo;
        mem_arquivos[mem_arquivos_count]->filhos_count = 0;
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
    free(mem_arquivos);
}

void free_arquivo(mem_arquivo_t *mem_arquivo)
{
    uint64_t i = 0;
    for (i = 0; i < mem_arquivo->filhos_count; i++)
    {
        free_arquivo(mem_arquivo->filhos[i]);
    }

    if (mem_arquivo->arquivo->atributos->tipo == DIRETORIO)
        free(mem_arquivo->filhos);

    free(mem_arquivo->arquivo->atributos);
    free(mem_arquivo->arquivo);
    free(mem_arquivo);
}

mem_arquivo_t *add_diretorio(mem_arquivo_t *pai, const char *nome)
{
    atributos_t *atributos = calloc(1, sizeof(atributos_t));
    mem_arquivo_t *mem_arquivo = NULL;

    atributos->tipo = DIRETORIO;
    mem_arquivo = add_arquivo(pai, atributos, nome);
    mem_arquivo->filhos = calloc(mem_arquivo->filhos_count, sizeof(mem_arquivo_t *));

    return mem_arquivo;
}

mem_arquivo_t *add_documento(mem_arquivo_t *pai, const char *nome)
{
    atributos_t *atributos = calloc(1, sizeof(atributos_t));
    mem_arquivo_t *mem_arquivo = NULL;

    atributos->tipo = DOCUMENTO;
    mem_arquivo = add_arquivo(pai, atributos, nome);
    mem_arquivo->filhos = calloc(mem_arquivo->filhos_count, sizeof(mem_arquivo_t *));

    return mem_arquivo;
}

void remover_arquivo(mem_arquivo_t *mem_arquivo)
{
    uint64_t i = 0;

    free(mem_arquivo->arquivo->atributos);
    free(mem_arquivo->arquivo);
    remover(mem_arquivo->endereco);

    for (i = 0; i < mem_arquivo->filhos_count; i++)
        remover_arquivo(mem_arquivo->filhos[i]);

    for (i = 0; i < mem_arquivo->pai->filhos_count; i++)
    {
        if (mem_arquivo->pai->filhos[i] == mem_arquivo)
        {
            i++;
            break;
        }
    }

    for (; i < mem_arquivo->pai->filhos_count; i++)
        mem_arquivo->pai->filhos[i - 1] = mem_arquivo->pai->filhos[i];

    mem_arquivo->pai->filhos_count--;

    free(mem_arquivo);
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
            free(caminho_pai);
            free(comando_info);
            return NULL;
        }

        nome = calloc(caminho_length - last_index, sizeof(char));
        strcpy(nome, caminho + last_index + 1);
        comando_info->nome = nome;
    }

    return comando_info;
}
