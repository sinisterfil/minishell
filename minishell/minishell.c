/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/03/15 18:35:42 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}

void	main_free(t_main program, char *line, int key)
{
	if (line)
		free(line);
	free_program(&program);
	if (key == 1)
		rl_clear_history();
}

void	print_token(t_token *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		printf("%d\n", list->space);
		printf("%d\n", list->flag);
		printf("RANK %d\n", list->rank);
		list = list->next;
	}
}
void	print_exec(t_exec *exec)
{
	while (exec)
	{
		
		if (exec->content)
			printf("%s\n", exec->content);
		if(exec->rank)
			printf("RANK %d\n", exec->rank);
		exec = exec->next;
	}
}

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
	//print_token(program->token->next);
}

void setting_str(t_main *program)
{
	t_exec *new;
	t_token *node;
	char *str;
	char *add;

	node = program->token->next;
	while (node->tick == 1)
		node = node->next;
	add = ft_strjoin(ft_strdup(""), ft_strdup(""));
	while(node)
	{
		str = ft_strjoin(ft_strdup(add), ft_strdup(node->content));
		if(node->next && node->space == 1 && node->next->rank == 4)
			add = ft_strjoin(ft_strdup(str), ft_strdup(" "));
		else
			add = ft_strdup(str);
		free(str);
		
		node = node->next;
	}
	new = ft_lstnew_exec(ft_strdup(add));
	new->rank = 4;
	ft_execadd_back(&program->exec, new);
	free(add);
}

void setting_sign(t_main *program)
{
	t_exec *new;
	t_exec *new_node;
	t_token *node;
	char *str;
	char *add;
	int first;

	first = 0;
	node = program->token->next;
	while (node->tick == 1)
		node = node->next;
	add = ft_strjoin(ft_strdup(""), ft_strdup(""));
	while(node && node->rank == 4)
	{
		str = ft_strjoin(ft_strdup(add), ft_strdup(node->content));
		if(node->next && node->space == 1 && node->next->rank == 4)
			add = ft_strjoin(ft_strdup(str), ft_strdup(" "));
		else
			add = ft_strdup(str);
		free(str);
		node->tick = 1;
		node = node->next;
		first = 1;
	}
	if(first == 1)
	{
		new = ft_lstnew_exec(ft_strdup(add));
		new->rank = 4;
		ft_execadd_back(&program->exec, new);
	}
	if (node && node->rank != 4)
	{
		new_node = ft_lstnew_exec(ft_strdup(node->content));
		new_node->rank = node->rank;
		ft_execadd_back(&program->exec, new_node);
		node->tick = 1;
		node = node->next;
	}
	free(add);
	if(node != NULL)
		setting_sign(program);
}

void setting_nodes(t_main *program)
{
	t_token *node;
	int sign;

	sign = 0;
	node = program->token->next;
	while(node)
	{
		if(node->rank != 4)
		{
			setting_sign(program);
			sign = 1;
			break;
		}
		node = node->next;
	}
	if(sign == 0)
		setting_str(program);
}

void	executing(t_main *program)
{
	setting_nodes(program);
	//print_exec(program->exec->next);
	printf("AAAAAAA");
	ft_builtin(program);
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
		line = readline("ilknur&&eslem<3 ");
		if (line == NULL) // Eğer Ctrl-D ile EOF alırsak,
			//readline() NULL döndürecektir
		{
			printf("exit\n");
			break ;
		}
		else if (ft_strlen(line) > 0)
		{ 
			add_history(line);
			if (parsing(line, &program) == 0)
				executing(&program);
		}
		main_free(program, line, 0);
	}
	main_free(program, line, 1);
	return (0);
}
