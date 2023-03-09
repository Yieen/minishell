/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 00:07:18 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/09 00:30:52 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	pipex_child_input_fd(t_shell *shell, int fd[2][2], int i)
{
	if (shell->auxilar[i]->input_fd >= 0)
	{
		if (dup2(shell->auxilar[i]->input_fd, STDIN_FILENO) == -1)
		{
			perror(PROG_NAME ": dup2()");
			exit(EXIT_FAILURE);
		}
		close(shell->auxilar[i]->input_fd);
	}
	else if (i != 0)
	{
		if (dup2(fd[(i + 1) % 2][0], STDIN_FILENO) == -1)
		{
			perror(PROG_NAME ": dup2()");
			exit(EXIT_FAILURE);
		}
	}
	if (i != 0)
	{
		close(fd[(i + 1) % 2][0]);
		close(fd[(i + 1) % 2][1]);
	}
}

static void	pipex_child_output_fd(t_shell *shell, int fd[2][2], int i)
{
	if (shell->auxilar[i]->output_fd > 0)
	{
		if (dup2(shell->auxilar[i]->output_fd, STDOUT_FILENO) == -1)
		{
			perror(PROG_NAME ": dup2()");
			exit(EXIT_FAILURE);
		}
		close(shell->auxilar[i]->output_fd);
	}
	else if (i < shell->pipe_cnts)
	{
		if (dup2(fd[i % 2][1], STDOUT_FILENO) == -1)
		{
			perror(PROG_NAME ": dup2()");
			exit(EXIT_FAILURE);
		}
	}
	if (i < shell->pipe_cnts)
	{
		close(fd[i % 2][0]);
		close(fd[i % 2][1]);
	}
}

void	pipex_child(t_shell *shell, int fd[2][2], int i)
{
	pipex_child_input_fd(shell, fd, i);
	pipex_child_output_fd(shell, fd, i);
	execve(get_pathname(shell, i), shell->parser_res[i], \
		remove_empty_var(shell));
	perror(shell->parser_res[i][0]);
	exit(EXIT_FAILURE);
}
