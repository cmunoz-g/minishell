#include "minishell.h"


// gestionar que si cambiamos el valor de SHLVL= de manera artificial a traves de las local variables, deba cambiarlo tambien en sh_lvl dentro de data

// void	update_sh_lvl(t_minishell *data, int operation)
// {
// 	char	*tmp;

// 	data->sh_lvl += operation;

// 	tmp = (char *)malloc(6 + get_nbr_digits(data->sh_lvl))
// 	//if (!tmp)
// 	// gestionar 

// 	// copiamos SHLVL= y el numero en sh_lvl a tmp.
// 	// si no existe tmp en env o exprot se crea con valor 1.
// 	// si ya existe se modifica el valor en env y export por tmp;


// 	mod_var(data->env_vars, data, data->cmd_table->args[i]);
// }