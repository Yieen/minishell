/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrk_w_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 15:50:40 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/07 14:01:53 by inovomli         ###   ########.fr       */
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

char	**del_ind_fr_array(char **src, int size, int del_i)
{
	int		i;
	char	**dest;
	int		old_cnt;

	i = 0;
	old_cnt = 0;
	dest = malloc(sizeof(char *) * (size + 1));
	while (src[i])
	{
		if (old_cnt != del_i)
		{
			dest[old_cnt] = src[i];
			old_cnt++;
		}
		else
			del_i = -1;
		i++;
	}
	dest[old_cnt] = 0;
	return (dest);
}

char	**copy_string_array(char **src, int size)
{
	int		i;
	int		len;
	char	**dest;

	i = 0;
	dest = malloc(sizeof(char *) * (size + 1));
	while (src[i])
	{
		len = ft_strlen(src[i]) + 1;
		dest[i] = malloc(len * sizeof(char));
		ft_strlcpy(dest[i], src[i], len);
		i++;
	}
	dest[i] = 0;
	return (dest);
}
