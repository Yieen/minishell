/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_with_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 11:37:07 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/08 18:08:21 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	dlr_mlc(t_shell *shell)
{
	int	res;
	int	i;
	int	max;

	i = -1;
	max = 0;
	while (shell->env_param[++i])
		if ((int)ft_strlen(shell->env_param[i]) > max)
			max = ft_strlen(shell->env_param[i]);
	res = ft_strlen(shell->prompt) / 2;
	res = res * max + 2;
	return (res);
}

void	first_part_wwd(t_dolar	*wwd, int i)
{
	wwd->save_pos = malloc(sizeof(int) * (ft_strlen(wwd->tlr[i]) + 1));
	wwd->s_p_cnt = 0;
	wwd->d_pos = char_srch(wwd->tlr[i], '$');
}

void	second_part_wwd(t_dolar	*wwd, int i, t_shell *shell)
{
	wwd->start = ft_substr(wwd->tlr[i], 0, wwd->d_pos);
	ft_strlcpy(wwd->rs_st, wwd->start, ft_strlen(wwd->start) + 1);
	free(wwd->start);
	wwd->end_key = find_end_key(wwd->tlr[i], wwd->d_pos + 1);
	wwd->key = ft_substr(wwd->tlr[i],
			wwd->d_pos + 1, wwd->end_key - wwd->d_pos);
	if ((wwd->key[0] == '\0') || (wwd->key == 0) || (wwd->key[0] == '\"'))
	{
		wwd->tlr[i][wwd->d_pos] = '|';
		wwd->save_pos[wwd->s_p_cnt] = wwd->d_pos;
		wwd->s_p_cnt++;
	}
	if (wwd->key[0] == '?')
		wwd->value = ft_itoa(shell->last_comm_ret);
	else
		wwd->value = ft_strdup(env_get_value(shell->env_param, wwd->key));
	free(wwd->key);
}

void	third_part_wwd(t_dolar	*wwd, int i, t_shell *shell)
{
	if ((wwd->value != 0) && (wwd->value[0] != '\0'))
	{
		ft_strlcat(wwd->rs_st, wwd->value,
			ft_strlen(wwd->rs_st) + ft_strlen(wwd->value) + 1);
		free(wwd->value);
	}
	if (wwd->end_key == wwd->d_pos)
		wwd->start = ft_substr(wwd->tlr[i], wwd->end_key,
				ft_strlen(wwd->tlr[i]) - wwd->end_key);
	else
		wwd->start = ft_substr(wwd->tlr[i], wwd->end_key + 1,
				ft_strlen(wwd->tlr[i]) - wwd->end_key);
	ft_strlcat(wwd->rs_st, wwd->start,
		ft_strlen(wwd->rs_st) + ft_strlen(wwd->start) + 1);
	free(wwd->start);
	free(wwd->tlr[i]);
	wwd->tlr[i] = malloc(sizeof(char) * dlr_mlc(shell));
	ft_strlcpy(wwd->tlr[i], wwd->rs_st, ft_strlen(wwd->rs_st) + 1);
	wwd->d_pos = char_srch(wwd->rs_st, '$');
}

void	work_with_dollar( t_shell *shell)
{
	t_dolar	wwd;

	wwd.i = 0;
	wwd.tlr = shell->lexer_res;
	wwd.rs_st = malloc(sizeof(char) * dlr_mlc(shell));
	while (wwd.tlr[wwd.i])
	{
		if ((wwd.tlr[wwd.i][0] == '\'') && (++wwd.i))
			continue ;
		first_part_wwd(&wwd, wwd.i);
		while (wwd.d_pos != -1)
		{
			second_part_wwd(&wwd, wwd.i, shell);
			third_part_wwd(&wwd, wwd.i, shell);
		}
		wwd.save_pos[wwd.s_p_cnt] = 0;
		wwd.j = -1;
		while (++wwd.j < wwd.s_p_cnt)
			wwd.tlr[wwd.i][wwd.save_pos[wwd.j]] = '$';
		if (wwd.save_pos[wwd.j] + 1 == (int)ft_strlen(wwd.tlr[wwd.i]))
			wwd.tlr[wwd.i][wwd.save_pos[wwd.j] + 1] = 0;
		free(wwd.save_pos);
		wwd.i++;
	}	
	free(wwd.rs_st);
}
