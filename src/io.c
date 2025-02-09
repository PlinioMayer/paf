#include "io.h"
#include <stdio.h>
#include <stdlib.h>

uint64_t ultimo_endereco = 0;

static FILE *file;

void init_io(char *file_name)
{
    ultimo_endereco = 0;
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

int8_t ler_se_diretorio()
{
    uint8_t diretorio = 0;
    return fread(&diretorio, 1, 1, file) ? diretorio : -1;
}

arquivo_t *ler_arquivo()
{
    arquivo_t *arquivo = calloc(1, sizeof(arquivo_t));
    fread(arquivo, sizeof(arquivo), 1, file);
    ultimo_endereco++;
    return arquivo;
}

uint64_t escrever(const uint8_t diretorio, const arquivo_t *arquivo)
{
    fwrite(&diretorio, 1, 1, file);
    fwrite(arquivo, sizeof(arquivo), 1, file);
    return ultimo_endereco += 273;
}
