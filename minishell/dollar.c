#include "minishell.h"

char *dollar_handle(char *dollar)
{
    char *new_dollar;
    char *biktim;

    new_dollar = getenv(dollar);
    if(new_dollar == NULL)
        return (NULL);
    biktim = malloc(ft_strlen(new_dollar) + 1);
    biktim = ft_strdup(new_dollar);
    return (biktim);
}

char *find_dollar(t_token *node, int index)
{
    char *dollar;
    char *str;
    char *yeter;
    int i;
    
    i = index + 1;
    while (ft_isalnum(node->content[i]) == 1)
        i++;
    dollar = ft_substr(node->content, index + 1, i - index - 1);
    dollar = dollar_handle(dollar);
    if (dollar == NULL)
    {
        str = ft_strdup(node->content);
        return (str);
    }
    str = ft_strdup(&node->content[i]);
    yeter = ft_strjoin(dollar, str);
    return (yeter);
}

void dollar_control(t_token *token)
{
    char *before_dollar;
    char *after_dollar;
    int index;

    after_dollar = NULL;
    token = token->next;
    while(token)
    {
        if(token->flag != 39)
        {
            index = ft_our_strchr(token->content, 36);
            before_dollar = ft_substr(token->content, 0, index);
            if(index >= 0)
            {
                after_dollar = find_dollar(token, index);
                token->content = ft_strjoin(before_dollar, after_dollar);
            }
        }
        token = token->next;
    }
}
