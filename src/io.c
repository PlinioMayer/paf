#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"

FILE *file;

static flags_t *ler_flags()
{
    flags_t *flags = NULL;
    uint8_t bits = 0;
    if (fread(&bits, FLAG_SIZE, 1, file))
    {
        flags = calloc(1, sizeof(flags_t));
        flags->lixo = 0;
        flags->usado = (bits & (uint8_t)2) >> 1;
        flags->tipo = bits & (uint8_t)1;
    }
    return flags;
}

static void escrever_flags(const flags_t *flags)
{
    uint8_t bits = 0;

    if (flags->usado)
        bits += 2;

    if (flags->tipo)
        bits += 1;

    fwrite(&bits, FLAG_SIZE, 1, file);
}

static uint64_t ler_endereco()
{
    uint64_t endereco = 0;
    fread(&endereco, ENDERECO_SIZE, 1, file);
    return endereco;
}

static void escrever_endereco(const uint64_t endereco)
{
    fwrite(&endereco, ENDERECO_SIZE, 1, file);
}

static void ler_nome(char nome[NOME_SIZE])
{
    fread(nome, 1, NOME_SIZE, file);
}

static void escrever_nome(const char *nome)
{
    fwrite(nome, 1, NOME_SIZE, file);
}

static atributos_t *ler_atributos()
{
    atributos_t *atributos = NULL;
    uint8_t bits = 0;

    if (fread(&bits, ATRIBUTOS_SIZE, 1, file))
    {
        atributos = calloc(1, sizeof(atributos_t));
        atributos->lixo = 0;
        atributos->diretorio = bits & (uint8_t)1;
    }

    return atributos;
}

static void escrever_atributos(const atributos_t *atributos)
{
    uint8_t bits = 0;

    if (atributos->diretorio)
        bits += 1;

    fwrite(&bits, ATRIBUTOS_SIZE, 1, file);
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

uint64_t get_file_pointer()
{
    return ftell(file);
}

arquivo_t *ler_prox_arquivo()
{
    flags_t *flag = NULL;
    arquivo_t *arquivo = NULL;

    if (!(flag = ler_flags()))
        return NULL;

    arquivo = calloc(1, sizeof(arquivo_t));
    arquivo->pai = ler_endereco();
    arquivo->atributos = ler_atributos();
    ler_nome(arquivo->nome);

    return arquivo;
}

void escrever(const arquivo_t *arquivo)
{
    flags_t flags = {.usado = 1, .tipo = 1};

    escrever_flags(&flags);
    escrever_endereco(arquivo->pai);
    escrever_atributos(arquivo->atributos);
    escrever_nome(arquivo->nome);
}
