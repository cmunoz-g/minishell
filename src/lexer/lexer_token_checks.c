#include "minishell.h"

void	check_local_var_aux(char *line, t_token **it_variable)
{
	if ((*it_variable)->type != PIPE && (*it_variable)->prev->variable == true
			&& (!is_there_space((*it_variable), line)))
		(*it_variable)->type = CMD;
	else if ((*it_variable)->type != PIPE && (*it_variable)->prev->type != CMD
			&& (!is_there_space((*it_variable), line))
			&& !check_if_variable((*it_variable)))
		(*it_variable)->type = CMD;
	else if ((*it_variable)->type != PIPE && (*it_variable)->prev && (*it_variable)->prev->variable == true
		&& is_quoted_var((*it_variable)->value) && check_first_cmd((*it_variable)))
			(*it_variable)->type = CMD;
}

void	check_local_var(t_token **token_list, char *line)
{
	t_token	*it_variable;

	it_variable = *(token_list);
	while (it_variable)
	{
		if (!is_pos_variable(it_variable->value))
			it_variable->variable = true;
		if (!it_variable->prev && it_variable->variable == true)
			it_variable->type = CMD;
		else if (it_variable->prev && it_variable->variable == true)
		{
			if (it_variable->prev->variable || it_variable->prev->type != CMD)
			{
				if (!check_first_cmd(it_variable) && !check_if_pipe(it_variable))
					it_variable->type = ARG;
				else
					it_variable->type = CMD;
			}
		}
		else if (it_variable->prev && it_variable->variable == false)
			check_local_var_aux(line, &it_variable);
		it_variable = it_variable->next;
	}
}

void	check_redirections(t_token **token_list)
{
	t_token	*it_filename;

	it_filename = *(token_list);
	while (it_filename)
	{
		if ((it_filename->next && !ft_strcmp(it_filename->value, "2")
				&& (it_filename->next->type == HEREDOC
					|| it_filename->next->type == TRUNC
					|| it_filename->next->type == APPEND
					|| it_filename->next->type == INPUT)))
			it_filename->type = STDERR;
		if ((it_filename->next && (!ft_strcmp(it_filename->value, "1"))
				&& (it_filename->next->type == HEREDOC
					|| it_filename->next->type == TRUNC
					|| it_filename->next->type == APPEND
					|| it_filename->next->type == INPUT)))
			it_filename->type = STDOUT;
		it_filename = it_filename->next;
	}
}
