/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/06 22:03:46 by jharrach         ###   ########.fr       */
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

	envp_size = twodimarr_str_calc(envp);
	
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
	ft_clear(shell->env_param);	// move it from free_lexer

	i = 0;
	while (shell->parser_res[i])
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
			if (cnt == 0)
			{
				j++;
				continue ;
			}
			if (shell->parser_res[i][j][cnt - 1] == ' ')
				shell->parser_res[i][j][cnt - 1] = 0;
			if (shell->parser_res[i][j][cnt - 1] == '\t')
				shell->parser_res[i][j][cnt - 1] = 0;
			j++;
		}
		i++;
	}
}

void	del_first_last(char *str)
{
	int len;
	int i;

	i = 0;
	len = ft_strlen(str);
	while(i < len - 2)
	{
		str[i] = str[i + 1];
		i++;
	}
	str[i] = 0;
}

void	remove_quotes(t_shell *shell)
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
			if ((shell->parser_res[i][j][0] == '\"') && (shell->parser_res[i][j][cnt - 1] == '\"'))
				del_first_last(shell->parser_res[i][j]);
			else if ((shell->parser_res[i][j][0] == '\'') && (shell->parser_res[i][j][cnt - 1] == '\''))
				del_first_last(shell->parser_res[i][j]);
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
		parser(shell);
		remove_spaces(shell);
		remove_quotes(shell);
		shell->auxilar = malloc(sizeof(t_pipex *) * (shell->pipe_cnts + 2));
		post_parser(shell);	
		if (shell->parser_res[0][0] == 0)	
		{
			if (shell->auxilar[0]->input_fd >= 0)
				close(shell->auxilar[0]->input_fd);
			if (shell->auxilar[0]->output_fd >= 0)
				close(shell->auxilar[0]->output_fd);
			free_lexer(shell);
			free(shell->parser_res);
			continue ;
		}
		tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
		shell->cont_wrk = 0;
		if (shell->pipe_cnts > 0)
			pipex(shell);
		else
			execute(shell);
		int	i = 0;
		int	j = 0;
		while (shell->parser_res[i])
		{
			j = 0;
			while (shell->parser_res[i][j])
			{
				free(shell->parser_res[i][j]);
				j++;
			}
			i++;
		}
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
	// close_env(&shell);	
}
