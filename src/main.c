#include "minishell.h"

int g_flag = 0; // global variable for signal management
void	minishell_loop(t_minishell *data);
void	create_main_fork(char *line, t_minishell *data);
void	reset_loop(char *line, t_minishell *data);
 
void	print_tokens(t_token *token_list) // borrar
{
	t_token *reference = token_list;

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
}

void	print_cmd_table(t_cmd_table *cmd_table) // borrar
{
	int i = 0; 

	while (cmd_table) 
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
		printf("\n");
	}
}

void	create_main_fork(char *line, t_minishell *data)
{
	t_token 	*token_tmp;
	pid_t		pid;

	add_history(line);
	lexer(line, &(data->token_list));
	token_tmp = data->token_list;
	parser(&(data->cmd_table), &(data->token_list));
	clean_token_list(&token_tmp);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid == 0)
		executor(data->cmd_table, data->env_vars);
	else
		wait(NULL);	
}

void	minishell_loop(t_minishell *data)
{
	char	*line;

	line = readline("\e[1;34m""minishell> ""\e[m");
	if (!line)
		exit(EXIT_SUCCESS);
	else if (check_spaces(line))
		reset_loop(line, data);
	else
	{
		create_main_fork(line, data);
		reset_loop(line, data);
	}
}

void	reset_loop(char *line, t_minishell *data)
{

	clean_cmd_table_list(&(data->cmd_table));
	free(line);
	minishell_loop(data);
}

/*
void	arguments(t_minishell *data) // en el git de referencia, hacen una comprobacion de que line no este vacio. Si lo esta, imprimen un salto de linea
{
	char 		*line;
	t_token 	*token_tmp;

	while (1)
	{
		line = readline("\e[1;34m""minishell> ""\e[m"); // dejarlo bonito
		if (!line)
			exit(EXIT_SUCCESS); // se puede sustituir esto por una ft con un mensaje de salida. El if es correcto cuando se registra ctrl+D
		else if (check_spaces(line))
			free(line);
		else 
		{
			add_history(line);
			lexer(line, &(data->token_list));
			token_tmp = data->token_list;
			parser(&(data->cmd_table), &(data->token_list));
			clean_token_list(&token_tmp);
			executor(data->cmd_table, data->env_vars);
			clean_cmd_table_list(&(data->cmd_table));
			free(line);
		}
	}
}
*/

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	if (argc != 1 || argv[1])
		exit(1);
	data = init(envp);
	// signals(false); currently compilation fails
	minishell_loop(data);
}

// int main()
// {
// 	int i = 0;
// 	char *str = "cat file1.txt file2.txt file3.txt | grep \"search_term\" -c > count.txt ; cat";
// 	t_token	*token_list;
// 	//t_token *token_list_head;
// 	//t_token *reference;
// 	t_cmd_table *cmd_table;
// 	//t_cmd_table *cmd_table_head;

// 	// atexit(leaks);
// 	token_list = NULL;
// 	lexer(str, &token_list);
	
// 	// reference = token_list;
// 	// while (reference)
// 	// {
// 	// 	token_list = reference;
// 	// 	while (token_list)
// 	// 	{
// 	// 		printf("value:%s type:%d\n",token_list->value, token_list->type);
// 	// 		token_list = token_list->next;
// 	// 	}
// 	// 	reference = reference->next_cmd;
// 	// 	printf("\n");
// 	// }

// 	cmd_table = NULL;
// 	//token_list_head = token_list;
// 	//cmd_table_head = cmd_table;
// 	parser(&cmd_table, &token_list);
// 	//clean_token_list(&token_list_head);
// 	//clean_cmd_table_list(&cmd_table);
// 	printf("%s\n\n",str);
// 	while (cmd_table) 
// 	{
// 		printf("CMD:%s\n",cmd_table->cmd);
// 		while (cmd_table->args[i])
// 		{
// 			printf("ARG%d:%s\n", i, cmd_table->args[i]);
// 			i++;
// 		}
// 		printf("IN:%d\n", cmd_table->in);
// 		printf("OUT:%d\n", cmd_table->out);
// 		printf("ERR:%d\n", cmd_table->err);
// 		i = 0;
// 		while (i < cmd_table->n_redirections)
// 		{
// 			printf("redir: %d type: %d value: %s\n", i, cmd_table->redirections[i]->type, cmd_table->redirections[i]->value);
// 			i++;
// 		}
// 		printf("nbr redir: %d\n", cmd_table->n_redirections);
// 		if (cmd_table->new_cmd)
// 			printf("new cdm TRUE\n");
// 		else 
// 			printf("new cdm FALSE\n");
// 		cmd_table = cmd_table->next;
// 		printf("\n");
// 	}
// 	return 0;
// }