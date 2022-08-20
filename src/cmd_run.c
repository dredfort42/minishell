/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_run.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 12:28:04 by msimon            #+#    #+#             */
/*   Updated: 2021/11/04 09:26:30 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_found_cmd
{
	int	b;
	int	i;
}	t_found_cmd;

static char	chk_found_cmd(char **res, t_found_cmd *pos,
							char *str_path, char *str_com)
{
	int			len;
	struct stat	buff;
	int			p;

	if (*res)
		free(*res);
	len = ft_strlen(str_com);
	if (pos->b > pos->i)
		pos->b = pos->i;
	*res = malloc(sizeof(char) * (pos->i - pos->b + len + 2));
	p = 1;
	if (pos->b == pos->i)
		p = 0;
	else
		(*res)[pos->i - pos->b] = '/';
	(*res)[pos->i - pos->b + len + p] = 0;
	ft_memcpy(*res, str_path + pos->b, pos->i - pos->b);
	ft_memcpy(*res + pos->i - pos->b + p, str_com, len);
	if (!stat(*res, &buff))
	{
		free(str_path);
		return (1);
	}
	return (0);
}

char	*found_cmd(char *str_path, char *str_com)
{
	char		*res;
	t_found_cmd	pos;

	res = NULL;
	pos.b = 0;
	pos.i = -1;
	while (str_path[++(pos.i)])
	{
		if (str_path[pos.i] == ':')
		{
			if (chk_found_cmd(&res, &pos, str_path, str_com))
				return (res);
			pos.b = pos.i + 1;
		}
	}
	if (chk_found_cmd(&res, &pos, str_path, str_com))
		return (res);
	free(res);
	free(str_path);
	res = NULL;
	return (res);
}

static void	run_file(t_shell_data *sh_d)
{
	struct stat	buff;

	if (!stat(sh_d->cmd->cmd_params[0], &buff))
	{
		if (S_ISDIR(buff.st_mode))
		{
			write_err_cmd(sh_d->cmd->cmd_params[0], "is a directory", NULL);
			exit(126);
		}
		if (!(S_IRUSR & buff.st_mode))
		{
			write_err_cmd(sh_d->cmd->cmd_params[0], "Permission denied", NULL);
			exit(126);
		}
	}
	execve(sh_d->cmd->cmd_params[0],
		sh_d->cmd->cmd_params, sh_d->cmd->cmd_env);
	if (errno == 8)
		exit(0);
	write_err_cmd(sh_d->cmd->cmd_params[0], strerror(errno), NULL);
	if (errno == 13)
		exit(126);
	else
		exit(127);
}

static int	run_cmd_err(t_shell_data *sh_d, char *full_cmd)
{
	if (errno == 13)
	{
		write_err_cmd(full_cmd, strerror(errno), NULL);
		free(full_cmd);
		return (126);
	}
	free(full_cmd);
	write_err_cmd(sh_d->cmd->cmd_params[0], "command not found", NULL);
	return (127);
}

void	run_cmd(t_shell_data *sh_d, int *fd_pipe, char i)
{
	char	*full_cmd;
	char	*path;

	if (!set_redirect(sh_d->cmd, fd_pipe, i))
		exit(1);
	if (chk_inner_cmd(sh_d, sh_d->inner_cmd) != -1)
		run_inner_cmd(sh_d);
	else if (!ft_strchr(sh_d->cmd->cmd_params[0], '/'))
	{
		path = get_env_by_name(sh_d, "PATH");
		if (ft_strlen(path) == 0)
		{
			free(path);
			run_file(sh_d);
		}
		else
		{
			full_cmd = found_cmd(path, sh_d->cmd->cmd_params[0]);
			if (full_cmd)
				execve(full_cmd, sh_d->cmd->cmd_params, sh_d->cmd->cmd_env);
			exit(run_cmd_err(sh_d, full_cmd));
		}
	}
	else
		run_file(sh_d);
}
