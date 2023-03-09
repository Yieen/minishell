/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:48:43 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:49:06 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	b_exit_check_argument(t_shell *shell, int i)
{
	long	status;

	status = ft_atol(shell->parser_res[i][1]);
	if (!ft_islong(shell->parser_res[i][1]))
	{
		ft_putstr_fd(PROG_NAME ": exit: ", STDERR_FILENO);
		ft_putstr_fd(shell->parser_res[i][1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		close_env(shell);
		exit(-1);
	}
	return (status);
}

int	b_exit(t_shell *shell, int i)
{
	int		status;

	status = EXIT_SUCCESS;
	if (!shell->pipe_cnts)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (shell->parser_res[i][1] != NULL)
		status = b_exit_check_argument(shell, i);
	if (shell->parser_res[i][1] != NULL && shell->parser_res[i][2] != NULL)
	{
		ft_putstr_fd(PROG_NAME ": exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	close_env(shell);
	exit(status);
	return (0);
}
