/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:07:06 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 00:03:48 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_clear(char	**lsttclear)
{
	int	i;

	i = 0;
	while (lsttclear[i])
	{
		free(lsttclear[i]);
		i++;
	}
	free(lsttclear);
}

void	free_lexer(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->lexer_res[i])
	{
		if (!((shell->lexer_res[i][0] == '|') || (shell->lexer_res[i][0] == '>')
			|| (shell->lexer_res[i][0] == '<')))
			free(shell->lexer_res[i]);
		i++;
	}
	free(shell->lexer_res);
}

void	close_env(t_shell *shell)
{
	int	i;

	free_lexer(shell);
	ft_clear(shell->env_param);
	i = 0;
	while (i < shell->pipe_cnts + 2)
	{
		free(shell->parser_res[i]);
		i++;
	}
	free(shell->parser_res);
	i = 0;
	while (shell->auxilar[i])
	{
		free(shell->auxilar[i]);
		i++;
	}
	free(shell->auxilar);
	if (shell->prompt)
		free(shell->prompt);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->term);
	*shell = (t_shell){0};
}

void	add_free(t_shell *shell)
{
	int	i;

	i = 0;
	free_lexer(shell);
	while (i < shell->pipe_cnts + 2)
	{	
		free(shell->parser_res[i]);
		i++;
	}
	free(shell->parser_res);
	i = 0;
	while (shell->auxilar[i])
	{
		free(shell->auxilar[i]);
		i++;
	}
	free(shell->auxilar);
	free(shell->prompt);
}
