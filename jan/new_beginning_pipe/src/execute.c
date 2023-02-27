/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:16:18 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/27 19:36:54 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	ft_echo(char **argv)
// {
// 	const int	n = argv[1] != NULL && argv[1][0] == '-' \
// 		&& argv[1][1] == 'n' && argv[1][2] == '\0' && ++argv;

// 	while (*++argv)
// 	{
// 		if (*(argv + 1))
// 			printf("%s ", *argv);
// 		else
// 			printf("%s", *argv);
// 	}
// 	if (!n)
// 		printf("\n");
// 	return (EXIT_SUCCESS);
// }

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

// int	ft_pwd(char **argv)
// {
// 	char	*buffer;
// 	char	*tmp;
// 	size_t	size;

// 	size = 1024;
// 	buffer = malloc(sizeof(*buffer) * size);
// 	if (!buffer)
// 	{
// 		ft_putstr_fd("pwd: Cannot allocate memory\n", STDERR_FILENO);
// 		return (EXIT_FAILURE);
// 	}
// 	while (!getcwd(buffer, size))
// 	{
// 		tmp = buffer;
// 		size += 1024;
// 		buffer = malloc(sizeof(*buffer) * size);
// 		if (!buffer)
// 		{
// 			free(tmp);
// 			ft_putstr_fd("pwd: Cannot allocate memory\n", STDERR_FILENO);
// 			return (EXIT_FAILURE);
// 		}
// 		ft_memcpy(buffer, tmp, size - 1024);
// 		free(tmp);
// 	}
// 	printf("%s\n", buffer);
// 	free(buffer);
// 	return (EXIT_SUCCESS);
// }

int	b_pwd(t_shell *shell, int i)
{
	char *const	*argv = shell->parser_res[i];
	char	*buffer;
	char	*tmp;
	size_t	size;

	size = 1024;
	buffer = malloc(sizeof(*buffer) * size);
	if (!buffer)
	{
		perror("pwd");
		return (EXIT_FAILURE);
	}
	while (!getcwd(buffer, size))
	{
		tmp = buffer;
		size += 1024;
		buffer = malloc(sizeof(*buffer) * size);
		if (!buffer)
		{
			perror("pwd");
			free(tmp);
			return (EXIT_FAILURE);
		}
		ft_memcpy(buffer, tmp, size - 1024);
		free(tmp);
	}
	printf("%s\n", buffer);
	free(buffer);
	return (EXIT_SUCCESS);
}

// int	ft_cd(char **argv)
// {
// 	if (!argv[1])
// 	{
// 		ft_putstr_fd("cd: usage: cd [relative or absolute path]\n", \
// 			STDERR_FILENO);
// 		return (EXIT_FAILURE);
// 	}
// 	if (argv[2])
// 	{
// 		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
// 		return (EXIT_FAILURE);
// 	}
// 	if (chdir(argv[1]))
// 	{
// 		ft_dprintf(STDERR_FILENO, "minishell: cd: ");
// 		perror(argv[1]);
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

int	b_cd(t_shell *shell, int i)
{
	char *const	*argv = shell->parser_res[i];

	if (!argv[1])
	{
		ft_putstr_fd("cd: usage: cd [relative or absolute path]\n", \
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (argv[2])
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (chdir(argv[1]))
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: ");
		perror(argv[1]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// int	ft_exit(char **argv)
// {
// 	printf("minishell: exit\n");
// 	exit(EXIT_SUCCESS);
// }

int	b_exit(t_shell *shell, int i)
{
	printf("minishell: exit\n");
	exit(EXIT_SUCCESS);
}

int b_env(t_shell *shell, int i)
{
	i = -1;
	while (shell->env_param[++i])
	{
		printf("%s\n", shell->env_param[i]);
	}
	return (EXIT_SUCCESS);
}

int	check_buildins(t_shell *shell)
{
	char const			buildin[][10] = {"echo", "pwd", "cd", "exit", "export", "unset", "env"};
	t_buildin const		ft_buildin[] = {b_echo, b_pwd, b_cd, b_exit, b_export, b_unset, b_env};
	int					i;
	int					std_fd[2];
	int					ret;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(buildin[i], shell->parser_res[0][0]) == 0)
		{
			if (shell->auxilar[0]->input_fd > 0)
			{
				std_fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
				ret = dup2(shell->auxilar[0]->input_fd, STDOUT_FILENO);//
				close(shell->auxilar[0]->input_fd);
			}
			if (shell->auxilar[0]->output_fd >= 0)
			{
				std_fd[0] = dup(STDIN_FILENO);
				ret = dup2(shell->auxilar[0]->output_fd, STDIN_FILENO);//
				close(shell->auxilar[0]->output_fd);
			}
			shell->last_comm_ret = ft_buildin[i](shell, 0);
			if (shell->auxilar[0]->input_fd > 0)
				dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
			if (shell->auxilar[0]->output_fd >= 0)
				dup2(std_fd[0], STDIN_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

void	execute(t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		stdout_fd;

	if (check_buildins(shell))
	{
		if (!shell->auxilar[0]->is_exec)
			return ;
		pid = fork();
		if (pid == 0)
		{
			if (shell->auxilar[0]->input_fd > 0)
			{
				dup2(shell->auxilar[0]->input_fd, STDOUT_FILENO);//
				close(shell->auxilar[0]->input_fd);
			}
			if (shell->auxilar[0]->output_fd >= 0)
			{
				dup2(shell->auxilar[0]->output_fd, STDIN_FILENO);//
				close(shell->auxilar[0]->output_fd);
			}
			execve(get_pathname(shell, 0), shell->parser_res[0], shell->env_param);
			perror(shell->parser_res[0][0]);
			exit(EXIT_FAILURE);
		}
		if (shell->auxilar[0]->input_fd > 0)
			close(shell->auxilar[0]->input_fd);
		waitpid(pid, &status, 0);
		shell->last_comm_ret = WEXITSTATUS(status);
	}
}
