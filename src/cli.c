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

void cli_mkdir(char *comando)
{
    if (strlen(comando) <= 6)
    {
        printf("Erro\n");
    }
    add_arquivo(arquivo_atual, comando);
}

void cli_ls()
{
    print_arquivos();
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
            cli_mkdir(comando);
            continue;
        }

        if (!strcmp(comando, "ls"))
        {
            cli_ls();
            continue;
        }

        sprintf(message_comando, "comando \"%s\" nao encontrado", comando);
        warning(message_comando);
    }
}