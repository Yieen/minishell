#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <readline/readline.h>
#include "../libft/include/libft.h"
#include "../include/minishell.h"
#include <errno.h>

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

char	*search_pathname(char *name, char **env)
{
	const char	*path = ft_getenv("PATH", env);
	char		**paths;
	char		*pathname;
	char		*tmp;

	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	while (*paths)
	{
		pathname = ft_strjoin(*paths, "/");
		free(*paths);
		paths++;
		tmp = pathname;
		pathname = ft_strjoin(pathname, name);
		free(tmp);
		if (access(pathname, F_OK) != 0)
		{
			free(pathname);
			continue ;
		}
		if (access(pathname, X_OK) != 0)
		{
			printf("minishell: permission denied: %s\n", pathname);
			free(pathname);
			return (NULL);
		}
		return (pathname);
	}
	return (NULL);
}

char	*get_pathname(char **argv, char **env)
{
	char const	buildin[][10] = {"echo", "pwd", "cd", "exit", "export", "unset", "env"};
	int			(*ft_buildin[])(char **) = {ft_echo, ft_pwd, ft_cd};
	int			i;

	if (ft_strchr(argv[0], '/') == 0)
	{
		i = 0;
		while (i < 7)
		{
			if (!ft_strcmp(buildin[i], argv[0]))
				exit(ft_buildin[i](argv));
			i++;
		}
		return (search_pathname(argv[0], env));
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

void	pipex(t_shell *shell)
{
	int		fd[2][2];
	int		i;
	int		num = shell->pipe_cnts + 1;
	pid_t	*pid = malloc(sizeof(*pid) * num);
	char	*pathname;

	i = num;
	while (i--)
	{
		if (i != 0)
		{
			pipe(fd[i % 2]);//can fail
		}
		pid[i] = fork();//can fail
		if (pid[i] == 0)
		{
			if (i != 0)
			{
				dup2(fd[i % 2][0], STDIN_FILENO);
				close(fd[i % 2][0]);
				close(fd[i % 2][1]);
			}
			if (i != num - 1)
			{
				dup2(fd[(i + 1) % 2][1], STDOUT_FILENO);
				close(fd[(i + 1) % 2][0]);
				close(fd[(i + 1) % 2][1]);
			}
			// if (programs[i].outputfile != -1)
			// {
			// 	dup2(programs[i].outputfile, STDOUT_FILENO);//can fail
			// 	close(programs[i].outputfile);
			// }
			// if (programs[i].inputfile != -1)
			// {
			// 	dup2(programs[i].inputfile, STDIN_FILENO);//can fail
			// 	close(programs[i].inputfile);
			// }
			pathname = get_pathname(shell->parser_res[i], shell->env_param);
			printf("pathname: %s\n", pathname);
			execve(pathname, &shell->parser_res[i][0], shell->env_param);//can fail
			// ft_putstr_fd("minishell: ", STDERR_FILENO);
			// ft_putstr_fd(programs[i].argv[0], STDERR_FILENO);
			// ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			exit(1);
		}
		// if (programs[i].outputfile != -1)
		// {
		// 	close(programs[i].outputfile);
		// }
		// if (programs[i].inputfile != -1)
		// {
		// 	close(programs[i].inputfile);
		// }
		if (i != num - 1)
		{
			close(fd[(i + 1) % 2][0]);
			close(fd[(i + 1) % 2][1]);
		}
	}
	for (int i = 0; i < shell->pipe_cnts + 1; i++)
		waitpid(pid[i], NULL, 0);
}
