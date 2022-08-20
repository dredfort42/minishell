/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 23:15:10 by msimon            #+#    #+#             */
/*   Updated: 2021/10/26 12:26:05 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cd(t_shell_data *sh_d)
{
	if (sh_d->fl_main_op == -1)
	{
		if (!lst_el_found(sh_d->env, "HOME") && !sh_d->cmd->cmd_params[1])
			write_err_cmd("cd", "HOME not set", NULL);
		else
			write_err_cmd("cd", sh_d->cmd->cmd_params[1],
				strerror(sh_d->errno_main_op));
		exit(1);
	}
	exit(0);
}

static void	cd_exec(t_shell_data *sh_d, char *pwd, int op)
{
	sh_d->fl_main_op = op;
	if (sh_d->fl_main_op == -1)
	{
		sh_d->errno_main_op = errno;
		return ;
	}
	if (sh_d->lst_cmd_size < 2)
	{
		set_env_by_name(sh_d, get_env_name("OLDPWD"),
			get_env_by_name(sh_d, "PWD"), 1);
		set_env_by_name(sh_d, get_env_name("PWD"), getcwd(NULL, 0), 1);
	}
	else
		chdir(pwd);
}

void	run_cd_main(t_shell_data *sh_d)
{
	char	*pwd;
	char	*path;

	pwd = getcwd(NULL, 0);
	path = NULL;
	if (sh_d->cmd->cmd_params[1] == 0)
		path = get_env_by_name(sh_d, "HOME");
	else if (ft_strlen(sh_d->cmd->cmd_params[1]) == 0)
		path = getcwd(NULL, 0);
	if (path)
		cd_exec(sh_d, pwd, chdir(path));
	else
		cd_exec(sh_d, pwd, chdir(sh_d->cmd->cmd_params[1]));
	if (path)
		free(path);
	free(pwd);
}
