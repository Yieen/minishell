/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 11:40:21 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/03 11:40:38 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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