/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 17:22:50 by msimon            #+#    #+#             */
/*   Updated: 2021/10/27 22:27:09 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_var_pos
{
	int		i;
	int		b;
	int		e;
	char	*val;
}	t_var_pos;

static void	init_val_var(char *str, t_var_pos *pos, char **p)
{
	*p = NULL;
	pos->i = -1;
	pos->b = 0;
	pos->e = ft_strlen(str);
	pos->val = NULL;
}

static void	add_to_res(t_command *res, char *param, char **str)
{
	t_var_pos	pos;

	(void)res;
	free(*str);
	init_val_var(param, &pos, str);
	while (++pos.i < pos.e)
	{
		if (!(param[pos.i] == 32 || (param[pos.i] > 8 && param[pos.i] < 14)))
		{
			pos.b = pos.i;
			pos.i = get_end_param(param, pos.i, pos.e);
			str_join_mem(&(pos.val), param, pos.b, pos.i);
			if (!chk_asterix(res, &(pos.val), param[pos.b]))
				join_str_arr(&res->cmd_params, pos.val);
			pos.val = NULL;
		}
	}
}

char	chk_variable(t_command *res, char **str, t_shell_data *sh_d)
{
	char		r;
	t_var_pos	pos;
	char		*param;

	r = 0;
	if (!sh_d->money_flag)
		return (0);
	init_val_var(*str, &pos, &param);
	while (++pos.i < pos.e)
	{
		if ((*str)[pos.i] == '$')
		{
			str_join_mem(&param, *str, pos.b, pos.i);
			pos.b = pos.i;
			pos.val = get_val_env(sh_d, *str, &(pos.b), pos.e);
			str_join_mem(&param, pos.val, 0, ft_strlen(pos.val));
			free(pos.val);
			r = 1;
		}
	}
	str_join_mem(&param, *str, pos.b, pos.e);
	if (r)
		add_to_res(res, param, str);
	free(param);
	return (r);
}
