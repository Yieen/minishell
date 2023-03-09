/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 12:10:34 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 12:21:32 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	two_left_arrow(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j + 1] != NULL)
	{
		if (shell->auxilar[i]->input_fd != -2)
			close(shell->auxilar[i]->input_fd);
		if (!is_sp_sim(shell->parser_res[i][j + 1][0]))
		{
			shell->auxilar[i]->input_fd
				= here_doc(shell->parser_res[i][j + 1]);
			if ((shell->auxilar[i]->input_fd == -1)
				|| (is_sp_sim(shell->parser_res[i][j + 1][0])))
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				perror(shell->parser_res[i][j + 1]);
				shell->auxilar[i]->is_exec = 0;
			}				
			pp->save_pos[pp->s_p_cnt] = j;
			pp->save_pos[pp->s_p_cnt + 1] = j + 1;
			pp->s_p_cnt += 2;
		}				
		else
		{
			shell->auxilar[i]->is_exec = 0;
			pp->is_break = 1;
		}
	}
}

void	left_arrow_pp(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j][0] == '<')
	{
		if (shell->parser_res[i][j][1] != '<')
		{
			one_left_arrow(shell, pp, i, j);
		}
		else
		{
			two_left_arrow(shell, pp, i, j);
		}
	}
}

void	one_right_arrow(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j + 1] != NULL)
	{
		if (shell->auxilar[i]->output_fd != -2)
			close(shell->auxilar[i]->output_fd);
		if (!is_sp_sim(shell->parser_res[i][j + 1][0]))
		{
			shell->auxilar[i]->output_fd = open(shell->parser_res[i][j + 1],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (shell->auxilar[i]->output_fd == -1)
				shell->auxilar[i]->is_exec = 0;
			pp->save_pos[pp->s_p_cnt] = j;
			pp->save_pos[pp->s_p_cnt + 1] = j + 1;
			pp->s_p_cnt += 2;
		}
		else
		{
			shell->auxilar[i]->is_exec = 0;
			pp->is_break = 1;
		}
	}
}

void	two_right_arrow(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j + 1] != NULL)
	{
		if (shell->auxilar[i]->output_fd != -2)
			close(shell->auxilar[i]->output_fd);
		if (!is_sp_sim(shell->parser_res[i][j + 1][0]))
		{
			shell->auxilar[i]->output_fd = open(shell->parser_res[i][j + 1],
					O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (shell->auxilar[i]->output_fd == -1)
				shell->auxilar[i]->is_exec = 0;
			pp->save_pos[pp->s_p_cnt] = j;
			pp->save_pos[pp->s_p_cnt + 1] = j + 1;
			pp->s_p_cnt += 2;
		}
		else
		{
			shell->auxilar[i]->is_exec = 0;
			pp->is_break = 1;
		}
	}
}

void	right_arrow_pp(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j][0] == '>')
	{
		if (shell->parser_res[i][j][1] != '>')
		{
			one_right_arrow(shell, pp, i, j);
		}
		else
		{
			two_right_arrow(shell, pp, i, j);
		}
	}	
}
