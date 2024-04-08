
// cuando encuentre un $ o * (revisar ~ que puede que tambien haya que expandir), expandirlo en la cmd table

void	parser(t_token **token_list)
{
	while (*token_list)
	{

	}
}



// typedef struct	s_cmd_table
// {
// 	char	*cmd;
// 	char	**args;
// 	int		in;
// 	int		out;
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