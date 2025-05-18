/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/18 16:32:21 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_token *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		printf("%d\n", list->space);
		printf("%d\n", list->flag);
		printf("RANK %d\n\n", list->rank);
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
	//print_token(program->token->next);
	return 0;
}

void print_exec_list(t_exec *cmd)
{
    while (cmd)
    {
        printf("NODE: content  =   %s, rank = %d, space = %d\n",
               cmd->content ? cmd->content : "(null)",
               cmd->rank, cmd->space);
        cmd = cmd->next;
    }
}
void print_executor(t_executor *executer)
{
	int i = 0;
	while (executer->argv[i])
	{
		printf("argv[%d]: ", i);
		int j = 0;
		while (executer->argv[i][j])
		{
			printf("\"%s\" ", executer->argv[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}


void set_executor(t_exec *exec)
{
	t_exec *node;
	t_executor *temp;
	int i;
	int j;
	
	i = 0;
	node = exec;
	temp = malloc(sizeof(t_executor));
	if (!temp)
		return ; // malloc başarısız

	// Komut sayısını bul
	while (node)
	{
		if (node->rank != 4)
			i++;
		node = node->next;
	}

	temp->argv = (char ***)malloc(sizeof(char **) * (i + 1)); // +1 NULL için
	if (!temp->argv)
		return ;

	node = exec;
	i = 0;
	while (node)
	{
		if (node->rank != 4)
		{
			// Her yeni komut bloğu için max 100 argümanlık yer ayır (veya arg count hesaplayabilirsin)
			temp->argv[i] = (char **)malloc(sizeof(char *) * 100); // sabit tuttum minimum değişiklik için
			if (!temp->argv[i])
				return ;
			j = 0;
			node = node->next;
			while (node && node->rank == 4)
			{
				temp->argv[i][j++] = ft_strdup(node->content);
				node = node->next;
			}
			temp->argv[i][j] = NULL; // argv[i] null-terminate
			i++;
		}
		else
			node = node->next;
	}
	temp->argv[i] = NULL; // dış array'i null-terminate et
	exec->executer = temp;
}

void	executing(t_main *program)
{
	setting_sign(program);
	
	set_executor(program->exec);
	print_executor(program->exec->executer);
	ft_builtin(program);
	//print_exec_list(program->exec->next);
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
		else if(space_control(line) == 0)
			continue;
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
