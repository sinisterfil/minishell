/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:30 by hbayram           #+#    #+#             */
/*   Updated: 2025/02/25 20:01:55 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char *quote(char *line, char quote, int *i)
// {
// 	int 	len;
// 	char 	*tirnakli;
// 	int		temp;

// 	temp = 0;
// 	while (line[*i])
// 	{
// 		len = 0;
// 		if (line[*i] && line[*i] != quote)
// 			(*i)++;
// 		temp = *i;
// 		while (line[*i] && line[*i] == quote)
// 			(*i)++;
// 		while (line[*i] && line[*i] != quote)
// 		{
// 			(*i)++;
// 			len++;
// 		}
// 		break;
// 	}
// 	tirnakli = ft_substr(line, temp, len + 2);
// 	return (tirnakli);
// }

// char	*ft_quote(char* line)
// {
// 	int	i;
// 	char *result;
// 	i = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '"')
// 		{
// 			result = quote(line, 34, &i);
// 			printf("result: %s\n", result);
// 		}
// 		i++;
// 	}
// 	return result;
// }


char *ft_quote(char *line, char quote)
{
	int i;
	// int j;
	int len;
	char *first;
	char *second;
	char *new;

	i = 0;
	while (line[i]) // "eslem""ilknur"
	{
		i = 0;
		len = 0;
		while (line[i] && line[i] != quote)
			i++;
		while (line[++i] && line[i] != quote)
			len++; // i = 6 len = 5 
		if(!line[i])
			break;
		if (len == 0)
		{
			first = ft_substr(line, 0, i - 1);
			printf("first %s\n", first);
			second = ft_substr(line, i + 1, ft_strlen(&line[i + 1]));
			printf("sec %s\n", second);
			new = ft_strjoin(first, second);
			printf("new %s\n", new);
		}
		else if(len != 0)
			break;
		line = new;
	}
	if (line[i + 1] != '\0')
	{
		first = ft_substr(line, 0, i);
		printf("2222 first %s\n", first);
		// ft_quote(&line[i], quote);
		line = ft_strjoin(first, &line[i]);
	}
	return (line);
}

int	ft_argument(char *line, int *a, int *z)
{
	int i;
	int j;
	char quote;

	i = *a;
	j = *z;
	quote = -99; 
	if ((line[j] == 34 && line[j + 1] == 34) || (line[j] == 39 && line[j + 1] == 39))
	{
		j = j + 2;
		i = j;
		*a = i;
		*z = j;
		return (int)quote;
	}
	if (line[i] == 34 || line[i] == 39)
    {
		j = i + 1;
        quote = line[i];
		i++;
        while (line[j] && line[j] != quote)
            j++;
    }
    else
    {
		j = i;
        while (line[j] && line[j] != 32 && line[j] != 39 && line[j] != 34)
            j++;
    }
	*a = i;
	*z = j;
	return (int)quote; // j = 0 i = 0
}


void arguman_bul(char *line, t_token *token)
{
    int i;
	int j;
    int quote;
    t_token *new_token;

	i = 0;
	j = 0;
    while (line[i])
    {
		if (ft_lstlast(token) && ft_lstlast(token)->space == 0 && ft_lstlast(token)->flag == -99)
			i--;
        while (line[i] == 32 || (line[i] >= 9 && line[i] <= 13)) 
            i++;

		int isspace = 0;
        if (!line[i]) 
            break;
		quote = ft_argument(line, &i, &j);
        if (j - i > 0)
		{
			if (quote != 34 || quote != 39)
			{
				while (line[i] == 32 || (line[i] >= 9 && line[i] <= 13) || (line[i] == 39 && line[i] == 34))
				{
					i++;
					isspace = 0;
				}
			}
			new_token = ft_lstnew(ft_substr(line, i, j - i));
			new_token->space = isspace;
        	i = j + 1;
	        ft_lstadd_back(&token, new_token);
			new_token->flag = quote;
			if((line[j + 1] == 32 || (line[j + 1] >= 9 && line[j + 1] <= 13)) && (quote == 34 || quote == 39))
				new_token->space = 1;
			else if(line[j] == 32 || (line[j] >= 9 && line[j] <= 13))
				new_token->space = 1;
		}
		else if(i == j && (quote == 34 || quote == 39))
			i++;
    }
}

int	quote_control(char *line)
{
	int	i;
    int flag;


    flag = 0;
	i = 0;
	while (line[i])
	{
	    if (line[i] == 34 && flag == 0)
			flag = 34;
        else if(line[i] == 39 && flag == 0)
			flag = 39;
        else if(line[i] == flag && flag != 0)
            flag = 0;
		i++;
	}
	if (flag != 0)
		return (printf("Error! Waiting for expression.\n"));
	return (0);
}

void	print_token(t_token *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		printf("%d\n", list->space);
		printf("%d\n", list->flag);
		list = list->next;
	}
}
void	parsing(char *line)
{
	//t_token	list;
	char *linenew;

    if(quote_control(line) != 0)
    {
    	return ;
	}
	// history ve line freelenmesi lazım
	//arguman_bul(ft_strtrim(line, WHITESPACE), &list);
	linenew = ft_quote(line, 34);
	printf("%s\n", linenew);
	// print_token(list.next);
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	(void)ac;
	(void)av;
	(void)env;
	
	signal_init();
	if (ac != 1)
		exit(61);
	// init();
	while (1)
	{
		line = readline("ilknur&&eslem<3 ");
		if (line == NULL)  // Eğer Ctrl-D ile EOF alırsak, readline() NULL döndürecektir
        {
            printf("exit\n"); //exit fonks yazılacak.
            break;
        }
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			parsing(line);
		}
	}
	return (0);
}
