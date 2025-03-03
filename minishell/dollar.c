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
    free(dollar);
    return (biktim);
}

char *find_dollar(char *content, int index)
{
    char *dollar;
    char *str;
    char *yeter;
    char *allahim;
    int i;
    
    i = index + 1;
    while (ft_isalnum(content[i]) == 1)
        i++;
    dollar = ft_substr(content, index + 1, i - index - 1);
    allahim = ft_strdup(dollar);
    free(dollar);
    dollar = dollar_handle(allahim);
    str = ft_strdup(&content[i]);
    if (dollar == NULL)
    {
        free(dollar);
        return (str);
    }
    yeter = ft_strjoin(dollar, str);
    return (yeter);
}

void dollar_control(t_token *token)
{
    char *before_dollar;
    char *after_dollar;
    int index;
    int new;

    new = 0;
    after_dollar = NULL;
    token = token->next;
    while(token)
    { 
        index = ft_our_strchr(token->content, 36);
        while(index >= 0 && token->flag != 39)
        {
            new = index;
            before_dollar = ft_substr(token->content, 0, index);
            after_dollar = find_dollar(token->content, index);
            token->content = ft_strjoin(before_dollar, after_dollar);
            index = ft_our_strchr(&token->content[index], 36);
            if(index <= 0)
                break;
            index = index + new;
        }
        token = token->next;
    }
}
