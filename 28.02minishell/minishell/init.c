/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:16:34 by hbayram           #+#    #+#             */
/*   Updated: 2025/02/28 14:22:51 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    program->token->flag = 0;
    program->token->next = NULL;
}
