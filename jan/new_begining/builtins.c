/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:07:06 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/24 16:14:52 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_shell *shell, char *new_str)
{
	int		pos;
	int		envp_size;
	char	**new_env;	

// new_str
	pos = pos_into_env(shell->env_param, new_str);
	// printf("pos_del=%d\n", pos);
	if (pos != -1)
	{
		envp_size = twodimarr_str_calc(shell->env_param);
		// printf("pos_del=%d\n", envp_size);	
		new_env = del_ind_fr_array(shell->env_param, envp_size+1, pos);
		// printf("%s\n", new_env[0]);
		// ft_clear(shell->env_param);
		shell->env_param = new_env;
		shell->env_param[envp_size+1] = 0;
	}
}

void	ft_clear(char	**lsttclear)
{
	int	i;

	i = 0;
	while (lsttclear[i])
	{
		free(lsttclear[i]);
		i++;
	}
	free(lsttclear);
}

void	add_export(t_shell *shell, char *new_str)
{
	int envp_size;
	char	**new_env;

	envp_size = twodimarr_str_calc(shell->env_param);
	new_env = copy_string_array(shell->env_param, envp_size + 2);
	ft_clear(shell->env_param);
	shell->env_param = new_env;
	shell->env_param[envp_size] = new_str;
	shell->env_param[envp_size + 1] = 0;
}

void	export(t_shell *shell, char *new_str)
{
	int env_ind;

	// write(1,"1\n",2);
	env_ind = pos_into_env(shell->env_param, cut_equ(new_str));
	if (env_ind == -1)
		add_export(shell, new_str);
	else
	{
		// free(shell->env_param[env_ind]);
		shell->env_param[env_ind] = new_str;
	}
}

void	env(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->env_param[++i])
	{
		printf("%s\n", shell->env_param[i]);
	}	
}
