/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:55:23 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:57:35 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	add_export(t_shell *shell, char *new_str)
{
	int		envp_size;
	char	**new_env;

	envp_size = tdar_str_calc(shell->env_param);
	new_env = copy_string_array(shell->env_param, envp_size + 2);
	ft_clear(shell->env_param);
	shell->env_param = new_env;
	shell->env_param[envp_size] = malloc(sizeof(char)
			* (ft_strlen(new_str) + 1));
	ft_strlcpy(shell->env_param[envp_size], new_str, ft_strlen(new_str) + 1);
	shell->env_param[envp_size + 1] = 0;
}

static int	check_key(char *str, int eq_pos)
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

static int	export_env(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->env_param[++i])
	{
		printf("declare -x %s\n", shell->env_param[i]);
	}
	return (EXIT_SUCCESS);
}

int	export(t_shell *shell, char *new_str)
{
	int		env_ind;
	char	*temp;

	if ((new_str == 0) || (new_str[0] == '\0'))
		return (export_env(shell));
	if (!check_key(new_str, char_srch(new_str, '=')))
	{
		ft_putstr_fd(PROG_NAME ": export: wrong input\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	temp = cut_equ(new_str);
	env_ind = pos_into_env(shell->env_param, temp);
	if (env_ind == -1)
		add_export(shell, new_str);
	else if (char_srch(new_str, '=') != -1)
	{
		free(shell->env_param[env_ind]);
		shell->env_param[env_ind] = malloc(sizeof(char)
				* (ft_strlen(new_str) + 1));
		ft_strlcpy(shell->env_param[env_ind], new_str, ft_strlen(new_str) + 1);
	}
	free(temp);
	return (EXIT_SUCCESS);
}

int	b_export(t_shell *shell, int i)
{
	int	ret;
	int	j;

	if (!shell->parser_res[i][1])
		return (export(shell, shell->parser_res[i][1]));
	j = 1;
	while (shell->parser_res[i][j])
		ret = export(shell, shell->parser_res[i][j++]);
	return (ret);
}
