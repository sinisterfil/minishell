#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# define WHITESPACE " \t\n\v\f\r"

typedef struct s_token
{
	char			*content;
	int				rank;
	int				flag;
	int				space;
	struct s_token	*next;

}					t_token;

typedef struct s_program
{
	t_token *token;

}					t_program;

// utils
char				**ft_split(char *s, char c);
void				ft_lstadd_back(t_token **lst, t_token *new);
t_token				*ft_lstlast(t_token *lst);
t_token				*ft_lstnew(void *content);
char				*ft_strchr(const char *s, int c);
char				*ft_strtrim(char *s1, char *set);
size_t				ft_strlen(char *s);
char				*ft_substr(char *s, unsigned int start, size_t len);
char				*ft_strjoin(char *s1, char *s2);
size_t				ft_strlcpy(char *dest, char *src, size_t destsize);
char				*ft_strdup(char *s1);

//signal
void				signal_init(void);
void				signal_handler(int signal);

// quotes
char				*empty_quotes(char *line);
int					find_quotes(char *line, int *ptr_i, int *ptr_j);
void				tokenize_args(char *line, t_token *token);

#endif
