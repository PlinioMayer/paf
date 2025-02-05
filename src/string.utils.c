#include "string.utils.h"
#include <string.h>
#include <stdlib.h>

char *trim(const char *str)
{
    char *temp = NULL;

    if (!str)
    {
        return NULL;
    }

    if (!strlen(str))
    {
        return "";
    }

    temp = calloc(strlen(str) + 1, sizeof(char));
    strcpy(temp, str);
    return strtok(temp, " \t\n\r");
}

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