#include "minishell.h"

// void leaks(void)
// {
// 	system("leaks a.out");
// }

int main()
{
	int i = 0;
	char *str = "cat file1.txt file2.txt file3.txt | grep \"search_term\" -c > count.txt ; cat";
	t_token	*token_list;
	t_token *token_list_head;
	//t_token *reference;
	t_cmd_table *cmd_table;
	t_cmd_table *cmd_table_head;

	// atexit(leaks);
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
	token_list_head = token_list;
	cmd_table_head = cmd_table;
	parser(&cmd_table, &token_list);
	//clean_token_list(&token_list_head);
	//clean_cmd_table_list(&cmd_table);
	printf("%s\n\n",str);
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