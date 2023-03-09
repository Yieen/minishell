/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:58:28 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:58:44 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*b_pwd_realloc_buf(char *buffer, size_t *size)
{
	char	*new_buffer;

	*size += PWD_BUF_SIZE;
	new_buffer = malloc(sizeof(*new_buffer) * (*size));
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	ft_memcpy(new_buffer, buffer, (*size) - PWD_BUF_SIZE);
	free(buffer);
	return (new_buffer);
}

char	*b_get_pwd(void)
{
	char	*buffer;
	size_t	size;

	size = PWD_BUF_SIZE;
	buffer = malloc(sizeof(*buffer) * size);
	if (!buffer)
		return (NULL);
	while (!getcwd(buffer, size))
	{
		buffer = b_pwd_realloc_buf(buffer, &size);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

int	b_pwd(t_shell *shell, int i)
{
	char	*pwd;

	(void)shell;
	(void)i;
	pwd = b_get_pwd();
	if (!pwd)
	{
		perror(PROG_NAME ": pwd: error: malloc()");
		return (EXIT_FAILURE);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (EXIT_SUCCESS);
}
