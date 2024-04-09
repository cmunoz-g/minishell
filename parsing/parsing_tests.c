#include "parsing_tests.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	size_t	s1_len;

	s1_len = ft_strlen(s1);
	s2 = (char *) malloc(s1_len * sizeof(char) + 1);
	if (!s2)
	{
		free(s2);
		return (0);
	}
	ft_strlcpy(s2, s1, s1_len + 1);
	return (s2);
}

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
	int i = 0;
	char *str = "grep \"specific_string\" * 2>errors.txt 1>hola.txt 0>gg.txt";
	t_token	*token_list;
	//t_token *reference;
	t_cmd_table *cmd_table;

	token_list = NULL;
	lexer(str, &token_list);
	
	// reference = token_list;
	// while (reference)
	// {
	// 	token_list = reference;
	// 	while (token_list)
	// 	{
	// 		printf("value:%s type:%d\n",token_list->value, token_list->type);
	// 		token_list = token_list->next;
	// 	}
	// 	reference = reference->next_cmd;
	// 	printf("\n");
	// }
	cmd_table = NULL;
	parser(&cmd_table, &token_list);
	while (cmd_table) // no entra en el while
	{
		printf("CMD:%s\n",cmd_table->cmd);
		while (cmd_table->args[i])
		{
			printf("ARG%d:%s\n", i, cmd_table->args[i]);
			i++;
		}
		printf("IN:%d\n", cmd_table->in);
		printf("OUT:%d\n", cmd_table->out);
		printf("ERR:%d\n", cmd_table->err);
		i = 0;
		while (i < cmd_table->n_redirections)
		{
			printf("redir: %d type: %d value: %s\n", i, cmd_table->redirections[i]->type, cmd_table->redirections[i]->value);
			i++;
		}
		printf("nbr redir: %d\n", cmd_table->n_redirections);
		if (cmd_table->new_cmd)
			printf("new cdm TRUE\n");
		else 
			printf("new cdm FALSE\n");
		cmd_table = cmd_table->next;
	}
	return 0;
}

/*
typedef struct s_cmd_table
{
	char				*cmd;
	char				**args;
	int					in;
	int					out;
	int 				err;
	t_token				*redirections;
	int					n_redirections;		
	bool				new_cmd;		
	struct s_cmd_table	*prev;
	struct s_cmd_table	*next;
}						t_cmd_table;*/

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