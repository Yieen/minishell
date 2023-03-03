/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 11:16:00 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/03 12:38:32 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	wrk_pipeor_sh(t_shell *shell, t_lexer	*lxr, char *str, int cnt)
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
		else if (lxr->arr_lf_cnt++)
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

int	is_space(char ch)
{
	if ((ch == ' ') || (ch == '\t') || (ch == '\n'))
		return (1);
	else
		return (0);
}

int	is_sp_sim(char ch)
{
	const char	*special_simbols = "|>< \t\n";
	int			cnt;

	cnt = 0;
	while (special_simbols[cnt])
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
		if (is_sp_sim(str[st]) || (str[st] == '$'))
			return (st - 1);
		st++;
	}
	return (st - 1);
}
