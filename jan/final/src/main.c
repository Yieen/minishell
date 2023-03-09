/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 16:05:59 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	create_env(&shell, envp);
	process_running(&shell);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	run_shell(&shell);
}
