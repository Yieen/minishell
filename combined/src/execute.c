/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:16:18 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/22 14:25:38 by jharrach         ###   ########.fr       */
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

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(buildin[i], argv[0]) == 0)
		{
			ft_buildin[i](argv);
			return (0);
		}
		i++;
	}
	return (1);
}

void	execute(t_shell *shell)
{
	if (check_buildins(shell->parser_res[0]))
		printf("call: %s\n", shell->parser_res[0][0]);
}
