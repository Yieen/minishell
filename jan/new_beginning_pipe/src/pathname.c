/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:08:26 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/27 16:37:44 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_getenv(char *name, char **env)
{
	int	i;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strcmp(env[i], name) == '=')
			return (&env[i][ft_strlen(name) + 1]);
		i++;
	}
	return (NULL);
}

static char	**get_paths(char **env)
{
	char	**paths;

	paths = ft_split(ft_getenv("PATH", env), ':');
	if (!paths)
	{
		perror("minishell");
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
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	tmp = pathname;
	pathname = ft_strjoin(pathname, name);
	free(tmp);
	if (!pathname)
	{
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	return (pathname);
}

char	*search_pathname(char *name, char **env)
{
	char	**paths;
	char	*pathname;
	char	*tmp;

	paths = get_paths(env);
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
	ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", name);
	exit(EXIT_FAILURE);
}

static void	check_buildings(t_shell *shell, int i)
{
	char const	buildin[][10] = \
		{"echo", "pwd", "cd", "exit", "export", "unset", "env"};
	t_buildin const b_func[] = {b_echo, b_pwd, b_cd, b_exit, b_export, b_unset, b_env};
	int			j;

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
	char *const	*argv = shell->parser_res[i];

	if (ft_strchr(argv[0], '/') == 0)
	{
		check_buildings(shell, i);
		return (search_pathname(argv[0], shell->env_param));
	}
	else
	{
		if (access(argv[0], F_OK) != 0)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		if (access(argv[0], X_OK) != 0)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
		return (argv[0]);
	}
}
