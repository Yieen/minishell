/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 23:39:59 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 23:41:41 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	b_cd_set_pwd_error(int num)
{
	char const	*str[] = {\
		PROG_NAME ": cd: error: export()", \
		PROG_NAME ": cd: error: b_get_pwd()", \
		PROG_NAME ": cd: error: ft_strjoin()"\
		};

	perror(str[num]);
	return (EXIT_FAILURE);
}

static int	b_cd_set_pwd(t_shell *shell)
{
	char		*pwd;
	char		*var;
	int			err;

	pwd = env_get_value(shell->env_param, "PWD");
	if (!pwd)
		var = ft_strdup("OLDPWD=");
	else
		var = ft_strjoin("OLDPWD=", pwd);
	err = export(shell, var);
	free(var);
	if (err)
		return (b_cd_set_pwd_error(0));
	pwd = b_get_pwd();
	if (!pwd)
		return (b_cd_set_pwd_error(1));
	var = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (!var)
		return (b_cd_set_pwd_error(2));
	err = export(shell, var);
	free(var);
	if (err)
		return (b_cd_set_pwd_error(0));
	return (EXIT_SUCCESS);
}

int	b_cd(t_shell *shell, int i)
{
	char *const	*argv = shell->parser_res[i];

	if (!argv[1])
	{
		ft_putstr_fd(PROG_NAME ": usage: cd [relative or absolute path]\n", \
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (chdir(argv[1]))
	{
		ft_putstr_fd(PROG_NAME ": cd: ", STDERR_FILENO);
		perror(argv[1]);
		return (EXIT_FAILURE);
	}
	return (b_cd_set_pwd(shell));
}
