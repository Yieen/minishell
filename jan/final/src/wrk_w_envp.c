/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrk_w_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 15:50:40 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 12:45:18 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	char_srch(char *str, char ch)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ch)
			return (i);
		i++;
	}
	return (-1);
}

char	*env_get_value(char **envp, char *key)
{
	int		pos;
	char	*res;

	pos = pos_into_env(envp, key);
	if (pos == -1)
		return (0);
	if (char_srch(envp[pos], '=') == -1)
		return (0);
	res = &envp[pos][char_srch(envp[pos], '=') + 1];
	return (res);
}

int	pos_into_env(char **envp, char *srch_str)
{
	int	res;
	int	len;

	res = -1;
	len = ft_strlen(srch_str);
	if (len == 0)
		return (res);
	while (envp[++res])
	{
		if ((ft_strnstr(envp[res], srch_str, len) == envp[res])
			&& (envp[res][len] == '='))
			return (res);
		if ((ft_strnstr(envp[res], srch_str, len) == envp[res])
			&& (envp[res][len] == '\0'))
			return (res);
	}	
	return (-1);
}

int	tdar_str_calc(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

char	*cut_equ(char *wrk_str)
{
	char	*rs_st;
	int		equ_pos;

	equ_pos = char_srch(wrk_str, '=');
	rs_st = ft_substr(wrk_str, 0, equ_pos);
	return (rs_st);
}
