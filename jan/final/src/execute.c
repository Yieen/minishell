/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:16:18 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/06 21:22:31 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	b_echo(t_shell *shell, int i)
{
	char *const	*argv = shell->parser_res[i];
	const int	n = argv[1] != NULL && argv[1][0] == '-' \
		&& argv[1][1] == 'n' && argv[1][2] == '\0' && ++argv;

	while (*++argv)
	{
		if (*(argv + 1))
			ft_printf("%s ", *argv);
		else
			ft_printf("%s", *argv);
	}
	if (!n)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}

static char	*b_pwd_realloc_buf(char *buffer, size_t *size)
{
	char	*new_buffer;

	*size += PWD_BUF_SIZE;
	new_buffer = malloc(sizeof(*new_buffer) * (*size));
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	ft_memcpy(new_buffer, buffer, (*size) - PWD_BUF_SIZE);
	free(buffer);
	return (new_buffer);
}

char *b_get_pwd(void)
{
	char	*buffer;
	size_t	size;

	size = PWD_BUF_SIZE;
	buffer = malloc(sizeof(*buffer) * size);
	if (!buffer)
		return (NULL);
	while (!getcwd(buffer, size))
	{
		buffer = b_pwd_realloc_buf(buffer, &size);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

int	b_pwd(t_shell *shell, int i)
{
	char	*pwd;

	(void)shell;
	(void)i;
	pwd = b_get_pwd();
	if (!pwd)
	{
		perror(PROG_NAME ": pwd: error: malloc()");
		return (EXIT_FAILURE);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (EXIT_SUCCESS);
}

static int	b_cd_set_pwd(t_shell *shell)
{
	char		*pwd;
	char		*var;
	char		*old_pwd;

	old_pwd = env_get_value(shell->env_param, "PWD");
	if (!old_pwd)
		var = ft_strdup("OLDPWD=");
	else
		var = ft_strjoin("OLDPWD=", old_pwd);
	if (export(shell, var))
	{
		perror(PROG_NAME ": cd: error: export()");
		return (EXIT_FAILURE);
	}
	pwd = b_get_pwd();
	if (!pwd)
	{
		perror(PROG_NAME ": cd: error: b_get_pwd()");
		return (EXIT_FAILURE);
	}
	var = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (!var)
	{
		perror(PROG_NAME ": cd: error: ft_strjoin()");
		return (EXIT_FAILURE);
	}
	if (export(shell, var))
	{
		perror(PROG_NAME ": cd: error: export()");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	b_cd(t_shell *shell, int i)
{
	char *const	*argv = shell->parser_res[i];

	if (!argv[1])
	{
		ft_putstr_fd(PROG_NAME ": usage: cd [relative or absolute path]\n", \
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (chdir(argv[1]))
	{
		ft_putstr_fd(PROG_NAME ": cd: ", STDERR_FILENO);
		perror(argv[1]);
		return (EXIT_FAILURE);
	}
	return (b_cd_set_pwd(shell));
}

void	free_minishell(t_shell *shell)
{
	(void)shell;
}

static int	b_exit_check_argument(t_shell *shell, int i)
{
	char	*tmp;
	int		status;
	int		is_num;

	status = ft_atoi(shell->parser_res[i][1]);
	tmp = ft_itoa(status);
	if (!tmp)
	{
		perror(PROG_NAME ": exit: error: malloc()");
		free_minishell(shell);
		exit(EXIT_FAILURE);
	}
	is_num = !ft_strcmp(shell->parser_res[i][1], tmp);
	free(tmp);
	if (!is_num)
	{
		ft_putstr_fd(PROG_NAME ": exit: ", STDERR_FILENO);
		ft_putstr_fd(shell->parser_res[i][1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		free_minishell(shell);
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
	free_minishell(shell);
	exit(status);
}

int	b_env(t_shell *shell, int i)
{
	i = -1;
	while (shell->env_param[++i])
	{
		if (ft_strchr(shell->env_param[i], '='))
			printf("%s\n", shell->env_param[i]);
	}
	return (EXIT_SUCCESS);
}

static void	buildin_error(t_shell *shell, int fd1, int fd2, int dup_num)
{
	if (dup_num == 1)
		perror(PROG_NAME ": error: dup()");
	else if (dup_num == 2)
		perror(PROG_NAME ": error: dup2()");
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
	free_minishell(shell);
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
	execve(get_pathname(shell, 0), shell->parser_res[0], remove_empty_var(shell));
	perror(PROG_NAME ": error: execve()");
	exit(EXIT_FAILURE);
}

void	execute(t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!shell->auxilar[0]->is_exec)
		return ;
	if (check_buildins(shell))
	{
		if (!shell->auxilar[0]->is_exec)
			return ;
		pid = fork();
		if (pid == -1)
		{
			perror(PROG_NAME ": error: fork()");
			free_minishell(shell);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			execute_child(shell);
		if (shell->auxilar[0]->output_fd > 0)
			close(shell->auxilar[0]->output_fd);
		if (shell->auxilar[0]->input_fd >= 0)
			close(shell->auxilar[0]->input_fd);
		waitpid(pid, &status, 0);
		shell->last_comm_ret = WEXITSTATUS(status);
	}
}
