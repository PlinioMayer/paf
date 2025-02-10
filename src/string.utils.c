#include "string.utils.h"
#include "boolean.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int last_index_of(const char *str, char c)
{
    int index = -1;
    int length = strlen(str);

    if (!str || !length)
    {
        return -1;
    }

    for (int i = 0; i < length; i++)
    {
        if (str[i] == c)
        {
            index = i;
        }
    }

    return index;
}

char *obter_parametros(const char *parametros)
{
    static char *memory;
    char *retorno = NULL;
    uint64_t parametros_length = 0, retorno_length = 0, inicio = 0, fim = 0, prox = 0;

    if (!parametros)
    {
        if (memory)
            return obter_parametros(memory);

        return NULL;
    }

    parametros_length = strlen(parametros);

    if (!parametros_length)
        return "";

    retorno = calloc(1, sizeof(char));
    retorno[0] = '\0';

    for (inicio = 0; parametros[inicio] == ' '; inicio++)
        ;

    if (parametros[inicio] == '"')
    {
        inicio++;
        for (fim = inicio; fim < parametros_length; fim++)
        {
            if (parametros[fim] == '"')
                break;
        }

        if (parametros[fim] != '"')
            return NULL;

        prox = fim + 1;
        fim--;
    }
    else
    {
        for (fim = inicio; fim < parametros_length; fim++)
        {
            if (parametros[fim] == ' ')
                break;
        }

        prox = fim;
        if (parametros[fim] == ' ')
        {
            fim--;
            prox++;
        }
    }

    retorno_length = fim - inicio + 1;
    retorno = calloc(retorno_length + 1, sizeof(char));
    strncpy(retorno, parametros + inicio, retorno_length);

    if (fim < parametros_length)
    {
        memory = calloc(parametros_length - prox + 1, sizeof(char));
        strcpy(memory, parametros + prox);
    }
    else
    {
        memory = NULL;
    }

    return retorno;
}