/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 18:16:33 by msimon            #+#    #+#             */
/*   Updated: 2021/11/02 16:06:55 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_arr_size(char **arr)
{
	int	res;

	res = 0;
	if (arr)
		while (arr[res])
			res++;
	return (res);
}

char	chk_inner_cmd(t_shell_data *sh_d, char **arr)
{
	int		i;
	char	*param0;
	int		len;
	int		str_len;

	change_cmd_this(sh_d);
	if (!arr || !sh_d->cmd->cmd_params || !sh_d->cmd->cmd_params[0])
		return (-1);
	i = -1;
	param0 = sh_d->cmd->cmd_params[0];
	str_len = ft_strlen(param0) + 1;
	len = ft_arr_size(arr);
	while (++i < len)
		if (!ft_strncmp(param0, arr[i], str_len))
			return (i);
	return (-1);
}

void	run_inner_cmd(t_shell_data *sh_d)
{
	char	cmd_n;

	cmd_n = chk_inner_cmd(sh_d, sh_d->inner_cmd);
	if (cmd_n == 0)
		run_echo(sh_d);
	else if (cmd_n == 1)
		run_cd(sh_d);
	else if (cmd_n == 2)
		run_pwd(sh_d);
	else if (cmd_n == 3)
		run_export(sh_d);
	else if (cmd_n == 4)
		run_unset(sh_d);
	else if (cmd_n == 5)
		run_env(sh_d);
	else if (cmd_n == 6)
		run_exit(sh_d);
}

void	run_inner_cmd_main(t_shell_data *sh_d)
{
	char	cmd_n;

	cmd_n = chk_inner_cmd(sh_d, sh_d->inner_cmd_main);
	if (cmd_n == 0)
		run_cd_main(sh_d);
	else if (cmd_n == 1)
		run_export_main(sh_d);
	else if (cmd_n == 2)
		run_unset_main(sh_d);
}
