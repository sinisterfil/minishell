#ifndef MINISHELL_H
# define MINISHELL_H

// # include <readline/history.h>
// # include <readline/readline.h>
#include <stdio.h>
// # include <stdlib.h>
// # include <string.h>
// # include <unistd.h>

typedef struct s_token 
{
    char *content;
    int rank;
    int flag;
    int space;
    struct s_token *next;

}   t_token;


char	**ft_split(char const *s, char c);
void	ft_lstadd_back(t_token **lst, t_token *new);
t_token	*ft_lstlast(t_token *lst);
t_token	*ft_lstnew(void *content);
char	*ft_strchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
size_t	ft_strlen(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif
