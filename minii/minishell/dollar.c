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

// char *find_dollar(t_token *node, int index)
// {
//     char *dollar;
//     char *str;
//     char *yeter;
//     int i;
    
//     i = index + 1;
//     while (ft_isalnum(node->content[i]) == 1)
//         i++;
//     dollar = ft_substr(node->content, index + 1, i - index - 1);
//     dollar = dollar_handle(dollar);
//     if (dollar == NULL)
//     {
//         str = ft_strdup(node->content);
//         return (str);
//     }
//     str = ft_strdup(&node->content[i]);
//     yeter = ft_strjoin(dollar, str);
//     return (yeter);
// }

char *find_dollar(char *content, int index)
{
    char *dollar;
    char *str;
    char *yeter;
    int i;
    
    i = index + 1;
    while (ft_isalnum(content[i]) == 1)
        i++;
    dollar = ft_substr(content, index + 1, i - index - 1);
    dollar = dollar_handle(dollar);
    str = ft_strdup(&content[i]);
    if (dollar == NULL)
        return (str);
    str = ft_strdup(&content[i]);
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
        index = ft_our_strchr(token->content, 36);
        while(token->flag != 39)
        {
            before_dollar = ft_substr(token->content, 0, index);
            printf("BEFORE .%s.\n", before_dollar);
            if(index >= 0)
            {
                after_dollar = find_dollar(token->content, index);
                printf("%d\n", index);
                printf("AFTER .%s.\n", after_dollar);
                token->content = ft_strjoin(before_dollar, after_dollar);
                printf("FULL .%s.\n", token->content);
                index = index + ft_our_strchr(after_dollar, 36);
                if(index < 0)
                    break;
            }
        }
        token = token->next;
    }
}
