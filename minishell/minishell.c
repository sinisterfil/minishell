/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/02/19 18:56:31 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include <stdio.h>    // FILE türü için gerekli
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

void arguman_bul(char *line, t_token *token)
{
    int i;
	int j;
    char quote;
    t_token *new_token;
	t_token *last;

	i = 0;
	last = ft_lstlast(token);
    while (line[i])
    {
        while (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)) 
            i++;
        if (!line[i]) 
            break;
		j = i + 1;
        if (line[i] == 34 || line[i] == 39)
        {
            quote = line[i];
			i++;
            while (line[j] && line[j] != quote)
                j++;
        }
        else
        {
            while (line[j] && line[j] != 32 && line[j] != 39 && line[j] != 34)
                j++;
        }
        if (j - i > 0)
		{
			new_token = ft_lstnew(ft_substr(line, i, j - i));
        	i = j + 1;
	        ft_lstadd_back(&token, new_token);
		}
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
	tokens = ft_split(line, ' ');
	if (!tokens)
		exit(11); // history ve line freelenmesi lazım
	//create_token(tokens, &list);
	arguman_bul(line, &list);
	print_token(list.next);
}

int	main(int ac, char **av, char **env)
{
	char	*line;

	signal_init();
	if (ac != 1)
		exit(61);
	// init();
	while (1)
	{
		line = readline("ilknur&&eslem<3 ");
		if (line == NULL)  // Eğer Ctrl-D ile EOF alırsak, readline() NULL döndürecektir
        {
            printf("exit\n");
            break;
        }
		if (ft_strlen(line) > 0)
			add_history(line);
		parsing(line);
	}
	return (0);
}
