/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:06:25 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/27 19:26:27 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	locate_parser_mem(t_shell *shell)
{
	int	i;

	i = 0;
	shell->parser_res = (char ***)malloc(sizeof(char **) * (shell->pipe_cnts + 1));
	while (i < shell->pipe_cnts + 1)
	{
		shell->parser_res[i] = (char **) malloc(sizeof(char *) * (1024));
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
			// printf("i=%d j=%d %s;\n",i, j, shell->parser_res[i][j]);
			j++;
			lex_cnt++;
		}
		shell->parser_res[i][j] = 0;
		lex_cnt++;
		i++;
	}	
	shell->parser_res[i] = 0;
}

char	**del_elms_fr_array(char **src, int size, int *del_arr) 
{
	int		i;
	char	**dest;
	int		j;
	int 	new_cnt;
	int		copy;

	i = 0;
	new_cnt = 0;
    dest = malloc(sizeof(char *) * (size + 1));
	while (src[i])
	{
		j = 0;
		copy = 1;
		while (del_arr[j])
		{
			if (i == del_arr[j])
				copy = 0;
			j++;
		}
		if (copy)
			dest[new_cnt++] = src[i];
		i++;
    }
	dest[new_cnt] = 0;
    return dest;
}

void	post_parser(t_shell *shell)
{
	int	i;
	int	j;
	int		*save_pos;
	int		s_p_cnt;
	int	size;
	char	**interim;	

	i = 0;
	while (shell->parser_res[i])
	{
		j = 0;
		shell->auxilar[i] = malloc(sizeof(t_pipex));
		shell->auxilar[i]->is_exec = 1;
		shell->auxilar[i]->input_fd = -2;
		shell->auxilar[i]->output_fd = -2;
		save_pos = malloc(sizeof(int) * 1024);	
		s_p_cnt = 0;					
		while (shell->parser_res[i][j])
		{
			if (shell->parser_res[i][j][0] == '>')
			{
				if (shell->parser_res[i][j][1] != '>')
				{
					if (shell->parser_res[i][j + 1] != NULL)
					{
						if (shell->auxilar[i]->input_fd != -2)
							close(shell->auxilar[i]->input_fd);
						if (!is_sp_sim(shell->parser_res[i][j + 1][0]))
	shell->auxilar[i]->input_fd = open(shell->parser_res[i][j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
						else
						{
							shell->auxilar[i]->is_exec = 0;
							break;
						}
						if (shell->auxilar[i]->input_fd == -1)
							shell->auxilar[i]->is_exec = 0;
						save_pos[s_p_cnt] = j;
						save_pos[s_p_cnt + 1] = j + 1;
						s_p_cnt += 2;	
					}
				}
				else
				{
					if (shell->parser_res[i][j + 1] != NULL)
					{
						if (shell->auxilar[i]->input_fd != -2)
							close(shell->auxilar[i]->input_fd);
						if (!is_sp_sim(shell->parser_res[i][j + 1][0]))							
	shell->auxilar[i]->input_fd = open(shell->parser_res[i][j + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
						else
						{
							shell->auxilar[i]->is_exec = 0;
							break;
						}	
						if (shell->auxilar[i]->input_fd == -1)
							shell->auxilar[i]->is_exec = 0;
						save_pos[s_p_cnt] = j;
						save_pos[s_p_cnt + 1] = j + 1;
						s_p_cnt += 2;							
					}	
				}			
			}
			if (shell->parser_res[i][j][0] == '<')
			{
				if (shell->parser_res[i][j][1] != '<')
				{
					if (shell->parser_res[i][j + 1] != NULL)
					{
						if (shell->auxilar[i]->output_fd != -2)
							close(shell->auxilar[i]->output_fd);
						if (!is_sp_sim(shell->parser_res[i][j + 1][0]))								
	shell->auxilar[i]->output_fd = open(shell->parser_res[i][j + 1], O_RDONLY);
						else
						{
							shell->auxilar[i]->is_exec = 0;
							break;
						}	
						if ((shell->auxilar[i]->output_fd == -1) || (is_sp_sim(shell->parser_res[i][j + 1][0])))
						
							shell->auxilar[i]->is_exec = 0;	
						save_pos[s_p_cnt] = j;
						save_pos[s_p_cnt + 1] = j + 1;
						s_p_cnt += 2;							
					}
				}
				else
				{
					if (shell->parser_res[i][j + 1] != NULL)
					{
						if (shell->auxilar[i]->output_fd != -2)
							close(shell->auxilar[i]->output_fd);
						if (!is_sp_sim(shell->parser_res[i][j + 1][0]))	
	shell->auxilar[i]->output_fd = open(shell->parser_res[i][j + 1], O_RDONLY);
						else
						{
							shell->auxilar[i]->is_exec = 0;
							break;
						}	
						if ((shell->auxilar[i]->output_fd == -1) || (is_sp_sim(shell->parser_res[i][j + 1][0])))
							shell->auxilar[i]->is_exec = 0;	
						save_pos[s_p_cnt] = j;
						save_pos[s_p_cnt + 1] = j + 1;
						s_p_cnt += 2;							
					}	
				}									
			}
			j++;
		}
		save_pos[s_p_cnt] = 0;		
		size = twodimarr_str_calc(shell->parser_res[i]);
		interim = del_elms_fr_array(shell->parser_res[i], size, save_pos);
		free(shell->parser_res[i]);
		shell->parser_res[i] = interim;
		i++;
		free(save_pos);

	}
}