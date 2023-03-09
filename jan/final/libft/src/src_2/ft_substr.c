/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 17:30:58 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/09 15:52:53 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"
#include <stdio.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	s_len;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	if (start > s_len)
	{	
		len = 0;
		start = 0;
	}
	else if (len > s_len - start)
		len = s_len - start;
	str = (char *)malloc(sizeof(char) * (len + 1 + 1024));
	if (!str)
		return (NULL);
	s += start;
	ft_strlcpy(str, s, len + 1);
	return (str);
}
