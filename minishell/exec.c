/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/27 19:09:52 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc(t_executor *cmd)
{
	int pipefd[2];
	char *line;
	char *delimiter;

	if (!cmd->heredoc_delimiters || !cmd->heredoc_delimiters[0])
		return;
	delimiter = cmd->heredoc_delimiters[0];
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	cmd->heredoc_file = pipefd[0]; // child process buradan okuyacak
}


void pipe_count(t_exec *node)
{
    t_exec *new;
    int i;

    new = node;
    i = 0;
    while (new)
    {
        if(new->rank == 1)
            i++;
        new = new->next;
    }
    node->pipe = i;
}

t_exec *set_argv(t_executor **node, t_exec *start, int i)
{
    t_exec *current = start;
    int j;
    int h_count;

    j = 0;
    h_count = 0;
    node[i]->argv = malloc(sizeof(char *) * 100);
    if (!node[i]->argv)
        return NULL;
    while (current)
    {
        if (current->rank == 1) // pipe
        {
            current = current->next;
            break;
        }
        if (current->rank == 4 && current->content)
        {
            node[i]->argv[j++] = ft_strdup(current->content);
        }
        if (current && current->rank == 3)
		{
			set_heredoc(current, node[i], h_count);
			h_count++;
            node[i]->heredoc_delimiters[h_count] = NULL;
			current = current->next->next;
			continue ;
		}
		if (current && current->rank != 4 && current->rank != 1)
		{
			set_redirect(current, node[i]);
			current = current->next->next;
			continue ;
		}
        current = current->next;
    }
    node[i]->argv[j] = NULL;
    return current;
}

char *join_path(const char *dir, const char *cmd)
{
    int len;
    char *full_path;

    len = strlen(dir) + strlen(cmd) + 2; // '/' + '\0'
    full_path = malloc(len);
    if (!full_path)
        return NULL;
    strcpy(full_path, dir);
    if (dir[strlen(dir)-1] != '/')
        strcat(full_path, "/");
    strcat(full_path, cmd);
    return full_path;
}

char *find_command_path(char *command)
{
    char *path_env = getenv("PATH");
    char *path_copy;
    char *dir;
    char *full_path;

    if (!command)
    return NULL;
    if (!path_env)
        return NULL;
    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;
    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = join_path(dir, command);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        if (access(full_path, X_OK) == 0) // çalıştırılabilir mi?
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


void run_execve(t_executor *node, int input_fd, int output_fd)
{
    char *cmd_path;

    if (input_fd != STDIN_FILENO)
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
    cmd_path = find_command_path(node->argv[0]);
    if (!cmd_path)
    {
        printf("%s: command not found\n", node->argv[0]);
        free_token(node->program);
        free_env(node->program);
        free_exec(node->program);
        free_executer(node->program);
        exit(127);
    }
    execve(cmd_path, node->argv, node->program->env_str);
    perror("execve failed");
    free(cmd_path);
    free_executer(node->program);
    exit(1);
}


void main_execute(t_executor *exec)
{
    int pipefds[2];
    pid_t pid;
    int prev_fd;
    int output_fd;
    t_executor *current;

    current = exec;
    prev_fd = STDIN_FILENO;
    while (current)
    {
        output_fd = STDOUT_FILENO;
        if (current->heredoc_delimiters && current->heredoc_delimiters[0])
        {
            handle_heredoc(current);  // pipe oluşturup yazıyor
        }
        if (current->next)
        {
            if (pipe(pipefds) == -1)
            {
                perror("pipe failed");
                free_executer(current->program);
                exit(1);
            }
            output_fd = pipefds[1];
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            free_executer(current->program);
            exit(1);
        }
        if (pid == 0)
        {
            if (current->next)
                close(pipefds[0]);
            if (current->heredoc_file != -1)
            {
                dup2(current->heredoc_file, STDIN_FILENO);
                close(current->heredoc_file);
            }
            else if (prev_fd != STDIN_FILENO)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (output_fd != STDOUT_FILENO)
            {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            redirect_handle(current);
            if (current->error || !current->argv[0])
            {
                if (current->error)
                    fprintf(stderr, "minishell: %s\n", current->error);
                free_env(current->program);
                free_token(current->program);
                free_exec(current->program);
                free_executer(current->program);
                exit(1);
            }
            run_execve(current, STDIN_FILENO, STDOUT_FILENO);
        }
        else
        {
            if (prev_fd != STDIN_FILENO)
                close(prev_fd);
            if (current->heredoc_file != -1)
                close(current->heredoc_file); // child zaten kullandı
            if (current->next)
                close(pipefds[1]);
            if (current->next)
                prev_fd = pipefds[0];
            else
                prev_fd = STDIN_FILENO;
            current = current->next;
        }
    }
    while (wait(NULL) > 0)
    ;
}

void	prep_exec(t_main *program)
{
	t_exec *current;
	t_executor **node;
	int i;
	int count;

	current = program->exec;
	count = 0;
	setting_sign(program);
	pipe_count(program->exec);
	i = program->exec->pipe + 1;
	node = malloc(sizeof(t_executor *) * (i + 1));
	if (!node)
		return ; // exit 
	while (count < i)
	{
		node[count] = malloc(sizeof(t_executor));
		if (!node[count])
		{
			while (--count >= 0)
				free(node[count]);
			free_executer(program);
            return ;
		}
		node[count]->infile = NULL;
		node[count]->outfile = NULL;
		node[count]->heredoc_file = -1;
		node[count]->append = NULL;
		node[count]->pipe = program->exec->pipe;
        node[count]->heredoc_delimiters = NULL;
        node[count]->program = program;
        node[count]->error = NULL;
        current = set_argv(node, current, count);
		if (count > 0)
		    node[count - 1]->next = node[count];
        node[count]->next = NULL;
        count++;
	}
    node[count] = NULL;
    program->executer = node;
    program->control = 1;
    main_execute(node[0]);
}
