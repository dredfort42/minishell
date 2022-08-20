/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimon <msimon@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 21:00:00 by msimon            #+#    #+#             */
/*   Updated: 2021/10/31 22:52:32 by msimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*lst_el_found(t_list *f_el, char *n_env)
{
	t_env	*el_con;

	while (f_el)
	{
		el_con = (t_env *)f_el->content;
		if (!ft_strncmp(n_env, el_con->name, ft_strlen(n_env) + 1))
			break ;
		f_el = f_el->next;
	}
	return (f_el);
}

void	set_env_by_name(t_shell_data *sh_d, char *n_env, char *v_env, char f_gl)
{
	t_list	*f_el;
	t_env	*el_con;

	f_el = lst_el_found(sh_d->env, n_env);
	if (f_el)
	{
		el_con = (t_env *)f_el->content;
		if (el_con->val)
			free(el_con->val);
		el_con->val = v_env;
		if (f_gl > el_con->f_gl)
			el_con->f_gl = f_gl;
		free(n_env);
	}
	else
	{
		el_con = malloc(sizeof(t_env));
		if (!el_con)
			exit_err(MEM_ERR);
		el_con->name = n_env;
		el_con->val = v_env;
		el_con->f_gl = f_gl;
		ft_lstadd_back(&(sh_d->env), ft_lstnew(el_con));
	}
}

static char	*set_full_env_el(t_env *el)
{
	int		len_n;
	int		len_v;
	char	*res;

	len_n = ft_strlen(el->name);
	len_v = ft_strlen(el->val);
	res = malloc(sizeof(char) * (len_n + len_v + 2));
	if (!res)
		exit_err(MEM_ERR);
	ft_memcpy(res, el->name, len_n);
	res[len_n] = '=';
	ft_memcpy(res + len_n + 1, el->val, len_v);
	res[len_n + len_v + 1] = 0;
	return (res);
}

char	**get_global_env(t_shell_data *sh_d)
{
	t_list	*lst_el;
	char	**res;
	int		i;

	lst_el = sh_d->env;
	set_env_by_name(sh_d, get_env_name("_"),
		get_env_name(sh_d->cmd->cmd_params[0]), 1);
	res = malloc(sizeof(char *) * (ft_lstsize(lst_el) + 1));
	if (!res)
		exit_err(MEM_ERR);
	i = 0;
	while (lst_el)
	{
		if (((t_env *)lst_el->content)->f_gl && ((t_env *)lst_el->content)->val)
		{
			res[i] = set_full_env_el((t_env *)lst_el->content);
			i++;
		}
		lst_el = lst_el->next;
	}
	res[i] = NULL;
	return (res);
}
