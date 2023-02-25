/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_fun.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 16:35:31 by inovomli          #+#    #+#             */
/*   Updated: 2023/02/19 20:07:22 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	if (!s)
		return (0);
	while (s[len] != '\0')
		len++;
	return (len);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	srcsize;
	size_t	i;

	srcsize = (size_t)ft_strlen(src);
	i = 0;
	if (dstsize != 0)
	{
		while ((src[i]) && (i < (dstsize - 1)))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srcsize);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;

	if (!s)
		return (0);
	if (ft_strlen(s) < start)
		len = 0;
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	ft_strlcpy(ret, s + start, len + 1);
	return (ret);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*needle)
		return ((char *)haystack);
	while ((i < len) && haystack[i])
	{
		j = 0;
		while ((needle[j] == haystack[i + j]) && ((i + j) < len))
		{
			if (!needle[j + 1])
			{
				return ((char *)haystack + i);
			}
			j++;
		}
		i++;
	}
	return (0);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	cnt;
	size_t	res;

	if (!dst && !src)
		return (0);
	if (!dstsize)
		return (ft_strlen(src));
	res = ft_strlen(src) + ft_strlen(dst);
	if (dstsize <= (size_t)ft_strlen(dst))
		return (dstsize + ft_strlen(src));
	dstlen = ft_strlen(dst);
	cnt = 0;
	while (src[cnt] != '\0' && dstlen + 1 < dstsize)
	{
		dst[dstlen] = src[cnt];
		dstlen++;
		cnt++;
	}
	dst[dstlen] = '\0';
	return (res);
}
