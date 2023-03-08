/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quot_connect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 19:31:07 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/08 16:16:57 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	del_n_last(char *str, int del)
{
	int len;
	int i;

	i = del;
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
	int 		cnt;
	int 		dp;
	int 		sp;

	i = 0;
	while (i < num)
	{
		j = 0;
		while (shell->parser_res[i][j])
		{
			cnt = ft_strlen(shell->parser_res[i][j]);
			dp = char_srch(shell->parser_res[i][j], '\"');
			sp = char_srch(shell->parser_res[i][j], '\'');
			if (dp > -1 && cnt > 1 && (shell->parser_res[i][j][dp] == '\"') && (shell->parser_res[i][j][cnt - 1] == '\"'))
				del_n_last(shell->parser_res[i][j], dp);
			else if (sp > -1 && cnt > 1 && (shell->parser_res[i][j][sp] == '\'') && (shell->parser_res[i][j][cnt - 1] == '\''))
				del_n_last(shell->parser_res[i][j], sp);				
			j++;
		}
		i++;
	}
}

typedef struct Catstr
{
	int			i;
	int			j;
	int		*save_pos;
	int		s_p_cnt;
	char	**interim;	
} t_combstr;

void into_cmb_st(char ***prr, t_combstr *cs)
{
	if ((prr[cs->i][cs->j + 1] != 0) && ((prr[cs->i][cs->j + 1][0] == '\"')
		|| (prr[cs->i][cs->j + 1][0] == '\''))
		&& (prr[cs->i][cs->j][ft_strlen(prr[cs->i][cs->j]) - 1] != ' ') 
		&& (prr[cs->i][cs->j][0] != '<') && (prr[cs->i][cs->j][0] != '>')
		&& !(((prr[cs->i][cs->j][ft_strlen(prr[cs->i][cs->j]) - 1] == '\'')
		&& (prr[cs->i][cs->j + 1][0] == '\"')) 
		|| ((prr[cs->i][cs->j][ft_strlen(prr[cs->i][cs->j]) - 1] == '\"')
		&& (prr[cs->i][cs->j + 1][0] == '\'')))
		&& (prr[cs->i][cs->j][ft_strlen(prr[cs->i][cs->j]) - 1] 
		!= (prr[cs->i][cs->j + 1][0])))
	{
		ft_strlcat(prr[cs->i][cs->j], prr[cs->i][cs->j + 1],
			ft_strlen(prr[cs->i][cs->j]) 
			+ ft_strlen(prr[cs->i][cs->j + 1]) + 1);
		cs->save_pos[cs->s_p_cnt] = cs->j + 1;
		cs->s_p_cnt += 1;	
	}					
}


void	combine_str(char ***prr)
{
	t_combstr	cs;

	cs.i = 0;
	while (prr[cs.i])
	{
		cs.j = 0;
		cs.save_pos = malloc(sizeof(int) * (1024 + 1));
		cs.s_p_cnt = 0;
		while (prr[cs.i][cs.j])
		{
			into_cmb_st(prr, &cs);
			cs.j++;
		}
		cs.save_pos[cs.s_p_cnt] = 0;
		cs.interim = del_elms_fr_array(prr[cs.i],
				tdar_str_calc(prr[cs.i]), cs.save_pos, cs.s_p_cnt);
		free(prr[cs.i]);
		prr[cs.i] = cs.interim;
		free(cs.save_pos);		
		cs.i++;
	}
}

void	into_cmb_str2(char ***prr, t_combstr *sc, int len)
{
	int	dp;
	int	sp;
	int	i;
	int	j;

	i = sc->i;
	j = sc->j;
	if ((prr[i][j] != 0) && (len >= 1)
		&& ((prr[i][j - 1][0] != '<') && (prr[i][j - 1][0] != '>'))
		&& ((prr[i][j - 1][len] == '\"') || (prr[i][j - 1][len] == '\'')))
	{
		dp = char_srch(prr[i][j - 1], '\"');
		sp = char_srch(prr[i][j - 1], '\'');
		if (dp >= 0 && prr[i][j - 1][dp] == '\"' && prr[i][j - 1][len] == '\"')
			del_n_last(prr[i][j - 1], dp);
		if (sp >= 0 && prr[i][j - 1][sp] == '\'' && prr[i][j - 1][len] == '\'')
			del_n_last(prr[i][j - 1], sp);				
		ft_strlcat(prr[i][j - 1], prr[i][j],
				ft_strlen(prr[i][j - 1]) + ft_strlen(prr[i][j]) + 1);
		sc->save_pos[sc->s_p_cnt] = j;
		sc->s_p_cnt += 1;					
	}
	if (prr[i][j - 1][len] == ' ')
		prr[i][j - 1][len] = 0;		
}

void	combine_str2(char ***prr)
{
	t_combstr	cs;	
	int len;

	cs.i = 0;
	while (prr[cs.i])
	{
		cs.j = tdar_str_calc(prr[cs.i]);
		cs.save_pos = malloc(sizeof(int) * (1024 + 1));
		cs.s_p_cnt = 0;
		while (cs.j > 0)
		{
			len = ft_strlen(prr[cs.i][cs.j - 1]) - 1;
			into_cmb_str2(prr, &cs, len);			
			cs.j--;
		}
		cs.save_pos[cs.s_p_cnt] = 0;
		cs.interim = del_elms_fr_array(prr[cs.i], tdar_str_calc(prr[cs.i]), cs.save_pos, cs.s_p_cnt);
		free(prr[cs.i]);
		prr[cs.i] = cs.interim;
		cs.i++;
		free(cs.save_pos);
	}
}
