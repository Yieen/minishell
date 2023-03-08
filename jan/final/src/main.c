/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/08 16:52:58 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// cd new_shell/jan/new_begining/
//	new_begining % gcc -lreadline  *.c
//	new_begining % gcc -I/goinfre/inovomli/.brew/opt/readline/include -lreadline  *.c

// gcc -lreadline  *.c -LLeakSanitizer -llsan -lc++   -Wno-gnu-include-next -I LeakSanitizer/include

int	create_env(t_shell *new_shell, char **envp)
{
	int envp_size;
	// char	**new_env;

	envp_size = tdar_str_calc(envp);
	
	// new_env = copy_string_array(envp, envp_size + 1);
	// new_shell->env_param = new_env;
	new_shell->env_param = copy_string_array(envp, envp_size + 1);

	// printf("size shell_env=%d\n", twodimarr_str_calc(new_shell->env_param));
	// new_shell->prompt = readline("get_curr_dirr>");
	new_shell->cont_wrk = 1;
	// new_shell->cmnd_cnt = 0;	
	new_shell->last_comm_ret = 0;

	return (0);
}

void	free_lexer(t_shell *shell)
{
	int	i;

	i = 0;	
	while (shell->lexer_res[i])
	{
		if (!((shell->lexer_res[i][0] == '|') || (shell->lexer_res[i][0] == '>') || (shell->lexer_res[i][0] == '<')))
			free(shell->lexer_res[i]);
		i++;
	}
	free(shell->lexer_res);
}

void	close_env(t_shell *shell)
{
	int	i;

	free_lexer(shell);	
	ft_clear(shell->env_param);	

	i = 0;
	while (i < shell->pipe_cnts + 2)
	{
		free(shell->parser_res[i]);
		i++;
	}
	free(shell->parser_res);
	i = 0;
	while (shell->auxilar[i])
	{
		free(shell->auxilar[i]);
		i++;
	}
	free(shell->auxilar);
}

void	remove_spaces(t_shell *shell)
{
	int const	num = shell->pipe_cnts + 1;
	int			i;
	int			j;
	int cnt;

	i = 0;
	while (i < num)
	{
		j = 0;
		while (shell->parser_res[i][j])
		{
			cnt = 0;
			while(shell->parser_res[i][j][cnt])
				cnt++;
			if (cnt == 0 && ++j)
				continue ;
			if (shell->parser_res[i][j][cnt - 1] == ' ')
				shell->parser_res[i][j][cnt - 1] = 0;
			if (shell->parser_res[i][j][cnt - 1] == '\t')
				shell->parser_res[i][j][cnt - 1] = 0;
			j++;
		}
		i++;
	}
}

int	process_running(t_shell *shell_p)
{
	static t_shell *shell;

	if (!shell)
		shell = shell_p;
	return (!shell->cont_wrk);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (process_running(NULL))
			printf("\n");
		else
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
			printf("\n" PROMPT);
		}
	}
	else if (sig == SIGQUIT)
	{
		if (process_running(NULL))
			printf("Quit: 3\n");
		else
			rl_redisplay();
	}
}

char	**remove_empty_var(t_shell *shell)
{
	int		i;

	if (!shell->env_param)
		return (NULL);
	i = 0;
	while (shell->env_param[i])
	{
		if (!ft_strchr(shell->env_param[i], '='))
		{
			unset(shell, shell->env_param[i]);
			i = 0;
			continue ;
		}
		i++;
	}
	return (shell->env_param);
}

