/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 12:15:57 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 12:21:44 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_auxilar(t_shell *shell, int i)
{
	shell->auxilar[i] = malloc(sizeof(t_pipex));
	shell->auxilar[i]->is_exec = 1;
	shell->auxilar[i]->input_fd = -2;
	shell->auxilar[i]->output_fd = -2;
}

void	init_post_parser_st(t_shell *shell, t_parser *pp)
{
		pp->save_pos = malloc(sizeof(int) * (ft_strlen(shell->prompt) + 1));
		pp->s_p_cnt = 0;
		pp->is_break = 0;
}

void	post_parser(t_shell *shell)
{
	int			i;
	int			j;
	t_parser	pp;

	i = 0;
	while (shell->parser_res[i])
	{
		j = 0;
		init_auxilar(shell, i);
		init_post_parser_st(shell, &pp);
		while (shell->parser_res[i][j] && !pp.is_break)
		{
			right_arrow_pp(shell, &pp, i, j);
			left_arrow_pp(shell, &pp, i, j);
			j++;
		}
		pp.save_pos[pp.s_p_cnt] = 0;
		pp.interim = del_elms_fr_array(shell->parser_res[i],
				tdar_str_calc(shell->parser_res[i]), pp.save_pos, pp.s_p_cnt);
		free(shell->parser_res[i]);
		shell->parser_res[i] = pp.interim;
		i++;
		free(pp.save_pos);
	}
	shell->auxilar[i] = 0;
}
