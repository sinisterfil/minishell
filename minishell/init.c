/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:16:34 by hbayram           #+#    #+#             */
/*   Updated: 2025/03/09 10:04:38 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*a_lstnew(char *before, char *after)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
	{
		perror("malloc failed");
		exit(1);
	}
	new->before_eq = before;
	new->after_eq = after;
	new->next = NULL;
	return (new);
}

void ft_envadd_back(t_env **lst, t_env *new)
{
    t_env *temp;

    if (!lst || !new)
        return ;
    if (!*lst)
    {
        *lst = new;
        return ;
    }
    temp = *lst;
    while (temp->next)
        temp = temp->next;
    temp->next = new;
}

void	print_e(t_env *list)
{
	while (list)
	{
		printf("before %s\n", list->before_eq);
		printf("after %s\n", list->after_eq);
		list = list->next;
	}
}

/**********************************************************************/

void token_init(t_main *program)
{
    program->token = malloc(sizeof(t_token));
    if (!program->token)
    {
        perror("Malloc failed");
        exit(1);
    }
    program->token->content = NULL;
    program->token->rank = 0;
    program->token->dollar = 0;
    program->token->flag = 0;
    program->token->next = NULL;
}

void get_env(t_env **envp, char **env)
{
    int i;
    int index;
    t_env *new;

    i = -1;
    *envp = NULL;
    while(env[++i])
    {
        index = of_strchr(env[i], 61);
        if (index == -1)
            continue;
        new = a_lstnew(ft_substr(env[i], 0, index),
            ft_substr(env[i], index + 1, ft_strlen(env[i]) - index - 1));
        ft_envadd_back(envp, new);
        
    }
    
}

void env_init(t_main *program, char **env)
{
    program->env = malloc(sizeof(t_env));
    if (!program->env)
    {
        perror("Malloc failed");
        exit(1);
    }
    program->env->full_str = NULL;
    program->env->next = NULL;
    get_env(&program->env, env);
    set_env(program, program->env);
    //print_e(program->env->next);
}

void ft_init(t_main *program, char **env)
{
    token_init(program);
    env_init(program, env);
}
