/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:16:18 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/23 19:14:57 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_echo(char **argv)
{
	const int	n = argv[1] != NULL && argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0' && ++argv;

	while (*++argv)
	{
		if (*(argv + 1))
			printf("%s ", *argv);
		else
			printf("%s", *argv);
	}
	if (!n)
		printf("\n");
	return (EXIT_SUCCESS);
}

int	ft_pwd(char **argv)
{
	char	*buffer;
	char	*tmp;
	size_t	size;

	size = 1024;
	buffer = malloc(sizeof(*buffer) * size);
	if (!buffer)
	{
		ft_putstr_fd("pwd: Cannot allocate memory\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	while (!getcwd(buffer, size))
	{
		tmp = buffer;
		size += 1024;
		buffer = malloc(sizeof(*buffer) * size);
		if (!buffer)
		{
			free(tmp);
			ft_putstr_fd("pwd: Cannot allocate memory\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		ft_memcpy(buffer, tmp, size - 1024);
		free(tmp);
	}
	printf("%s\n", buffer);
	free(buffer);
	return (EXIT_SUCCESS);
}

int	ft_cd(char **argv)
{
	if (!argv[1])
	{
		ft_putstr_fd("cd: usage: cd [relative or absolute path]\n", STDERR_FILENO);
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

int	check_buildins(char **argv)
{
	char const	buildin[][10] = {"echo", "pwd", "cd", "exit", "export", "unset", "env"};
	int			(*ft_buildin[])(char **) = {ft_echo, ft_pwd, ft_cd};
	int			i;
	int			std_fd[2];

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(buildin[i], argv[0]) == 0)
		{
			std_fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
			printf("std_fd: %d\n", std_fd[STDOUT_FILENO]);
			int file = open("out.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
			dup2(file, STDOUT_FILENO);
			ft_buildin[i](argv);
			dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
			printf("afterwards\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void	execute(t_shell *shell)
{
	pid_t	pid;

	if (check_buildins(shell->parser_res[0]))
	{
		pid = fork();
		if (pid == 0)
		{
			execve(get_pathname(shell->parser_res[0], shell->env_param), shell->parser_res[0], shell->env_param);
			perror(shell->parser_res[0][0]);
			exit(EXIT_FAILURE);
		}
		waitpid(pid, NULL, 0);
	}
}
