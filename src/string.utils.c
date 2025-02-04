#include "string.utils.h"
#include <string.h>
#include <stdlib.h>

char *trim(const char *str)
{
    char *temp = calloc(strlen(str), sizeof(char));
    char *result;
    strcpy(temp, str);
    return strtok(temp, " \t\n\r");
}