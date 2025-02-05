#include "arquivo.h"
#include "cli.h"
#include "log.h"
#include "string.utils.h"
#include <stdio.h>
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
    char *trimmed = trim(caminho);

    if (!trimmed || !strlen(trimmed))
    {
        error("informe o caminho do diretorio a ser criado");
        return;
    }

    comando_info_t *comando_info = obter_comando_info(trimmed);
    add_arquivo(comando_info->pai, true, comando_info->nome);
    free(comando_info->nome);
    free(comando_info);
}

void cli_ls(char *caminho)
{
    uint64_t i = 0;
    mem_arquivo_t *mem_arquivo = caminho ? buscar_arquivo(caminho) : arquivo_atual;

    if (!mem_arquivo)
    {
        warning("arquivo nao encontrado");
        return;
    }

    for (i = 0; i < mem_arquivo->filhos_count; i++)
    {
        char *tipo = mem_arquivo->filhos[i]->arquivo->diretorio ? "[diretorio]" : "[arquivo]  ";
        printf("%s %s\n", tipo, mem_arquivo->filhos[i]->arquivo->nome);
    }
}

void init_cli(char *file_name)
{
    char comando[256];
    FILE *file = fopen(file_name, "rb+");
    int comando_length = 0;

    system("clear");

    if (file == NULL)
    {
        file = fopen(file_name, "wb");
        fclose(file);
        file = fopen(file_name, "rb+");
    }

    arquivo_atual = add_root();

    while (1)
    {
        printf("manfile>");
        fgets(comando, 255, stdin);
        comando_length = strlen(comando);
        if (comando[comando_length - 1] == '\n')
        {
            comando[comando_length - 1] = '\0';
        }
        else
        {
            warning("comando ultrapassou 255 caracteres");
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

        warning("comando nao encontrado");
    }
}