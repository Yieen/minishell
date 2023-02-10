#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include "../libft/include/libft.h"

extern char **environ;

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

int	main(int argc, char **argv)
{
	char const	*path = getenv("PATH");
	char		**paths = ft_split(path, ':');
	DIR			*dir;
	struct dirent	*ent;
	char		*file;

	// (void)file;
	// (void)argv;
	if (argc == 1)
		return (1);
	int	i = 0;
	while (argv[i])
	{
		if (strcmp(argv[i], "x") == 0)
		{
			printf("pipe\n");
			return (0);
		}
		i++;
	}
	if (ft_strchr(argv[1], '/'))
	{
		execve(argv[1], &argv[1], environ);
	}
	else
	{
		while (*paths)
		{
			dir = opendir(*(paths));
			ent = readdir(dir);
			while (ent != NULL)
			{
				if ((ent->d_type == DT_LNK || ent->d_type == DT_REG) && ft_strcmp(argv[1], ent->d_name) == 0)
				{
					file = ft_strjoin(*paths, "/");
					file = ft_strjoin(file, argv[1]);
					execve(file, &argv[1], environ);
				}
				// printf("%s/%s\n", *paths, ent->d_name);
				ent = readdir(dir);
			}
			paths++;
		}
	}
	
	printf("minishell: command not found: %s\n", argv[1]);
	// DIR *dir = opendir("/bin");
	// struct dirent *entity;

	// entity = readdir(dir);
	// while (entity != NULL)
	// {
	// 	if (entity->d_type == DT_LNK || entity->d_type == DT_REG)
	// 		printf("%s\n", entity->d_name);
	// 	entity = readdir(dir);
	// }
}

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
