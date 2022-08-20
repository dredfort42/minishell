/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 22:27:56 by msimon            #+#    #+#             */
/*   Updated: 2021/10/26 16:35:19 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	chk_n_flag(char *str, int *f)
{
	if (f)
		*f = 1;
	if (*str != '-')
		return (1);
	str++;
	while (*str)
	{
		if (*str != 'n')
			return (1);
		str++;
	}
	return (0);
}

void	run_echo(t_shell_data *sh_d)
{
	int	i;
	int	fl_n;
	int	fl_cmp;

	i = 0;
	fl_n = 0;
	if (sh_d->cmd->cmd_params[1])
	{
		fl_cmp = chk_n_flag(sh_d->cmd->cmd_params[1], NULL);
		while (sh_d->cmd->cmd_params[++i])
		{
			if (!fl_cmp)
				fl_cmp = chk_n_flag(sh_d->cmd->cmd_params[i + 1], &fl_n);
			else
			{
				if (sh_d->cmd->cmd_params[i + 1])
					printf("%s ", sh_d->cmd->cmd_params[i]);
				else
					printf("%s", sh_d->cmd->cmd_params[i]);
			}
		}
	}
	if (!fl_n)
		printf("\n");
	exit(0);
}
