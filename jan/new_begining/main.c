/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/28 15:36:41 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	new_shell->cmnd_cnt = 0;	//	TODO
	new_shell->last_comm_ret = 0;
	new_shell->lexer_res = malloc(sizeof(char *) * 100); // TODO

	return (0);
}

void	close_env(t_shell *shell)
{
	int	i;




	//	free lexer_res
	i = 0;	
	while (shell->lexer_res[i])
	{
		if (!((shell->lexer_res[i][0] == '|') || (shell->lexer_res[i][0] == '>') || (shell->lexer_res[i][0] == '<')))
			free(shell->lexer_res[i]);
		i++;
	}
	free(shell->lexer_res);


	// free parser_res
i = 0;
int j = 0;
	while (shell->parser_res[i])
	{
		// while (shell->parser_res[i][j])
		// {
		// 	// printf("i=%d j=%d", i,j);
		// 	if (!((shell->parser_res[i][j][0] == '|') || (shell->parser_res[i][j][0] == '>') || (shell->parser_res[i][j][0] == '<')))
		// 		free(shell->parser_res[i][j]);
		// 	j++;
		// }
		free(shell->parser_res[i]);
		i++;
	}
	free(shell->parser_res);

	// free auxilar
	i = 0;
	while (shell->auxilar[i])
	{
		free(shell->auxilar[i]);
		i++;
	}
	free(shell->auxilar);

ft_clear(shell->env_param);	

	// ft_clear(shell->lexer_res);
	// free(shell->lexer_res);	
	// free(shell->prompt);	// uncomment in final version

}

void	read_prompt(t_shell *shell)
{
	// shell->prompt = readline("get_curr_dirr>");
	// shell->prompt = malloc(sizeof(char *) * (wrds_s_cnt(shell->prompt) + 1));	
	// shell->prompt = malloc(sizeof(char) * 1000);	// TODO
	// shell->prompt = " 123 a>b 555555555555666666| a=b 123 555555555555666666| fgh"; 
	shell->prompt = "echo ls>>bs $a $$ as 1| wc -l | exp<<ort a=abs"; 
	// shell->prompt = "\'echo abs>$a$|no$a2$\'1|\"wc -l\"| export a=abs"; 	
	// shell->prompt = "\"echo a>bs$a$|no$a2 $\"1|\"wc -l\"| export a=abs"; 
	// shell->prompt = "echo ls > 35 $  | test add sth > > sth1 | asd 123"; 	
		// shell->prompt = "echo h < asddff";	
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

void	first_part_wwd(t_dolar	*wwd, int i)
{
	wwd->save_pos = malloc(sizeof(char) * ft_strlen(wwd->tlr[i]));
	wwd->s_p_cnt = 0;
	printf("%d %s;\n",i, wwd->tlr[i]);	//	delete
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

void	third_part_wwd(t_dolar	*wwd, int i)
{
	if (wwd->value != 0)
		ft_strlcat(wwd->rs_st, wwd->value, ft_strlen(wwd->rs_st) + ft_strlen(wwd->value) + 1);
	wwd->start = ft_substr(wwd->tlr[i], wwd->end_key + 1, ft_strlen(wwd->tlr[i]) - wwd->end_key);
	ft_strlcat(wwd->rs_st, wwd->start, ft_strlen(wwd->rs_st) + ft_strlen(wwd->start) + 1);
	free(wwd->start);
	free(wwd->tlr[i]);
	wwd->tlr[i] = malloc(sizeof(char) * 1024);
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
	wwd.rs_st = malloc(sizeof(char) * 1024);	// TODO	(save longest value * $cnt)
	while (wwd.tlr[i])
	{
		if ((wwd.tlr[i][0] == '\'') && (i++))
			continue;
		first_part_wwd(&wwd, i);
		while(wwd.d_pos != -1)
		{
			second_part_wwd(&wwd, i, shell);
			third_part_wwd(&wwd, i);	
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
		printf("%s\n", lexer_st.command);
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
	// printf("word cnt=%d\n", lexer_st.l_cnt);
	printf("pipe cnt=%d\n", lexer_st.pipe_cnt);
	// work_with_dollar(&lexer_st, shell);
	work_with_dollar(shell);
}

void	run_shell(t_shell *shell)
{
	while(1)
	{
		// work_with_signals();

		read_prompt(shell);
		if (!shell->cont_wrk)
			break;
		add_history(shell->prompt);

		lexer(shell); // devide for tokens
		parser(shell); // combine to commands
		// if (shell->pipe_cnts > 0)
		// 	pipex(shell);	// run pipes
		// else 
		// 	execute(shell);	//  exec 1 command / end shell
	}
}

// void	change_env(char	**envp, char *new_str, int ind)
// {
// 	envp[ind] = new_str;
// }

void	checkleaks(void)
{
	system("leaks a.out");
}

int main(int argc, char **argv, char **envp)
{
atexit(checkleaks);

	t_shell	shell;

	(void)argc;
	(void)argv;
	(void)envp;
	create_env(&shell, envp);

	// run_shell(&shell);
// env(&shell);	

	read_prompt(&shell);
	lexer(&shell);
	int	i;
	int j;
	// // i = 0;
	// // while (shell.lexer_res[i])
	// // {
	// // 	printf("%d %s;\n",i, shell.lexer_res[i]);
	// // 	// fill_env_collection(new_shell, envp[i]);
	// // 	i++;
	// // }		
	// // printf("\n");	
	
	parser(&shell);
	shell.auxilar = malloc(sizeof(t_pipex *) * shell.pipe_cnts + 1);
	// printf("\n");
	// i = 0;
	// j = 0;
	// while (shell.parser_res[i])
	// {
	// 	while (shell.parser_res[i][j] != 0)
	// 	{
	// 		printf("%d %d %s;\n",i,j, shell.parser_res[i][j]);
	// 		j++;
	// 	}
	// 	j = 0;
	// 	i++;
	// }
	// printf("\n");

	post_parser(&shell);

	i = 0;
	j = 0;
	while (shell.parser_res[i])
	{
		while (shell.parser_res[i][j] != 0)
		{
			printf("%d %d %s;\n",i,j, shell.parser_res[i][j]);
			j++;
		}
		j = 0;
		i++;
	}
	printf("\n");	

	i = 0;
	while (i < 1)
	{
		printf("input_fd %d \n",shell.auxilar[i]->input_fd);
		printf("output_fd %d \n", shell.auxilar[i]->output_fd);
		printf("is_exec %d \n",shell.auxilar[i]->is_exec);
		i++;
	}	
		printf("\n");	

	// env(&shell);

	// printf("strr_calc=%d\n", twodimarr_str_calc(shell.parser_res[1]));
// int test = export(&shell, "a1234&=123");
// printf("exp_res = %d", test);

// // int test = pos_into_env(shell.env_param, "SECURITYSESSIONID=");
// // printf("pos_a=%d\n", test);
// // unset(&shell, "SECURITYSESSIONID=");
// env(&shell);
// // unset(&shell, "a=");
export(&shell, "");
// export(&shell, "a=new_a2");
// export(&shell, "a=new_a");
// env(&shell);
// unset(&shell, "a");
// env(&shell);
// env_get_value(shell.env_param, "a");
	// printf("cmnd_cnt=%d\n", shell.cmnd_cnt);
	close_env(&shell);
	return (0);
}