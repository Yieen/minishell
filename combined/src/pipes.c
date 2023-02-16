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

extern char **environ;
extern int const	sys_nerr;

typedef struct s_program
{
	pid_t	pid;
	char *pathname;
	char **argv;
	int	inputfile;
	int outputfile;
}	t_program;

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

void	ft_echo(char **argv)
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
	exit(0);
}

char	*get_pathname(char **argv, char **env)
{
	char const	buildin[][10] = {"echo", "pwd", "cd", "exit", "export", "unset", "env"};
	void		(*ft_buildin[])(char **) = {ft_echo};
	int			i;

	if (ft_strchr(argv[0], '/') == 0)
	{
		i = 0;
		while (i < 7)
		{
			if (!ft_strcmp(buildin[i], argv[0]))
				ft_buildin[i](argv);
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
	// paths = NULL;
	// i = 0;
	// while (shell->env_param[i])
	// {
	// 	if (strcmp(shell->env_param[i], "PATH") == '=')
	// 		paths = ft_split(&shell->env_param[i][5], ':');
	// 	i++;
	// }
	// if (!paths)
	// 	exit(1);
	// char *pathname;
	// char *tmp;
	// while (*paths)
	// {
	// 	pathname = ft_strjoin(*paths++, "/");
	// 	pathname = ft_strjoin(pathname, shell->parser_res[0][0]);
	// 	if (access(pathname, F_OK) != 0)
	// 		continue ;
	// 	if (access(pathname, X_OK) != 0)
	// 	{
	// 		printf("minishell: %s: permission denied\n", pathname);
	// 		return ;
	// 	}
	// 	printf("found: %s\n", pathname);
	// 	break ;
	// }
	i = num;
	while (i--)
	{
		if (i != 0)
		{
			pipe(fd[i % 2]);//can fail
		}
		printf("call: %s\n", shell->parser_res[i][0]);
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

// int	here_document(char *delimiter)
// {
// 	int	fd[2];
// 	char	*line;

// 	pipe(fd);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		ft_putstr_fd(line, fd[1]);
// 		free(line);
// 		ft_putchar_fd('\n', fd[1]);
// 	}
// 	close(fd[1]);
// 	return (fd[0]);
// }

// int	main(int argc, char **argv)
// {
// 	t_program	*programs = malloc(sizeof(*programs) * 4);
// 	int			num = 4;


// 	char *argv1[] = {"grep", "Make", NULL};

// 	programs[0].pathname = "/usr/bin/grep";
// 	programs[0].argv = argv1;
// 	programs[0].outputfile = -1;
// 	if (argc != 1)
// 		programs[0].inputfile = here_document(argv[1]);
// 	else
// 		programs[0].inputfile = open("input.txt", O_RDONLY, S_IRUSR);
// 	if (programs[0].inputfile == -1)
// 	{
// 		printf("error file open\n");
// 		exit(1);
// 	}


// 	char *argv2[] = {"grep", "Make", NULL};

// 	programs[1].pathname = "/usr/bin/grep";
// 	programs[1].argv = argv2;
// 	programs[1].outputfile = -1;
// 	programs[1].inputfile = -1;


// 	char *argv3[] = {"grep", "Make", NULL};

// 	programs[2].pathname = "/usr/bin/grep";
// 	programs[2].argv = argv3;
// 	programs[2].outputfile = -1;
// 	programs[2].inputfile = -1;


// 	char *argv4[] = {"grep", "Make", NULL};

// 	programs[3].pathname = "/usr/bin/grep";
// 	programs[3].argv = argv4;
// 	programs[3].outputfile = open("tmp.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
// 	programs[3].inputfile = -1;


// 	execute(programs, num, environ);
// 	for (int i = 0; i < num; i++)
// 	{
// 		waitpid(programs[i].pid, NULL, 0);
// 	}
// }

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!n)
// 		return (0);
// 	while (s1[i] != '\0' && s1[i] == s2[i] && i < n - 1)
// 		i++;
// 	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// }

// size_t	ft_strlen(const char *s)
// {
// 	size_t	len;

// 	len = 0;
// 	while (s[len])
// 		len++;
// 	return (len);
// }

// int	main(int argc, char **argv)
// {
// 	char const	*path = getenv("PATH");
// 	char		**paths = ft_split(path, ':');
// 	DIR			*dir;
// 	struct dirent	*ent;
// 	char		*file;

// 	// (void)file;
// 	// (void)argv;
// 	if (argc == 1)
// 		return (1);
// 	int	i = 0;
// 	while (argv[i])
// 	{
// 		if (strcmp(argv[i], "x") == 0)
// 		{
// 			printf("pipe\n");
// 			return (0);
// 		}
// 		i++;
// 	}
// 	if (ft_strchr(argv[1], '/'))
// 	{
// 		execve(argv[1], &argv[1], environ);
// 	}
// 	else
// 	{
// 		while (*paths)
// 		{
// 			dir = opendir(*(paths));
// 			ent = readdir(dir);
// 			while (ent != NULL)
// 			{
// 				if ((ent->d_type == DT_LNK || ent->d_type == DT_REG) && ft_strcmp(argv[1], ent->d_name) == 0)
// 				{
// 					file = ft_strjoin(*paths, "/");
// 					file = ft_strjoin(file, argv[1]);
// 					execve(file, &argv[1], environ);
// 				}
// 				// printf("%s/%s\n", *paths, ent->d_name);
// 				ent = readdir(dir);
// 			}
// 			paths++;
// 		}
// 	}
	
// 	printf("minishell: command not found: %s\n", argv[1]);
// 	// DIR *dir = opendir("/bin");
// 	// struct dirent *entity;

// 	// entity = readdir(dir);
// 	// while (entity != NULL)
// 	// {
// 	// 	if (entity->d_type == DT_LNK || entity->d_type == DT_REG)
// 	// 		printf("%s\n", entity->d_name);
// 	// 	entity = readdir(dir);
// 	// }
// }

// int	main(int argc, char *argv[], char *envp[])
// {
// 	int fd[2];

// 	pipe(fd);
// 	int pid1 = fork();
// 	char *args[] = {"ls", NULL, NULL};
// 	if (pid1 == 0)
// 	{
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[0]);
// 		close(fd[1]);
// 		execve("/bin/ls", args, NULL);
// 		printf("error");
// 		exit(1);
// 	}
// 	close(fd[1]);
// 	int pid2 = fork();
// 	args[0] = "./grep";
// 	args[1] = argv[1];
// 	if (pid2 == 0)
// 	{
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		execve("/usr/bin/grep", args, NULL);
// 		printf("error");
// 		exit(1);
// 	}
// 	close(fd[0]);
// 	waitpid(pid1, NULL, 0);
// }

// int	main(int argc, char *argv[], char *envp[])
// {
// 	dprintf(STDERR_FILENO, "Hello WOrdl\n");
// 	dprintf(STDOUT_FILENO, "IDK\n");
// 	// char *args[] = {"ls", NULL};
// 	// printf("i will become ls\n");
// 	// int file = open("log.txt", O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
// 	// printf("file: %d\n", file);
// 	// dup2(file, STDOUT_FILENO);
// 	// close(file);
// 	// execve("/bin/ls", argv, envp);
// 	// printf("failure %s\n", strerror(errno));
// }
