/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:08:26 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/03 18:32:50 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**get_paths(char **env)
{
	char	**paths;
	char	*path_value;

	path_value = env_get_value(env, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
	{
		perror(PROG_NAME ": ft_split()");
		exit(EXIT_FAILURE);
	}
	return (paths);
}

static char	*create_pathname(char *path, char *name)
{
	char	*pathname;
	char	*tmp;

	pathname = ft_strjoin(path, "/");
	free(path);
	if (!pathname)
	{
		perror(PROG_NAME ": ft_strjoin()");
		exit(EXIT_FAILURE);
	}
	tmp = pathname;
	pathname = ft_strjoin(pathname, name);
	free(tmp);
	if (!pathname)
	{
		perror(PROG_NAME ": ft_strjoin()");
		exit(EXIT_FAILURE);
	}
	return (pathname);
}

char	*search_pathname(char *name, char **env)
{
	char	**paths;
	char	*pathname;

	paths = get_paths(env);
	if (paths)
	{
		while (*paths)
		{
			pathname = create_pathname(*paths++, name);
			if (access(pathname, F_OK) != 0)
			{
				free(pathname);
				continue ;
			}
			if (access(pathname, X_OK) != 0)
			{
				perror(pathname);
				exit(EXIT_FAILURE);
			}
			return (pathname);
		}
	}
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static void	check_buildings(t_shell *shell, int i)
{
	char const		buildin[][10] = \
		{"echo", "pwd", "cd", "exit", "export", "unset", "env"};
	t_buildin const	b_func[] = \
		{b_echo, b_pwd, b_cd, b_exit, b_export, b_unset, b_env};
	int				j;

	j = 0;
	while (j < 7)
	{
		if (!ft_strcmp(buildin[j], shell->parser_res[i][0]))
			exit(b_func[j](shell, i));
		j++;
	}
}

char	*get_pathname(t_shell *shell, int i)
{
	if (ft_strchr(shell->parser_res[i][0], '/') == 0)
	{
		check_buildings(shell, i);
		return (search_pathname(shell->parser_res[i][0], shell->env_param));
	}
	else
	{
		if (access(shell->parser_res[i][0], F_OK) != 0)
		{
			perror(shell->parser_res[i][0]);
			exit(EXIT_FAILURE);
		}
		if (access(shell->parser_res[i][0], X_OK) != 0)
		{
			perror(shell->parser_res[i][0]);
			exit(EXIT_FAILURE);
		}
		return (shell->parser_res[i][0]);
	}
}
