/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wwenv_qutcon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 12:42:35 by inovomli          #+#    #+#             */
/*   Updated: 2023/03/09 12:45:57 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	del_n_last(char *str, int del)
{
	int	len;
	int	i;

	i = del;
	len = ft_strlen(str);
	while (i < len - 2)
	{
		str[i] = str[i + 1];
		i++;
	}
	str[i] = 0;
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
