/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/20 19:46:22 by hbayram          ###   ########.fr       */
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

char **set_argv(t_exec *temp, int i)
{
	t_exec *current;
    t_executor **node;
    int j;

    int j;
    current = temp;
    while (temp)
    {
        while (current && current->rank == 4)
        {
            node[i]->argv[j] = ft_strdup(current->content);
            j++;
            current = current->next;
        }
        if (current && current->rank == 3)
            heredoc_handle();
    }    
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
		node[i]->argv = set_argv(current, i);
		node[i]->infile = NULL;
		node[i]->outfile = NULL;
		node[i]->heredoc = NULL;
		node[i]->append_outfile = NULL;
		node[i]->pipe = program->exec->pipe;
		node[i]->next = NULL;
		current = current->next;
	}
	print_exec_list(program->exec->next);
}

int main_execute(t_executor *execute)
{
	
}