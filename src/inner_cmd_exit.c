/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 18:41:18 by msimon            #+#    #+#             */
/*   Updated: 2021/11/02 21:23:40 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	trim_sp(char *str, int *i, int *len)
{
	*i = -1;
	while (++*i < *len)
		if (str[*i] != 32 && !(str[*i] > 8 && str[*i] < 14))
			break ;
	if (*i == *len)
		return (1);
	while (*len > (*i - 1))
	{
		if (str[*len - 1] != 32 && str[*len - 1] != 9)
			break ;
		*len -= 1;
	}
	return (0);
}

static int	get_exit_nmb(char *nmb_str)
{
	int	res;
	int	len;
	int	i;
	int	sz;

	res = ft_atoi(nmb_str);
	res &= 255;
	len = ft_strlen(nmb_str);
	if (len == 0)
		return (256);
	if (trim_sp(nmb_str, &i, &len))
		return (256);
	if (nmb_str[i] == 43 || nmb_str[i] == 45)
		i++;
	i--;
	sz = 0;
	while (++i < len)
	{
		if ((nmb_str[i] > '0' && nmb_str[i] < ':') || sz)
			sz++;
		if (!ft_isdigit(nmb_str[i]) || sz > 19)
			return (256);
	}
	return (res);
}

void	cmd_exit(t_shell_data *sh_d, t_command *cmd)
{
	int	nmb;

	if (!cmd->cmd_params[1])
		nmb = 0;
	else
		nmb = get_exit_nmb(cmd->cmd_params[1]);
	if (nmb == 256)
	{
		write_err_cmd("exit", cmd->cmd_params[1],
			"numeric argument required");
		nmb = 255;
	}
	free_all(sh_d);
	exit(nmb);
}

char	chk_exit(t_shell_data *sh_d, t_list *lst_cmd)
{
	t_command	*cmd;
	int			len;

	sh_d->lst_cmd_size = ft_lstsize(lst_cmd);
	cmd = (t_command *)lst_cmd->content;
	if (!cmd->cmd_params || !cmd->cmd_params[0])
		return (0);
	if (ft_strncmp("exit", cmd->cmd_params[0], 5))
		return (0);
	if (!lst_cmd->next)
		if (!sh_d->c_flag)
			printf("exit\n");
	len = ft_arr_size(cmd->cmd_params);
	if (len > 1)
		if (get_exit_nmb(cmd->cmd_params[1]) == 256)
			return (1);
	if (len > 2)
		return (0);
	if (lst_cmd->next)
		return (0);
	return (1);
}

void	run_exit(t_shell_data *sh_d)
{
	int	ext;
	int	len;

	ext = 0;
	len = ft_arr_size(sh_d->cmd->cmd_params);
	if (len > 1)
	{
		ext = get_exit_nmb(sh_d->cmd->cmd_params[1]);
		if (ext == 256)
		{
			write_err_cmd("exit", sh_d->cmd->cmd_params[1],
				"numeric argument required");
			exit(255);
		}
	}
	if (len > 2)
	{
		write_err_cmd("exit", "too many arguments", NULL);
		exit(1);
	}
	exit(ext);
}
