
// cuando encuentre un $ o * (revisar ~ que puede que tambien haya que expandir), expandirlo en la cmd table
// sintaxix pipex  “<infile ls -l | wc -l >outfile”, como se gestiona?

t_cmd_table	*parser(t_token **token_list)
{
	t_cmd_table *cmd_list;


	return (cmd_list);
}

void	gen_cmd_table(t_token *token_list, t_cmd_table **cmd_list, int nbr_tokens)
{
	t_cmd_table	*cmd_table;
	t_cmd_table *last;

	cmd_table = (t_cmd_table *)malloc(sizeof(t_cmd_table));
	if (!cmd_table)
		//ft para limpiar 
	if (!(*cmd_list))
	{
		(*cmd_list) = cmd_table;
		cmd_table->prev = NULL;
	}
	// get last
	else
	{
		last->next = cmd_table;
		cmd_table->prev = last;
	}
	populate_cmd_table(token_list, &cmd_table, nbr_tokens);
}
void	populate_cmd_table(t_token *token_list, t_cmd_table **cmd_table, int nbr_tokens) // una vez este terminada, revisar que pasa con la memoria en strdup y demas ft
{
	int		i;
	int		j;
	char	redir;

	i = 0;
	j = 0;
	redir = '\0';
	if ((*cmd_table)->prev)
		(*cmd_table)->in = "pipe";
	else
		(*cmd_table)->in = "0";
	(*cmd_table)->out = "1";
	(*cmd_table)->err = "2";
	while (i < nbr_tokens)
	{
		if (token_list->type == CMD)
			(*cmd_table)->cmd = ft_strdup(token_list->value); 
		else if (token_list->type == 2)
			(*cmd_table)->args[j++] = ft_strdup(token_list->value);
		else if (token_list->type == 6)
			(*cmd_table)->out = "pipe";
		else if (token_list->type) == 3
			redir == 't';
		else if (token_list->type) == 4
			redir == 'a';
		else if (token_list->type) == 5
			redir == 'i';
		else if (token_list->type = 8)
		{
			if (redir == 't')
				(*cmd_table)->out = ft_strdup(token_list->value);
			else if (redir == 'a')
				(*cmd_table)->out = ft_strdup(token_list->value); // COMO MOSTRAR QUE ES APPEND? hacer char**out y que en la segunda pos sea si es trunc o append?
			else if (redir == 'i')
				(*cmd_table)->in = ft_strdup(token_list->value);
		}
		i++;
		token_list = token_list->next;
	}
	(*cmd_table)->cmd[j] = NULL;
}

/*
# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7
# define FILENAME 8
*/

/*
	else if (token->value[0] == '<')
			token->type = INPUT;
		else if (token->value[0] == '>')
			token->type = TRUNC;
		else if (token->value[0] == '>' && token->value[1] == '>')
			token->type = APPEND;
*/


// typedef struct	s_cmd_table
// {
// 	char	*cmd;
// 	char	**args;
// 	char	in;
// 	char	out;
//	char	err;
// 	struct 	s_cmd_table *next;
// 	struct 	s_cmd_table *prev;
// }				t_cmd_table;


// cat file1.txt file2.txt file3.txt | grep "search_term" -c > count.txt

// cmd = cat
// args = file1.txt file2.txt file3.txt 
// in = stdin 
// out = pipe

// cmd = grep
// args = "search_term" -c 
// in = pipe 
// out = count.txt