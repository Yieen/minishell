/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:59:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:59:41 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	unset(t_shell *shell, char *new_str)
{
	int		pos;
	int		envp_size;
	char	**new_env;	

	pos = pos_into_env(shell->env_param, new_str);
	if (pos != -1)
	{
		envp_size = tdar_str_calc(shell->env_param);
		new_env = del_ind_fr_array(shell->env_param, envp_size + 1, pos);
		free(shell->env_param[pos]);
		free(shell->env_param);
		shell->env_param = new_env;
		shell->env_param[envp_size + 1] = 0;
	}
}

int	b_unset(t_shell *shell, int i)
{
	int	j;

	j = 1;
	while (shell->parser_res[i][j])
		unset(shell, shell->parser_res[i][j++]);
	return (EXIT_SUCCESS);
}
