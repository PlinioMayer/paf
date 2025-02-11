#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"

uint64_t ultimo_endereco = 0;

static FILE *file;

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

flag_t *ler_flag()
{
    flag_t *flag = NULL;
    uint8_t arquivo = 0;
    if (fread(&arquivo, 1, 1, file))
    {
        flag = calloc(1, sizeof(flag_t));
        flag->lixo = 0;
        flag->arquivo = arquivo & (uint8_t)1;
        ultimo_endereco += FLAG_SIZE;
    }
    return flag;
}

arquivo_t *ler_prox_arquivo()
{

    flag_t *flag = NULL;
    arquivo_t *arquivo = NULL;

    while (TRUE)
    {
        if (!(flag = ler_flag()))
            return NULL;

        if (!flag->arquivo)
        {
            fseek(file, DATA_SIZE, SEEK_CUR);
            ultimo_endereco += DATA_SIZE;
            continue;
        }

        arquivo = calloc(1, sizeof(arquivo_t));
        fread(arquivo, sizeof(arquivo_t), 1, file);
        ultimo_endereco += ARQUIVO_SIZE;
        break;
    }

    return arquivo;
}

void escrever(const arquivo_t *arquivo)
{
    fwrite(&(int){1}, 1, 1, file);
    ultimo_endereco += FLAG_SIZE;
    fwrite(arquivo, sizeof(arquivo_t), 1, file);
    ultimo_endereco += ARQUIVO_SIZE;
}
