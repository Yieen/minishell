/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:06:25 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/25 13:53:04 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	locate_parser_mem(t_shell *shell)
{
	int	i;

	i = 0;
	shell->parser_res = (char ***)malloc(sizeof(char **) * (shell->pipe_cnts + shell->arr_cnts + 2));
	while (i < shell->pipe_cnts + 2)
	{
		shell->parser_res[i] = (char **) malloc(sizeof(char *) * (shell->pipe_cnts + shell->arr_cnts + 2));
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