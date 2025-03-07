/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:39 by hbayram           #+#    #+#             */
/*   Updated: 2025/03/07 15:17:09 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_rank(t_token *token)
{
	t_token	*node;

	node = token;
    node = node->next;
	while (node != NULL)
	{
        if (node->content != NULL && ft_strchr(node->content, '|') && node->flag == -99)
			node->rank = 1;
		else if (node->content != NULL && (ft_strcmp(node->content, "<") == 0
				|| ft_strcmp(node->content, ">") == 0
				|| ft_strcmp(node->content, ">>") == 0))
			node->rank = 2;
		else if (node->content != NULL && ft_strcmp(node->content, "<<") == 0)
			node->rank = 3;
		else
			node->rank = 4;
        node = node->next;
	}
	if (pipe_control(token) == 0)
		return (0);
	return (1);
}

int	pipe_control(t_token *token)
{
	t_token	*node;

	node = token->next;
	if (node != NULL && (node->rank == 1 || node->rank == 3
			|| ft_lstlast(node)->rank != 4))
		return (printf("error, unexpected token\n"), 1);
	while (node != NULL && node->next != NULL)
	{
		if (node->next && node->next->rank != 4)
		{
			if (node->rank != 4 || (node->next->next != NULL
					&& node->next->next->rank != 4))
				return (printf("error, unexpected token\n"), 1);
		}
		node = node->next;
	}
	node = token->next;
	return (0);
}

// int builtin_control()
// {

// }
// int redirection_control()
// {

// }