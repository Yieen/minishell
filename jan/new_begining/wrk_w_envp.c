/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrk_w_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 15:50:40 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/27 17:45:25 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	// int		len;

	pos = pos_into_env(envp, key);
	if(pos == -1)
		return (0);
	// len = ft_strlen(envp[pos]);
	// res = malloc(sizeof(char) * (len + 1));
	res = &envp[pos][char_srch(envp[pos],'=') + 1]; // TODO
	// printf("%s", res);
	return (res);
}

int	pos_into_env(char **envp, char *srch_str)
{
	int res;
	int	len;

	res = -1;
	len = ft_strlen(srch_str);
	if	(len == 0)
		return (res);
	while (envp[++res])
	{
		if ((ft_strnstr(envp[res], srch_str, len) == envp[res]) 
			&& (envp[res][len] == '='))
			return (res);
	}	
	return (-1);
}

int	twodimarr_str_calc(char **arr)
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

	// write(1,"0\n",2);
	equ_pos = char_srch(wrk_str, '=');
	rs_st = ft_substr(wrk_str, 0, equ_pos);
	// printf("%s", rs_st);
	// dest[old_cnt] = malloc((ft_strlen(src[i]) + 1) * sizeof(char));
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
			// dest[old_cnt] = malloc((ft_strlen(src[i]) + 1) * sizeof(char));
			// ft_strlcpy(dest[old_cnt], src[i], ft_strlen(src[i]) + 1);
			dest[old_cnt] = src[i];
			old_cnt++;
		}
		else
			del_i =-1;
		i++;
    }
	dest[old_cnt] = 0;
    return dest;
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
    return dest;
}

// int	find_sd(char *str, char ch) 
// {
//     int first;
//     int second;
//     int i;

// 	first = -1;
// 	second = -1;
// 	i = 0;
//     while (str[i] && second == -1) 
// 	{
//         if (str[i] == ch) 
// 		{
//             if (first == -1) 
//                 first = i;
// 			else if (second == -1)
//                 second = i;
//         }
//         i++;
//     }
//     return second;
// }