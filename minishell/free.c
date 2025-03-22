/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:26:05 by hbayram           #+#    #+#             */
/*   Updated: 2025/03/22 02:38:28 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_main *program)
{
	t_env	*node;
	t_env *temp;
	int i;

	i = 0;
	node = program->env;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->before_eq);
		free(temp->after_eq);
		free(temp->full_str);
		free(temp);
	}
	program->token = NULL;
	program->env = NULL;

	if (program->env_str)
	{
		i = 0;
		while (program->env_str[i])
			free(program->env_str[i++]);
		free(program->env_str);
		program->env_str = NULL;
	}
}

void free_exec(t_main *program)
{
	t_exec	*node;
	t_exec	*temp;
	
	node = program->exec;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
	program->token = NULL;
}

void	free_program(t_main *program)
{
	t_token	*node;
	t_token	*temp;
	
	node = program->token;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->content);
		free(temp);
	}
	program->token = NULL;
	free_env(program);
	free_exec(program);
}

void	main_free(t_main program, char *line, int key)
{
	if (line)
		free(line);
	free_program(&program);
	if (key == 1)
		rl_clear_history();	
}