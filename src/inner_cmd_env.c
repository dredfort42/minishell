/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 11:02:25 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 22:53:06 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_cmd_this(t_shell_data *sh_d)
{
	char	*path;
	char	*full_cmd;
	int		i;

	path = get_env_by_name(sh_d, "PATH");
	full_cmd = found_cmd(path, sh_d->cmd->cmd_params[0]);
	if (full_cmd)
	{
		i = -1;
		while (sh_d->cmd->cmd_env[++i])
		{
			if (ft_strlen(sh_d->cmd->cmd_env[i]) > 1)
				if (sh_d->cmd->cmd_env[i][0] == '_' &&
					sh_d->cmd->cmd_env[i][1] == '=')
					break ;
		}
		if (!sh_d->cmd->cmd_env[i])
			join_str_arr(&(sh_d->cmd->cmd_env), ft_strjoin("_=", full_cmd));
		else
		{
			free(sh_d->cmd->cmd_env[i]);
			sh_d->cmd->cmd_env[i] = ft_strjoin("_=", full_cmd);
		}
		free(full_cmd);
	}
}

void	run_env(t_shell_data *sh_d)
{
	int	i;

	i = -1;
	while (sh_d->cmd->cmd_env[++i])
		printf("%s\n", sh_d->cmd->cmd_env[i]);
	exit(0);
}
