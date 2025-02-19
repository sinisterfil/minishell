/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:27 by hbayram           #+#    #+#             */
/*   Updated: 2025/02/19 16:34:15 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# include <readline/history.h>
# include <readline/readline.h>
// # include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	char	*d;
	size_t	slen;

	slen = ft_strlen((char *)s1);
	d = (char *)malloc((slen + 1) * (sizeof(char)));
	if (!d)
		return (NULL);
	while (*s1 != '\0')
	{
		*d = *s1;
		d++;
		s1++;
	}
	*d = '\0';
	return ((char *)(d - slen));
}

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize)
{
	size_t	i;

	i = 0;
	if (!destsize)
		return (ft_strlen((char *)src));
	while (src[i] && (i < destsize - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (ft_strlen((char *)src));
}
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	end;
	size_t	s_len;
	char	*d;

	if (!s)
		return (NULL);
	s_len = ft_strlen((char *)s);
	if (start > (s_len))
		return (ft_strdup(""));
	end = start + len;
	if (end > (s_len))
		len = s_len - start;
	d = malloc((len + 1) * sizeof(char));
	if (!d)
		return (NULL);
	ft_strlcpy(d, s + start, len + 1);
	return ((char *)d);
}

static size_t	count_word(const char *p, char c)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (p[i])
	{
		while (p[i] == c && p[i])
			i++;
		if (p[i] != c && p[i])
		{
			len++;
			while (p[i] != c && p[i])
				i++;
		}
	}
	return (len);
}

static size_t	check(char **list, size_t count)
{
	if (!list[count])
	{
		while (count > 0)
			free(list[--count]);
		free(list);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char		**list;
	size_t		count;
	const char	*begin;

	count = 0;
	list = (char **)malloc(sizeof(char *) * (count_word(s, c) + 1));
	if (!list)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			begin = s;
			while (*s && *s != c)
				s++;
			list[count] = ft_substr(begin, 0, s - begin);
			if (!check(list, count))
				return (NULL);
			count++;
		}
	}
	list[count] = NULL;
	return (list);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == '\0' && c == s[i])
		return ((char *)&s[i]);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		len;
	char	*s;

	while (*s1 && ft_strchr(set, *s1))
		s1++;
	len = ft_strlen((char *)s1);
	while (len && s1[len - 1] && ft_strchr(set, s1[len]))
		len--;
	s = ft_substr(s1, 0, len + 1);
	return (s);
}

t_token	*ft_lstnew(void *content)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (0);
	node->content = content;
	node->next = NULL;
	return (node);
}

t_token	*ft_lstlast(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	ft_lstlast(*lst)->next = new;
}
