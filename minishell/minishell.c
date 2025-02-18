#include "minishell.h"
// #include <stdio.h>    // FILE türü için gerekli
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

// if(*line == 39)
//         {
//             flag = 39;
//             new_token = ft_lstnew(ft_subtr(line, 3, 13));
//         }
//         else if(*line == 34)
//         {

//         }
//         else

void arguman_bul(char *line, t_token *token)
{
    int i;
    int j;
    t_token	*temp;
	t_token	*new_token;

    temp = token;
    i = 0;
    while(line[i])
    {
        j = i;
        if((line[i] == 34 && temp->flag == 0) || (line[i] == 39 && temp->flag == 0))
        {
            temp->flag = line[i];
            while(line[j] != temp->flag)
                j++;
            if((line[++j] >= 9 && line[j] <= 13) || line[j] == 32)
                temp->space = 1;
        }
        new_token = ft_lstnew((ft_substr((const char*)line[i], i, j)));
		temp->next = new_token;
		temp = temp->next;
        i++;
    }
}

void	create_token(char **tokens, t_token *list)
{
	int		i;
	t_token	*temp;
	t_token	*new_token;

	temp = list;
	i = 0;
	while (tokens[i])
	{
		new_token = ft_lstnew(tokens[i]);
		temp->next = new_token;
		temp = temp->next;
		i++;
	}
}
int	quote_control(char *line)
{
	int	i;
    int flag;

    flag = 0;
	i = 0;
	while (line[i])
	{
	    if (line[i] == 34 && flag == 0)
			flag = 34;
        else if(line[i] == 39 && flag == 0)
			flag = 39;
        else if(line[i] == flag && flag != 0)
            flag = 0;
		i++;
	}
	if (flag != 0)
		return (printf("Error! Waiting for expression.\n"));
	return (0);
}



void	print_token(t_token *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		list = list->next;
	}
}
void	parsing(char *line)
{
	char	**tokens;
	t_token	list;

    if(quote_control(line) != 0)
        return ;
    // arguman_bul(ft_strtrim(line, " "));
	// tokens = ft_split(line, ' ');
	// if (!tokens)
	// 	exit(11); // history ve line freelenmesi lazım
	//create_token(tokens, &list);
    arguman_bul(line, &list);
	print_token(list.next);
}

void	history(char *line)
{
	char	*control;
	int		i;

	i = 0;
	control = line;
	while (control[i])
	{
		while ((line[i] >= 9 && line[i] <= 13) || line[i] == 32)
			i++;
		if (!control[i])
			return ;
		i++;
	}
	if (ft_strlen(line) > 0)
		add_history(line);
}

int	main(int ac, char **av, char **env)
{
	char	*line;

	if (ac != 1)
		exit(61);
	// init();
	while (1)
	{
		line = readline("ilknur&&eslem<3 ");
		history(line);
		parsing(line);
	}
	return (0);
}
