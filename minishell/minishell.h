#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <sys/wait.h>
# define WHITESPACE " \t\n\v\f\r"


typedef struct s_token t_token;
typedef struct s_main t_main;

typedef struct s_token
{
	char			*content;
	int				rank;
	// int				pipe;
	// int				redirect;
	// int 			heredoc;
	// int 			cmd;
	// int				txt;
	int				word;
	int				flag;
	int				space;
	int				dollar;
	t_main *program;
	
	struct s_token	*next;

}					t_token;

typedef struct s_main
{
	char			*full_str;
	t_token			*token;

}					t_main;

// utils
char				**ft_split(char *s, char c);
void				ft_lstadd_back(t_token **lst, t_token *new);
t_token				*ft_lstlast(t_token *lst);
t_token				*ft_lstnew(void *content);
int					ft_our_strchr( char *s, int c);
char				*ft_strchr(const char *s, int c);
char				*ft_strtrim(char *s1, char *set);
size_t				ft_strlen(char *s);
char				*ft_substr(char *s, unsigned int start, size_t len);
char				*ft_strjoin(char *s1, char *s2);
char				*my_join(char *line, char *s1, char *s2);
size_t				ft_strlcpy(char *dest, char *src, size_t destsize);
char				*ft_strdup(char *s1);
int					ft_isalnum(int c);
int					ft_strcmp(char *s1, char *s2);

// signal
void				signal_init(void);
void				signal_handler(int signal);

// quotes
char				*empty_quotes(char *line);
int					find_quotes(char *line, int *ptr_i, int *ptr_j);
void				tokenize_args(char *line, t_token **token);

// init
void token_init(t_main *program);

//dollar
void dollar_control(t_token *token);
char *find_dollar(char *content, int index);
char *dollar_handle(char *dollar);

//control
int pipe_control(t_token *token);
int set_rank(t_token *token);

#endif
