#include "io.h"
#include <stdio.h>

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

uint64_t escrever(const arquivo_t *arquivo)
{
    return ultimo_endereco += 1;
}
