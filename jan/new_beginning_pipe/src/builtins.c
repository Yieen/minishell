/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:07:06 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/27 16:59:38 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	unset(t_shell *shell, char *new_str)
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
	return (EXIT_SUCCESS);
}

int	b_unset(t_shell *shell, int i)
{
	if (shell->parser_res[i][1])
		return (unset(shell, shell->parser_res[i][1]));
	else
		return (EXIT_FAILURE);
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

int check_key(char *str, int eq_pos)
{
	int	i;

	i = 0;
	if (!(((str[0] >= 'a') && (str[0] <= 'z')) || ((str[0] >= 'A') && (str[0] <= 'Z'))))
		return (0);
	while (i < eq_pos)
	{
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= '0' && str[i] <= '9'))
			i++;
		else
			return (0);
	}
	return (1);
}

int	export(t_shell *shell, char *new_str)
{
	int env_ind;

	if (!check_key(new_str, char_srch(new_str, '=')))
		return (0);
	env_ind = pos_into_env(shell->env_param, cut_equ(new_str));
	if (env_ind == -1)
		add_export(shell, new_str);
	else
	{
		// free(shell->env_param[env_ind]);
		shell->env_param[env_ind] = new_str;
	}
	return (EXIT_SUCCESS);
}

int	b_export(t_shell *shell, int i)
{
	if (shell->parser_res[i][1])
		return (export(shell, shell->parser_res[i][1]));
	else
		return (EXIT_FAILURE);
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
