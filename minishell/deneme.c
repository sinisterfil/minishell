#include <stdio.h>
#include "minishell.h"


size_t	checkchar(char *set, char a)
{
	size_t	i;
	size_t	setlen;

	i = 0;
	setlen = ft_strlen(set);
	while (i < setlen)
	{
		if (set[i] == a)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char *s1, char *set)
{
	char	*new;
	size_t	start;
	size_t	finish;

	start = 0;
	finish = ft_strlen(s1);
	while (s1[start] && checkchar(set, s1[start]))
		start++;
	while (finish > start && checkchar(set, s1[finish - 1]))
		finish--;
	new = (char *)malloc(sizeof(char) * (finish - start + 1));
	if (!new)
		return (NULL);
	ft_strlcpy(new, (s1 + start), (finish - start + 1));
	new[finish - start] = '\0';
	return (new);
}

int main()
{
    printf("%s", ft_strtrim("env a", "env"));
}