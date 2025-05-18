/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/18 13:41:38 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	open_pipe(t_exec *node)
{
	int	**fd;
	int	i;

	i = -1;
	if (node->pipe == 0)
		return ;
	fd = (int **)malloc(sizeof(int *) * node->pipe);
	fd[node->pipe] = NULL;
	while (++i < node->pipe)
		fd[i] = malloc(sizeof(int) * 2);
	node->fd = fd;
	i = -1;
	while (++i < node->pipe)
		pipe(node->fd[i]);
}

// void	exec_commands(t_exec *cmd, int i, int check, int flag)
// {
// 	t_executor	*temp;

// 	open_pipe(cmd);
// 	temp = cmd->executor;
// 	while (temp)
// 	{
// 		if (flag == 1)
// 			heredoc_check(cmd, temp->files, 0, &flag);
// 		if (cmd->check == 3)
// 			break ;
// 		redirect_handle(cmd, temp, &i);
// 		cmd->status = 0;
// 		if (temp->redirect != NULL)
// 			init_redirect(temp->files, temp);
// 		if (temp->files->error == 1 || temp->files->error == 2)
// 		{
// 			file_error(cmd, temp, temp->files);
// 			temp = temp->next;
// 			i++;
// 			continue ;
// 		}
// 		executor_helper(cmd, temp, &check, &i);
// 		temp = temp->next;
// 	}
// 	free_executor(cmd, &cmd->executor, check);
// }