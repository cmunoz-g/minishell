/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:02:55 by juramos           #+#    #+#             */
/*   Updated: 2024/04/09 12:22:58 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int handle_outfile(t_token tkn)
{
    int fd;

    if (tkn.type == TRUNC)
        fd = open(tkn.value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (tkn.type == APPEND)
        fd = open(tkn.value, O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd == -1)
        return (1);
    if (dup2(fd, 1) < 0)
        return (1);
    return (0);
}

int handle_infile(t_token tkn)
{
    int fd;
    
    fd = open(tkn.value, O_RDONLY, 0777);
    if (fd == -1)
        return (1);
    if (dup2(fd, 0) < 0)
        return (1);
    // ref. point closes fd after opening them. Why?????
    /*
        Maybe they close the fd because only the number is needed,
        and they´re going to be opened again during the exec_process?
        or maybe, they only close them if cmds > 1, in which case those
        fd will be opened again during the do_pipe function.
    */
    return (0);
}

int redirect(t_cmd_table *tbl)
{
    int i;

    i = 0;
    while (i < tbl->n_redirections)
    {
        if (tbl->redirections[i].type == TRUNC || tbl->redirections[i].type == APPEND)
        {
            if (handle_outfile(tbl->redirections[i]))
                return (1);
        }
        else if (tbl->redirections[i].type == INPUT)
        {
            if (handle_infile(tbl->redirections[i]))
                return (1);
        }
        i++;
    }
    return (0);
}
