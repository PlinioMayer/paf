#include "log.h"
#include <stdio.h>

void error(char *message)
{
    printf("ERRO: %s\n", message);
}

void warning(char *message)
{
    printf("AVISO: %s\n", message);
}

void message(char *message)
{
    printf("%s\n", message);
}