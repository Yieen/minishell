#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/history.h>
#include <unistd.h>

void	handler(int signum)
{
	if (signum == SIGINT)
		printf("Sigint\n");
	usleep(50000000);
}


int	main(void)
{
	char *line;

	// line = getenv("HOSTNAME");
	// printf("%s\n", line);
	struct sigaction	sa;

	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		line = readline("username$ ");
		add_history(line);
		rl_on_new_line();
		printf("%s\n", line);
		free(line);
	}
}
