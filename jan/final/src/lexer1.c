/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 11:16:00 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 01:05:23 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	wrk_pipeor_sh(t_shell *shell, t_lexer	*lxr, char *str, int cnt)
{
	if (str[cnt] == '>')
	{
		if (str[cnt + 1] == '>')
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
	if (str[cnt] == '<')
	{
		if (str[cnt + 1] == '<')
		{
			shell->lexer_res[lxr->l_cnt] = "<<";
			lxr->s_cnt++;
		}
		else if (++lxr->arr_lf_cnt)
			shell->lexer_res[lxr->l_cnt] = "<";
	}	
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
			shell->lexer_res[lxr->l_cnt] = ft_substr(shell->prompt,
				lxr->st_nlm, (lxr->s_cnt - lxr->st_nlm));
			lxr->l_cnt++;
			lxr->st_nlm = -1;
	}
	if (str[cnt] == '|')
	{
		shell->lexer_res[lxr->l_cnt] = "|";
		lxr->pipe_cnt++;
	}
	wrk_pipeor_sh(shell, lxr, str, cnt);
	lxr->l_cnt++;
	return (0);
}

static int	check_two_pipes(t_shell *shell)
{
	int		i;
	char	**tlr;

	i = 0;
	tlr = shell->lexer_res;
	while (tlr[i])
	{
		if ((tlr[0][0] == '|' || tlr[0][0] == '>' || tlr[0][0] == '<')
			&& tdar_str_calc(tlr) == 1)
			return (1);
		if (tlr[i + 1] != 0)
		{
			if ((tlr[i][0] == '|') && (tlr[i + 1][0] == '|'))
				return (1);
			if ((tlr[i][0] == '>' || tlr[i][0] == '<') && tlr[i + 1][0] == '|')
				return (1);
			if (((tlr[i][0] == '>') && (tlr[i + 1][0] == '<'))
				|| ((tlr[i][0] == '<') && (tlr[i + 1][0] == '>')))
				return (1);
		}
		else if (tlr[i][0] == '>' || tlr[i][0] == '<' || tlr[i][0] == '|')
			return (1);
		i++;
	}
	return (0);
}

int	lexer_end(t_shell *shell, t_lexer *lr)
{
	shell->lexer_res[lr->l_cnt] = 0;
	shell->pipe_cnts = lr->pipe_cnt;
	shell->arr_cnts = lr->arr_cnt;
	shell->arr_lf_cnts = lr->arr_lf_cnt;
	if (shell->env_param != 0)
		work_with_dollar(shell);
	if (lr->l_cnt == 0)
		return (2);
	if (check_two_pipes(shell))
		return (1);
	return (0);
}
