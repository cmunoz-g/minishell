#include "minishell.h"

void	parser(t_cmd_table **cmd_table, t_token **token_list)
{
	t_token		*tmp;
	int			s_e[2];
	bool		new_cmd;

	init_parser(s_e, &tmp, token_list, &new_cmd);	
	while (tmp)
	{
		(*token_list) = tmp;
		while (*token_list)
		{
			if ((*token_list)->type == PIPE)
			{
				(alloc_cmd_table(cmd_table, new_cmd), gen_cmd_table(tmp, cmd_table, s_e[0], s_e[1] + 1));
				s_e[0] = s_e[1];
				if (new_cmd)
					new_cmd = false;
			}
			parser_aux(token_list, s_e);
		}
		if (*cmd_table && !new_cmd)
			(alloc_cmd_table(cmd_table, new_cmd), gen_cmd_table(tmp, cmd_table, s_e[0] + 1, s_e[1]));
		else
			(alloc_cmd_table(cmd_table, new_cmd), gen_cmd_table(tmp, cmd_table, s_e[0], s_e[1]));
		reset_parser(&tmp, s_e, &new_cmd);
	} 
}

void	init_parser(int start_end[2], t_token **tmp, t_token **token_list, bool *new_cmd)
{
	start_end[0] = 0;
	start_end[1] = 0;
	(*tmp) = (*token_list);
	*new_cmd = true;
}

void	reset_parser(t_token **tmp, int start_end[2], bool *new_cmd)
{
	(*tmp) = (*tmp)->next_cmd;
	start_end[0] = 0;
	start_end[1] = 0;
	*new_cmd = true;
}

void	parser_aux(t_token **token_list, int *s_e)
{
	s_e[1]++;
	(*token_list) = (*token_list)->next;
}