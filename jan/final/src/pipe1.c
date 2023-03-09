/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 14:01:51 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/09 00:30:38 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	wait_for_pipe(t_shell *shell)
{
	int	exit_status;
	int	i;

	i = 0;
	while (i < shell->pipe_cnts + 1)
	{
		if (shell->auxilar[i]->is_exec)
			waitpid(shell->auxilar[i]->pr_id, &exit_status, 0);
		i++;
	}
	shell->last_comm_ret = WEXITSTATUS(exit_status);
}

static void	pipex_close_fds(t_shell *shell, int fd[2][2], int i)
{
	if (i != 0)
	{
		close(fd[(i + 1) % 2][0]);
		close(fd[(i + 1) % 2][1]);
	}
	if (shell->auxilar[i]->input_fd >= 0)
		close(shell->auxilar[i]->input_fd);
	if (shell->auxilar[i]->output_fd >= 0)
		close(shell->auxilar[i]->output_fd);
}

static void	pipex_fatal_error(t_shell *shell, int fd[2][2], int i, int ft)
{
	int	j;

	if (ft == 0)
		perror(PROG_NAME ": pipe()");
	else if (ft == 1)
		perror(PROG_NAME ": fork()");
	pipex_close_fds(shell, fd, i);
	j = i;
	while (++j < shell->pipe_cnts + 1)
	{
		if (shell->auxilar[j]->input_fd >= 0)
			close(shell->auxilar[j]->input_fd);
		if (shell->auxilar[j]->output_fd >= 0)
			close(shell->auxilar[j]->output_fd);
	}
	while (i--)
		kill(shell->auxilar[i]->pr_id, SIGKILL);
	close_env(shell);
	exit(EXIT_FAILURE);
}

void	pipex(t_shell *shell)
{
	int		fd[2][2];
	int		i;

	i = 0;
	while (i < shell->pipe_cnts + 1)
	{
		if (i < shell->pipe_cnts)
		{
			if (pipe(fd[i % 2]) == -1)
				pipex_fatal_error(shell, fd, i, 0);
		}
		if (shell->auxilar[i]->is_exec && shell->parser_res[i][0])
		{
			shell->auxilar[i]->pr_id = fork();
			if (shell->auxilar[i]->pr_id == -1)
				pipex_fatal_error(shell, fd, i, 1);
			if (shell->auxilar[i]->pr_id == 0)
				pipex_child(shell, fd, i);
		}
		else
			shell->auxilar[i]->pr_id = 0;
		pipex_close_fds(shell, fd, i);
		i++;
	}
	wait_for_pipe(shell);
}
