#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"

static char *file_name;
static FILE *file;

static flags_t *ler_flags(const uint64_t *endereco)
{
    uint64_t file_pointer = 0;
    flags_t *flags = NULL;
    uint8_t bits = 0;

    if (endereco)
    {
        file_pointer = get_file_pointer();
        fseek(file, *endereco, SEEK_SET);
    }

    if (fread(&bits, FLAGS_SIZE, 1, file))
    {
        flags = calloc(1, sizeof(flags_t));
        flags->lixo = 0;
        flags->usado = bits & 1;
    }

    if (endereco)
        fseek(file, file_pointer, SEEK_SET);

    return flags;
}

static void escrever_flags(const uint64_t *endereco, const flags_t *flags)
{
    uint64_t file_pointer = 0;
    uint8_t bits = 0;

    if (endereco)
    {
        file_pointer = get_file_pointer();
        fseek(file, *endereco, SEEK_SET);
    }

    if (flags->usado)
        bits += 1;

    fwrite(&bits, FLAGS_SIZE, 1, file);

    if (endereco)
        fseek(file, file_pointer, SEEK_SET);
}

static uint64_t ler_endereco(const uint64_t *at)
{
    uint64_t file_pointer = 0;
    uint64_t endereco = 0;

    if (at)
    {
        file_pointer = get_file_pointer();
        fseek(file, *at, SEEK_SET);
    }

    fread(&endereco, ENDERECO_SIZE, 1, file);

    if (at)
        fseek(file, file_pointer, SEEK_SET);

    return endereco;
}

static void escrever_endereco(const uint64_t *at, const uint64_t endereco)
{
    uint64_t file_pointer = 0;

    if (at)
    {
        file_pointer = get_file_pointer();
        fseek(file, *at, SEEK_SET);
    }

    fwrite(&endereco, ENDERECO_SIZE, 1, file);

    if (at)
        fseek(file, file_pointer, SEEK_SET);
}

static atributos_t *ler_atributos(const uint64_t *endereco)
{
    uint64_t file_pointer = 0;
    atributos_t *atributos = NULL;
    uint8_t bits = 0;

    if (endereco)
    {
        file_pointer = get_file_pointer();
        fseek(file, *endereco, SEEK_SET);
    }

    if (fread(&bits, ATRIBUTOS_SIZE, 1, file))
    {
        atributos = calloc(1, sizeof(atributos_t));
        atributos->lixo = 0;
        atributos->diretorio = bits & (uint8_t)1;
    }

    if (endereco)
        fseek(file, file_pointer, SEEK_SET);

    return atributos;
}

static void escrever_atributos(const uint64_t *endereco, const atributos_t *atributos)
{
    int64_t file_pointer = 0;
    uint8_t bits = 0;

    if (endereco)
    {
        file_pointer = get_file_pointer();
        fseek(file, *endereco, SEEK_SET);
    }

    if (atributos->diretorio)
        bits += 1;

    fwrite(&bits, ATRIBUTOS_SIZE, 1, file);

    if (endereco)
        fseek(file, file_pointer, SEEK_SET);
}

static void ler_nome(const uint64_t *endereco, char nome[NOME_SIZE])
{
    uint64_t file_pointer = 0;

    if (endereco)
    {
        file_pointer = get_file_pointer();
        fseek(file, *endereco, SEEK_SET);
    }

    fread(nome, 1, NOME_SIZE, file);

    if (endereco)
        fseek(file, file_pointer, SEEK_SET);
}

static void escrever_nome(const uint64_t *endereco, const char *nome)
{
    uint64_t file_pointer = 0;

    if (endereco)
    {
        file_pointer = get_file_pointer();
        fseek(file, *endereco, SEEK_SET);
    }

    fwrite(nome, 1, NOME_SIZE, file);

    if (endereco)
        fseek(file, file_pointer, SEEK_SET);
}

static void clear_buffer()
{
    uint64_t file_pointer = get_file_pointer();
    fclose(file);
    file = fopen(file_name, "rb+");
    fseek(file, file_pointer, SEEK_SET);
}

void init_io(char *name)
{
    file_name = name;
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
    flags_t *flags = NULL;
    arquivo_t *arquivo = NULL;

    if (!(flags = ler_flags(NULL)))
        return NULL;

    if (!flags->usado)
    {
        fseek(file, ARQUIVO_SIZE - FLAGS_SIZE, SEEK_CUR);
        return ler_prox_arquivo();
    }

    arquivo = calloc(1, sizeof(arquivo_t));
    arquivo->pai = ler_endereco(NULL);
    arquivo->atributos = ler_atributos(NULL);
    ler_nome(NULL, arquivo->nome);

    return arquivo;
}

void escrever(const arquivo_t *arquivo)
{
    escrever_flags(NULL, &(flags_t){.usado = 1});
    escrever_endereco(NULL, arquivo->pai);
    escrever_atributos(NULL, arquivo->atributos);
    escrever_nome(NULL, arquivo->nome);
    clear_buffer();
}

void remover(const uint64_t endereco)
{
    flags_t *flags = ler_flags(&endereco);
    flags->usado = FALSE;
    escrever_flags(&endereco, flags);
    clear_buffer();
}