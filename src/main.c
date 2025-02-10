#include <stdio.h>
#include "cli.h"

int main(int argc, char **args)
{
    init_cli(argc >= 2 ? args[1] : "HD");
    return 0;
}