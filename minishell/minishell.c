#include "minishell.h"
// #include <stdio.h>    // FILE türü için gerekli
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int create_token(char **tokens, t_token *list)
{
    int i;
    t_token *temp;
    t_token *new_token;

    temp = list;
    i = 0;
    while(tokens[i])
    {
        new_token = ft_lstnew(tokens[i]);
        temp->next = new_token; 
        temp = temp->next; 
        i++;
    }
}

void print_token(t_token *list)
{
    while(list)
    {
        printf("%s\n", list->content);
        list = list->next;
    }
}
void parsing(char *line)
{
    char **tokens;
    t_token list;

    tokens = ft_split(line, ' ');
    if(!tokens)
        exit(11); //history ve line freelenmesi lazım
    create_token(tokens, &list);
    // print_token(list.next);

}

int main(int ac, char **av, char **env)
{
    char *line;

    if (ac != 1)
        exit(61);
    // init();
    while(1)
    {
        line = readline("ilknur&&eslem<3");
        if(line)
            add_history(line);
        parsing(line);
    }
    return (0);
}
