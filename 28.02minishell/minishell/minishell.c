/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/02/28 14:31:20 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


void main_free(char *line, int key)
{
	if(line)
		free(line);
	if(key == 1)
		rl_clear_history();
}

void	print_token(t_token *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		printf("%d\n", list->space);
		printf("%d\n", list->flag);
		list = list->next;
	}
}

void	parsing(char *line, t_main *program)
{
	char *linenew;

    if(quote_control(line) != 0)
    {
    	return ;
	}
	linenew = empty_quotes(line);
	tokenize_args(linenew, &program->token);
	print_token(program->token->next);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_main program;
	(void)av;
	(void)env;
	
	if (ac != 1)
		exit(61);
	signal_init();
	while (1)
	{
		token_init(&program);
		line = readline("ilknur&&eslem<3 ");
		if (line == NULL)  // Eğer Ctrl-D ile EOF alırsak, readline() NULL döndürecektir
        {
            printf("exit\n");
            break;
        }
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			parsing(line, &program);
			//join_tokens();
			//executing();
		}
		main_free(line, 0);
	}
	main_free(line, 1);
	return (0);
}
