/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/06 18:32:20 by inovomli         ###   ########.fr       */
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

void	read_prompt(t_shell *shell)
{
	// shell->prompt = readline("get_curr_dirr>");
	// shell->prompt = malloc(sizeof(char *) * (wrds_s_cnt(shell->prompt) + 1));	
	// shell->prompt = "|"; 
	// shell->prompt = "echo ls>>bs $a $$ as 1| wc -l | exp<<ort a=abs"; 
	// shell->prompt = "\'echo abs>$a$|no$a2$\'1|\"wc -l\"| export a=abs"; 	
	shell->prompt = "\"echo a>bs$a$|no$a2 $\"1|\"wc -l\"| export a=abs"; 
	// shell->prompt = "echo ls << << 35 $  | test add sth << sth1     | asd < 123"; 	
	// shell->prompt = "echo ls >> >> 35 $  | test add sth > sth1 | asd >> 123"; 		
	// shell->prompt = "echo ls > > 35 $  | test add sth > sth1 | asd 123 "; 		
		// shell->prompt = "echo 123 $a dfg | sth";	
		// shell->lexer_res = malloc(sizeof(char *) * (ft_strlen(shell->prompt) + 1));


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


void	run_shell(t_shell *shell)
{
	// int i;

	struct termios term;
	struct termios term_old;
	tcgetattr(STDIN_FILENO, &term_old);
	tcgetattr(STDIN_FILENO, &term);
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
				tcsetattr(STDIN_FILENO, TCSANOW, &term_old);
				exit(EXIT_SUCCESS);
			}
			if (shell->prompt[0] != '\0')
				break ;
		}
		add_history(shell->prompt);
			// write(1,"0\n",2);	
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

		parser(shell);

// write(1,"1\n", 2);
	// i = 0;
	// while (shell->parser_res[0][i])
	// {
	// 	printf("%d %s;\n",i, shell->parser_res[0][i]);

	// 	i++;
	// }

		remove_spaces(shell);
		remove_quotes(shell);
		shell->auxilar = malloc(sizeof(t_pipex *) * (shell->pipe_cnts + 2));
		post_parser(shell);	
		// write(1,"7\n", 2);
		if (shell->parser_res[0][0] == 0)	
		{
			free_lexer(shell);
			free(shell->parser_res);
			continue ;
		}
	// i = 0;
	// while (shell->parser_res[0][i])
	// {
	// 	printf("%d %s;\n",i, shell->parser_res[0][i]);

	// 	i++;
	// }


		tcsetattr(STDIN_FILENO, TCSANOW, &term_old);		
		shell->cont_wrk = 0;
		if (shell->pipe_cnts > 0)
			pipex(shell);
		else
			execute(shell);	
		free(shell->prompt);
		tcgetattr(STDIN_FILENO, &term_old);
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		shell->cont_wrk = 1;
	}
}

// void	change_env(char	**envp, char *new_str, int ind)
// {
// 	envp[ind] = new_str;
// }

// 'echo $USER'

int main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	create_env(&shell, envp);
	run_shell(&shell);
	// close_env(&shell);	
}
