/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/02 18:13:03 by jharrach         ###   ########.fr       */
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
	ft_clear(shell->env_param);	
}

void	close_env(t_shell *shell)
{
	int	i;

	free_lexer(shell);	
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



int	is_sp_sim(char ch)
{
	const char	*special_simbols = "|>< \t\n";
	int cnt;

	cnt = 0;
	while(special_simbols[cnt])
	{
		if (ch == special_simbols[cnt])
			return (1);
		else
			cnt++;
	}
	if (ch == '\0')
	    return (1);	
	return (0);
}

int	find_end_key(char *str, int st)
{
	while (str[st])
	{
		if (str[st] == '?')
			return (st);
		if(is_sp_sim(str[st]) || (str[st] == '$'))
			return (st - 1);
		st++;
	}
	return (st - 1);
}

void	first_part_wwd(t_dolar	*wwd, int i)
{
	wwd->save_pos = malloc(sizeof(char) * ft_strlen(wwd->tlr[i]));
	wwd->s_p_cnt = 0;
	wwd->d_pos = char_srch(wwd->tlr[i], '$');	
}

void	second_part_wwd(t_dolar	*wwd, int i,  t_shell *shell)
{
	wwd->start = ft_substr(wwd->tlr[i], 0, wwd->d_pos);			
	ft_strlcpy(wwd->rs_st, wwd->start, ft_strlen(wwd->start) + 1);
	free(wwd->start);
	wwd->end_key = find_end_key(wwd->tlr[i], wwd->d_pos + 1);
	wwd->key = ft_substr(wwd->tlr[i], wwd->d_pos + 1, wwd->end_key - wwd->d_pos);
	if ((wwd->key[0] == '\0') || (wwd->key == 0) || (wwd->key[0] == '\"'))
	{
		wwd->tlr[i][wwd->d_pos] = '|';
		wwd->save_pos[wwd->s_p_cnt] = wwd->d_pos;
		wwd->s_p_cnt++;				
	}
	if (wwd->key[0] == '?')
		wwd->value = ft_itoa(shell->last_comm_ret);
	else
		wwd->value = env_get_value(shell->env_param, wwd->key);
	free(wwd->key);	
}

int	dlr_mlc(t_shell *shell)
{
	int	res;
	int	i;
	int	max;

	i = -1;
	max = 0;
	while (shell->env_param[++i])
		if (ft_strlen(shell->env_param[i]) > max)
			max = ft_strlen(shell->env_param[i]);
	res = ft_strlen(shell->prompt) / 2;
	res = res * max + 2;
	return (res);
}

void	third_part_wwd(t_dolar	*wwd, int i, t_shell *shell)
{
	if (wwd->value != 0)
		ft_strlcat(wwd->rs_st, wwd->value, ft_strlen(wwd->rs_st) + ft_strlen(wwd->value) + 1);
	wwd->start = ft_substr(wwd->tlr[i], wwd->end_key + 1, ft_strlen(wwd->tlr[i]) - wwd->end_key);
	ft_strlcat(wwd->rs_st, wwd->start, ft_strlen(wwd->rs_st) + ft_strlen(wwd->start) + 1);
	free(wwd->start);
	free(wwd->tlr[i]);
	wwd->tlr[i] = malloc(sizeof(char) * dlr_mlc(shell));
	ft_strlcpy(wwd->tlr[i], wwd->rs_st, ft_strlen(wwd->rs_st) + 1);	
	wwd->d_pos = char_srch(wwd->rs_st, '$');
}

void	work_with_dollar( t_shell *shell) 
{
	int		i;
	int 	j;
	t_dolar	wwd; 

	i = 0;
	wwd.tlr = shell->lexer_res;	
	wwd.rs_st = malloc(sizeof(char) * dlr_mlc(shell));
	while (wwd.tlr[i])
	{
		if ((wwd.tlr[i][0] == '\'') && (i++))
			continue;
		first_part_wwd(&wwd, i);
		while(wwd.d_pos != -1)
		{
			second_part_wwd(&wwd, i, shell);
			third_part_wwd(&wwd, i, shell);	
		}
		j = -1;
		while (++j < wwd.s_p_cnt)
			wwd.tlr[i][wwd.save_pos[j]] = '$';
		free(wwd.save_pos);	
		i++;
	}	
	free(wwd.rs_st);
}

int	start_new_lexem(t_shell *shell, t_lexer	*lxr)
{
	char	*str;
	int		cnt;
	int		res;

	str = lxr->command;
	cnt = lxr->s_cnt;
	res = 0;
	if ((lxr->st_nlm < 0) && (!is_sp_sim(str[cnt])))
	{
		lxr->st_nlm = cnt;
		res = 1;
		if (str[cnt] == '\'')
			lxr->sng_qut = 1;
		else if (str[cnt] == '\"')
			lxr->dub_qut = 1;	
	}
	return (res);
}

int	work_pipe_or_ec(t_shell *shell, t_lexer	*lxr)
{
	char	*str;
	int		cnt;

	str = lxr->command;
	cnt = lxr->s_cnt;
	if ((lxr->sng_qut == 1) || (lxr->dub_qut == 1))
		return (0);
	if (lxr->st_nlm >= 0)
	{
			shell->lexer_res[lxr->l_cnt] = ft_substr(shell->prompt, lxr->st_nlm, (lxr->s_cnt - lxr->st_nlm ));
			lxr->l_cnt++;
			lxr->st_nlm = -1;		
	}
	if (str[cnt] == '|')
	{
		shell->lexer_res[lxr->l_cnt] = "|";
		lxr->pipe_cnt++;
	}
	else if (str[cnt] == '>')
	{
		if (str[cnt+1] == '>')
		{
			shell->lexer_res[lxr->l_cnt] = ">>";
			lxr->s_cnt++;
		}
		else
		{
			shell->lexer_res[lxr->l_cnt] = ">";
			lxr->arr_cnt++;
		}
	}
	else if (str[cnt] == '<')
	{
		if (str[cnt+1] == '<')
		{
			shell->lexer_res[lxr->l_cnt] = "<<";
			lxr->s_cnt++;
		}
		else
		{
			shell->lexer_res[lxr->l_cnt] = "<";
			lxr->arr_lf_cnt++;
		}
	}	
	lxr->l_cnt++;
	return (0);
}

int	is_space(char ch)
{
	if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
		return (1);
	else
		return (0);
}

int	end_lexem(t_shell *shell, t_lexer	*lxr)
{
	char	*str;
	int		cnt;
	int		res;

	str = lxr->command;
	cnt = lxr->s_cnt;
	res = 0;
	if ((lxr->st_nlm >= 0) && (is_sp_sim(str[cnt]) || (str[cnt] == '\'') || (str[cnt] == '\"')))
	{
		if ((lxr->sng_qut == 1) && (str[cnt] != '\''))
			return (res);
		else if ((lxr->dub_qut == 1) && (str[cnt] != '\"'))
			return (res);
		if (str[cnt] == '\'')
			lxr->sng_qut = 0;
		else if (str[cnt] == '\"')
			lxr->dub_qut = 0;
		res = 1;
	}
	return (res);
}

int	check_two_pipes(t_shell *shell)
{
	int		i;
	char	**tlr;

	i = 0;
	tlr = shell->lexer_res;	
	while (tlr[i])
	{
		if ((tlr[0][0] == '|')  || ((tlr[0][0] == '>')
			|| (tlr[0][0] == '<')) && (twodimarr_str_calc(tlr) == 1))
			return (1);
		if (tlr[i+1] != 0)
		{
			if ((tlr[i][0] == '|') && (tlr[i + 1][0] == '|'))
				return (1);
			if ((tlr[i][0] == '>' || tlr[i][0] == '<') && tlr[i + 1][0] == '|')
				return (1);
			if (((tlr[i][0] == '>') && (tlr[i + 1][0] == '<')) 
				|| ((tlr[i][0] == '<') && (tlr[i + 1][0] == '>')) )
				return (1);	
		}
		else if (tlr[i][0] == '>' || tlr[i][0] == '<' || tlr[i][0] == '|')
			return (1);
		i++;
	}
	return (0);
}

void	init_lexer(t_shell *shell, t_lexer	*lexer_st)
{
	lexer_st->s_cnt = 0;
	lexer_st->l_cnt = 0;
	lexer_st->st_nlm = -1;
	lexer_st->sng_qut = 0;
	lexer_st->dub_qut = 0;
	lexer_st->command = shell->prompt;
	lexer_st->pipe_cnt = 0;
	lexer_st->arr_cnt = 0;
	lexer_st->arr_lf_cnt = 0;
}

int	lexer(t_shell *shell)
{
	t_lexer	lexer_st;

	shell->lexer_res = malloc(sizeof(char *) * (ft_strlen(shell->prompt) + 1));
	init_lexer(shell, &lexer_st);
		// printf("%s\n", lexer_st.command);
	while (is_space(shell->prompt[lexer_st.s_cnt]))
		lexer_st.s_cnt++;
	while (lexer_st.s_cnt <= ft_strlen(shell->prompt))
	{
		if ((shell->prompt[lexer_st.s_cnt] == '|') || (shell->prompt[lexer_st.s_cnt] == '>') || (shell->prompt[lexer_st.s_cnt] == '<'))
			work_pipe_or_ec(shell, &lexer_st);
		else if (start_new_lexem(shell, &lexer_st))
			lexer_st.st_nlm = lexer_st.s_cnt;
		else if (end_lexem(shell, &lexer_st))
		{
			shell->lexer_res[lexer_st.l_cnt] = ft_substr(shell->prompt, lexer_st.st_nlm, (lexer_st.s_cnt - lexer_st.st_nlm + 1));		
			lexer_st.l_cnt++;
			lexer_st.st_nlm = -1;
		}
		lexer_st.s_cnt++;
	}
	shell->lexer_res[lexer_st.l_cnt] = 0;
	shell->pipe_cnts = lexer_st.pipe_cnt;
	shell->arr_cnts = lexer_st.arr_cnt;
	shell->arr_lf_cnts = lexer_st.arr_lf_cnt;
	// printf("pipe cnt=%d\n", lexer_st.pipe_cnt);
	work_with_dollar(shell);
	if (check_two_pipes(shell))
		return (1);
	return (0);
}

void	remove_spaces(t_shell *shell)
{
	int const	num = shell->pipe_cnts + 1;
	int			i;
	int			j;
	char		*space;

	i = 0;
	while (i < num)
	{
		j = 0;
		while (shell->parser_res[i][j])
		{
			space = ft_strrchr(shell->parser_res[i][j], ' ');
			if (space)
				*space = 0;
			j++;
		}
		i++;
	}
}

void	sigint_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
	printf("\nminishell$ ");
}

void	sigquit_hander(int sig)
{
	(void)sig;
	rl_redisplay();
}

void	run_shell(t_shell *shell)
{
	struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_hander);
	while(1)
	{
		while (1)
		{
			shell->prompt = readline("minishell$ ");
			if (!shell->prompt)
			{
				ft_clear(shell->env_param);
				exit(EXIT_SUCCESS);
			}
			if (shell->prompt[0] != '\0')
				break ;
		}
		add_history(shell->prompt);
		// lexer(shell);
		if (lexer(shell))
		{
			printf("wrong input");
			free_lexer(shell);
			exit(1);
			// return (1);
		}		
		parser(shell);
		remove_spaces(shell);
		shell->auxilar = malloc(sizeof(t_pipex *) * (shell->pipe_cnts + 2));
		post_parser(shell);
		if (shell->pipe_cnts > 0)
			pipex(shell);
		else
			execute(shell);
		free(shell->prompt);
	}
}

// void	change_env(char	**envp, char *new_str, int ind)
// {
// 	envp[ind] = new_str;
// }

int main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	create_env(&shell, envp);
	run_shell(&shell);
}

	// close_env(&shell);