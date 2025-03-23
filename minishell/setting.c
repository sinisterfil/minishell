/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:25:08 by hbayram           #+#    #+#             */
/*   Updated: 2025/03/23 18:46:09 by hbayram          ###   ########.fr       */
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
	while(node && node->rank == 4 && node->space == 0)
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
    if (node && node->space == 1)
    {
        new_node = ft_lstnew_exec(ft_strdup(node->content));
		new_node->rank = node->rank;
        new_node->space = node->space;
		ft_execadd_back(&program->exec, new_node);
		node->tick = 1;
		node = node->next;
    }
	if (node && node->rank != 4)
	{
		new_node = ft_lstnew_exec(ft_strdup(node->content));
		new_node->rank = node->rank;
        new_node->space = node->space;
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
	// t_token *node;
	// int sign;

	// sign = 0;
	// node = program->token->next;
	// while(node)
	// {
	// 	if(node->rank != 4)
	// 	{
	// 		setting_sign(program);
	// 		sign = 1;
	// 		break;
	// 	}
	// 	node = node->next;
	// }
	setting_sign(program);
	// if(sign == 0)
	// 	setting_str(program);
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
    t_exec *cur = cmd;

    while (cur)
    {
        if (cur->rank == 2)
        {
            if (cur->next == NULL)
                return;
            fd = open(cur->next->content, O_RDONLY);
            if (fd == -1)
            {
                perror("open input file");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (cur->rank == 5)
        {
            if (cur->next == NULL)
                return;
            fd = open(cur->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("open output file (append)");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (cur->rank == 6)
        {
            if (cur->next == NULL)
                return;
            fd = open(cur->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open output file (truncate)");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        cur = cur->next;
    }
}

char **command_join(t_exec *cmd)
{
    t_exec *temp;
    char **str;
    int i;
    int j;

    temp = cmd;
    i = 0;
    j = 0;
    while(temp && temp->tick == 1)
        temp = temp->next;
    while(temp && temp->tick == 0)
    {
        if(temp->rank != 4)
            break;
        temp->tick = 1;
        i++;
        temp = temp->next;
    }
	str = (char **)malloc(sizeof(char *) * (i + 1));
	if (!str)
		return (NULL);
    while(cmd && i--)
    {
        str[j] = ft_strdup(cmd->content);
        cmd = cmd->next;
        j++;
    }
    j = 0;
    while (str[j])
    {
        printf("%d - %s\n", j, str[j]);
        j++;
    }
    return (str);
}

void execute_single_command(t_exec *cmd, char **envp)
{
    char *path;
    char **str;

    if (!cmd || !cmd->content)
        return;
    str = command_join(cmd);
    path = find_executable_path(str[0], envp);
    if (!path)
    {
        printf("minishell: %s: command not found\n", cmd->content);
        exit(127);
    }
    handle_redirections(cmd);
    execve(path, str, envp);
    perror("execve failed");
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
        perror("execve failed");
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
