/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jan-arvid <jan-arvid@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 14:01:51 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 22:08:00 by jan-arvid        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	ft_close_pipe(int *pipe)
{
	close(pipe[0]);
	close(pipe[1]);
}

static void	dup2_error(void)
{
	perror(PROG_NAME ": dup2()");
	exit(EXIT_FAILURE);
}

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
		ft_close_pipe(fd[(i + 1) % 2]);
	if (shell->auxilar[i]->input_fd >= 0)
		close(shell->auxilar[i]->input_fd);
	if (shell->auxilar[i]->output_fd >= 0)
		close(shell->auxilar[i]->output_fd);
}

static void	pipex_child_input_fd(t_shell *shell, int fd[2][2], int i)
{
	if (shell->auxilar[i]->input_fd >= 0)
	{
		if (dup2(shell->auxilar[i]->input_fd, STDIN_FILENO) == -1)
			dup2_error();
		close(shell->auxilar[i]->input_fd);
	}
	else if (i != 0)
	{
		if (dup2(fd[(i + 1) % 2][0], STDIN_FILENO) == -1)
			dup2_error();
	}
	if (i != 0)
		ft_close_pipe(fd[(i + 1) % 2]);
}

static void	pipex_child_output_fd(t_shell *shell, int fd[2][2], int i)
{
	if (shell->auxilar[i]->output_fd > 0)
	{
		if (dup2(shell->auxilar[i]->output_fd, STDOUT_FILENO) == -1)
			dup2_error();
		close(shell->auxilar[i]->output_fd);
	}
	else if (i < shell->pipe_cnts)
	{
		if (dup2(fd[i % 2][1], STDOUT_FILENO) == -1)
			dup2_error();
	}
	if (i < shell->pipe_cnts)
		ft_close_pipe(fd[i % 2]);
}

static void	pipex_child(t_shell *shell, int fd[2][2], int i)
{
	pipex_child_input_fd(shell, fd, i);
	pipex_child_output_fd(shell, fd, i);
	execve(get_pathname(shell, i), shell->parser_res[i], remove_empty_var(shell));
	perror(shell->parser_res[i][0]);
	exit(EXIT_FAILURE);
}

static void	pipex_fatal_error(t_shell *shell, int fd[2][2], int i, int ft)
{
	if (ft == 0)
		perror(PROG_NAME ": pipe()");
	else if (ft == 1)
		perror(PROG_NAME ": fork()");
	pipex_close_fds(shell, fd, i);
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
