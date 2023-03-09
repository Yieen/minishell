/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:45:33 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:47:12 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	b_env(t_shell *shell, int i)
{
	i = -1;
	while (shell->env_param[++i])
	{
		if (ft_strchr(shell->env_param[i], '='))
			printf("%s\n", shell->env_param[i]);
	}
	return (EXIT_SUCCESS);
}
