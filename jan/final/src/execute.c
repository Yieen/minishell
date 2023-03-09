/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:16:18 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:58:27 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	buildin_error(t_shell *shell, int fd1, int fd2, int num)
{
	if (num == 1)
		perror(PROG_NAME ": error: dup()");
	else if (num == 2)
		perror(PROG_NAME ": error: dup2()");
	else if (num == 3)
		perror(PROG_NAME ": error: fork()");
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	close_env(shell);
	exit(EXIT_FAILURE);
}

static void	buildin_with_redirection(t_shell *shell, t_buildin ft_buildin)
{
	int	std_out;

	std_out = dup(STDOUT_FILENO);
	if (std_out == -1)
		buildin_error(shell, shell->auxilar[0]->output_fd, -1, 1);
	if (dup2(shell->auxilar[0]->output_fd, STDOUT_FILENO) == -1)
		buildin_error(shell, shell->auxilar[0]->output_fd, std_out, 2);
	close(shell->auxilar[0]->output_fd);
	shell->last_comm_ret = ft_buildin(shell, 0);
	if (dup2(std_out, STDOUT_FILENO) == -1)
		buildin_error(shell, -1, std_out, 2);
	close(std_out);
}

int	check_buildins(t_shell *shell)
{
	char const		buildin[][10] = {"echo", "pwd", "cd",
		"exit", "export", "unset", "env"};
	t_buildin const	ft_buildin[] = {b_echo, b_pwd, b_cd,
		b_exit, b_export, b_unset, b_env};
	int				i;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(buildin[i], shell->parser_res[0][0]) == 0)
		{
			if (shell->auxilar[0]->input_fd >= 0)
				close(shell->auxilar[0]->input_fd);
			if (shell->auxilar[0]->output_fd > 0)
				buildin_with_redirection(shell, ft_buildin[i]);
			else
				shell->last_comm_ret = ft_buildin[i](shell, 0);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	execute_child(t_shell *shell)
{
	if (shell->auxilar[0]->output_fd > 0)
	{
		if (dup2(shell->auxilar[0]->output_fd, STDOUT_FILENO) == -1)
		{
			perror(PROG_NAME ": error: dup2()");
			exit(EXIT_FAILURE);
		}
		close(shell->auxilar[0]->output_fd);
	}
	if (shell->auxilar[0]->input_fd >= 0)
	{
		if (dup2(shell->auxilar[0]->input_fd, STDIN_FILENO) == -1)
		{
			perror(PROG_NAME ": error: dup2()");
			exit(EXIT_FAILURE);
		}
		close(shell->auxilar[0]->input_fd);
	}
	execve(get_pathname(shell, 0), shell->parser_res[0], \
		remove_empty_var(shell));
	perror(PROG_NAME ": error: execve()");
	exit(EXIT_FAILURE);
}

void	execute(t_shell *shell)
{
	int		status;

	if (!shell->auxilar[0]->is_exec || !shell->parser_res[0][0])
	{
		if (shell->auxilar[0]->output_fd > 0)
			close(shell->auxilar[0]->output_fd);
		if (shell->auxilar[0]->input_fd >= 0)
			close(shell->auxilar[0]->input_fd);
		return ;
	}
	if (check_buildins(shell))
	{
		shell->auxilar[0]->pr_id = fork();
		if (shell->auxilar[0]->pr_id == -1)
			buildin_error(shell, shell->auxilar[0]->output_fd, \
				shell->auxilar[0]->input_fd, 3);
		else if (shell->auxilar[0]->pr_id == 0)
			execute_child(shell);
		if (shell->auxilar[0]->output_fd > 0)
			close(shell->auxilar[0]->output_fd);
		if (shell->auxilar[0]->input_fd >= 0)
			close(shell->auxilar[0]->input_fd);
		waitpid(shell->auxilar[0]->pr_id, &status, 0);
		shell->last_comm_ret = WEXITSTATUS(status);
	}
}
