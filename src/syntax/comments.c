#include "minishell.h"

void    take_out_tokens(t_token **tmp)
{
    t_token *hold;

    if ((*tmp)->prev)
        (*tmp)->prev->next = NULL;
    while ((*tmp))
    {
        hold = (*tmp)->next;
        free((*tmp)->value);
        free((*tmp));
        (*tmp) = hold;
    }
    (*tmp) = NULL;
}

void    check_comments(t_token **token_list)
{
    t_token *tmp;

    tmp = (*token_list);
    while (tmp)
    {
        if (tmp->value[0] == '#')
        {
            take_out_tokens(&tmp);
            return ;
        }
        tmp = tmp->next;
    }
}