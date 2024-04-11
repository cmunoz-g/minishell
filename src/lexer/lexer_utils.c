#include "minishell.h"

void	set_qt(bool *quotes, char *quote_type, char *cmd_line, int i)
{
	(*quotes) = true;
	(*quote_type) = cmd_line[i];
}

void	init_lexer(bool *qt, int *i, int *start)
{
	*qt = false;
	*i = -1;
	*start = 0;
}
