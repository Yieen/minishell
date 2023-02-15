#include <stdio.h>

int	main(int argc, char **argv)
{
	const int	n = argv[1] != NULL && argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0' && ++argv;

	(void)argc;
	while (*++argv)
	{
		if (*(argv + 1))
			printf("%s ", *argv);
		else
			printf("%s", *argv);
	}
	if (!n)
		printf("\n");
}
