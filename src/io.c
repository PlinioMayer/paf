#include "io.h"
#include <stdio.h>
#include <stdlib.h>

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
    uint8_t diretorio = 0;
    if (fread(&diretorio, 1, 1, file))
    {
        flag = calloc(1, sizeof(flag_t));
        flag->lixo = 0;
        flag->diretorio = diretorio & (uint8_t)1;
        ultimo_endereco += FLAG_SIZE;
    }
    return flag;
}

arquivo_t *ler_arquivo()
{
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));
    fread(arquivo, sizeof(arquivo_t), 1, file);
    ultimo_endereco += ARQUIVO_SIZE;
    return arquivo;
}

uint64_t escrever(const uint8_t diretorio, const arquivo_t *arquivo)
{
    uint64_t endereco_atual = ultimo_endereco;
    fwrite(&diretorio, 1, 1, file);
    ultimo_endereco += FLAG_SIZE;
    fwrite(arquivo, sizeof(arquivo_t), 1, file);
    ultimo_endereco += ARQUIVO_SIZE;
    return endereco_atual;
}
