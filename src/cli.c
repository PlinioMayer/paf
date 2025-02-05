#include "arquivo.h"
#include "cli.h"
#include "log.h"
#include "string.utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cli_flush()
{
    char c = '\0';

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

void cli_exit(FILE *file)
{
    free_arquivos();
    fclose(file);
    printf("Bye ^^\n");
    exit(0);
}

void cli_mkdir(const char *caminho)
{
    if (!caminho || !trim(caminho))
    {
        error("informe o caminho do diretorio a ser criado");
        return;
    }

    comando_info_t *comando_info = obter_comando_info(caminho);
    printf("pai: %s\nnome: %s\n", comando_info->pai->arquivo->nome, comando_info->nome);
    return;

    add_arquivo(arquivo_atual, trim(caminho));
}

void cli_ls(char *caminho)
{
    mem_arquivo_t *mem_arquivo = buscar_arquivo(caminho);
    if (mem_arquivo)
    {
        printf("nome: %s\n", mem_arquivo->arquivo->nome);
    }
    else
    {
        printf("nao encontrado\n");
    }
}

void init_cli(char *file_name)
{
    char comando[257];
    char message_comando[304];
    FILE *file = fopen(file_name, "rb+");
    int comando_length = 0;

    system("clear");

    if (file == NULL)
    {
        file = fopen(file_name, "wb");
        fclose(file);
        file = fopen(file_name, "rb+");
    }

    mem_arquivo_t *root = add_root();
    mem_arquivo_t *teste1 = add_arquivo(root, "teste1");
    mem_arquivo_t *teste2 = add_arquivo(teste1, "teste2");
    mem_arquivo_t *teste3 = add_arquivo(teste2, "teste3");
    arquivo_atual = root;

    while (1)
    {
        printf("manfile>");
        fgets(comando, 257, stdin);
        comando_length = strlen(comando);
        if (comando[comando_length - 1] == '\n')
        {
            comando[comando_length - 1] = '\0';
        }
        else
        {
            warning("comando ultrapassou 256 caracteres");
            cli_flush();
        }

        if (!strcmp(comando, "exit"))
        {
            cli_exit(file);
        }

        if (!strncmp(comando, "mkdir ", 6))
        {
            cli_mkdir(comando + 6);
            continue;
        }

        if (!strncmp(comando, "mkdir", 5))
        {
            error("informe o nome do diretorio a ser criado");
            continue;
        }

        if (!strncmp(comando, "ls ", 3))
        {
            cli_ls(comando + 3);
            continue;
        }

        if (!strcmp(comando, "ls"))
        {
            cli_ls(NULL);
            continue;
        }

        sprintf(message_comando, "comando \"%s\" nao encontrado", comando);
        warning(message_comando);
    }
}