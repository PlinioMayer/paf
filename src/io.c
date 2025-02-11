#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"

uint64_t file_pointer = 0;

FILE *file;

static flag_t *ler_flag()
{
    flag_t *flag = NULL;
    uint8_t bits = 0;
    if (fread(&bits, 1, 1, file))
    {
        flag = calloc(1, sizeof(flag_t));
        flag->lixo = 0;
        flag->usado = (bits & (uint8_t)2) >> 1;
        flag->tipo = bits & (uint8_t)1;
        file_pointer += FLAG_SIZE;
    }
    return flag;
}

static void escrever_flag(const flag_t *flag)
{
    uint8_t bits = 0;

    if (flag->usado)
        bits += 2;

    if (flag->tipo)
        bits += 1;

    fwrite(&bits, FLAG_SIZE, 1, file);
    file_pointer += FLAG_SIZE;
}

static void escrever_endereco(uint64_t endereco)
{
    fwrite(&endereco, ENDERECO_SIZE, 1, file);
    file_pointer += ENDERECO_SIZE;
}

void init_io(char *file_name)
{
    file = fopen(file_name, "rb+");

    if (file == NULL)
    {
        file = fopen(file_name, "wb");
    }
}

void free_io()
{
    fclose(file);
}

arquivo_t *ler_prox_arquivo()
{

    flag_t *flag = NULL;
    arquivo_t *arquivo = NULL;

    while (TRUE)
    {
        if (!(flag = ler_flag()))
            return NULL;

        if (!flag->tipo)
        {
            fseek(file, DATA_SIZE, SEEK_CUR);
            file_pointer += DATA_SIZE;
            continue;
        }

        arquivo = calloc(1, sizeof(arquivo_t));
        fread(arquivo, sizeof(arquivo_t), 1, file);
        file_pointer += ARQUIVO_SIZE;
        break;
    }

    return arquivo;
}

void escrever(const arquivo_t *arquivo)
{
    flag_t flag = {.usado = 1, .tipo = 1};
    escrever_flag(&flag);
    fwrite(arquivo, sizeof(arquivo_t), 1, file);
    file_pointer += ARQUIVO_SIZE;
}

void escrever_conteudo(arquivo_t *arquivo, const uint64_t tamanho, const uint8_t *conteudo)
{
    flag_t flag = {.usado = 1, .tipo = 1};
    uint64_t i = 0, j = 0;
    escrever_flag(&flag);
    arquivo->conteudo = file_pointer + ARQUIVO_SIZE;
    escrever(arquivo);

    for (i = 0; i < tamanho; i += DATA_SIZE)
    {
        flag.tipo = DATA;
        escrever_flag(DATA);

        for (j = 0; j < DATA_SIZE; j++)
        {
        }

        // escrever_endereco(tamanho - i > 0 ? )
    }
}
