//#include "minishell.h"
#include <stdio.h>    // FILE türü için gerekli
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int ac, char **av, char **env)
{
    char *line;

    if (ac != 1)
        exit(61);
    // init();
    while(1)
    {
        line = readline("ilknur&&eslem%");
        if(line)    
            add_history(line);
        //parsing();
    }
    return (0);
}

