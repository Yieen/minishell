/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jan-arvid <jan-arvid@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/08 22:46:49 by jan-arvid        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_lexer(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->lexer_res[i])
	{
		if (!((shell->lexer_res[i][0] == '|') || (shell->lexer_res[i][0] == '>')
			|| (shell->lexer_res[i][0] == '<')))
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
	if (shell->prompt)
		free(shell->prompt);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
	*shell = (t_shell){0};
}

void	add_free(t_shell *shell)
{
	int	i;

	i = 0;
	free_lexer(shell);
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
	free(shell->prompt);
}

void	infinity(t_shell *shell)
{
	while (1)
	{
		shell->prompt = readline(PROMPT);
		if (!shell->prompt)
		{
			ft_clear(shell->env_param);
			tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
			ft_putstr_fd("exit\n", STDERR_FILENO);
			exit(EXIT_SUCCESS);
		}
		if (shell->prompt[0] != '\0')
			return ;
		free(shell->prompt);
	}
}

void	runsh_part(t_shell *shell)
{
	parser(shell);
	combine_str(shell->parser_res);
	combine_str2(shell->parser_res);
	remove_spaces(shell);
	remove_quotes(shell);
	shell->auxilar = malloc(sizeof(t_pipex *) * (shell->pipe_cnts + 2));
	post_parser(shell);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
	shell->cont_wrk = 0;
	if (shell->pipe_cnts > 0)
		pipex(shell);
	else
		execute(shell);
	add_free(shell);
}

void	run_shell(t_shell *shell)
{
	struct termios	term;
	int				lx_res;

	while (1)
	{
		tcgetattr(STDIN_FILENO, &shell->term);
		term = shell->term;
		term.c_lflag &= ~(ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		infinity(shell);
		add_history(shell->prompt);
		lx_res = lexer(shell);
		if (lx_res == 1)
			printf("wrong input\n");
		if (lx_res)
		{
			free_lexer(shell);
			free(shell->prompt);
			continue ;
		}
		runsh_part(shell);
		shell->cont_wrk = 1;
	}
}

void	leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	// atexit(leaks);
	(void)argc;
	(void)argv;
	create_env(&shell, envp);
	process_running(&shell);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	run_shell(&shell);
	printf("main exit\n");
	close_env(&shell);
}
