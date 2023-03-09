/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 11:40:21 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 01:03:55 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	start_new_lexem(t_lexer	*lxr)
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

static void	into_end_lex(t_lexer *lxr, int *res)
{
	char	*str;
	int		cnt;

	str = lxr->command;
	cnt = lxr->s_cnt;
	if (str[cnt] == '\'')
	{
		if ((str[cnt + 1] != 0) && (str[cnt + 1] == ' '))
			lxr->s_cnt += 1;
		lxr->sng_qut = 0;
	}
	else if (str[cnt] == '\"')
	{
		if ((str[cnt + 1] != 0) && (str[cnt + 1] == ' '))
			lxr->s_cnt += 1;
		lxr->dub_qut = 0;
	}
	*res = 1;
}

static int	end_lexem(t_lexer *lxr)
{
	char *const	str = lxr->command;
	int const	cnt = lxr->s_cnt;
	int			res;

	res = 0;
	if ((lxr->st_nlm >= 0) && (is_sp_sim(str[cnt])
			|| (str[cnt] == '\'') || (str[cnt] == '\"')))
	{
		if ((lxr->sng_qut == 1) && (str[cnt] != '\''))
			return (0);
		else if ((lxr->dub_qut == 1) && (str[cnt] != '\"'))
			return (0);
		if ((lxr->sng_qut == 0) && (str[cnt] == '\''))
		{
			lxr->s_cnt--;
			return (1);
		}
		if ((lxr->dub_qut == 0) && (str[cnt] == '\"'))
		{
			lxr->s_cnt--;
			return (1);
		}
		into_end_lex(lxr, &res);
	}
	return (res);
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

int	lexer(t_shell *shell)
{
	t_lexer	lr;

	shell->lexer_res = malloc(sizeof(char *) * (ft_strlen(shell->prompt) + 1));
	lr = (t_lexer){0};
	lr.command = shell->prompt;
	lr.st_nlm = -1;
	while (ft_isspace(shell->prompt[lr.s_cnt]))
		lr.s_cnt++;
	while (lr.s_cnt <= (int)ft_strlen(shell->prompt))
	{
		if ((shell->prompt[lr.s_cnt] == '|') || (shell->prompt[lr.s_cnt] == '>')
			|| (shell->prompt[lr.s_cnt] == '<'))
			work_pipe_or_ec(shell, &lr);
		else if (start_new_lexem(&lr))
			lr.st_nlm = lr.s_cnt;
		else if (end_lexem(&lr))
		{
			shell->lexer_res[lr.l_cnt] = ft_substr(shell->prompt,
					lr.st_nlm, (lr.s_cnt - lr.st_nlm + 1));
			lr.l_cnt++;
			lr.st_nlm = -1;
		}
		lr.s_cnt++;
	}
	return (lexer_end(shell, &lr));
}
