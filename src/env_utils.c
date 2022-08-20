/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/25 20:25:50 by msimon            #+#    #+#             */
/*   Updated: 2021/10/26 12:00:23 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Нет проверки на валидность имени переменной*/
/*Нет подстановки значений вместо имени переменной*/
/*Нет обработки двойных и одинарных кавычек*/
/*Двойные и одинарные кавычки работают также как в echo*/
static void	init_env_el(t_env **res, char *str, size_t len, int i)
{
	(*res)->name = malloc(sizeof(char) * (i + 1));
	if (!(*res)->name)
		exit_err(MEM_ERR);
	ft_memcpy((*res)->name, str, i);
	(*res)->name[i] = 0;
	if (!str[i])
		(*res)->val = NULL;
	else
	{
		(*res)->val = malloc(sizeof(char) * (len - i));
		ft_memcpy((*res)->val, str + i + 1, len - i);
	}
}

t_env	*get_env_el(char *str, char f_gl)
{
	t_env	*res;
	int		i;
	size_t	len;

	res = malloc(sizeof(t_env));
	if (!res)
		exit_err(MEM_ERR);
	res->f_gl = f_gl;
	len = ft_strlen(str);
	if (len > 0)
	{
		if (str[0] == '=')
			i = 0;
		else
			i = -1;
		while (str[++i] != '=' && str[i] != 0)
			;
		init_env_el(&res, str, len, i);
	}
	else
	{
		res->name = NULL;
		res->val = NULL;
	}
	return (res);
}

static char	*get_str_by_el(t_env *f)
{
	char	*res;
	int		l;

	if (f)
	{
		l = ft_strlen(f->val);
		res = malloc(sizeof(char) * (l + 1));
		if (!res)
			exit_err(MEM_ERR);
		res[l] = 0;
		ft_memcpy(res, f->val, l);
	}
	else
	{
		res = malloc(sizeof(char));
		if (!res)
			exit_err(MEM_ERR);
		res[0] = 0;
	}
	return (res);
}

char	*get_env_by_name(t_shell_data *sh_d, char *str)
{
	t_list	*el;
	t_env	*el_env;
	t_env	*f_el_env;

	f_el_env = NULL;
	if (sh_d->env)
	{
		el = sh_d->env;
		while (el)
		{
			el_env = (t_env *)el->content;
			if (!ft_strncmp(str, el_env->name, ft_strlen(str) + 1))
			{
				f_el_env = el_env;
				break ;
			}
			el = el->next;
		}
	}
	return (get_str_by_el(f_el_env));
}

char	*get_env_name(char *str)
{
	char	*res;
	int		len;

	len = ft_strlen(str);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		exit_err(MEM_ERR);
	res[len] = 0;
	ft_memcpy(res, str, len);
	return (res);
}
