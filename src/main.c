#include <stdio.h>
#include "cli.h"
#include "log.h"

int main(int argc, char **args)
{
    if (argc < 2)
    {
        error("forneça o caminho para o arquivo a ser gerenciado");
        message("Exemplo: manfile caminho_do_arquivo");
        return 1;
    }

    init_cli(args[1]);
    return 0;
}