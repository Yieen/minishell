/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:48:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:51 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static int	ft_print_arg(int fd, t_placeholder ph, va_list args)
{
	if (ph.type == 'c')
		return (ft_printf_fd_c(fd, va_arg(args, int), ph));
	if (ph.type == 's')
		return (ft_printf_fd_s(fd, va_arg(args, char *), ph));
	if (ph.type == 'p')
		return (ft_printf_fd_p(fd, (uintptr_t)va_arg(args, void *), ph));
	if (ph.type == 'd' || ph.type == 'i')
		return (ft_printf_fd_d(fd, va_arg(args, int), ph));
	if (ph.type == 'u')
		return (ft_printf_fd_u(fd, va_arg(args, unsigned int), ph));
	if (ph.type == 'X' || ph.type == 'x')
		return (ft_printf_fd_x(fd, va_arg(args, unsigned int), ph));
	if (ph.type == '%')
		return (ft_printf_fd_c(fd, '%', ph));
	return (0);
}

static int	ft_formatlen(const char *format)
{
	int	len;

	len = 0;
	while (format[len] && format[len] != '%')
		len++;
	return (len);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	int				len;
	va_list			args1;
	t_placeholder	ph;

	va_start(args1, format);
	len = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format = ft_getph(&ph, ++format);
			len += ft_print_arg(fd, ph, args1);
		}
		else
		{
			len += write(fd, format, ft_formatlen(format));
			format += ft_formatlen(format);
		}
	}
	va_end(args1);
	return (len);
}
