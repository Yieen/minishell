/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:57:33 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/08 19:41:31 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include "../libft/include/libft.h"
# include <termios.h>

# define PROMPT			"minishell$ "
# define PROG_NAME		"minishell"
# define PWD_BUF_SIZE	1024


typedef struct Catstr
{
	int		i;
	int		j;
	int		*save_pos;
	int		s_p_cnt;
	char	**interim;	
}	t_combstr;

typedef struct PostParser
{
	int		*save_pos;
	int		s_p_cnt;
	char	**interim;
	int		is_break;
}	t_parser;

typedef struct Dollar
{
	int		d_pos;
	int		end_key;
	char	*key;
	char	*rs_st;
	char	*value;
	char	*start;
	int		*save_pos;
	int		s_p_cnt;
	char	**tlr;
	int		i;
	int		j;	
}	t_dolar;

typedef struct pipex
{
	int		pr_id;
	int		input_fd;
	int		output_fd;
	int		is_exec;
}	t_pipex;

typedef struct Lexer
{
	int		s_cnt;
	int		l_cnt;
	int		st_nlm;
	int		pipe_cnt;
	int		arr_cnt;
	int		arr_lf_cnt;
	int		sng_qut;
	int		dub_qut;
	int		dollar;
	char	*command;
}	t_lexer;

typedef struct Shell
{
	char			**env_param;
	char			**lexer_res;
	char			*prompt;
	int				cont_wrk;
	int				pipe_cnts;
	int				arr_cnts;
	int				arr_lf_cnts;
	char			***parser_res;
	t_pipex			**auxilar;
	int				last_comm_ret;
	struct termios	term;
}	t_shell;

// wrk_w_envp
char	*env_get_value(char **envp, char *key);
int		pos_into_env(char **envp, char *srch_str);
int		tdar_str_calc(char **arr);
char	*cut_equ(char *wrk_str);
int		char_srch(char *str, char ch);
char	**del_ind_fr_array(char **src, int size, int del_i);
char	**copy_string_array(char **src, int size);

// builtins
void	unset(t_shell *shell, char *new_str);
int		export(t_shell *shell, char *new_str);
void	add_export(t_shell *shell, char *new_str);
void	env(t_shell *shell);
void	ft_clear(char	**lsttclear);

// parser and add
void	locate_parser_mem(t_shell *shell);
void	parser(t_shell *shell);
void	post_parser(t_shell *shell);
char	**del_elms_fr_array(char **src, int size, int *del_arr, int s_p_cnt);

// main
int		is_sp_sim(char ch);

void	pipex(t_shell *shell);
void	execute(t_shell *shell);
char	*ft_getenv(char *name, char **env);
char	*search_pathname(char *name, char **env);
char	*get_pathname(t_shell *shell, int i);

typedef int	(*t_buildin)(t_shell *, int);

int		b_echo(t_shell *shell, int i);
int		b_pwd(t_shell *shell, int i);
int		b_cd(t_shell *shell, int i);
int		b_exit(t_shell *shell, int i);
int		b_env(t_shell *shell, int i);
int		b_export(t_shell *shell, int i);
int		b_unset(t_shell *shell, int i);

// lexer
int		work_pipe_or_ec(t_shell *shell, t_lexer	*lxr);
int		is_space(char ch);
int		is_sp_sim(char ch);
int		find_end_key(char *str, int st);

// lexer2
int		start_new_lexem(t_lexer	*lxr);
int		end_lexem(t_lexer	*lxr);
int		check_two_pipes(t_shell *shell);
void	init_lexer(t_shell *shell, t_lexer	*lexer_st);
int		lexer(t_shell *shell);

// work with dollar
void	first_part_wwd(t_dolar	*wwd, int i);
void	second_part_wwd(t_dolar	*wwd, int i, t_shell *shell);
int		dlr_mlc(t_shell *shell);
void	third_part_wwd(t_dolar	*wwd, int i, t_shell *shell);
void	work_with_dollar( t_shell *shell);

char	**remove_empty_var(t_shell *shell);
char	*b_get_pwd(void);
void	free_lexer(t_shell *shell);

//	quot_connect.c
void	del_n_last(char *str, int del);
void	remove_quotes(t_shell *shell);
void	combine_str(char ***prr);
void	combine_str2(char ***prr);

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