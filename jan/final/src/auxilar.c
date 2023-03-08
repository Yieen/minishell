/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 20:17:16 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/08 20:50:57 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_env(t_shell *new_shell, char **envp)
{
	int	envp_size;

	envp_size = tdar_str_calc(envp);
	new_shell->env_param = copy_string_array(envp, envp_size + 1);
	new_shell->cont_wrk = 1;
	new_shell->last_comm_ret = 0;
	return (0);
}

void	remove_spaces(t_shell *shell)
{
	int const	num = shell->pipe_cnts + 1;
	int			i;
	int			j;
	int			cnt;

	i = 0;
	while (i < num)
	{
		j = 0;
		while (shell->parser_res[i][j])
		{
			cnt = 0;
			while (shell->parser_res[i][j][cnt])
				cnt++;
			if (cnt == 0 && ++j)
				continue ;
			if (shell->parser_res[i][j][cnt - 1] == ' ')
				shell->parser_res[i][j][cnt - 1] = 0;
			if (shell->parser_res[i][j][cnt - 1] == '\t')
				shell->parser_res[i][j][cnt - 1] = 0;
			j++;
		}
		i++;
	}
}

int	process_running(t_shell *shell_p)
{
	static t_shell	*shell;

	if (!shell)
		shell = shell_p;
	return (!shell->cont_wrk);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (process_running(NULL))
			printf("\n");
		else
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			printf("\n" PROMPT);
		}
	}
	else if (sig == SIGQUIT)
	{
		if (process_running(NULL))
			printf("Quit: 3\n");
		else
			rl_redisplay();
	}
}

char	**remove_empty_var(t_shell *shell)
{
	int		i;

	if (!shell->env_param)
		return (NULL);
	i = 0;
	while (shell->env_param[i])
	{
		if (!ft_strchr(shell->env_param[i], '='))
		{
			unset(shell, shell->env_param[i]);
			i = 0;
			continue ;
		}
		i++;
	}
	return (shell->env_param);
}
