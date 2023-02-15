/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 10:48:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/15 21:03:16 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//	new_begining % gcc -lreadline  *.c
//	new_begining % gcc -I/goinfre/inovomli/.brew/opt/readline/include -lreadline  *.c


int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len] != '\0')
		len++;
	return (len);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	srcsize;
	size_t	i;

	srcsize = (size_t)ft_strlen(src);
	i = 0;
	if (dstsize != 0)
	{
		while ((src[i]) && (i < (dstsize - 1)))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srcsize);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;

	if (!s)
		return (0);
	if (ft_strlen(s) < start)
		len = 0;
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	ft_strlcpy(ret, s + start, len + 1);
	return (ret);
}

int	create_env(t_shell *new_shell, char **envp)
{
	int	i;

	i = -1;
	// while (envp[++i])
	// {
	// 	printf("%s\n", envp[i]);
	// 	// fill_env_collection(new_shell, envp[i]);
	// }	

	new_shell->env_param = envp;
	// new_shell->prompt = readline("get_curr_dirr>");
	new_shell->cont_wrk = 1;
	new_shell->cmnd_cnt = 0;	//	TODO

	new_shell->lexer_res = malloc(sizeof(char *) * 100); // TODO

	return (0);
}

void	read_prompt(t_shell *shell)
{
	// shell->prompt = readline("get_curr_dirr>");
	// shell->prompt = malloc(sizeof(char *) * (wrds_s_cnt(shell->prompt) + 1));	
	shell->prompt = malloc(sizeof(char) * 1000);	// TODO
	// shell->prompt = "\"12\'7\'3\"45$1|78  a=1"; 
	// shell->prompt = "/bin/echo 123 456 | /usr/bin/wc -l"; 
		shell->prompt = "/bin/echo 1 2 3"; 
} 

int	is_sp_sim(char ch)
{
	const char	*special_simbols = "=| \t\n"; // $ not here
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

int work_with_dollar(t_lexer *lxr)
{
	int	res = 1;

	// if  (str[cnt] == '$')
	// 	res = work_with_dollar(lxr);
	
	return (res);
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
		// printf("start_n_l=%d\n", lxr->st_nlm);
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
	else if (str[cnt] == '=')
		shell->lexer_res[lxr->l_cnt] = "=";
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
		if ((str[cnt] == '\'') || (str[cnt] == '\"'))
			lxr->s_cnt++;	
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
		if ((shell->prompt[lexer_st.s_cnt] == '|') || (shell->prompt[lexer_st.s_cnt] == '='))
			work_pipe_or_ec(shell, &lexer_st);
		else if (start_new_lexem(shell, &lexer_st))
			lexer_st.st_nlm = lexer_st.s_cnt;
		else if (end_lexem(shell, &lexer_st))
		{
			// printf("start=%d end =%d\n",lexer_st.st_nlm, lexer_st.s_cnt);
			shell->lexer_res[lexer_st.l_cnt] = ft_substr(shell->prompt, lexer_st.st_nlm, (lexer_st.s_cnt - lexer_st.st_nlm));
			// printf("n_w=%s\n",shell->lexer_res[lexer_st.l_cnt]);			
			lexer_st.l_cnt++;
			lexer_st.st_nlm = -1;
		}
		lexer_st.s_cnt++;
	}
	shell->lexer_res[lexer_st.l_cnt] = 0;
	shell->pipe_cnts = lexer_st.pipe_cnt;
	printf("word cnt=%d\n", lexer_st.l_cnt);
	printf("pipe cnt=%d\n", lexer_st.pipe_cnt);
	// work_with_dollar(&lexer_st);
}

void	locate_parser_mem(t_shell *shell)
{
	int	i;

	i = 0;
	shell->parser_res = (char ***)malloc(sizeof(char **) * (shell->pipe_cnts + 2));
	while (i < shell->pipe_cnts + 2)
	{
		shell->parser_res[i] = (char **) malloc(sizeof(char *) * (shell->pipe_cnts + 2));
		i++;
	}
}

void	parser(t_shell *shell)
{
	int	i;
	int	j;
	int	lex_cnt;

	i = 0;
	lex_cnt = 0;
	locate_parser_mem(shell);	
	while (i <= shell->pipe_cnts)
	{
		j = 0;
		while ((shell->lexer_res[lex_cnt]) && (shell->lexer_res[lex_cnt][0] != '|'))
		{
			shell->parser_res[i][j] = shell->lexer_res[lex_cnt];
			// printf("i=%d j=%d %s;\n",i, j, shell->lexer_res[lex_cnt]);
			// if (shell->lexer_res[j] == '=')
			// 	rewrite equval expression
			j++;
			lex_cnt++;
		}
		shell->parser_res[i][j] = 0;
		lex_cnt++;
		i++;
	}	
	shell->parser_res[i] = 0;
}

void	run_shell(t_shell *shell)
{
	while(1)
	{
		// work_with_signals();

		// update_prompt(shell);
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

int main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	create_env(&shell, envp);

	// run_shell(&shell);

	read_prompt(&shell);
	lexer(&shell);
	int	i;

	i = 0;
	while (shell.lexer_res[i])
	{
		printf("%d %s;\n",i, shell.lexer_res[i]);
		// fill_env_collection(new_shell, envp[i]);
		i++;
	}		
	printf("\n");	
	
	parser(&shell);

	printf("\n");
	i = 0;
	int j = 0;
	while (shell.parser_res[i])
	{
		while (shell.parser_res[i][j])
		{
			printf("%d %d %s;\n",i,j, shell.parser_res[i][j]);
			j++;
		}
		// printf("new command\n");
		j = 0;
		i++;
	}
		// if (shell.pipe_cnts > 0)
			pipex(&shell);	// run pipes
		// else 
		// 	execute(shell);	//  exec 1 command / end shell	
	// printf("cmnd_cnt=%d\n", shell.cmnd_cnt);
	return (0);
}