/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:44:52 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:45:09 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	b_echo(t_shell *shell, int i)
{
	char *const	*argv = shell->parser_res[i];
	const int	n = argv[1] != NULL && argv[1][0] == '-' \
		&& argv[1][1] == 'n' && argv[1][2] == '\0' && ++argv;

	while (*++argv)
	{
		if (*(argv + 1))
			ft_printf("%s ", *argv);
		else
			ft_printf("%s", *argv);
	}
	if (!n)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
