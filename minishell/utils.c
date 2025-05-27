/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:44:27 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/27 14:22:31 by hbayram          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int 	ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (to_find[j] == '\0')
		return (-1);
	while (str[i] != '\0')
	{
		while (str[i + j] == to_find[j] && str[i + j] != '\0')
			j++;
		if (to_find[j] == '\0')
			return (i);
		i++;
		j = 0;
	}
	return (-1);
}

char	*ft_strdup(char *s1)
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

size_t	ft_strlcpy(char *dest, char *src, size_t destsize)
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
char	*ft_substr(char *s, unsigned int start, size_t len)
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

 size_t	count_word(char *p, char c)
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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

 size_t	check(char **list, size_t count)
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

char	**ft_split(char *s, char c)
{
	char		**list;
	size_t		count;
	char	*begin;

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

int	ft_our_strchr( char *s, int c)
{
	int i;

	i = 0;
	c = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == c && ft_isalnum(s[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

int 	of_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	if (s[i] == '\0' && c == s[i])
		return (i);
	return (-1);
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

char	*ft_strtrim(char *s1, char *set)
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

char	*my_join(char *line, char *s1, char *s2)
{
	int i;
	int j;

	if (!s1)
	{
		s1 = malloc(1 * sizeof(char));
		*s1 = '\0';
	}
	i = -1;
	while (*(s1 + ++i))
		*(line + i) = *(s1 + i);
	j = -1;
	while (*(s2 + ++j))
		*(line + i + j) = *(s2 + j);
	*(line + i + j) = '\0';
	free(s1);
	free(s2);
	return (line);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char *str;
	int i;
	int j;

	if (!s1)
	{
		s1 = malloc(1 * sizeof(char));
		*s1 = '\0';
	}
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = -1;
	while (*(s1 + ++i))
		*(str + i) = *(s1 + i);
	j = -1;
	while (*(s2 + ++j))
		*(str + i + j) = *(s2 + j);
	*(str + i + j) = '\0';
	free(s1);
	free(s2);
	return (str);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (1);
	}
	return (0);
}

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		return (1);
	}
	return (0);
}

int	ft_isalnum(int c)
{
	if(c == 0)
		return (0);
	return (ft_isalpha(c) || ft_isdigit(c));
}

t_token	*ft_lstnew(void *content)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (0);
	node->content = content;  // ls|
	node->rank = 0;
	node->tick = 0;
	node->flag = -99;
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

void	ft_lstdelone(t_token *lst, void (*del)(void*))
{
	if (!del)
		return ;
	if (lst)
	{
		(*del)(lst->content);
		free(lst);
	}
}

void	ft_lstadd(t_token *node, t_token *new)
{
    t_token *temp;

    if (!node || !new )
        return;
    temp = node->next;
    node->next = new;
	new->next = temp;
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

t_exec	*ft_lstnew_exec(void *content)
{
	t_exec	*node;

	node = malloc(sizeof(t_exec));
	if (!node)
		return (0);
	node->content = content;
	node->space = 0;
	node->tick = 0;
	node->next = NULL;
	return (node);
}

void ft_execadd_back(t_exec **lst, t_exec *new)
{
    t_exec *temp;

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

int	space_control(char *s)
{
	int	i;

	i = 0;
	if (!s[0])
		return (0);
	while (s[i])
	{
		if (s[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}
