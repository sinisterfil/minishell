/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/23 17:14:28 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_count(t_exec *node)
{
	t_exec	*new;
	int		i;

	new = node;
	i = 0;
	while (new)
	{
		if (new->rank == 1)
			i++;
		new = new->next;
	}
	node->pipe = i;
}
void	set_heredoc(t_exec *current, t_executor *cmd, int i)
{
	if (current && current->next)
	{
		if (!cmd->heredoc_delimiters)
		{
			cmd->heredoc_delimiters = malloc(sizeof(char *) * 10);
			if (!cmd->heredoc_delimiters)
				exit(1); // exit fonksiyonu
		}
		cmd->heredoc_delimiters[i] = ft_strdup(current->next->content);
	}
}

void	is_directory(const char *path, char **error)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return ;
	if (S_ISDIR(st.st_mode))
	{
		*error = ": Is a directory";
	}
}

void	check_redirect_access_input(const char *filename, char **error)
{
	is_directory(filename, &error);
	if(error)
		return;
	if (access(filename, F_OK) < 0)
	{
		*error = ": No such file or directory";
		return;
	}
	if (access(filename, R_OK) < 0)
	{
		*error = ": Permission denied";
		return;
	}
}
	
int	check_redirect_access(const char *filename, int rank, char **error)
{
	int	fd;
	int	flags;

	is_directory(filename, error);
	if(error)
		return(-1);
	if (access(filename, F_OK) == 0 && access(filename, W_OK) < 0)
	{
		*error = ": Permission denied";
		return (-1);
	}
	if (rank == 6) // >
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (rank == 5) // >>
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (-1);
	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		*error = ": Failed to open file";
		return -1;
	}
	close(fd);
	return (0);
}

void	check_redirect_file(t_executor *cmd, char *filename, int rank)
{
	char	*error;
	
	error = NULL;
	if (rank == 2)
	{
		check_redirect_access_input(filename, &error);
		if (*error)
		{
			printf("minishell: %s %s\n", filename, error);
			exit(1); // exit fonksiyonu yazılmalı
		}
		free(cmd->infile);
		cmd->infile = ft_strdup(filename);
	}
	else if (rank == 6 || rank == 5)
	{
		if (check_redirect_access(filename, rank, &error) < 0)
		{
			printf("minishell: %s %s\n", filename, error);
			exit(1); // exit fonksiyonu yazılmalı
		}
		if (rank == 6)
		{
			free(cmd->outfile);
			cmd->outfile = ft_strdup(filename);
		}
		else if (rank == 5)
		{
			free(cmd->append);
			cmd->append = ft_strdup(filename);
		}
	}
}

void	set_redirect(t_exec *current, t_executor *cmd)
{
	char	*filename;

	while (current && current->next && current->rank != 1 && current->rank != 4)
	{
		filename = ft_strdup(current->next->content);
		if (!filename)
			exit(1); // free fonksiyonu
		check_redirect_file(cmd, filename, current->rank);
		current = current->next->next;
		free(filename);
	}
}

void	prep_exec(t_main *program)
{
	t_exec		*current;
	t_executor	**node;
	int			i;
	int			count;

	current = program->exec;
	count = 0;
	setting_sign(program);
	pipe_count(program->exec);
	i = program->exec->pipe + 2;
	node = malloc(sizeof(t_executor *) * (i));
	if (!node)
		return ; // exit
	while (count < i)
	{
		node[i] = malloc(sizeof(t_executor *) * (i));
		if (!node[i])
		{
			while (--i > 0)
				free(node[i]);
			free(node);
		}
		set_argv(current, i);
		node[i]->infile = NULL;
		node[i]->outfile = NULL;
		node[i]->heredoc_file = NULL;
		node[i]->append = NULL;
		node[i]->pipe = program->exec->pipe;
		node[i]->next = NULL;
		current = current->next;
	}
	print_exec_list(program->exec->next);
}

void	set_argv(t_exec *temp, int i)
{
	t_exec		*current;
	t_executor	**node;
	int			h_count;
	int			j;

	j = 0;
	h_count = 0;
	current = temp;
	while (current)
	{
		while (current && current->rank == 4)
		{
			node[i]->argv[j] = ft_strdup(current->content);
			j++;
			current = current->next;
		}
		if (current && current->rank == 3)
		{
			set_heredoc(current, node[i], h_count);
			h_count++;
			current = current->next->next;
			continue ;
		}
		if (current && current->rank != 3 && current->rank != 1)
		{
			set_redirect(current, node[i]);
			current = current->next->next;
			continue ;
		}
	}
}

void	redirect_handle(t_executor *node)
{
	int	fd;

	if (node->infile)
	{
		fd = open(node->infile, O_RDONLY);
		if (fd < 0)
		{
			perror("minishell: open infile");
			return ;
		}
		dup2(fd, STDIN_FILENO); 
		close(fd);
	}
	if (node->outfile)
	{
		fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("minishell: open outfile");
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (node->append)
	{
		fd = open(node->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("minishell: open append outfile");
			return ;
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (node->heredoc_delimiters) // Heredoc işlemi varsa
	{
		// Heredoc işlemine göre özel bir yönlendirme yapabilirsin.
		// Burada sadece placeholder olarak bırakıyorum:
		// heredoc_handle(node->heredoc);
	}
}

