/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/31 15:03:24 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int	parsing(char *line, t_main *program)
{
	char	*linenew;

	if (quote_control(line) != 0)
	{
		return 1;
	}
	linenew = empty_quotes(line);
	tokenize_args(linenew, &program->token);
	find_keys(&program->token);
	dollar_control(program->token);
	if (set_rank(program->token) == 1)
	{
		return 1;
	}
	return 0;
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_main	program;

	(void)av;
	if (ac != 1)
		exit(61);
	signal_init();
	while (1)
	{
		ft_init(&program, env);
		line = readline("minishell> ");
		if (line == NULL) // Eğer Ctrl-D ile EOF alırsak,
			//readline() NULL döndürecektir
		{
			printf("exit\n");
			break ;
		}
		else if(space_control(line) == 0)
		{
			free(line);
			free_program(&program, 2);
			continue;
		}
		else if (ft_strlen(line) > 0)
		{ 
			add_history(line);
			if (parsing(line, &program) == 0)
				prep_exec(&program);
		}
		main_free(program, line, 0);
	}
	main_free(program, line, 1);
	return (0);
}
