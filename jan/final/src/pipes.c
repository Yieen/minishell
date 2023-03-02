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

static void	ft_close_pipe(int *pipe)
{
	close(pipe[0]);
	close(pipe[1]);
}

static void	dup2_error(int *pipe)
{
	ft_close_pipe(pipe);
	perror("minishell");
	exit(EXIT_FAILURE);
}

void	pipex(t_shell *shell)
{
	int		fd[2][2];
	int		i;
	int		num = shell->pipe_cnts + 1;
	int		status;

	i = 0;
	while (i < num)
	{
		if (i < num - 1)
		{
			if (pipe(fd[i % 2]) == -1)
			{
				if (i != 0)
					ft_close_pipe(fd[(i + 1) % 2]);
				perror("minishell");
				return ;
			}
		}
		if (!shell->auxilar[i]->is_exec)
		{
			shell->auxilar[i]->pr_id = 0;
			continue ;
		}
		shell->auxilar[i]->pr_id = fork();
		if (shell->auxilar[i]->pr_id == -1)
			perror("minishell");
		if (shell->auxilar[i]->pr_id == 0)
		{
			if (shell->auxilar[i]->output_fd >= 0)
			{
				dup2(shell->auxilar[i]->output_fd, STDIN_FILENO);//
				close(shell->auxilar[i]->output_fd);
			}
			else if (i != 0)
			{
				if (dup2(fd[(i + 1) % 2][0], STDIN_FILENO) == -1)
					dup2_error(fd[(i + 1) % 2]);
				ft_close_pipe(fd[(i + 1) % 2]);
			}
			if (shell->auxilar[i]->input_fd > 0)
			{
				dup2(shell->auxilar[i]->input_fd, STDOUT_FILENO);//
				close(shell->auxilar[i]->input_fd);
			}
			else if (shell->auxilar[i]->input_fd <= 0 && i < num - 1)
			{
				if (dup2(fd[i % 2][1], STDOUT_FILENO) == -1)
					dup2_error(fd[i % 2]);
				ft_close_pipe(fd[i % 2]);
			}
			execve(get_pathname(shell, i), shell->parser_res[i], shell->env_param);
			perror(shell->parser_res[i][0]);
			exit(EXIT_FAILURE);
		}
		if (i != 0)
			ft_close_pipe(fd[(i + 1) % 2]);
		i++;
	}
	for (int i = 0; i < num; i++)
	{
		waitpid(shell->auxilar[i]->pr_id, &status, 0);
		shell->last_comm_ret = WEXITSTATUS(status);
	}
}

// void	pipex(t_shell *shell)
// {
// 	int		fd[2][2];
// 	int		i;
// 	int		num = shell->pipe_cnts + 1;
// 	pid_t	*pid = malloc(sizeof(*pid) * num);
// 	char	*pathname;

// 	i = num;
// 	while (i--)
// 	{
// 		if (i != 0)
// 		{
// 			pipe(fd[i % 2]);//can fail
// 		}
// 		pid[i] = fork();//can fail
// 		if (pid[i] == 0)
// 		{
// 			if (i != 0)
// 			{
// 				dup2(fd[i % 2][0], STDIN_FILENO);
// 				close(fd[i % 2][0]);
// 				close(fd[i % 2][1]);
// 			}
// 			if (i != num - 1)
// 			{
// 				dup2(fd[(i + 1) % 2][1], STDOUT_FILENO);
// 				close(fd[(i + 1) % 2][0]);
// 				close(fd[(i + 1) % 2][1]);
// 			}
// 			// if (programs[i].outputfile != -1)
// 			// {
// 			// 	dup2(programs[i].outputfile, STDOUT_FILENO);//can fail
// 			// 	close(programs[i].outputfile);
// 			// }
// 			// if (programs[i].inputfile != -1)
// 			// {
// 			// 	dup2(programs[i].inputfile, STDIN_FILENO);//can fail
// 			// 	close(programs[i].inputfile);
// 			// }
// 			pathname = get_pathname(shell->parser_res[i], shell->env_param);
// 			printf("pathname: %s\n", pathname);
// 			execve(pathname, &shell->parser_res[i][0], shell->env_param);//can fail
// 			// ft_putstr_fd("minishell: ", STDERR_FILENO);
// 			// ft_putstr_fd(programs[i].argv[0], STDERR_FILENO);
// 			// ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
// 			exit(1);
// 		}
// 		// if (programs[i].outputfile != -1)
// 		// {
// 		// 	close(programs[i].outputfile);
// 		// }
// 		// if (programs[i].inputfile != -1)
// 		// {
// 		// 	close(programs[i].inputfile);
// 		// }
// 		if (i != num - 1)
// 		{
// 			close(fd[(i + 1) % 2][0]);
// 			close(fd[(i + 1) % 2][1]);
// 		}
// 	}
// 	for (int i = 0; i < shell->pipe_cnts + 1; i++)
// 		waitpid(pid[i], NULL, 0);
// }
