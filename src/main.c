#include "minishell.h"

int g_flag; // global variable for signal management
 
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

void	arguments(t_minishell *data, char **envp) // en el git de referencia, hacen una comprobacion de que line no este vacio. Si lo esta, imprimen un salto de linea
{
	char 		*line;
	t_token 	*token_tmp;
	t_cmd_table *cmd_tmp;

	cmd_tmp = data->cmd_table;
	while (1)
	{
		line = readline("\e[1;34m""minishell: ""\e[m"); // dejarlo bonito
		if (!line)
			error(data, "readline() error");
		else
		{
			add_history(line);
			lexer(line, &(data->token_list));
			token_tmp = data->token_list;
			parser(&(data->cmd_table), &(data->token_list));
			clean_token_list(&token_tmp);
			// print_cmd_table(data->cmd_table);
			executor(data->cmd_table, envp); // esto es temporal, hasta que hagamos en init() la copia de envp
			clean_cmd_table_list(&cmd_tmp);
		}
		break ; // quitar
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell *data;

	if (argc != 1 || argv[1])
		exit(1);
	data = init(); // iniciar env variables aqui
	// signals
	arguments(data, envp);

	
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