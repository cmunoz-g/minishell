#ifndef PARSING_TESTS_H
#define PARSING_TESTS_H

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7
# define FILENAME 8

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct	s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

char	*ft_strdup_mod(const char *s, size_t size);
int	ft_strcmp(const char *str1, const char *str2);
int main();
void	lexer(char *cmd_line, t_token **token_list);
int	add_token(t_token **token_list, char *cmd_line, int start, int end);
void	get_token_type(t_token *token);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
int		ft_isspace(int c);
size_t	ft_strlen(const char *str);
t_token *get_last_token(t_token *token_list);

#endif