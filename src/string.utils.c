#include "string.utils.h"
#include <string.h>

char *trim(char *str)
{
    return strtok(str, " \t\n\r");
}