#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

char *ft_getcwd(void)//should not update $PWD
{
	char buffer[PATH_MAX];
	char *r;

	r = getcwd(buffer, PATH_MAX);
	if (!r)
		return (NULL);
	return (strdup(buffer));
}

int	ft_chdir(char *pathname)//should update $PWD
{
	if (chdir(pathname))
	{
		printf("cd: invalid argument: %s\n", pathname);
		return (1);
	}
	return (0);
}

int	main(void)
{
	char *cwd = ft_getcwd();
	printf("%s\n", cwd);\
	ft_chdir("...");
	printf("%s\n", ft_getcwd());
}