void	run_shell(t_shell *shell)
{

	int i;
	i = 0;
	int j;
	j = 0;
	struct termios term;

	tcgetattr(STDIN_FILENO, &shell->term);
	term = shell->term;
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	process_running(shell);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while(1)
	{
		while (1)
		{
			shell->prompt = readline(PROMPT);
			if (!shell->prompt)
			{
				ft_clear(shell->env_param);
				tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
				exit(EXIT_SUCCESS);
			}
			if (shell->prompt[0] != '\0')
				break ;
		}
		add_history(shell->prompt);
		if (lexer(shell))
		{
			if (lexer(shell) == 1)
				printf("wrong input\n");
			free_lexer(shell);
			continue ;
		}	

	// i = 0;
	// while (shell->lexer_res[i])
	// {
	// 	printf("%d %s;\n",i, shell->lexer_res[i]);
	// 	i++;
	// }	
	// printf("\n");

		parser(shell);
	// i = 0;
	// j = 0;
	// while (shell->parser_res[i])
	// {
	// 	while (shell->parser_res[i][j] != 0)
	// 	{
	// 		printf("%d %d %s;\n",i,j, shell->parser_res[i][j]);
	// 		j++;
	// 	}
	// 	j = 0;
	// 	i++;
	// }
	// printf("\n");

		combine_str(shell->parser_res);
	// i = 0;
	// j = 0;
	// while (shell->parser_res[i])
	// {
	// 	while (shell->parser_res[i][j] != 0)
	// 	{
	// 		printf("%d %d %s;\n",i,j, shell->parser_res[i][j]);
	// 		j++;
	// 	}
	// 	j = 0;
	// 	i++;
	// }
	// printf("\n");		
		combine_str2(shell->parser_res);		
	// i = 0;
	// j = 0;
	// while (shell->parser_res[i])
	// {
	// 	while (shell->parser_res[i][j] != 0)
	// 	{
	// 		printf("%d %d %s;\n",i,j, shell->parser_res[i][j]);
	// 		j++;
	// 	}
	// 	j = 0;
	// 	i++;
	// }
	// printf("\n");		
		remove_spaces(shell);
		remove_quotes(shell);

	// i = 0;
	// j = 0;
	// while (shell->parser_res[i])
	// {
	// 	while (shell->parser_res[i][j] != 0)
	// 	{
	// 		printf("%d %d %s;\n",i,j, shell->parser_res[i][j]);
	// 		j++;
	// 	}
	// 	j = 0;
	// 	i++;
	// }
	// printf("\n");

		shell->auxilar = malloc(sizeof(t_pipex *) * (shell->pipe_cnts + 2));
		post_parser(shell);	

	// i = 0;
	// j = 0;
	// while (shell->parser_res[i])
	// {
	// 	while (shell->parser_res[i][j] != 0)
	// 	{
	// 		printf("%d %d %s;\n",i,j, shell->parser_res[i][j]);
	// 		j++;
	// 	}
	// 	j = 0;
	// 	i++;
	// }
	// printf("\n");

	// combine_str(shell->parser_res);

	// combine_str2(shell->parser_res);
			
	// 	remove_spaces(shell);
	// 	remove_quotes(shell);

		tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
		shell->cont_wrk = 0;
		if (shell->pipe_cnts > 0)
			pipex(shell);
		else
			execute(shell);
		// int	i = 0;
		// int	j = 0;
		// while (shell->parser_res[i])
		// {
		// 	j = 0;
		// 	while (shell->parser_res[i][j])
		// 	{
		// 		free(shell->parser_res[i][j]);
		// 		j++;
		// 	}
		// 	i++;
		// }

	int i = 0;
	// while (shell->parser_res[0][i])
	// {
	// 	printf("%d %s;\n",i, shell->parser_res[0][i]);

	// 	i++;
	// }
	
	free_lexer(shell);

 	// i = 0;
	// while (shell->parser_res[0][i])
	// {
	// 	printf("%d %s;\n",i, shell->parser_res[0][i]);

	// 	i++;
	// }	
	// int i = 0;
	// int j = 0;	

	while (i < shell->pipe_cnts + 2)
	{	
		free(shell->parser_res[i]);
		i++;
	}
	free(shell->parser_res);

	i = 0;

	while (shell->auxilar[i])
	{
		free(shell->auxilar[i]);
		i++;
	}
	free(shell->auxilar);
// write(1,"1\n",2);

		free(shell->prompt);
		tcgetattr(STDIN_FILENO, &shell->term);
		term = shell->term;
		term.c_lflag &= ~(ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		shell->cont_wrk = 1;
	}
}

// void	change_env(char	**envp, char *new_str, int ind)
// {
// 	envp[ind] = new_str;
// }

// 'echo $USER'

void	leaks(void)
{
	system("leaks minishell");
}

int main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	// atexit(leaks);
	(void)argc;
	(void)argv;
	create_env(&shell, envp);
	run_shell(&shell);
	close_env(&shell);	
}
