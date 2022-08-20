/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_cmd_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 17:02:43 by msimon            #+#    #+#             */
/*   Updated: 2021/10/26 22:06:23 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	chk_env_name(char *n)
{
	if (!n)
		return (0);
	if (!ft_isalpha(*n) && *n != '_')
		return (0);
	n++;
	while (*n)
	{
		if (!(ft_isalnum(*n) || *n == '_'))
			return (0);
		n++;
	}
	return (1);
}

int	write_err_export(char *op, char *str)
{
	char	*new_str;
	int		len;

	len = ft_strlen(str);
	new_str = malloc(len + 3);
	if (new_str)
	{
		new_str[0] = '`';
		new_str[len + 1] = '\'';
		new_str[len + 2] = 0;
		ft_memcpy(new_str + 1, str, len);
		write_err_cmd(op, new_str,
			"not a valid identifier");
		free(new_str);
	}
	return (1);
}

void	run_export(t_shell_data *sh_d)
{
	t_env	*env;
	int		i;
	int		ext;

	ext = 0;
	if (ft_arr_size(sh_d->cmd->cmd_params) == 1)
		write_export(sh_d);
	else
	{
		i = 0;
		while (sh_d->cmd->cmd_params[++i])
		{
			env = get_env_el(sh_d->cmd->cmd_params[i], 1);
			if (!chk_env_name(env->name))
				ext = write_err_export("export", env->name);
			free(env->val);
			free(env->name);
			free(env);
		}
	}
	exit(ext);
}

void	run_export_main(t_shell_data *sh_d)
{
	t_env	*env;
	int		i;

	if (sh_d->lst_cmd_size == 1)
	{
		i = 0;
		while (sh_d->cmd->cmd_params[++i])
		{
			env = get_env_el(sh_d->cmd->cmd_params[i], 1);
			if (chk_env_name(env->name))
				set_env_by_name(sh_d, env->name, env->val, env->f_gl);
			else
			{
				free(env->val);
				free(env->name);
			}
			free(env);
		}
	}
}
