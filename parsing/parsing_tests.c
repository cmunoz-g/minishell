#include "parsing_tests.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		ft_isspace(int c)
{
	c = (unsigned char)c;
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ')
		return (1);
	return (0);
}

char	*ft_strdup_mod(const char *s, size_t size)
{
	char	*cpy;

	cpy = (char *)malloc(size);
	if (!cpy)
		return (NULL);
	ft_strlcpy(cpy, s, size + 1);
	return (cpy);
}

int	ft_strcmp(const char *str1, const char *str2)
{
	size_t	i;

	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
}

t_token *get_last_token(t_token *token_list)
{
	while (token_list->next)
		token_list = token_list->next;
	return (token_list);
}
int main()
{
	char *str = "ls -la | cat >output";
	t_token	*token_list;

	lexer(str, &token_list);
	while (token_list)
	{
		printf("value:%s type:%d\n",token_list->value, token_list->type);
		token_list = token_list->next;
	}
	return 0;
}