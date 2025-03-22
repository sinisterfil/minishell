/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:25:08 by hbayram           #+#    #+#             */
/*   Updated: 2025/03/22 07:09:46 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setting_str(t_main *program)
{
	t_exec *new;
	t_token *node;
	char *str;
	char *add;

	node = program->token->next;
	while (node && node->tick == 1)
		node = node->next;
	add = ft_strjoin(ft_strdup(""), ft_strdup(""));
	while(node)
	{
		str = ft_strjoin(ft_strdup(add), ft_strdup(node->content));
		free(add);
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
		free(add);
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

char *find_executable_path(char *cmd, char **envp)
{
	(void)envp;
	if (cmd[0] == '/')
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        else
            return NULL;
    }
    char *path_env = getenv("PATH");
    if (!path_env)
        return NULL;
    char *path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;
    char *dir = strtok(path_copy, ":");
    while (dir)
    {
        char *full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        sprintf(full_path, "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

void handle_redirections(t_exec *cmd)
{
    int fd;
    while (cmd)
    {
		if (cmd->rank == 2)
        {
            fd = open(cmd->next->content, O_RDONLY);
            if (fd == -1)
                perror("open");
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
		else if (cmd->rank == 5)
        {
            fd = open(cmd->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                perror("open");
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
		else if (cmd->rank == 6)
        {
            fd = open(cmd->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
                perror("open");
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        // else if (cmd->rank == HEREDOC)
        // {
        //     // Heredoc işlemi burada yapılacak
        // }
        cmd = cmd->next;
    }
}

void execute_single_command(t_exec *cmd, char **envp)
{
    char *path;
	char **str;

    if (!cmd || !cmd->content)
	{
        return;
	}
	str = ft_split(cmd->content, 32);
    path = find_executable_path(str[0], envp);
    if (!path)
    {
        printf("minishell: %s: command not found\n", cmd->content);
        exit(127);
    }
    handle_redirections(cmd);
    execve(path, str, envp);
    perror("execve");
    exit(1);
}

void execute_commands(t_exec *cmds, char **envp, int prev_fd)
{
    int pipe_fd[2];
    pid_t pid;
    t_exec *cmd = cmds;

    if (!cmd)
        return;
    if (cmd->next && cmd->next->rank == 1)
        pipe(pipe_fd);
    pid = fork();
    if (pid == 0)
    {
		if (cmd->next && cmd->next->rank == 1)
        {
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);
            close(pipe_fd[0]);
        }
        if (prev_fd != -1)
        {
            dup2(prev_fd, STDIN_FILENO);
            close(prev_fd);
        }
        execute_single_command(cmd, envp);
		handle_redirections(cmd);
        perror("execve");
        exit(1);
    }
    if (prev_fd != -1)
        close(prev_fd);
    if (cmd->next && cmd->next->rank == 1)
    {
        close(pipe_fd[1]);
        execute_commands(cmd->next->next, envp, pipe_fd[0]);
    }
    waitpid(pid, NULL, 0);
}
