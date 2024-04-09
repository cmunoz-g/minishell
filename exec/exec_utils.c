/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:59:52 by juramos           #+#    #+#             */
/*   Updated: 2024/04/08 12:27:56 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int  get_str_arr_len(char **strarr)
{
    int n;

    n = 0;
    while (strarr[n])
        n++;
    return (n);
}

char    **ft_str_arr_join_exec(char *s1, char **strarr)
{
    int     i;
    char    **str3;
    int     len;

    len = get_str_arr_len(strarr);
    i = 0;
    str3 = ft_calloc(sizeof(char *), len + 2);
    str3[i++] = s1;
    while (i < len + 1)
    {
        str3[i] = strarr[i - 1];
        i++;
    }
    return (str3);
}
