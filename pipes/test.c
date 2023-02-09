#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include "../../libft/include/libft.h"

extern char **environ;

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!n)
		return (0);
	while (s1[i] != '\0' && s1[i] == s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 3)
		return (1);
	DIR *dir = opendir("/bin");

	struct dirent *entity;

	entity = readdir(dir);
	size_t	len = ft_strlen(argv[1]) + 1;
	while (entity != NULL)
	{
		printf("%d, %s\n", entity->d_type, entity->d_name);
		if (ft_strncmp(argv[1], entity->d_name, len) == 0)
			execve("")
		entity = readdir(dir);
	}
	printf("%d\n", ft_strncmp(argv[1], argv[2], len + 1));
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
