/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:57:33 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/15 20:20:25 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct Lexer
{
	int		s_cnt;	// counter for string
	int		l_cnt;	// new word (lexem) counter
	int		st_nlm;	// start nem lexem
	int		pipe_cnt;

	int		sng_qut;
	int		dub_qut;
	int		dollar;
	char	*command;
}	t_lexer;

typedef struct Shell
{
	char	**env_param;
	char	**lexer_res;
	char	*prompt;
	int		cmnd_cnt; // amoun lexer array (now pipe_cnts+1)
	int		cont_wrk;
	int		pipe_cnts;
	char	***parser_res;
	
}	t_shell;

typedef struct Environment
{
	char	*key;
	char	*value;
	char	*str;
	struct Environment	*next;
	struct Environment	*prev;
}	t_env_stract;

void	pipex(t_shell *shell);

#endif