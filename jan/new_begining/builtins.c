/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:07:06 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/27 18:12:55 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_shell *shell, char *new_str)
{
	int		pos;
	int		envp_size;
	char	**new_env;	

	pos = pos_into_env(shell->env_param, new_str);
	if (pos != -1)
	{
		envp_size = twodimarr_str_calc(shell->env_param);
		new_env = del_ind_fr_array(shell->env_param, envp_size + 1, pos);
		free(shell->env_param[pos]);
		free(shell->env_param);
		shell->env_param = new_env;
		shell->env_param[envp_size + 1] = 0;
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
	int		envp_size;
	char	**new_env;

	envp_size = twodimarr_str_calc(shell->env_param);
	new_env = copy_string_array(shell->env_param, envp_size + 2);
	ft_clear(shell->env_param);
	shell->env_param = new_env;
	shell->env_param[envp_size] = malloc(sizeof(char)
			* (ft_strlen(new_str) + 1));
	ft_strlcpy(shell->env_param[envp_size], new_str, ft_strlen(new_str) + 1);
	shell->env_param[envp_size + 1] = 0;
}

int	check_key(char *str, int eq_pos)
{
	int	i;

	i = 0;
	if (!(((str[0] >= 'a') && (str[0] <= 'z'))
			|| ((str[0] >= 'A') && (str[0] <= 'Z'))))
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
	int		env_ind;
	char	*temp;

	if (!check_key(new_str, char_srch(new_str, '=')))
		return (0);
	temp = cut_equ(new_str);
	env_ind = pos_into_env(shell->env_param, temp);
	if (env_ind == -1)
		add_export(shell, new_str);
	else
	{
		free(shell->env_param[env_ind]);
		shell->env_param[env_ind] = malloc(sizeof(char)
				* (ft_strlen(new_str) + 1));
		ft_strlcpy(shell->env_param[env_ind], new_str, ft_strlen(new_str) + 1);
	}
	free(temp);
	return (1);
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
