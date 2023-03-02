/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:06:25 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/02 16:28:53 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	locate_parser_mem(t_shell *shell)
{
	int	i;

	i = 0;
	shell->parser_res = (char ***)malloc(sizeof(char **)
			* (shell->pipe_cnts + 1));
	while (i < shell->pipe_cnts + 1)
	{
		shell->parser_res[i] = (char **) malloc(sizeof(char *)
				* (ft_strlen(shell->prompt) + 1));
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
		while ((shell->lexer_res[lex_cnt])
			&& (shell->lexer_res[lex_cnt][0] != '|'))
		{
			shell->parser_res[i][j] = shell->lexer_res[lex_cnt];
			j++;
			lex_cnt++;
		}
		shell->parser_res[i][j] = 0;
		lex_cnt++;
		i++;
	}	
	shell->parser_res[i] = 0;
}

int	here_doc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	pipe(pipe_fd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(pipe_fd[1]);
			return (pipe_fd[0]);
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putchar_fd('\n', pipe_fd[1]);
		free(line);
	}
}

char	**del_elms_fr_array(char **src, int size, int *del_arr)
{
	int		i;
	char	**dest;
	int		j;
	int		new_cnt;
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
	return (dest);
}

void	one_left_arrow(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j + 1] != NULL)
	{
		if (shell->auxilar[i]->input_fd != -2)
			close(shell->auxilar[i]->input_fd);
		if (!is_sp_sim(shell->parser_res[i][j + 1][0]))
		{
			shell->auxilar[i]->input_fd
				= open(shell->parser_res[i][j + 1], O_RDONLY);
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

void	two_left_arrow(t_shell *shell, t_parser *pp, int i, int j)
{
	if (shell->parser_res[i][j + 1] != NULL)
	{
		if (shell->auxilar[i]->input_fd != -2)
			close(shell->auxilar[i]->input_fd);
		if (!is_sp_sim(shell->parser_res[i][j + 1][0]))
		{
			shell->auxilar[i]->output_fd
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
				twodimarr_str_calc(shell->parser_res[i]), pp.save_pos);
		free(shell->parser_res[i]);
		shell->parser_res[i] = pp.interim;
		i++;
		free(pp.save_pos);
	}
	shell->auxilar[i] = 0;
}
