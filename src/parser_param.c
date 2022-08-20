/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 14:07:27 by msimon            #+#    #+#             */
/*   Updated: 2021/10/27 22:17:01 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_get_str_param
{
	char	*res;
	int		i;
	char	quo;
	char	*val_e;
}	t_get_str_param;

void	str_join_mem(char **des, char *src, int b, int e)
{
	char	*buf;
	int		len;

	if (!*des)
	{
		*des = malloc(sizeof(char));
		if (!*des)
			exit_err(MEM_ERR);
		*des[0] = 0;
	}
	if (e > b)
	{
		len = ft_strlen(*des);
		buf = malloc(sizeof(char) * (len + e - b + 1));
		if (!buf)
			exit_err(MEM_ERR);
		buf[len + e - b] = 0;
		ft_memcpy(buf, *des, len);
		ft_memcpy(buf + len, src + b, e - b);
		free(*des);
		*des = buf;
	}
}

static char	*space_str(char *name_e)
{
	char	*res;

	free(name_e);
	res = malloc(sizeof(char));
	if (!res)
		exit_err(MEM_ERR);
	res[0] = 0;
	return (res);
}

char	*get_val_env(t_shell_data *sh_d, char *str, int *b, int e)
{
	char	*res;
	char	*name_e;
	int		i;

	name_e = NULL;
	i = *b;
	if (++i < e && (ft_isalpha(str[i]) || str[i] == '_'))
		while (++i < e && (ft_isalnum(str[i]) || str[i] == '_'))
			;
	str_join_mem(&name_e, str, *b + 1, i);
	if (ft_strlen(name_e) > 0)
		*b = i;
	else if (i < e && !(str[i] == 32 || (str[i] > 8 && str[i] < 14)))
	{
		*b = i + 1;
		str_join_mem(&name_e, str + i, 0, 1);
	}
	else
		return (space_str(name_e));
	res = get_env_by_name(sh_d, name_e);
	free(name_e);
	return (res);
}

static void	get_str_param_plus(t_get_str_param *m_val, char *str, int *b)
{
	if (!m_val->quo && (str[m_val->i] == '"' || str[m_val->i] == '\''))
	{
		str_join_mem(&(m_val->res), str, *b, m_val->i);
		*b = m_val->i + 1;
		m_val->quo = str[m_val->i++];
	}
	if (m_val->quo && str[m_val->i] == m_val->quo)
	{
		str_join_mem(&(m_val->res), str, *b, m_val->i);
		*b = m_val->i + 1;
		m_val->quo = 0;
	}
}

char	*get_str_param(t_shell_data *sh_d, char *str, int b, int e)
{
	t_get_str_param	m_v;

	m_v.res = NULL;
	m_v.i = b - 1;
	m_v.quo = 0;
	sh_d->money_flag = 0;
	while (++m_v.i < e)
	{
		get_str_param_plus(&m_v, str, &b);
		if (str[m_v.i] == '$' && (!m_v.quo || m_v.quo == '"'))
		{
			if (!m_v.quo)
				sh_d->money_flag = 1;
			else
			{
				str_join_mem(&(m_v.res), str, b, m_v.i);
				b = m_v.i;
				m_v.val_e = get_val_env(sh_d, str, &b, e);
				str_join_mem(&(m_v.res), m_v.val_e, 0, ft_strlen(m_v.val_e));
				free(m_v.val_e);
			}
		}
	}
	str_join_mem(&(m_v.res), str, b, e);
	return (m_v.res);
}
