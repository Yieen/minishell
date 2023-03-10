/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/27 20:16:54 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// cd new_shell/jan/new_begining/
//	new_begining % gcc -lreadline  *.c
//	new_begining % gcc -I/goinfre/inovomli/.brew/opt/readline/include -lreadline  *.c

int	create_env(t_shell *new_shell, char **envp)
{
	int envp_size;
	char	**new_env;

	envp_size = twodimarr_str_calc(envp);
	
	new_env = copy_string_array(envp, envp_size + 1);
	// new_shell->env_param = envp;
	new_shell->env_param = new_env;

	// new_shell->prompt = readline("get_curr_dirr>");
	new_shell->cont_wrk = 1;
	new_shell->cmnd_cnt = 0;	//	TODO
	new_shell->last_comm_ret = 0;
	new_shell->lexer_res = malloc(sizeof(char *) * 100); // TODO

	return (0);
}

void	read_prompt(t_shell *shell)
{
	// shell->prompt = readline("get_curr_dirr>");
	// shell->prompt = malloc(sizeof(char *) * (wrds_s_cnt(shell->prompt) + 1));	
	shell->prompt = malloc(sizeof(char) * 1000);	// TODO
	// shell->prompt = " 123 a>b 555555555555666666| a=b 123 555555555555666666| fgh"; 
	// shell->prompt = "echo ls>>bs $a$$ as 1| wc -l | exp<<ort a=abs"; 
	// shell->prompt = "\'echo abs>$a$|no$a2$\'1|\"wc -l\"| export a=abs"; 	
	// shell->prompt = "\"echo a>bs$a$|no$a2 $\"1|\"wc -l\"| export a=abs"; 
	shell->prompt = "echo ls > 35   | test add sth > > sth1 | asd 123"; 			
} 

int	is_sp_sim(char ch)
{
	const char	*special_simbols = "|>< \t\n"; // $ not here
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

void	work_with_dollar(t_lexer *lxr, t_shell *shell)
{
	int		i;
	int		d_pos;
	int		end_key;
	char	*key;
	char	*rs_st;
	char	*value;
	char	*start;
	int		*save_pos;
	int		s_p_cnt;
	int 	j;

	i = 0;
	rs_st = malloc(sizeof(char) * 1024);	// TODO		
	while (shell->lexer_res[i])
	{
		if ((shell->lexer_res[i][0] == '\'') && (i++))
			continue;
		save_pos = malloc(sizeof(char) * ft_strlen(shell->lexer_res[i]));
		s_p_cnt = 0;
		d_pos = char_srch(shell->lexer_res[i], '$');
		while(d_pos != -1)
		{
			start = ft_substr(shell->lexer_res[i], 0, d_pos);			
			ft_strlcpy(rs_st, start, ft_strlen(start) + 1);
			free(start);
			end_key = find_end_key(shell->lexer_res[i], d_pos + 1);
			key = ft_substr(shell->lexer_res[i], d_pos + 1, end_key - d_pos);
			if ((key[0] == '\0') || (key == 0) || (key[0] == '\"'))
			{
				shell->lexer_res[i][d_pos] = '|';
				save_pos[s_p_cnt] = d_pos;
				s_p_cnt++;				
			}
			if (key[0] == '?')
				value = ft_itoa(shell->last_comm_ret);
			else
				value = env_get_value(shell->env_param, key);
			if (value != 0)
				ft_strlcat(rs_st, value, ft_strlen(rs_st) + ft_strlen(value) + 1);
			start = ft_substr(shell->lexer_res[i], end_key + 1, ft_strlen(shell->lexer_res[i]) - end_key);		
			ft_strlcat(rs_st, start, ft_strlen(rs_st) + ft_strlen(start) + 1);
			free(start);
			free(shell->lexer_res[i]);
			shell->lexer_res[i] = malloc(sizeof(char) * 1024);
			ft_strlcpy(shell->lexer_res[i], rs_st, ft_strlen(rs_st) + 1);	
			d_pos = char_srch(rs_st, '$');
		}
		j = -1;
		while (++j < s_p_cnt)
			shell->lexer_res[i][save_pos[j]] = '$';
		free(save_pos);		
		i++;
	}	
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
		// if ((str[cnt] == '\'') || (str[cnt] == '\"'))
		// 	lxr->s_cnt++;	
		res = 1;			
	}
	return (res);
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

void	lexer(t_shell *shell)
{
	t_lexer	lexer_st;

	init_lexer(shell, &lexer_st);
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
	work_with_dollar(&lexer_st, shell);
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

void	quit_handler(int sig)
{
	rl_replace_line("minishell$", 0);
}

void	run_shell(t_shell *shell)
{
	signal(SIGQUIT, quit_handler);
	while(1)
	{
		shell->prompt = readline("minishell$ ");
		if (!shell->prompt)
		{
			rl_replace_line("minishell$ ", 1);
			// ft_putchar_fd('\n', STDOUT_FILENO);
			b_exit(shell, 0);
		}
		add_history(shell->prompt);
		lexer(shell);
		parser(shell);
		remove_spaces(shell);
		shell->auxilar = malloc(sizeof(t_pipex *) * shell->pipe_cnts+ 1);
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