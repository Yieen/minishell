/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:57:33 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/25 13:05:38 by inovomli         ###   ########.fr       */
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
	int		arr_cnt;
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
	int		arr_cnts;
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

// libft_fun
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
int		ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_substr(const char *s, unsigned int start, size_t len);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);

// wrk_w_envp
char	*env_get_value(char **envp, char *key);
int		pos_into_env(char **envp, char *srch_str);
int		twodimarr_str_calc(char **arr);
char	*cut_equ(char *wrk_str);
int		char_srch(char *str, char ch);
char	**del_ind_fr_array(char **src, int size, int del_i);
char	**copy_string_array(char **src, int size) ;

// builtins
void	unset(t_shell *shell, char *new_str);
void	export(t_shell *shell, char *new_str);
void	add_export(t_shell *shell, char *new_str);
void	env(t_shell *shell);
void	ft_clear(char	**lsttclear);

// parser and add
void	locate_parser_mem(t_shell *shell);
void	parser(t_shell *shell);

#endif

// void	checkleaks(void)
// {
// 	system("leaks a.out");
// }

	// save begining of str into res
	// find end $
	// cut substr key = ft_substr(shell->lexer_res[i], $next, ($end - $next))	
	// value = env_get_value(shell->env_param, "a")
	// if value != 0 => // add value to res
	// add end str to res
	// shell->lexer_res[i] = rs_st;