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
int main()
{
	char *str = "echo hi > \"doble\"quote;cat doblequote;rm doblequote@echo hi > \"doble\"quote";
	t_token	*token_list;
	t_token *reference;

	token_list = NULL;
	lexer(str, &token_list);
	
	reference = token_list;
	while (reference)
	{
		token_list = reference;
		while (token_list)
		{
			printf("value:%s type:%d\n",token_list->value, token_list->type);
			token_list = token_list->next;
		}
		reference = reference->next_cmd;
		printf("\n");
	}
	//parser(&token_list);
	return 0;
}


/*
	"ls -la | cat >output"
	head -n 10 large_file.txt > excerpt.txt
	cat file1.txt file2.txt file3.txt | grep "search_term" -c > count.txt
	grep "search_term" < input_file.txt
	echo "New line of text" >> existing_file.txt
	grep "specific_string" * 2>errors.txt
	"echo hi > \"doble\"quote;cat doblequote;rm doblequote@echo hi > \"doble\"quote"


*/

/*

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7

*/