#include "arquivo.h"
#include "cli.h"
#include "log.h"
#include "string.utils.h"
#include <stdio.h>
#include <string.h>

static void cli_flush()
{
    char c = '\0';

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

static void cli_exit()
{
    free_arquivo(root);
    free_io();
    printf("Bye ^^\n");
    exit(0);
}

static void cli_clear()
{
    system("clear");
}

static void cli_mkdir(const char *parametros)
{
    char *caminho = obter_parametros(parametros);

    if (!caminho || !strlen(caminho))
    {
        error("informe o caminho do diretorio a ser criado");
        return;
    }

    comando_info_t *comando_info = obter_comando_info(caminho);

    if (strlen(comando_info->nome))
    {
        add_diretorio(comando_info->pai, comando_info->nome);
    }
    else
    {
        error("tentando recriar a pasta /, serio?");
    }
    free(comando_info->nome);
    free(comando_info);
}

static void cli_ls(char *parametros)
{
    char *caminho = obter_parametros(parametros);
    uint64_t i = 0;
    mem_arquivo_t *mem_arquivo = caminho ? buscar_arquivo(caminho) : arquivo_atual;

    if (!mem_arquivo)
    {
        warning("arquivo nao encontrado");
        return;
    }

    for (i = 0; i < mem_arquivo->filhos_count; i++)
    {
        char *tipo = mem_arquivo->filhos[i]->arquivo->atributos->diretorio ? "[diretorio]" : "[arquivo]  ";
        printf("%s %s\n", tipo, mem_arquivo->filhos[i]->arquivo->nome);
    }
}

static void cli_cd(char *parametros)
{
    char *caminho = obter_parametros(parametros);
    mem_arquivo_t *temp_mem_arquivo = buscar_arquivo(caminho);

    if (!temp_mem_arquivo)
    {
        error("arquivo nao encontrado");
        return;
    }

    if (!temp_mem_arquivo->arquivo->atributos->diretorio)
    {
        error("arquivo nao e um diretorio");
        return;
    }

    arquivo_atual = temp_mem_arquivo;
}

static void cli_pwd()
{
    char *caminho = calloc(1, sizeof(char));
    char *caminho_atual = calloc(1, sizeof(char));
    uint64_t caminho_length, nome_length = 0;
    mem_arquivo_t *temp_mem_arquivo = arquivo_atual;

    while (temp_mem_arquivo != root)
    {
        caminho_length = strlen(caminho);
        nome_length = strlen(temp_mem_arquivo->arquivo->nome);

        caminho = realloc(caminho, caminho_length + nome_length + 2);
        caminho_atual = realloc(caminho_atual, caminho_length + nome_length + 2);
        strcpy(caminho, "/");
        strcat(caminho, temp_mem_arquivo->arquivo->nome);
        strcat(caminho, caminho_atual);
        strcpy(caminho_atual, caminho);
        temp_mem_arquivo = temp_mem_arquivo->pai;
    }

    printf("%s%s\n", strlen(caminho) ? "" : "/", caminho);
    free(caminho);
    free(caminho_atual);
}

void init_cli(char *file_name)
{
    char comando[256];
    int comando_length = 0;

    init_io(file_name);
    init_arquivo();
    cli_clear();

    while (1)
    {
        printf("paf>");
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
            cli_exit();
        }

        if (!strcmp(comando, "clear"))
        {
            cli_clear();
            continue;
        }

        if (!strncmp(comando, "mkdir ", 6))
        {
            cli_mkdir(comando + 6);
            continue;
        }

        if (!strcmp(comando, "mkdir"))
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

        if (!strncmp(comando, "cd ", 3))
        {
            cli_cd(comando + 3);
            continue;
        }

        if (!strcmp(comando, "cd"))
        {
            error("informe o nome do diretorio para onde ir");
            continue;
        }

        if (!strcmp(comando, "pwd"))
        {
            cli_pwd();
            continue;
        }

        warning("comando nao encontrado");
    }
}