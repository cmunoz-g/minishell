/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_order.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: camunozg <camunozg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:00:36 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/05/17 11:24:38 by camunozg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap(char **x, char **y)
{
	char	*temp;

	temp = *x;
	*x = *y;
	*y = temp;
}

int	partition(char *arr[], int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = arr[high];
	i = (low - 1);
	j = low;
	while (j <= (high - 1))
	{
		if (ft_strcmp(arr[j], pivot) < 0)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
		j++;
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	quicksort(char **arr, int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		quicksort(arr, low, pi - 1);
		quicksort(arr, pi + 1, high);
	}
}

void	env_order_aux(int i, int *j, char **sorted_env)
{
	printf("=\"");
	(*j)++;
	while (sorted_env[i][(*j)])
		printf("%c", sorted_env[i][(*j)++]);
	printf("\"\n");
}

void	env_order(t_minishell *data)
{
	char	**sorted_env;
	int		n;
	int		i;
	int		j;

	sorted_env = ft_arrdup(data->export_vars);
	n = get_nbr_env(sorted_env);
	quicksort(sorted_env, 0, n - 1);
	i = 0;
	j = 0;
	while (sorted_env[i])
	{
		printf("declare -x ");
		while (sorted_env[i][j] && sorted_env[i][j] != '=')
			printf("%c", sorted_env[i][j++]);
		if (sorted_env[i][j] == '=')
			env_order_aux(i, &j, sorted_env);
		else
			printf("\n");
		j = 0;
		i++;
	}
	free_arr(sorted_env);
}
